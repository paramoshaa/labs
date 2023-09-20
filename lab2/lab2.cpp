//Вариант 10. Сгенерировать массив из 300 строк, содержащих случайные 200000 цифр.
//Посчитать число вхождений цифры "9" в каждой строке, используя N потоков.
//Измерить время работы программы для N = 1, 2, 4, 10. Измерить время работы
//программы в каждом случае.

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>
#include <chrono>

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

// Функция, которую выполняют потоки
void CountNinesInStrings(int start, int end, const std::vector<string>& strings, int& result) {
    result = 0;
    for (int i = start; i < end; ++i) {
        result += CountOccurrences(strings[i], target_digit);
    }
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
        vector<thread> threads(num_threads);
        vector<int> thread_results(num_threads, 0);

        auto start_time = chrono::high_resolution_clock::now();

        int strings_per_thread = num_strings / num_threads;
        for (int i = 0; i < num_threads; ++i) {
            int start_index = i * strings_per_thread;
            int end_index = (i == num_threads - 1) ? num_strings : (i + 1) * strings_per_thread;
            threads[i] = thread(CountNinesInStrings, start_index, end_index, ref(data), ref(thread_results[i]));
        }

        for (int i = 0; i < num_threads; ++i) {
            threads[i].join();
        }

        int total_occurrences = 0;
        for (int i : thread_results) {
            total_occurrences += i;
        }

        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);

        cout << "Для " << num_threads << " потоков: " << total_occurrences << " раз. Время работы программы: " << duration.count() << " милли сек." << endl;
    }

    return 0;
}