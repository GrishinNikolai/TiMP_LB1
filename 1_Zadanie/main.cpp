#include <iostream>
#include <locale>
#include <codecvt>
#include "modAlphaCipher.h"

using namespace std;

void check(const wstring& text, const wstring& key)
{
    modAlphaCipher cipher(key);
    wstring encrypted = cipher.encrypt(text);
    wstring decrypted = cipher.decrypt(encrypted);

    wcout << L"Ключ: " << key << endl;
    wcout << L"Исходный текст: " << text << endl;
    wcout << L"Зашифрованный текст: " << encrypted << endl;
    wcout << L"Расшифрованный текст: " << decrypted << endl;

    if (text == decrypted)
        wcout << L"OK" << endl;
    else
        wcout << L"Ошибка!" << endl;
    wcout << endl;
}

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    locale loc("ru_RU.UTF-8");
    wcout.imbue(loc);
    wcin.imbue(loc);

    check(L"ПРИВЕТМИР", L"КЛЮЧ");
    check(L"ШИФРОВАНИЕ", L"ПАРОЛЬ");
    check(L"ПРОГРАММИРОВАНИЕ", L"КОД");

    return 0;
}
