/**
 * @file modAlphaCipher.cpp
 * @author Гришин Н.С.
 * @version 1.0
 * @date 03.12.2025
 * @copyright ИБСТ ПГУ
 * @brief Реализация модуля шифрования методом Гронсфельда
 */

#include "modAlphaCipher.h"
#include <algorithm>

/**
 * @brief Конструктор класса modAlphaCipher
 * @param skey Ключ шифрования в виде строки
 * @throw cipher_error Если ключ невалиден
 */
modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
    // Инициализация алфавита и ассоциативного массива
    for (size_t i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    // Валидация и установка ключа
    key = convert(getValidKey(skey));
}

/**
 * @brief Метод зашифровывания открытого текста
 * @param open_text Открытый текст для шифрования
 * @return Зашифрованная строка
 * @throw cipher_error Если текст пустой или не содержит русских букв
 */
std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    std::wstring text = getValidOpenText(open_text);
    std::vector<int> work = convert(text);
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

/**
 * @brief Метод расшифровывания зашифрованного текста
 * @param cipher_text Зашифрованный текст для расшифрования
 * @return Расшифрованная строка
 * @throw cipher_error Если текст пустой или содержит недопустимые символы
 */
std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    std::wstring text = getValidCipherText(cipher_text);
    std::vector<int> work = convert(text);
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

/**
 * @brief Преобразование строки в числовой вектор
 * @param s Входная строка
 * @return Вектор чисел, соответствующих символам строки
 */
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

/**
 * @brief Преобразование числового вектора в строку
 * @param v Входной вектор чисел
 * @return Строка, составленная из символов алфавита
 */
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

/**
 * @brief Приведение строки к верхнему регистру с удалением пробелов
 * @param s Входная строка
 * @return Строка в верхнем регистре без пробелов
 */
std::wstring modAlphaCipher::toUpper(const std::wstring& s)
{
    std::wstring result;
    std::locale loc("ru_RU.UTF-8");

    for (wchar_t c : s) {
        if (c != L' ') {
            wchar_t upper_c = std::toupper(c, loc);
            result.push_back(upper_c);
        }
    }
    return result;
}

/**
 * @brief Валидация и нормализация ключа
 * @param s Ключ в виде строки
 * @return Валидированный ключ в верхнем регистре
 * @throw cipher_error Если ключ пустой, содержит недопустимые символы или слишком слабый
 */
std::wstring modAlphaCipher::getValidKey(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Empty key");
    }

    std::wstring tmp;
    std::locale loc("ru_RU.UTF-8");

    // Приведение к верхнему регистру
    for (wchar_t c : s) {
        wchar_t upper_c = std::toupper(c, loc);
        tmp.push_back(upper_c);
    }

    if (tmp.empty()) {
        throw cipher_error("Invalid key - no valid characters");
    }

    // Проверка на русские буквы
    for (wchar_t c : tmp) {
        if (!((c >= L'А' && c <= L'Я') || c == L'Ё')) {
            throw cipher_error("Invalid key - contains non-Russian characters");
        }
    }

    // Проверка на слабый ключ (слишком много нулевых сдвигов)
    int n = 0;
    for (auto e : tmp) {
        if (e == L'А' || e == L'Ё') {
            n++;
        }
    }

    if (2 * n > tmp.size()) {
        throw cipher_error("Weak key - too many zero-shift characters");
    }

    return tmp;
}

/**
 * @brief Валидация открытого текста
 * @param s Открытый текст
 * @return Валидированный текст в верхнем регистре
 * @throw cipher_error Если текст пустой или не содержит русских букв
 */
std::wstring modAlphaCipher::getValidOpenText(const std::wstring& s)
{
    std::wstring tmp = toUpper(s);
    if (tmp.empty()) {
        throw cipher_error("Empty open text");
    }

    // Проверка наличия русских букв
    bool hasValidChars = false;
    for (wchar_t c : tmp) {
        if ((c >= L'А' && c <= L'Я') || c == L'Ё') {
            hasValidChars = true;
            break;
        }
    }

    if (!hasValidChars) {
        throw cipher_error("Invalid open text - no Russian letters");
    }

    return tmp;
}

/**
 * @brief Валидация зашифрованного текста
 * @param s Зашифрованный текст
 * @return Валидированный зашифрованный текст
 * @throw cipher_error Если текст пустой или содержит недопустимые символы
 */
std::wstring modAlphaCipher::getValidCipherText(const std::wstring& s)
{
    if (s.empty()) {
        throw cipher_error("Empty cipher text");
    }

    // Проверка на русские буквы
    for (wchar_t c : s) {
        if (!((c >= L'А' && c <= L'Я') || c == L'Ё')) {
            throw cipher_error("Invalid cipher text - contains non-Russian characters");
        }
    }
    return s;
}
