#pragma once
#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>

class modAlphaCipher
{
private:
    std::wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; // русский алфавит
    std::map<wchar_t, int> alphaNum; // ассоциативный массив "символ -> номер"
    std::vector<int> key; // ключ в числовом виде
    std::vector<int> convert(const std::wstring& s); // преобразование строки в вектор чисел
    std::wstring convert(const std::vector<int>& v); // преобразование вектора чисел в строку
    std::wstring toUpper(const std::wstring& s); // приведение к верхнему регистру
public:
    modAlphaCipher() = delete;
    modAlphaCipher(const std::wstring& skey); // ключ передаётся как wstring
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};
