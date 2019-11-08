#include "sort.h"

#include <cstring>
#include <vector>
#include <iostream>
#include <ctime>
#include <utility>


/** Сортировка пузырьком */
//void Sort(unsigned int* arr, unsigned int size) {
//    for (unsigned int i = 0; i < size; i++)
//        for (unsigned int j = i + 1; j < size; j++) {
//            if(arr[i] < arr[j]) continue;
//            unsigned int tmp = arr[j];
//            arr[j] = arr[i];
//            arr[i] = tmp;
//        }
//}
/** Сортировка пузырьком */


/** Сортировка склейкой */
//void Merge(unsigned int *a, unsigned int n, unsigned int* b, unsigned int m, unsigned int* c) {
//    int i = 0;
//    int j = 0;
//    // Пока один из массивов не пуст
//    while (i < n && j < m) {
//        // Выбираем наименьший элемент и добавляем в массив-результат
//        if (a[i] <= b[j]) {
//            c[i + j] = a[i];
//            ++i;
//        } else {
//            c[i + j] = b[j];
//            ++j;
//        }
//    }
//    // Копируем остаток первого массива в конец массива-результата
//    if (i < n) memmove(&c[i + j], &a[i], (n - i) * 4);
//    // Копируем остаток второго массива в конец массива-результата
//    if (j < m) memmove(&c[i + j], &b[j], (m - j) * 4);
//}
//
//void Sort(unsigned int* arr, unsigned int size) {
//    if( size <= 1 ) {
//        return;
//    }
//    unsigned int mid = size / 2;
//    // Сортируем левую часть
//    Sort(arr, mid);
//    // Сортируем правую часть
//    Sort(arr + mid, size - mid);
//    unsigned int* c = new unsigned int[size];
//    // Склеиваем
//    Merge( arr, mid, arr + mid, size - mid, c);
//    memmove(arr, c, size * 4);
//    delete[] c;
//}
/** Сортировка склейкой */


/** Быстрая сортировка */
// Стратегия выбора опорного элемента “медиана трех”
int SelectReference(unsigned int* arr, unsigned int n) {
    unsigned int a = arr[0],
        b = arr[n-1],
        c = arr[n / 2];
    if (a < b) {
        if (b < c)
            return n-1;
        else if (a < c)
            return n / 2;
        else
            return 0;
    } else {
        if (b > c)
            return n-1;
        else if (a < c)
            return 0;
        else
            return n / 2;
    }
}

void Sort(unsigned int* arr, unsigned int size) {
    while (1) {
        if (size == 0)
            return;
        if (size == 1)
            return;
        if (size == 2) {
            if (arr[0] > arr[1]) std::swap(arr[0], arr[1]);
            return;
        }
        // Получаем опорный элемент
        int refIndex = SelectReference(arr, size),
                ref = arr[refIndex];
        // Меняем местами последний и опорный элементы
        arr[refIndex] = arr[size - 1];
        arr[size - 1] = ref;
        // Реализация методом прохода двумя итераторами от начала массива к концу.
        int i = 0;
        for (int j = 0; j < size - 1; ++j) {
            // Если j-й элемент небольше опорного, меняем с i-м и инкрементируем i
            if (arr[j] <= ref) {
                if (i != j) std::swap(arr[j], arr[i]);
                ++i;
            }
        }
        // Меняем местами i-й и опорный элементы
        std::swap(arr[i], arr[size - 1]);
        // Сортируем правую часть
        if (i < size - 1) {
            Sort(&arr[i + 1], size - i - 1);
        }
        // А левую сортируем на месте
        size = i;
    }
}
/** Быстрая сортировка */