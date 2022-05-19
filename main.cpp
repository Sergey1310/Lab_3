#include <iostream>
#include <fstream>

/* Это пока не полноценная двухстороняя очередь, чьего функционала пока достаточно.
 * Практически это тот же двусвязный список, но без возможности вставить или удалить элементы из произвольного места.
 *
 * ExtractLast - функция, которая извлекает данные из первого элемента очереди и удаляет первый элемент
 * Она используется для работы "Буфера" в односвязном списке на основе массива.
 * Остальные функции стандартные.
 */
class Queue{
    class Node{
    public:
        Node(int _data, Node* _prev = nullptr, Node* _next = nullptr): data(_data), prev(_prev),next(_next){}
        Node* next;
        Node* prev;
        int data;
    };
    Node* head = nullptr;
    Node* current = nullptr;
    Node* tail = nullptr;
    int size = 0;

public:
    void PushBack(int data){
        if (!head){
            tail = head = new Node(data);
        } else{
            current = new Node(data);
            tail->next = current;
            current->prev = tail;
            tail = current;
        }
        ++size;
    }
    void PushFront(int data){
        if (!head){
            tail = head = new Node(data);
        } else {
            current = new Node(data, nullptr, head);
            head->prev = current;
            head = current;
        }
        ++size;
    }
    void PopBack(){
        if (!head){
            std::cout << "Queue is empty" << std::endl;
            return;
        }else if(head == tail){
            delete head;
            head = tail = nullptr;
        }else{
            current = tail->prev;
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        --size;
    }
    void PopFront(){
        if (!head){
            std::cout << "Queue is empty" << std::endl;
            return;
        }else if(head == tail){
            delete head;
            head = tail = nullptr;
        }else{
            current = head->next;
            delete head;
            head = current;
            tail->prev = nullptr;
        }
        --size;
    }

    int ExtractFirst(){
        int temp = head->data;
        PopFront();
        return temp;
    };

    void Show(){
        if (!head){
            std::cout << "Queue is empty" << std::endl;
            return;
        } else{
            current = head;
            while (current){
                std::cout << current->data << "\t";
                current = current->next;
            }
            std::cout << std::endl;
        }
    }
    void Clear(){
        while (head){
            PopBack();
        }
    };

    friend std::ostream& operator<< (std::ostream& out, const Queue& queue);
    friend std::istream& operator>> (std::istream& in, Queue & queue);
};

std::ostream &operator<<(std::ostream &out, const Queue &queue) {
    Queue::Node* current = queue.head;
    if (current) {
        while (current) {
            out << current->data << "\t";
            current = current->next;
        }
    } else{
        std::cout << "Queue is empty" << std::endl;
    }
    return out;
}
std::istream &operator>>(std::istream &in, Queue &queue) {
    int temp;
    in >> temp;
    queue.PushBack(temp);

    return in;
}

/*
 *Идея данной реализации данного списка следующая:
 *Односвязный список состоит из Нод, в которых указан адрес следующего элемента.
 * Если размещать ноды в массиве, то в принципе какая разница, в какую ячейку добавлять, если список будет работать по своей
 * внутренней адресации. Следовательно, что добавление в начало списка, что в конец, можно осуществить просто вставив Ноду
 * в следующую свободную ячейку массива, а удаление можно осуществить, просто исключив адрес элемента из внутренней адресации списка.
 *
 * Чтобы постоянно е расширять массив при добавлении нового элемента, предусмотренно capacity, при добавлении первого элемента
 * - оно составляет 10 после заполнения массив перезаписывается в новый с удвоенным capacity.
 *
 * Встаёт проблема, если постоянно добавлять новые элементы списка в следующую ячейку массива, а удалять из произвольных мест, то в скором времени
 * образуется множество не заполненных мест в массиве.
 *
 * Решение: Буфер на основе ранее реализованной очереди.
 * Принцип работы:
 * При создании первого элемента массива, равно как и при изменении размера, индексы не задействованных элементом массива, помещаются в очередь - операцией PushBack.
 * Таким образом в начале очереди, элементы будут доступны в правильном порядке.
 * При добавлении нового элемента его индекс берётся из первого элемента буфера с последующим его удалением при помощи функции очереди ExtractFirst.
 * При удалении элемента, его индекс добавляется в буфер, в начало очереди, для того чтобы пробел в массиве, был как можно скорее заполнен новым элементом.
 *
 * Для работы списка на основе массива, вместо адресов в памяти - используются индексы в массиве.
 */

class L1List{
private:
    int head = -1;
    int current = -1;
    int tail = -1;
    int size = 0;
    int capacity = 0;
    Queue bufer;

    class Node{
    public:
        // Конструктор по умолчанию, для создания ссылок на Ноды.
        Node(){
            it = 0;
            next = 0;
            data = 0;
        };
        // Конструктор принимающий только данные
        Node(int _data){
            data = _data;
        }
        // Конструктор принимающий индексы на текущий элемент и на следующий.
        Node(int _it, int _next): Node(data){
            it = _it;
            next = _next;
        }
        int it;
        int next;
        int data;
    };

