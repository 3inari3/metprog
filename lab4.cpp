#include <iostream>
#include <string>
#include <map>
using namespace std;

class NumberInterpreter {
    map<int, string> units{
        {1, "один"}, {2, "два"}, {3, "три"}, {4, "четыре"}, {5, "пять"},
        {6, "шесть"}, {7, "семь"}, {8, "восемь"}, {9, "девять"}
    };

    map<int, string> teens{
        {10, "десять"}, {11, "одиннадцать"}, {12, "двенадцать"}, {13, "тринадцать"}, {14, "четырнадцать"},
        {15, "пятнадцать"}, {16, "шестнадцать"}, {17, "семнадцать"}, {18, "восемнадцать"}, {19, "девятнадцать"}
    };

    map<int, string> tens{
        {20, "двадцать"}, {30, "тридцать"}, {40, "сорок"}, {50, "пятьдесят"},
        {60, "шестьдесят"}, {70, "семьдесят"}, {80, "восемьдесят"}, {90, "девяносто"}
    };

    map<int, string> hundreds{
        {100, "сто"}, {200, "двести"}, {300, "триста"}, {400, "четыреста"},
        {500, "пятьсот"}, {600, "шестьсот"}, {700, "семьсот"}, {800, "восемьсот"}, {900, "девятьсот"}
    };

    string interpretHundreds(int number) {
        string result = "";
        if (number >= 100) {
            int h = number / 100 * 100;
            result += hundreds[h];
            number %= 100;
            if (number > 0) result += " ";
        }
        if (number >= 20) {
            int t = number / 10 * 10;
            result += tens[t];
            number %= 10;
            if (number > 0) result += " ";
        }
        if (number >= 10 && number <= 19) {
            result += teens[number];
            number = 0;
        }
        if (number >= 1 && number <= 9) {
            result += units[number];
        }
        return result;
    }

    string interpretThousands(int number) {
        string result = "";
        if (number >= 1000) {
            int t = number / 1000;
            int rem = number % 1000;

            if (t == 1) {
                result += "одна тысяча";
            }
            else if (t == 2) {
                result += "две тысячи";
            }
            else if (t >= 3 && t <= 4) {
                result += interpretHundreds(t) + " тысячи";
            }
            else {
                result += interpretHundreds(t) + " тысяч";
            }
            if (rem > 0) result += " " + interpretHundreds(rem);
        }
        else {
            result = interpretHundreds(number);
        }
        return result;
    }

    string interpretMillions(int number) {
        string result = "";
        if (number >= 1000000) {
            int m = number / 1000000;
            int rem = number % 1000000;

            if (m == 1) {
                result += "один миллион";
            }
            else if (m >= 2 && m <= 4) {
                result += interpretHundreds(m) + " миллиона";
            }
            else {
                result += interpretHundreds(m) + " миллионов";
            }
            if (rem > 0) result += " " + interpretThousands(rem);
        }
        else {
            result = interpretThousands(number);
        }
        return result;
    }

public:
    string interpret(int number) {
        if (number == 0) return "ноль";
        return interpretMillions(number);
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    NumberInterpreter interpreter;
    int number;
    cout << "Введите число: ";
    cin >> number;
    cout << "Число прописью: " << interpreter.interpret(number) << endl;
    return 0;
}