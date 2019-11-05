/**
 * Реализовать дек с динамическим зацикленным буфером.
 * Обрабатывать команды push * и pop *.
 */
#include <assert.h>

#include <iostream>
#include <vector>

class Deque {
public:
    Deque();
    // Добавление элемента
    void push_front(int value);
    void push_back(int value);
    // Извлечение элемента
    int pop_front();
    int pop_back();

private:
    // Получение соседнего индекса массива(с учетом цикличности)
    int dec_position(int position);
    int inc_position(int position);
    // Расширение массива
    void expand();

private:
    static const int ORIGINAL_DATA_SIZE = 8;
    // Массив данных
    std::vector<int> data;
    // Позиции для добавления новых элементов
    int head, tail;
};

Deque::Deque() {
    data.resize(ORIGINAL_DATA_SIZE);
    head = data.size() - 1;
    tail = 0;
};

void Deque::push_front(int value) {
    // Если хвост и голова дека встретились => расширяем массив данных
    if (head == tail) expand();
    data[head] = value;
    head = dec_position(head);
}

void Deque::push_back(int value) {
    // Если хвост и голова дека встретились => расширяем массив данных
    if (head == tail) expand();
    data[tail] = value;
    tail = inc_position(tail);
}

int Deque::pop_front() {
    // Нашли предыдущий индекс головы
    int index = inc_position(head);
    // Если наступили на хвост, значит дек пустой, возвращаем -1
    if (index == tail) {
        return -1;
    }
    // Получаем значение
    int value = data[index];
    // На всякий случай зануляем (возможно лишнее)
    data[index] = 0;
    // Переставляем индекс головы
    head = index;
    return value;
}

int Deque::pop_back() {
    // Нашли предыдущий индекс хвоста
    int index = dec_position(tail);
    // Если наступили на голову, значит дек пустой, возвращаем -1
    if (index == head) {
        return -1;
    }
    // Получаем значение
    int value = data[index];
    // На всякий случай зануляем (возможно лишнее)
    data[index] = 0;
    // Переставляем индекс хвоста
    tail = index;
    return value;
}

int Deque::dec_position(int position) {
    if (position == 0) {
        return data.size() - 1;
    }
    return position - 1;
}

int Deque::inc_position(int position) {
    if (position == (data.size() - 1)) {
        return 0;
    }
    return position + 1;
}

void Deque::expand() {
    // Индекс где "начинаются" данные
    int begin_index = inc_position(head);
    // Индекс где "заканчиваются" данные
    int end_index = dec_position(tail);
    // Данные не могут начинаться и заканчиваться в одном месте
    assert(begin_index != end_index);
    // Если данные хранятся без зацикливания
    if (begin_index < end_index) {
        // Единственный сценарий когда данные хранятся в массиве с 0 и до конца
        assert(begin_index == 0);
        // В этом случае просто удваиваем массив и передвигаем индексы головы и хвоста
        int size = data.size();
        data.resize(size * 2);
        tail = inc_position(end_index);
        head = dec_position(begin_index);
    } else {
        // Удваиваем массив
        int size = data.size();
        int new_size = size * 2;
        data.resize(new_size);
        // Сдвигаем все элементы правой части массива (от головы до конца массива) к новому концу массива
        for (int i = begin_index; i < size; i++) {
            data[i + new_size - size] = data[i];
            // На всякий случай зануляем (возможно лишнее)
            data[i] = 0;
        }
        // Передвигаем индекс головы (хвост остался на месте)
        head = dec_position(begin_index + new_size - size);
    }
}

int main() {
    int commands_count = 0;
    std::cin >> commands_count;
    int command = 0;
    int value = 0;
    Deque deque;
    for (int i = 0; i < commands_count; ++i) {
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
            default:
                assert(false);
        }
    }
    std::cout << std::endl << "YES";
    return 0;
}


