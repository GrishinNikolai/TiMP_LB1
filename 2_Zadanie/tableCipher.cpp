/**
 * @file tableCipher.cpp
 * @author Гришин Н.С.
 * @version 1.0
 * @date 03.12.2025
 * @copyright ИБСТ ПГУ
 * @brief Реализация модуля шифрования методом табличной маршрутной перестановки
 */

#include "tableCipher.h"
#include <algorithm>
#include <sstream>
#include <string>

/**
 * @brief Валидация ключа
 * @param k Проверяемый ключ
 * @throw tableCipher_error Если ключ невалиден
 */
void tableCipher::validateKey(int k) {
    if (k <= 0) {
        throw tableCipher_error("Неверный ключ: Ключ должен быть положительным числом.");
    }
    if (k == 1 || k == 2) {
        throw tableCipher_error("Неверный ключ: ключ не может быть 1 или 2 (слишком слабый для шифрования)");
    }
}

/**
 * @brief Валидация длины текста относительно ключа
 * @param text Проверяемый текст
 * @param operation Название операции (для сообщения об ошибке)
 * @throw tableCipher_error Если длина текста недостаточна для операции
 */
void tableCipher::validateTextLength(const std::wstring& text, const std::string& operation) {
    if (text.length() <= key) {
        throw tableCipher_error(
            "Длина текста должна быть больше ключа для" + operation +
            ". Длина текста: " + std::to_string(text.length()) +
            ", ключ: " + std::to_string(key)
        );
    }
}

/**
 * @brief Конструктор класса tableCipher
 * @param k Ключ шифрования (количество столбцов)
 * @throw tableCipher_error Если ключ невалиден
 */
tableCipher::tableCipher(int k)
{
    validateKey(k);
    key = k;
}

/**
 * @brief Метод зашифровывания
 * @param open_text Открытый текст для шифрования
 * @return Зашифрованная строка
 * @throw tableCipher_error Если текст пустой или недостаточной длины
 */
std::wstring tableCipher::encrypt(const std::wstring& open_text)
{
    std::wstring text = prepareText(open_text);

    if (text.empty()) {
        throw tableCipher_error("Пустой текст для шифрования");
    }

    // Проверяем длину текста для шифрования
    validateTextLength(text, "encryption");

    int text_len = text.length();
    int rows = (text_len + key - 1) / key;

    // Создаем таблицу
    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(key, L' '));

    // Заполняем таблицу (слева направо, сверху вниз)
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (index < text_len) {
                table[i][j] = text[index++];
            }
        }
    }

    // Считываем таблицу (сверху вниз, справа налево)
    std::wstring result;
    for (int j = key - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            if (table[i][j] != L' ')
                result += table[i][j];
        }
    }

    return result;
}

/**
 * @brief Метод расшифровывания
 * @param cipher_text Зашифрованный текст для расшифрования
 * @return Расшифрованная строка
 * @throw tableCipher_error Если текст пустой или недостаточной длины
 */
std::wstring tableCipher::decrypt(const std::wstring& cipher_text)
{
    std::wstring text = prepareText(cipher_text);

    if (text.empty()) {
        throw tableCipher_error("Пустой текст для расшифровки");
    }

    // Проверяем длину текста для расшифрования
    validateTextLength(text, "decryption");

    int text_len = text.length();
    int rows = (text_len + key - 1) / key;

    int total_cells = rows * key;
    int empty_cells = total_cells - text_len;

    // Добавляем пробелы в конец текста, чтобы заполнить пустые ячейки
    if (empty_cells > 0) {
        for(int i = 1; i <= empty_cells; i++) {
            text.insert((i*(rows - 1)+(i-1)), L" ");
        }
    }

    // Создаем таблицу для заполнения при чтении
    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(key, L' '));

    // Заполняем таблицу при чтении (сверху вниз, справа налево)
    int index = 0;
    for (int j = key - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            if (index < text.length()) {
                table[i][j] = text[index++];
            }
        }
    }

    // Читаем таблицу (слева направо, сверху вниз)
    std::wstring result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            result += table[i][j];
        }
    }

    // Удаляем добавленные пробелы из результата
    size_t pos = result.find_last_not_of(L' ');
    if (pos != std::wstring::npos) {
        result.erase(pos + 1);
    } else {
        result.clear();
    }

    return result;
}

/**
 * @brief Приведение строки к верхнему регистру
 * @param s Входная строка
 * @return Строка в верхнем регистре
 */
std::wstring tableCipher::toUpper(const std::wstring& s)
{
    std::wstring result = s;
    std::locale loc("ru_RU.UTF-8");
    for (wchar_t& c : result) {
        c = std::toupper(c, loc);
    }
    return result;
}

/**
 * @brief Проверка текста на соответствие русскому алфавиту
 * @param text Проверяемый текст
 * @return true если текст содержит только русские буквы и пробелы, иначе false
 */
bool tableCipher::isValidRussianText(const std::wstring& text)
{
    for (wchar_t c : text) {
        if (c != L' ' && !(c >= L'А' && c <= L'Я') && !(c >= L'а' && c <= L'я') && c != L'Ё' && c != L'ё') {
            return false;
        }
    }
    return true;
}

/**
 * @brief Подготовка текста к шифрованию
 * @param s Исходный текст
 * @return Текст в верхнем регистре без пробелов
 * @throw tableCipher_error Если текст пустой, содержит недопустимые символы или только пробелы
 */
std::wstring tableCipher::prepareText(const std::wstring& s)
{
    if (s.empty()) {
        throw tableCipher_error("Пустой вводимый текст");
    }

    if (!isValidRussianText(s)) {
        throw tableCipher_error("Текст содержит недопустимые символы. Допускаются только русские буквы и пробелы.");
    }

    // Удаляем пробелы и приводим к верхнему регистру
    std::wstring result;
    std::locale loc("ru_RU.UTF-8");

    for (wchar_t c : s) {
        if (c != L' ') {
            result += std::toupper(c, loc);
        }
    }

    if (result.empty()) {
        throw tableCipher_error("Текст содержит только пробелы");
    }

    return result;
}
