/**
 * Для сложения чисел используется старый компьютер. Время, затрачиваемое на нахождение суммы двух чисел равно их сумме.
 * Таким образом для нахождения суммы чисел 1,2,3 может потребоваться разное время, в зависимости от порядка вычислений.
 * ((1+2)+3) -> 1+2 + 3+3 = 9
 * ((1+3)+2) -> 1+3 + 4+2 = 10
 * ((2+3)+1) -> 2+3 + 5+1 = 11
 * Требуется написать программу, которая определяет минимальное время, достаточное для вычисления суммы заданного набора чисел.
 * Решение всех задач данного раздела предполагает использование кучи.
 */
#include <assert.h>

#include <iostream>
#include <vector>

// Очередь с приоритетом (min-heap)
class PriorityQueue {
public:
    bool empty() const;
    // Получить наименьший элемент
    int top() const;
    // Вытолкнуть наименьший элемент
    int pop();
    // Добавить элемент
    void push(int value);

private:
    // Массив данных
    std::vector<int> data;
    // "Просеять" вверх (нормализация кучи)
    void sift_up(int position);
    // "Просеять" вниз (нормализация кучи)
    void sift_down(int position);
};

bool PriorityQueue::empty() const {
    return !data.size();
}

int PriorityQueue::top() const {
    return data[0];
}

int PriorityQueue::pop() {
    // Наименьший элемент - самый первый
    int value = data[0];
    // На место первого элемента ставим самый последний
    data[0] = data[data.size() - 1];
    data.resize(data.size() - 1);
    // Просеиваем первый элемент вниз (чтобы вернуть свойства кучи)
    sift_down(0);
    return value;
}

void PriorityQueue::push(int value) {
    // Вставляем в конец массива
    data.push_back(value);
    // Просеиваем первый элемент вверх (чтобы вернуть свойства кучи)
    sift_up(data.size() - 1);
}

void PriorityQueue::sift_up(int position) {
    // Пока не дошли до первого элемента
    while (position > 0) {
        // Позиция родительского элемента
        int parent_postion = (position - 1) / 2;
        // Значения элементов
        int value = data[position];
        int parent = data[parent_postion];
        // Если элемент и так больше родителя, то все в порядке, уходим
        if (value > parent) {
            break;
        }
        // Если элемент не больше родителя, то меняем их местами
        data[parent_postion] = value;
        data[position] = parent;
        // Теперь разбираемся с предком
        position = parent_postion;
    }
}

void PriorityQueue::sift_down(int position) {
    while (1) {
        // Позиции элементов-потомков
        int children1_postion = position * 2 + 1,
            children2_postion = position * 2 + 2;
        int min_children, min_children_postion;
        if (children1_postion >= data.size()) {
            // Если потомков нет (позиции потомков выходят за границы массива) - выходим
            break;
        } else if (children1_postion == (data.size() - 1)) {
            // Если потомок только один - работаем с ним
            min_children = data[children1_postion];
            min_children_postion = children1_postion;
        } else {
            // Если потомка два - выбираем наименьший потомок
            assert(children1_postion < data.size());
            assert(children2_postion < data.size());
            int children1 = data[children1_postion],
                children2 = data[children2_postion];
            if (children1 <= children2) {
                min_children = children1;
                min_children_postion = children1_postion;
            } else {
                min_children = children2;
                min_children_postion = children2_postion;
            }
        }
        // Если position меньше наименьшего потомка - все ок, выходим
        int value = data[position];
        if (value <= min_children) {
            break;
        }
        // Если элемент больше наименьшего потомка, меняем местами
        data[position] = min_children;
        data[min_children_postion] = value;
        // Теперь работаем с потомком
        position = min_children_postion;
    }
}

int main() {
    int values_count = 0;
    std::cin >> values_count;
    int value = 0;
    PriorityQueue queue;
    // Читаем все значения в очередь с приоритетом
    for (int i = 0; i < values_count; ++i) {
        std::cin >> value;
        queue.push(value);
    }
    // Если только одно число - возвращаем его
    if (values_count == 1) {
        std::cout << std::endl << queue.pop();
        return 0;
    }
    int sum = 0;
    // Пока непустая очередь
    while (!queue.empty()) {
        // Берем два наименьших элемента
        int value1 = queue.pop();
        int value2 = queue.pop();
        // Складываем
        int part_sum = value1 + value2;
        // Полученную сумму добавляем к общей сумме
        sum += part_sum;
        if (queue.empty())
            break;
        // Полученную сумму добавляем в очередь с приоритетом (т.к. далее участвует в сложении)
        queue.push(part_sum);
    }
    std::cout << std::endl << sum;
    return 0;
}

