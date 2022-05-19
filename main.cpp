#include <iostream>
#include <fstream>

// Двусвязный список добавление только в конец, чтение только из начала.
template <class T>
class L2Queue {
private:
    int head;
    int current;
    int tail;

    class Node{
    public:
        int* next;
        int* prev;
        T data;
    };

public:
    void PushBack(T _data);
    void PushFront(T _data);
    void PopBack();
    void PopFront();

};

template <class T>
class L1List{
private:
    int head;
    int current;
    int teil;
    int size;
    int capacity;
    L2Queue<int> bufer;

    class Node{
    public:
        int it;
        int next;
        T data;
    };


};


//Считываем из файла в список.
L1List<int> ReadFileToList(std::ifstream& fileIn){}
//Записать файл в список
void WriteQueueToFile(const L2Queue<int>& queue, std::ostream& fileOut){}
//Поделить по чётности
void Split(const L1List<int>& list, L2Queue<int>& queue1, L2Queue<int>& queue2){}




int main() {}
