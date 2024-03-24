#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctime>
#include <map>
#include <unordered_map>
#include <set>

using namespace std;

// Структура для представления записи о браке
struct MarriageRecord {
    string groomName;
    string groomDate;
    string brideName;
    string brideDate;
    string marriageDate;
    string registryNumber;

    // Операторы сравнения для сортировки
    bool operator == (const MarriageRecord& MarRec) const {
        return this->registryNumber == MarRec.registryNumber && this->marriageDate == MarRec.marriageDate && this->groomName == MarRec.groomName;
    }

    bool operator < (const MarriageRecord& MarRec) const {
        if (this->registryNumber == MarRec.registryNumber)
            if (this->marriageDate == MarRec.marriageDate)
                return this->groomName < MarRec.groomName;
            else return this->marriageDate < MarRec.marriageDate;
        else return this->registryNumber < MarRec.registryNumber;
    }
};

// Хэш-функция для структуры MarriageRecord
struct MarriageRecordHash {
    size_t operator()(const MarriageRecord& record) const {
        return hash<string>()(record.groomName) ^ hash<string>()(record.groomDate) ^ hash<string>()(record.brideName) ^
            hash<string>()(record.brideDate) ^ hash<string>()(record.marriageDate) ^ hash<string>()(record.registryNumber);
    }
};

// Функция для считывания данных из файла
vector<MarriageRecord> readDataFromFile(string file, int size) {
    ifstream inputFile(file);
    vector<MarriageRecord> result(size);
    for (int i = 0; i < size; i++) {
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

// Бинарное дерево поиска
class BinarySearchTree {
private:
    struct Node {
        MarriageRecord data;
        Node* left;
        Node* right;
        Node(const MarriageRecord& item) : data(item), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void insert(Node*& node, const MarriageRecord& item) {
        if (!node) {
            node = new Node(item);
            return;
        }

        if (item < node->data)
            insert(node->left, item);
        else
            insert(node->right, item);
    }

    bool search(Node* node, const string& key, MarriageRecord& result) const {
        if (!node)
            return false;

        if (node->data.groomName == key) {
            result = node->data;
            return true;
        }

        if (key < node->data.groomName)
            return search(node->left, key, result);
        else
            return search(node->right, key, result);
    }

public:
    BinarySearchTree() : root(nullptr) {}

    void insert(const MarriageRecord& item) {
        insert(root, item);
    }

    bool search(const string& key, MarriageRecord& result) const {
        return search(root, key, result);
    }
};

// Хэш-таблица
class HashMap {
private:
    vector<list<MarriageRecord>> table;
    int size;

    int hashFunction(const string& key) const {
        int hash = 0;
        for (char c : key)
            hash = (hash * 31 + c) % table.size();
        return hash;
    }

public:
    HashMap(int initialSize) : size(initialSize) {
        table.resize(size);
    }

    void insert(const MarriageRecord& record) {
        int index = hashFunction(record.groomName);
        table[index].push_back(record);
    }

    bool search(const string& key, MarriageRecord& result) const {
        int index = hashFunction(key);
        for (const auto& record : table[index]) {
            if (record.groomName == key) {
                result = record;
                return true;
            }
        }
        return false;
    }
};

// Функция для выполнения поиска по всем методам
void performSearch(const vector<MarriageRecord>& records, const set<string>& keys) {
    BinarySearchTree bst;
    HashMap hashmap(100); // Устанавливаем начальный размер хэш-таблицы

    for (const auto& record : records) {
        bst.insert(record);
        hashmap.insert(record);
    }

    clock_t bst_start = clock();
    for (const auto& key : keys) {
        MarriageRecord result;
        bst.search(key, result);
    }
    cout << "Time taken for Binary Search Tree: " << static_cast<double>(clock() - bst_start) / CLOCKS_PER_SEC << " seconds" << endl;

    cout << "Hash Map:" << endl;
    clock_t hashmap_start = clock();
    for (const auto& key : keys) {
        MarriageRecord result;
        hashmap.search(key, result);
    }
    cout << "Time taken for Hash Map: " << static_cast<double>(clock() - hashmap_start) / CLOCKS_PER_SEC << " seconds" << endl;
}

// Функция для выполнения поиска с использованием красно-черного дерева
void performRedBlackTreeSearch(const vector<MarriageRecord>& records, const set<string>& keys) {
    map<string, MarriageRecord> red_black_tree;

    // Заполняем красно-черное дерево
    for (const auto& record : records) {
        red_black_tree.emplace(record.groomName, record);
    }

    clock_t rbtree_start = clock();
    for (const auto& key : keys) {
        red_black_tree.find(key);
    }
    cout << "Time taken for Red-Black Tree (std::map): " << static_cast<double>(clock() - rbtree_start) / CLOCKS_PER_SEC << " seconds" << endl;
}

// Функция для выполнения поиска по всем методам, включая красно-черное дерево
void performAllSearches(const vector<MarriageRecord>& records, const set<string>& keys) {
    BinarySearchTree bst;
    clock_t bst_start = clock();
    for (const auto& record : records) {
        bst.insert(record);
    }
    for (const auto& key : keys) {
        MarriageRecord result;
        bst.search(key, result);
    }
    cout << "Time taken for Binary Search Tree: " << static_cast<double>(clock() - bst_start) / CLOCKS_PER_SEC << " seconds" << endl;

    HashMap hashmap(100);
    clock_t hashmap_start = clock();
    for (const auto& record : records) {
        hashmap.insert(record);
    }
    for (const auto& key : keys) {
        MarriageRecord result;
        hashmap.search(key, result);
    }
    cout << "Time taken for Hash Map: " << static_cast<double>(clock() - hashmap_start) / CLOCKS_PER_SEC << " seconds" << endl;

    performRedBlackTreeSearch(records, keys);
}

// Функция для выполнения поиска в мультимапе
void performMultimapSearch(const multimap<string, MarriageRecord>& records, const set<string>& keys) {
    cout << "Multimap Search:" << endl;
    clock_t multimap_start = clock();
    for (const auto& key : keys) {
        auto range = records.equal_range(key);
    }
    cout << "Time taken for Multimap Search: " << static_cast<double>(clock() - multimap_start) / CLOCKS_PER_SEC << " seconds" << endl;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int sizes[7] = { 100, 500, 1000, 2000, 8000, 10000, 100000 };

    for (auto size : sizes) {
        char buffer[50];
        sprintf_s(buffer, "input_%d.txt", size);
        vector<MarriageRecord> records_vector = readDataFromFile(buffer, size);
        multimap<string, MarriageRecord> records_multimap;

        // Заполняем мультимапу
        for (const auto& record : records_vector) {
            records_multimap.emplace(record.groomName, record);
        }
        
        set<string> keys; // Для хранения уникальных ключей

        // Заполняем ключи
        for (const auto& record : records_vector) {
            keys.insert(record.groomName);
        }

        cout << "Performing search for size: " << size << endl;
        performAllSearches(records_vector, keys);
        performMultimapSearch(records_multimap, keys); // Добавляем поиск в мультимапу
        cout << endl;
    }
    return 0;
}