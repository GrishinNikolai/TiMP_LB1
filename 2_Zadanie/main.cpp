#include <iostream>
#include <string>
#include <limits>
#include <locale>
#include <codecvt>
#include "tableCipher.h"

using namespace std;

// Функция для конвертации string в wstring
std::wstring string_to_wstring(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
}

int getKey() {
    int key;
    while (true) {
        wcout << L"Введите ключ (количество столбцов): ";
        wcin >> key;

        if (wcin.fail()) {
            wcout << L"Ошибка: ключ должен быть числом!" << endl;
            wcin.clear();
            wcin.ignore(numeric_limits<streamsize>::max(), L'\n');
        } else {
            break;
        }
    }
    return key;
}

wstring getText(const wstring& prompt) {
    wstring text;
    wcout << prompt;
    wcin.ignore();
    getline(wcin, text);
    return text;
}

int main() {
    // Устанавливаем локаль для корректного отображения русских символов
    setlocale(LC_ALL, "ru_RU.UTF-8");
    locale loc("ru_RU.UTF-8");
    wcout.imbue(loc);
    wcin.imbue(loc);

    try {
        int key = getKey();
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
                    try {
                        wstring encrypted = cipher.encrypt(text);
                        wcout << L"Зашифрованный текст: " << encrypted << endl;
                    } catch (const tableCipher_error& e) {
                        wstring error_msg = string_to_wstring(e.what());
                        wcerr << L"Ошибка шифрования: " << error_msg << endl;
                    } catch (const exception& e) {
                        wstring error_msg = string_to_wstring(e.what());
                        wcerr << L"Неизвестная ошибка: " << error_msg << endl;
                    }
                    break;
                }

                case 2: {
                    wstring text = getText(L"Введите текст для расшифрования: ");
                    try {
                        wstring decrypted = cipher.decrypt(text);
                        wcout << L"Расшифрованный текст: " << decrypted << endl;
                    } catch (const tableCipher_error& e) {
                        wstring error_msg = string_to_wstring(e.what());
                        wcerr << L"Ошибка расшифрования: " << error_msg << endl;
                    } catch (const exception& e) {
                        wstring error_msg = string_to_wstring(e.what());
                        wcerr << L"Неизвестная ошибка: " << error_msg << endl;
                    }
                    break;
                }

                default:
                    wcout << L"Ошибка: неверная операция!" << endl;
                    break;
            }
            wcout << endl;

        } while (operation != 0);

    } catch (const tableCipher_error& e) {
        wstring error_msg = string_to_wstring(e.what());
        wcerr << L"Ошибка создания шифратора: " << error_msg << endl;
        return 1;
    } catch (const exception& e) {
        wstring error_msg = string_to_wstring(e.what());
        wcerr << L"Критическая ошибка: " << error_msg << endl;
        return 1;
    }

    return 0;
}