    //Ссылка на массив Нод, так как мы не сразу вносим элементы, не стоит забивать память до первого элемента.
    Node *array = nullptr;
    //Функция первого объявления массива
    void NewArray(){
        capacity = 10;
        array = new Node[capacity];
        for (int i = 1; i < capacity ; ++i) {
            bufer.PushBack(i);
        }
    }
    // Функция меняющая размер массива - просто переписывает все элементы массива в массив большего размера.
    void Resize(){
        int addInBufer = capacity+1;
        capacity *=2;
        Node* temp = new Node[capacity];
        for (int i = 0; i < capacity; ++i) {
            temp[i] = array[i];
        }
        delete[] array;
        array = temp;
        for (int i = addInBufer; i < capacity ; ++i) {
            bufer.PushBack(i);
        }
    }

public:
    void PushBack(int data){
        //Проверка на наличие элементов в массиве
        if (head < 0){
            NewArray();
            head = tail = 0;
            array[head].data = data;
            array[head].it = 0;
            array[head].next = 0;
            ++size;
            return;
        }else if (size == capacity){ // Проверка на заполненность массива, при переполнении - увеличивает размер.
            Resize();
        }
        // Добавление элемента по сути аналогично добавлению в обычный список
        int elem = bufer.ExtractFirst(); // Берём свободный элемент из буфера.
        array[tail].next = elem;
        array[elem].it = elem;
        array[elem].next = elem;
        array[elem].data = data;
        tail = elem;
        ++size;

    };
    void PushFront(int data){
        if (head < 0){
            NewArray();
            head = tail = 0;
            array[head].data = data;
            array[head].it = 0;
            array[head].next = 0;
            ++size;
            return;
        }else if (size == capacity){
            Resize();
        }
        int elem = bufer.ExtractFirst();
        array[elem].next = head;
        array[elem].it = elem;
        array[elem].data = data;
        head = elem;
        ++size;
    };
    void PopBack(){
        //Проверка на пустоту
        if (head < 0){
            std::cout << "L1list is empty" << std::endl;
            return;
        }
        //Проверка на последний элемент, при удалении последнего элемента, массив удаляется, параметры приводятся в исходное состояние.
        if (head == tail){
            delete[] array;
            head = -1;
            current = -1;
            tail = -1;
            size = 0;
            capacity = 0;
            bufer.Clear();
        }else{
            current = head;
            while (array[current].next != tail){ // Ищем предпоследний элемент
                current = array[current].next;
            }
            bufer.PushFront(tail); //Отправляем последний элемент в начало очереди буфера.
            tail = current;
            array[current].next = current;
            --size;
        }

    };
    void PopFront(){
        if (head < 0){
            std::cout << "L1list is empty" << std::endl;
            return;
        }
        if (head == tail){
            delete[] array;
            head = -1;
            current = -1;
            tail = -1;
            size = 0;
            capacity = 0;
            bufer.Clear();
        }else{
            current = head;
            head = array[head].next;
            bufer.PushFront(current);
            --size;
        }
    };
    void Show(){
        current = head;
        int count = 0;
        if (head < 0){
            std::cout << "L1list is empty" << std::endl;
            return;
        }
        while(count < size){
            std::cout << array[current].data << "\t";
            current = array[current].next;
            ++count;
        }
        std::cout << std::endl;
    }
    void GetSize(){
        std::cout << size << std::endl;
    }
    void GetCapacity(){
        std::cout << capacity << std::endl;
    }
    void ShowArray(){
        for (int i = 0; i < capacity; ++i) {
            std::cout << "Index = " << i << "\t" << "Object in array\t  Data: " << array[i].data << "\t Next element: " << array[i].next << std::endl;
        }
    }
    void ShowBufer(){
        bufer.Show();
    }


};
/*

//Считываем из файла в список.
L1List<int> ReadFileToList(std::ifstream& fileIn){}
//Записать файл в список
void WriteQueueToFile(const L2Queue<int>& queue, std::ostream& fileOut){}
//Поделить по чётности
void Split(const L1List<int>& list, L2Queue<int>& queue1, L2Queue<int>& queue2){}*/




int main() {
L1List list;
list.PushBack(10);
list.PushBack(11);
list.PushBack(12);
list.PushBack(13);
list.PushBack(14);
list.PushBack(15);
list.Show();
list.PushFront(9);
list.PushFront(8);
list.PushFront(7);
list.PushFront(6);
list.PushFront(5);
list.Show();
list.GetSize();
list.GetCapacity();
list.PopBack();
list.PopFront();
list.Show();
list.GetSize();
list.GetCapacity();
list.ShowArray();
list.ShowBufer();
list.PushBack(99999);
list.Show();
list.GetSize();
list.GetCapacity();
list.ShowArray();
list.ShowBufer();
}
