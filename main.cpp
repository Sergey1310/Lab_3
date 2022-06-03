#include <iostream>
#include <fstream>
/*
 * Двунаправленная очередь.
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
    Queue() = default;
    //Методы добавления элементов в очередь.
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
    //Методы извлечения элементов из очереди
    int PopBack(){
        int temp;
        if (!head){
            std::cout << "Queue is empty" << std::endl;
            return 0;
        }else if(head == tail){
            temp = head->data;
            delete head;
            head = tail = nullptr;
        }else{
            temp = tail->data;
            current = tail->prev;
            delete tail;
            tail = current;
            tail->next = nullptr;
        }
        --size;
        return temp;
    }
    int PopFront(){
        int temp;
        if (!head){
            std::cout << "Queue is empty" << std::endl;
            return 0;
        }else if(head == tail){
            temp = head->data;
            delete head;
            head = tail = nullptr;
        }else{
            temp = head->data;
            current = head->next;
            delete head;
            head = current;
            tail->prev = nullptr;
        }
        --size;
        return temp;
    }
    //Методы получения значений на концах очереди.
    int GetFirst(){
        return head->data;
    }
    int GetLast(){
        return tail->data;
    }
    //Вспомогательные методы.
    int GetSize(){
        return size;
    }
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
    ~Queue(){
        Clear();
    }
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
    int head = -1; // Переменная хранящая индекс первого элемента списка.
    int current = -1; // Переменная служащая для навигации, хранит индекс текущего элемента.
    int tail = -1; // Переменная хранящая индекс последнего элемента списка.
    int size = 0; // Переменная хранящая размер списка.
    int capacity = 10; // Переменная хранящая размер массива
    int* array = nullptr; // Указатель на массив в динамической памяти.
    //Уменьшение размера массива.
    void CatSize(){
/* Для оптимизации памяти, разработано решение по сокращению размера массива если фактический размер будет меньше четверти capacity.
 *  Чтобы оставить запас для добавления новых элементов, новый массив будет составлять половину от исходного.*/
        capacity /=2;
        int* temp = new int [capacity];

        for (int i = 0, j = head; i < size; ++i, ++j) {
            temp[i] = array[(j%(capacity*2))];
        }
        delete[] array;
        array = temp;
        current = head = 0;
        tail = (size -1);

    }
    //Увеличение размера массива и/или создание нового.
    void ReSize(){
/* Функция имеет двойное назначение:
 * Если список пуст, создаётся новый массив на 10 элементов, объявляется значение переменных отвечающих за управление списком.
 * Если необходимо внести в список больше элементов, чем есть свободных ячеек в массиве, то данные перезаписываются в новый массив, вдвое больше исходного.*/
        if (!size){
            array = new int[capacity];
            head = tail = current = 0;
        }else{
            capacity *= 2;
            int* temp = new int [capacity];
            for (int i = 0, j = head; i < size; ++i, ++j) {
                temp[i] = array[(j%(capacity/2))];
            }
            delete[] array;
            array = temp;
            current = head = 0;
            tail = (size -1);
        }
    }
    //Удаление последнего элемент.
    void DeleteLastElem(){
/* Для удаления последнего элемента списка предусмотрена отдельная функция, которая также удаляет массив и приводит параметры экземпляра списка к исходным*/
        delete[] array;
        head = -1;
        current = -1;
        tail = -1;
        size = 0;
        capacity = 10;
        array = nullptr;
    }
