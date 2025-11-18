#include "modAlphaCipher.h"
#include <algorithm>

modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
    // Инициализируем алфавит правильно
    numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    for (size_t i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(getValidKey(skey));
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    std::wstring text = getValidOpenText(open_text);
    std::vector<int> work = convert(text);
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    std::wstring text = getValidCipherText(cipher_text);
    std::vector<int> work = convert(text);
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s)
{
    std::vector<int> result;
    for (wchar_t c : s) {
        if (alphaNum.find(c) != alphaNum.end()) {
            result.push_back(alphaNum[c]);
        }
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v)
{
    std::wstring result;
    for (int i : v) {
        if (i >= 0 && i < static_cast<int>(numAlpha.size())) {
            result.push_back(numAlpha[i]);
        }
    }
    return result;
}

std::wstring modAlphaCipher::toUpper(const std::wstring& s)
{
    std::wstring result;
    std::locale loc("ru_RU.UTF-8");

    for (wchar_t c : s) {
        if (c != L' ') { // Удаляем только пробелы
            wchar_t upper_c = std::toupper(c, loc);
            result.push_back(upper_c); // Добавляем ВСЕ символы в верхнем регистре
        }
    }
    return result;
}

std::wstring modAlphaCipher::getValidKey(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Empty key");
    }
    std::wstring tmp = toUpper(s);
    if (tmp.empty()) {
        throw cipher_error("Invalid key - no valid characters");
    }

    // Проверяем, что ключ содержит только русские буквы
    for (wchar_t c : tmp) {
        if (!((c >= L'А' && c <= L'Я') || c == L'Ё')) {
            throw cipher_error("Invalid key - contains non-Russian characters");
        }
    }

    return tmp;
}

std::wstring modAlphaCipher::getValidOpenText(const std::wstring& s)
{
    std::wstring tmp = toUpper(s);
    if (tmp.empty()) {
        throw cipher_error("Empty open text");
    }

    // Проверяем, что открытый текст содержит только русские буквы
    for (wchar_t c : tmp) {
        if (!((c >= L'А' && c <= L'Я') || c == L'Ё')) {
            throw cipher_error("Invalid open text - contains non-Russian characters");
        }
    }

    return tmp;
}

std::wstring modAlphaCipher::getValidCipherText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Empty cipher text");
    }

    // Проверяем, что зашифрованный текст содержит только русские буквы
    for (wchar_t c : s) {
        if (!((c >= L'А' && c <= L'Я') || c == L'Ё')) {
            throw cipher_error("Invalid cipher text - contains non-Russian characters");
        }
    }
    return s;
}
