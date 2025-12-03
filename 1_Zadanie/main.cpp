/**
 * @file main.cpp
 * @author Гришин Н.С.
 * @version 1.0
 * @date 03.12.2025
 * @copyright ИБСТ ПГУ
 * @brief Главный модуль для тестирования шифра Гронсфельда
 */

#include <iostream>
#include <locale>
#include <codecvt>
#include "modAlphaCipher.h"

using namespace std;

/**
 * @brief Функция для проверки работы шифра
 * @param text Текст для обработки
 * @param key Ключ шифрования
 * @param destructCipherText Флаг для порчи зашифрованного текста (для тестирования обработки ошибок)
 */
void check(const wstring& text, const wstring& key, bool destructCipherText = false)
{
    try {
        modAlphaCipher cipher(key);
        wstring encrypted = cipher.encrypt(text);

        // Порча текста для тестирования обработки ошибок
        if (destructCipherText && !encrypted.empty()) {
            encrypted[0] = tolower(encrypted[0], locale("ru_RU.UTF-8"));
        }

        wstring decrypted = cipher.decrypt(encrypted);

        wcout << L"Ключ: " << key << endl;
        wcout << L"Исходный текст: " << text << endl;
        wcout << L"Зашифрованный текст: " << encrypted << endl;
        wcout << L"Расшифрованный текст: " << decrypted << endl;
        wcout << endl;

    } catch (const cipher_error& e) {
        wcerr << L"Ошибка шифрования: " << e.what() << endl << endl;
    }
}

/**
 * @brief Функция тестирования шифра Гронсфельда
 * @details Выполняет серию тестов с различными входными данными
 */
void testAlphaCipher() {
    wcout << L"=== Тесты для шифра Гронсфельда ===" << endl;

    // Тест 1: Корректные данные
    check(L"ПРИВЕТМИР", L"КЛЮЧ");

    // Тест 2: Ключ с цифрами
    wcout << L"Тест с ключом 'КЛЮЧ123':" << endl;
    check(L"ПРИВЕТМИР", L"КЛЮЧ123");

    // Тест 3: Текст с цифрами
    wcout << L"Тест с текстом 'ПРИВЕТ123':" << endl;
    check(L"ПРИВЕТ123", L"КЛЮЧ");

    // Тест 4: Пустой ключ
    wcout << L"Тест с пустым ключом:" << endl;
    check(L"ПРИВЕТМИР", L"");

    // Тест 5: Текст с пробелами
    check(L"ПРИВЕТ МИР", L"КЛЮЧ");

    // Тест 6: Текст в другом регистре
    check(L"привет мир", L"ключ");

    wcout << L"=== Конец тестов для шифра Гронсфельда ===" << endl << endl;
}

/**
 * @brief Главная функция программы
 * @return 0 при успешном выполнении
 */
int main()
{
    // Настройка локали для работы с русскими символами
    setlocale(LC_ALL, "ru_RU.UTF-8");
    locale loc("ru_RU.UTF-8");
    wcout.imbue(loc);
    wcin.imbue(loc);

    // Запуск тестов
    testAlphaCipher();

    return 0;
}