public:
    //L1List() = default;
    // Методы добавления элементов в список.
    void PushBack(int data){
        // Если Лист пуст, или заполнен, изменяем размер массива.
        if (!size || (size + 1) > capacity){
            ReSize();
        }
        // При создании нового массива, текущий элемент по умолчанию равен 0ж
        if(size){
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
        current = head;
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
    void insert (int index, int data){
        if (index < 0 || index > size+1){
            std::cout << "Index is not correct" << std::endl;
            return;
        }
        if (index == (size)){
            PushBack(data);
            return;
        }
        if ( index == 0){
            PushFront(data);
            return;
        }
        if(head == 0 && size == 0){
            PushBack(data);
            return;
        }
        if (!size || (size + 1) > capacity){
            ReSize();
        }
        int nextEl = tail;
        PushBack(array[tail]);
        current = nextEl;
        //При добавлении нового элемента в конец, мы сразу передаём ему значение последнего элемента
        //Следовательно требуется на одну операцию перестановки меньше -> из size вычитаем index + 1
        for (int i = size - (index + 2); i ; --i) {
            Prev();
            array[nextEl] = array[current];
            nextEl = current;
        }
        array[current] = data;
    }
    // Методы удаления элементов списка
    int PopBack(){
        int temp = array[tail];
        if (!(size - 1)){
            temp = array[head];
            DeleteLastElem();
            return temp;
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
        current = head;
        if(size < capacity/4 && capacity != 10){
            CatSize();
        }
        return temp;
    }
    int  PopFront(){
        int temp = array[head];
        if (!(size - 1)){
            DeleteLastElem();
            return temp;
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
        current = head;
        if(size < capacity/4 && capacity != 10){
            CatSize();
        }
        return temp;
    }
    int remove (int index){
        if (index < 0 || index >= size){
            std::cout << "Index is not correct" << std::endl;
            return 0;
        }
        if (index == 0){
            return PopFront();
        }
        if (index == (size -1)){
            return PopBack();
        }
        int Elem = current = (head + index)%capacity;
        int temp = array[current];
        while (current != tail){
            Next();
            array[Elem] = array[current];
            Elem = current;
        }
        PopBack();
        if(size < capacity/4 && capacity != 10){
            CatSize();
        }
        return temp;
    }
    void Clear(){
        while(size){
            PopFront();
        }
    }
    //Методы отображения
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
    //Методы навигации.
    void ShowCurrentElem(){
        if(!size){
            std::cout << "List is Empty!" << std::endl;
            return;
        }
        std::cout << array[current] << std::endl;
    }
    void Next(){
        if(!size){
            std::cout << "List is Empty!" << std::endl;
            return;
        }
        if( current == tail){
            std::cout << "Next Element is not exist, this is the end of the list." << std::endl;
            return;
        }
        current = (current + 1) % capacity;
    };
    void Prev(){
        if(!size){
            std::cout << "List is Empty!" << std::endl;
            return;
        }
        if( current == head){
            std::cout << "Previous Element is not exist, this is the start of the list." << std::endl;
            return;
        }
        --current;
        if (current < 0){
            current = capacity-1;
        }
    }
    void GoToStart(){
        current = head;
    }
    void GoToEnd(){
        current = tail;
    }
    //Методы получения данных из списка.
    int GetCurrentElem(){
        if(!size){
            std::cout << "List is Empty!" << std::endl;
            return 0;
        }
        return array[current];
    }
    int GetHead(){
        return array[head];
    }
    int GetTail(){
        return array[tail];
    }

    int& operator [](int index){
        if (index < 0){
            std::cout << "Negative index, will be returned first element!" << std::endl;
            return array[head];
        }
        if(index > size){
            std::cout << "Too big index, will be returned last element!" << std::endl;
            return array[tail];
        }
        index = (head + index)%capacity;
        return array[index];
    }
    //Вспомогательные методы.
    int IsEmpty() const {
        return size <= 0;
    }
    int GetSize(){
        return size;
    }
    int GetCapacity(){
        return capacity;
    }

    ~L1List(){
        Clear();
    }
};

//Считываем из файла в список.
void ReadFileToList(L1List& list, const char* way){
    int temp;
    std::ifstream fileIn;
    fileIn.open(way);
    if (fileIn.is_open()){
        std::cout << "File:\t" << way << "\topened" << std::endl;
    }else{
        std::cout << "Error file:\t" << way << "\tis not opened!" << std::endl;
    }
    do{
        fileIn >> temp;
        list.PushBack(temp);
    } while (!fileIn.eof());
    fileIn.close();
}
//Записать файл в список
void WriteQueueToFile(Queue& queue, char* way){
    std::ofstream fileOut(way);
    if (!fileOut.is_open()){
        std::cout << "Error file:\t" << way << "\tis not opened!" << std::endl;
        return;
    }else{
        std::cout << "File:\t" << way << "\topened" << std::endl;
    }
    int size = queue.GetSize();
    while (size){
        fileOut << queue.PopFront() << " ";
        --size;
    }
    fileOut.close();
}
//Поделить по чётности
void Split(L1List& list, Queue& even, Queue& odd){
    int temp;
    while (!(list.IsEmpty())){
        temp = list.PopBack();
        if (temp%2){
            odd.PushBack(temp);
        }else{
            even.PushBack(temp);
        }
    }

}

int main() {
    L1List list;
    Queue even;
    Queue odd;
    std::ifstream input;
    char inputWay[50] = "./Files/input.txt";
    char outputEvenWay[50] = "./Files/output_even.txt";
    char outputOddWay[50] = "./Files/output_odd.txt";
    ReadFileToList(list,inputWay);
    list.ShowList() ;
    Split(list,even,odd);
    even.Show();
    odd.Show();
    WriteQueueToFile(even,outputEvenWay);
    WriteQueueToFile(odd,outputOddWay);
    }