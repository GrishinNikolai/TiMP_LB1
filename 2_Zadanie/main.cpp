#include <iostream>
#include <string>
#include <limits>
#include <locale>
#include <codecvt>
#include "tableCipher.h"

using namespace std;

// Функция для проверки корректности ввода числа
int getValidKey() {
    int key;
    while (true) {
        wcout << L"Введите ключ (количество столбцов): ";
        wcin >> key;

        if (wcin.fail() || key <= 0) {
            wcout << L"Ошибка: ключ должен быть положительным целым числом!" << endl;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        } else {
            break;
        }
    }
    return key;
}

// Функция для получения текста от пользователя
wstring getText(const wstring& prompt) {
    wstring text;
    wcout << prompt;
    wcin.ignore(); // Игнорируем предыдущий символ новой строки
    getline(wcin, text);
    return text;
}

// Функция проверки, что строка содержит только русские буквы и пробелы
bool isValidRussianText(const wstring& text) {
    for (wchar_t c : text) {
        if (c != L' ' && !(c >= L'А' && c <= L'Я') && !(c >= L'а' && c <= L'я') && c != L'Ё' && c != L'ё') {
            return false;
        }
    }
    return true;
}

// Функция для удаления пробелов из текста
wstring removeSpaces(const wstring& text) {
    wstring result;
    for (wchar_t c : text) {
        if (c != L' ') {
            result += c;
        }
    }
    return result;
}

int main() {
    // Устанавливаем локаль для русского языка
    setlocale(LC_ALL, "ru_RU.UTF-8");
    locale loc("ru_RU.UTF-8");
    wcout.imbue(loc);
    wcin.imbue(loc);

    try {
        // Ввод и проверка ключа
        int key = getValidKey();
        tableCipher cipher(key);

        wcout << L"Шифр табличной маршрутной перестановки готов к работе!" << endl;
        wcout << L"Ключ: " << key << L" столбцов" << endl << endl;

        int operation;
        do {
            wcout << L"Выберите операцию:" << endl;
            wcout << L"0 - Выход" << endl;
            wcout << L"1 - Зашифровать" << endl;
            wcout << L"2 - Расшифровать" << endl;
            wcout << L"Ваш выбор: ";
            wcin >> operation;

            switch (operation) {
                case 0:
                    wcout << L"Выход из программы..." << endl;
                    break;

                case 1: {
                    wstring text = getText(L"Введите текст для шифрования: ");
                    if (!text.empty()) {
                        if (isValidRussianText(text)) {
                            // Удаляем пробелы перед шифрованием
                            wstring textWithoutSpaces = removeSpaces(text);
                            if (text != textWithoutSpaces) {
                                wcout << L"Текст без пробелов: " << textWithoutSpaces << endl;
                            }
                            wstring encrypted = cipher.encrypt(textWithoutSpaces);
                            wcout << L"Зашифрованный текст: " << encrypted << endl;
                        } else {
                            wcout << L"Ошибка: текст должен содержать только русские буквы и пробелы!" << endl;
                        }
                    } else {
                        wcout << L"Ошибка: введен пустой текст!" << endl;
                    }
                    break;
                }

                case 2: {
                    wstring text = getText(L"Введите текст для расшифрования: ");
                    if (!text.empty()) {
                        if (isValidRussianText(text)) {
                            // Удаляем пробелы перед расшифрованием
                            wstring textWithoutSpaces = removeSpaces(text);
                            if (text != textWithoutSpaces) {
                                wcout << L"Текст без пробелов: " << textWithoutSpaces << endl;
                            }
                            wstring decrypted = cipher.decrypt(textWithoutSpaces);
                            wcout << L"Расшифрованный текст: " << decrypted << endl;
                        } else {
                            wcout << L"Ошибка: текст должен содержать только русские буквы и пробелы!" << endl;
                        }
                    } else {
                        wcout << L"Ошибка: введен пустой текст!" << endl;
                    }
                    break;
                }

                default:
                    wcout << L"Ошибка: неверная операция!" << endl;
                    break;
            }
            wcout << endl;

        } while (operation != 0);

    } catch (const exception& e) {
        wcerr << L"Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
