#include <assert.h>
#include <iostream>
#include <vector>

class Deque {
public:
    Deque();
    // Проверка очереди на пустоту
    bool empty() const;
    // Добавление элемента
    void push_front(int value);
    void push_back(int value);
    // Извлечение элемента
    int pop_front();
    int pop_back();

    std::vector<int> data;
    int head, tail;
    int size();

private:
    int dec_position(int position);
    int inc_position(int position);
    void expand();
};

Deque::Deque() {
    data.resize(7);
    head = data.size() - 1;
    tail = 0;
};

void Deque::push_front(int value) {
    if (head == tail) expand();
//    std::cout << "push_front" << " ";
//    std::cout << value << " ";
    data[head] = value;
    head = dec_position(head);
//    for (int val: data){
//        std::cout << val;
//    }
//    std::cout << std::endl;
}

void Deque::push_back(int value) {
    if (head == tail) expand();
//    std::cout << "push_back" << " ";
//    std::cout << value << " ";
    data[tail] = value;
    tail = inc_position(tail);
//    for (int val: data){
//        std::cout << val;
//    }
//    std::cout << std::endl;
}

int Deque::pop_front() {
    int index = inc_position(head);
    int value = -1;
    if (index != tail) {
        value = data[index];
        data[index] = 0;
        head = index;
    }
//    std::cout << "pop_front" << " ";
//    std::cout << value << " ";
//    for (int val: data){
//        std::cout << val;
//    }
//    std::cout << std::endl;
    return value;
}

int Deque::pop_back() {
    int index = dec_position(tail);
    int value = -1;
    if (index != head) {
        value = data[index];
        data[index] = 0;
        tail = index;
    }
//    std::cout << "pop_back" << " ";
//    std::cout << value << " ";
//    for (int val: data){
//        std::cout << val;
//    }
//    std::cout << std::endl;
    return value;
}

int Deque::size() {
    int size = tail - head - 1;
    if (size < 0) {
        size += data.size();
    }
    return size;
}

int Deque::dec_position(int position) {
    int prev_pos = position - 1;
    if (prev_pos < 0) {
        prev_pos = data.size() - 1;
    }
    return prev_pos;
}

int Deque::inc_position(int position) {
    int next_pos = position + 1;
    if (next_pos > data.size() - 1) {
        next_pos = 0;
    }
    return next_pos;
}

void Deque::expand() {
    int begin_index = inc_position(head);
    int end_index = dec_position(tail);
    if (begin_index < end_index) {
        assert(begin_index == 0);
        int size = data.size();
        data.resize(size * 2);
        tail = inc_position(end_index);
        head = dec_position(begin_index);
    } else {
        assert(begin_index != end_index);
        int size = data.size();
        int new_size = size * 2;
        data.resize(new_size);
        for (int i = begin_index; i < size; i++) {
            data[i + new_size - size] = data[i];
        }
        head = dec_position(begin_index + new_size - size);
    }
}

int main() {
    int commands_count = 0;
    std::cin >> commands_count;
    Deque deque;
    for (int i = 0; i < commands_count; ++i) {
        int command = 0;
        int value = 0;
        std::cin >> command >> value;
        switch (command) {
            case 1: {
                deque.push_front(value);
                break;
            }
            case 2: {
                if (value != deque.pop_front()) {
                    std::cout << "NO";
                    return 0;
                }
                break;
            }
            case 3: {
                deque.push_back(value);
                break;
            }
            case 4: {
                if (value != deque.pop_back()) {
                    std::cout << "NO";
                    return 0;
                }
                break;
            }
        }
    }
//    for (int val: deque.data){
//        std::cout << val;
//    }
    std::cout << std::endl << "YES";
    return 0;
}

