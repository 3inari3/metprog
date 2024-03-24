#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>

using namespace std;

struct MarriageRecord {
    string groomName;
    string groomDate;
    string brideName;    
    string brideDate;
    string marriageDate;
    string registryNumber;


    bool operator == (const MarriageRecord& MarRec)
    {
        return this->registryNumber == MarRec.registryNumber && this->marriageDate == MarRec.marriageDate && this->groomName == MarRec.groomName;
    }

    bool operator < (const MarriageRecord & MarRec)
    {
        if (this->registryNumber == MarRec.registryNumber)
            if (this->marriageDate == MarRec.marriageDate)
                    return this->groomName < MarRec.groomName;
            else return this->marriageDate < MarRec.marriageDate;
        else return this->registryNumber < MarRec.registryNumber;
    }

    bool operator <= (const MarriageRecord& MarRec)
    {
        return *this < MarRec || *this == MarRec;
    }

    bool operator > (const MarriageRecord& MarRec)
    {
        return !(*this <= MarRec);
    }

    bool operator >= (const MarriageRecord& MarRec)
    {
        return !(*this < MarRec);
    }

};

    ostream& operator<<(ostream& os, const MarriageRecord& MarRec)
    {
        os << MarRec.registryNumber << ' ' << MarRec.marriageDate << ' ' << MarRec.groomName;
        return os;
    }

// Функция для считывания данных из файла
vector<MarriageRecord> readDataFromFile(string file, int size)
{
    ifstream inputFile(file);
    vector<MarriageRecord> result(size);
    for (int i = 0; i < size; i++)
    {
        inputFile >> result[i].groomName;
        inputFile >> result[i].groomDate;
        inputFile >> result[i].brideName;
        inputFile >> result[i].brideDate;
        inputFile >> result[i].marriageDate;
        inputFile >> result[i].registryNumber;
    }
    inputFile.close();
    return result;
}

// Сортировка выбором n^2
void selectionSort(vector<MarriageRecord>& records) {
    for (size_t i = 0; i < records.size() - 1; ++i) {
        size_t minIndex = i;

        for (size_t j = i + 1; j < records.size(); ++j) {
            if (records[j] < records[minIndex]) {
                minIndex = j;
            }
        }

        if (minIndex != i) {
            swap(records[i], records[minIndex]);
        }
    }
}

// Шейкер-сортировка n^2
void shakerSort(std::vector<MarriageRecord>& records) {
    int left = 0;
    int right = records.size() - 1;
    bool swapped;
    do {
        // Проход слева направо
        swapped = false;
        for (int i = left; i < right; ++i) {
            if (records[i] > records[i + 1]) {
                std::swap(records[i], records[i + 1]);
                swapped = true;
            }
        }

        // Если не было обменов, массив уже отсортирован
        if (!swapped) {
            break;
        }

        // Уменьшаем правую границу, так как самый большой элемент уже находится на правильной позиции
        --right;

        // Проход справа налево
        swapped = false;
        for (int i = right; i > left; --i) {
            if (records[i - 1] > records[i]) {
                std::swap(records[i - 1], records[i]);
                swapped = true;
            }
        }

        // Увеличиваем левую границу, так как самый маленький элемент уже находится на правильной позиции
        ++left;
    } while (swapped);
}

// Быстрая сортировка
void quickSort(vector<MarriageRecord>& records, int start, int end)  // n log n
{
    int l = start;
    int r = end;
    MarriageRecord pivot = records[(l + r) / 2];

    while (l <= r)
    {
        while (records[l] < pivot)
            l++;
        while (records[r] > pivot)
            r--;
        if (l <= r)
            swap(records[l++], records[r--]);
    }
    if (start < r)
        quickSort(records, start, r);
    if (end > l)
        quickSort(records, l, end);
}

int main()
{
    setlocale(LC_ALL, "Russian");
    int sizes[7] {100, 500, 1000, 2000, 8000, 10000, 100000};
    for (auto i : sizes)
    {
        char buffer[50];
        sprintf_s(buffer, "input_%d.txt", i);
        vector<MarriageRecord> record1 = readDataFromFile(buffer, i);
        vector<MarriageRecord> record2 = readDataFromFile(buffer, i);
        vector<MarriageRecord> record3 = readDataFromFile(buffer, i);


        clock_t start_time1 = clock();
        selectionSort(record1);
        clock_t end_time1 = clock();
        cout << "Selection sort " << i << ": " << static_cast<double>(end_time1 - start_time1) / CLOCKS_PER_SEC << endl;

        clock_t start_time2 = clock();
        quickSort(record2, 0, record2.size() - 1);
        clock_t end_time2 = clock();
        cout << "Quick sort " << i << ": " << static_cast<double>(end_time2 - start_time2) / CLOCKS_PER_SEC << endl;

        clock_t start_time3 = clock();
        shakerSort(record3);
        clock_t end_time3 = clock();
        cout << "Shaker Sort " << i << ": " << static_cast<double>(end_time3 - start_time3) / CLOCKS_PER_SEC << endl;

        cout << endl;
        sprintf_s(buffer, "output_%d.txt", i);
        ofstream outputFile(buffer);
        for (int j = 0; j < record1.size(); ++j)
            outputFile << record2[j] << endl;
        outputFile.close();
    }
}
