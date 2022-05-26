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


/*Список на основе массива, в этот раз более правдоподобный.
 * В данном классе, используется только массив и переменные для хранения размеров и индексов.
 * Работа с индексами ведётся таким образом, что массив по сути - закольцован.
 * */
class L1List{
private:
    int head = -1;
    int current = -1;
    int tail = -1;
    int size = 0;
    int capacity = 10;
    int* array = nullptr;

    //Изменение размера массива, если массива ещё нет, он создаётся, если есть - создаётся новый, вдвое больше
    //данные переносятся в том же парядке что и в списке.
    void ReSize(){
        if (!size){
            array = new int[capacity];
            head = tail = current = 0;
        }else{
            capacity *= 2;
            int* temp = new int [capacity];
            //Для того чтобы данные записывались в новый массив правильным образом, то-есть head соответствовал нулевому элементу массива
            //В новый массив записываем данные начиная с 0, в текущем массиве назначаем переменной j индекс головного элемента списка,
            //чтобы получить нужный элемент, берём остаток от деления текущего элемента на capacity.
            //Если индекс j не превышает capacity - то остатком от деления, будет тот же индекс.
            //Если индекс j превышает capacity - то остатком от деления, даст индекс из начала массива.
            for (int i = 0, j = head; i < size; ++i, ++j) {
                temp[i] = array[(j%size)];
            }
            delete[] array;
            array = temp;
            current = head = 0;
            tail = (size -1);
        }
    }
    void DeleteLastElem(){
        delete[] array;
        head = -1;
        current = -1;
        tail = -1;
        size = 0;
        capacity = 10;
        array = nullptr;
    }

public:
    void PushBack(int data){
        // Если Лист пуст, или заполнен, изменяем размер массива.
        if (!size || (size + 1) > capacity){
            ReSize();
        }
        // При создании нового массива, текущий элемент по умолчанию равен 0ж
        if (size){
            current = tail+1;
        }
        // Если текущий элемент больше или равен capacity, то переносим его в начало массива - установив его равным остатку от деления
        // самого себя на capacity. Проверка на то, является ли следующий элемент началом списка - не требуется, поскольку это может произойти только
        // при size > capacity, но если это так, то размер массива будет изменён до этой проверки, а новый элемент будет добавлен на позицию (capacity|2)+1
        if (current >= capacity){
            current %= capacity;
        }

        array[current] = data;
        tail = current;
        ++size;
    }

    void PushFront(int data){
        if (!size || (size + 1) > capacity){
            ReSize();
        }
        if (size){
            current = head-1;
        }

        if (current < 0){
            current = (capacity - 1);
        }

        array[current] = data;
        head = current;
        ++size;
    }

    void PopBack(){
        if (!(size - 1)){
            DeleteLastElem();
            return;
        }
        array[tail] = 0;
        current = (tail-1);
        // Если предыдущий элемент меньше 0, то он выходит за рамки массива, следовательно, отправляем его в конец массива.
        if (current < 0){
            tail = capacity - 1;
            --size;
        }else{
            --tail;
            --size;
        }
    }

    void  PopFront(){
        if (!(size - 1)){
            DeleteLastElem();
            return;
        }
        // Если следующий элемент массива выходит за его пределы, делаем head - первый элемент массива.
        array[head] = 0;
        if ((head + 1) >= capacity){
            head = 0;
            --size;
        } else{
            ++head;
            --size;
        }
    }

    void Clear(){
        while(size){
            PopFront();
        }
    }

    void ShowList(){
        if(!size){
            std::cout << "List is Empty!" << std::endl;
            return;
        }
        for (int i = 0, j = head; i < size; ++i, ++j) {
            std::cout << array[(j%capacity)] << "\t";
        }
        std::cout << std::endl;
    }

    void ShowArray(){
        if(!size){
            std::cout << "List is Empty!" << std::endl;
            return;
        }
        for (int i = 0; i < capacity; ++i) {
            std::cout << array[i] << "\t";
        }
        std::cout << std::endl;
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
list.PushBack(11);
list.PushFront(13);
list.PushFront(19);
list.PushBack(42);
list.PushBack(133);
list.PushFront(189);
list.PushFront(1329);
list.PushBack(742);
list.PushBack(444);
list.PushBack(555);


list.ShowList();
list.ShowArray();

std::cout << "--------------------------------------------------" << std::endl;
list.PopBack();
list.PopFront();

list.Clear();

list.ShowList();
list.ShowArray();

}
