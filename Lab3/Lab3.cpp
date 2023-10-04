//Вариант 10. Сгенерировать массив из 300 строк, содержащих случайные 200000 цифр.
//Посчитать число вхождений цифры "9" в каждой строке, используя N потоков.
//Измерить время работы программы для N = 1, 2, 4, 10. Измерить время работы
//программы в каждом случае.


#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <omp.h>

using namespace std;

const int num_strings = 300;
const int string_length = 200000;
const char target_digit = '9';

// Генерация случайной строки заданной длины
string GenerateRandomString(int length) {
    string random_string;
    for (int i = 0; i < length; ++i) {
        random_string.push_back('0' + rand() % 10);
    }
    return random_string;
}

// Число вхождений цифры в строке
int CountOccurrences(const string& str, char digit) {
    int count = 0;
    for (char c : str) {
        if (c == digit) {
            count++;
        }
    }
    return count;
}

int main() {
    setlocale(LC_ALL, "ru");

    srand(time(NULL));

    vector<string> data(num_strings);

    // Генерация случайных строк
    for (int i = 0; i < num_strings; ++i) {
        data[i] = GenerateRandomString(string_length);
    }

    // Количество потоков для тестирования
    vector<int> thread_counts = { 1, 2, 4, 10 };

    for (int num_threads : thread_counts) {
        vector<int> thread_results(num_threads, 0);

        auto start_time = chrono::high_resolution_clock::now();

#pragma omp parallel num_threads(num_threads)
        {
            int thread_id = omp_get_thread_num();
            int start_index = thread_id * (num_strings / num_threads);
            int end_index = (thread_id == num_threads - 1) ? num_strings : (thread_id + 1) * (num_strings / num_threads);

            int local_count = 0;
            for (int i = start_index; i < end_index; ++i) {
                local_count += CountOccurrences(data[i], target_digit);
            }

#pragma omp critical
            {
                thread_results[thread_id] = local_count;
            }
        }

        int total_occurrences = 0;
        for (int i : thread_results) {
            total_occurrences += i;
        }

        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

        cout << "Для " << num_threads << " потоков: " << total_occurrences << " раз. Время работы программы: " << duration.count() << " миллисекунд." << endl;
    }

    return 0;
}
