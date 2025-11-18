#include <iostream>
#include <locale>
#include <codecvt>
#include "modAlphaCipher.h"

using namespace std;

void check(const wstring& text, const wstring& key, bool destructCipherText = false)
{
    try {
        modAlphaCipher cipher(key);
        wstring encrypted = cipher.encrypt(text);

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

void testAlphaCipher() {
    wcout << L"=== Тесты для шифра Гронсфельда ===" << endl;

    // Тест 1: Корректные данные
    check(L"ПРИВЕТМИР", L"КЛЮЧ");

    // Тест 2: Ключ с цифрами (должна быть ошибка)
    wcout << L"Тест с ключом 'КЛЮЧ123':" << endl;
    check(L"ПРИВЕТМИР", L"КЛЮЧ123");

    // Тест 3: Текст с цифрами (должна быть ошибка)
    wcout << L"Тест с текстом 'ПРИВЕТ123':" << endl;
    check(L"ПРИВЕТ123", L"КЛЮЧ");

    // Тест 4: Пустой ключ (должна быть ошибка)
    wcout << L"Тест с пустым ключом:" << endl;
    check(L"ПРИВЕТМИР", L"");

    // Тест 5: Текст с пробелами (должен обработаться)
    check(L"ПРИВЕТ МИР", L"КЛЮЧ");

    // Тест 6: Текст в разном регистре (должен обработаться)
    check(L"привет мир", L"ключ");

    wcout << L"=== Конец тестов для шифра Гронсфельда ===" << endl << endl;
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    locale loc("ru_RU.UTF-8");
    wcout.imbue(loc);
    wcin.imbue(loc);

    testAlphaCipher();

    return 0;
}
