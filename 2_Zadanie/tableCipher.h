#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <locale>
#include <codecvt>

class tableCipher
{
private:
    int key; // количество столбцов таблицы
    std::wstring toUpper(const std::wstring& s); // приведение к верхнему регистру
public:
    tableCipher() = delete;
    tableCipher(int k);
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};