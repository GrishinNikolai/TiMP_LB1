/**
 * @file tableCipher.h
 * @author Гришин Н.С,
 * @version 1.0
 * @date 03.12.2025
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля шифрования методом табличной маршрутной перестановки
 */

#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <locale>
#include <codecvt>

/**
 * @brief Класс-исключение для ошибок шифра табличной перестановки
 */
class tableCipher_error : public std::runtime_error {
private:
    std::string message; ///< Сообщение об ошибке
public:
    /**
     * @brief Конструктор с параметром типа std::string
     * @param what_arg Сообщение об ошибке
     */
    explicit tableCipher_error(const std::string& what_arg) : std::runtime_error(what_arg), message(what_arg) {}

    /**
     * @brief Конструктор с параметром типа const char*
     * @param what_arg Сообщение об ошибке
     */
    explicit tableCipher_error(const char* what_arg) : std::runtime_error(what_arg), message(what_arg) {}

    /**
     * @brief Получение сообщения об ошибке
     * @return Сообщение об ошибке в виде C-строки
     */
    const char* what() const noexcept override { return message.c_str(); }
};

/**
 * @brief Класс для шифрования методом табличной маршрутной перестановки
 * @details Реализует табличную маршрутную перестановку для русского алфавита.
 *          Маршрут записи: по горизонтали слева направо, сверху вниз.
 *          Маршрут считывания: сверху вниз, справа налево.
 *          Ключ - количество столбцов таблицы.
 */
class tableCipher
{
private:
    int key; ///< Ключ шифрования (количество столбцов)

    /**
     * @brief Приведение строки к верхнему регистру
     * @param s Входная строка
     * @return Строка в верхнем регистре
     */
    std::wstring toUpper(const std::wstring& s);

    /**
     * @brief Проверка текста на соответствие русскому алфавиту
     * @param text Проверяемый текст
     * @return true если текст содержит только русские буквы и пробелы, иначе false
     */
    bool isValidRussianText(const std::wstring& text);

    /**
     * @brief Подготовка текста к шифрованию
     * @param s Исходный текст
     * @return Текст в верхнем регистре без пробелов
     * @throw tableCipher_error Если текст пустой, содержит недопустимые символы или только пробелы
     */
    std::wstring prepareText(const std::wstring& s);

    /**
     * @brief Валидация ключа
     * @param k Проверяемый ключ
     * @throw tableCipher_error Если ключ невалиден
     */
    void validateKey(int k);

    /**
     * @brief Валидация длины текста относительно ключа
     * @param text Проверяемый текст
     * @param operation Название операции (для сообщения об ошибке)
     * @throw tableCipher_error Если длина текста недостаточна для операции
     */
    void validateTextLength(const std::wstring& text, const std::string& operation);

public:
    /**
     * @brief Запрет конструктора без параметров
     */
    tableCipher() = delete;

    /**
     * @brief Конструктор с установкой ключа
     * @param k Ключ шифрования (количество столбцов)
     * @throw tableCipher_error Если ключ невалиден
     */
    tableCipher(int k);

    /**
     * @brief Метод зашифровывания
     * @param open_text Открытый текст для шифрования
     * @return Зашифрованная строка
     * @throw tableCipher_error Если текст пустой или недостаточной длины
     */
    std::wstring encrypt(const std::wstring& open_text);

    /**
     * @brief Метод расшифровывания
     * @param cipher_text Зашифрованный текст для расшифрования
     * @return Расшифрованная строка
     * @throw tableCipher_error Если текст пустой или недостаточной длины
     */
    std::wstring decrypt(const std::wstring& cipher_text);
};
