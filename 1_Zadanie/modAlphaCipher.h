/**
 * @file modAlphaCipher.h
 * @author Гришин Н.С.
 * @version 1.0
 * @date 03.12.2025
 * @copyright ИБСТ ПГУ
 * @brief Заголовочный файл для модуля шифрования методом Гронсфельда
 */

#pragma once
#include <vector>
#include <string>
#include <map>
#include <locale>
#include <codecvt>
#include <stdexcept>

/**
 * @brief Класс-исключение для ошибок шифрования
 * @details Наследуется от std::invalid_argument для удобства обработки
 */
class cipher_error : public std::invalid_argument {
public:
    /**
     * @brief Конструктор с параметром типа std::string
     * @param what_arg Сообщение об ошибке
     */
    explicit cipher_error(const std::string& what_arg) : std::invalid_argument(what_arg) {}

    /**
     * @brief Конструктор с параметром типа const char*
     * @param what_arg Сообщение об ошибке
     */
    explicit cipher_error(const char* what_arg) : std::invalid_argument(what_arg) {}
};

/**
 * @brief Класс для шифрования методом Гронсфельда
 * @details Реализует шифр Гронсфельда для русского алфавита.
 *          Ключ устанавливается в конструкторе. Для зашифровывания и
 *          расшифровывания предназначены методы encrypt и decrypt.
 * @warning Реализация только для русского языка
 */
class modAlphaCipher
{
private:
    std::wstring numAlpha = L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"; ///< Алфавит по порядку
    std::map<wchar_t, int> alphaNum; ///< Ассоциативный массив "символ-номер"
    std::vector<int> key; ///< Ключ в числовом представлении

    /**
     * @brief Преобразование строки в числовой вектор
     * @param s Входная строка
     * @return Вектор чисел, соответствующих символам строки
     */
    std::vector<int> convert(const std::wstring& s);

    /**
     * @brief Преобразование числового вектора в строку
     * @param v Входной вектор чисел
     * @return Строка, составленная из символов алфавита
     */
    std::wstring convert(const std::vector<int>& v);

    /**
     * @brief Приведение строки к верхнему регистру с удалением пробелов
     * @param s Входная строка
     * @return Строка в верхнем регистре без пробелов
     */
    std::wstring toUpper(const std::wstring& s);

    /**
     * @brief Валидация и нормализация ключа
     * @param s Ключ в виде строки
     * @return Валидированный ключ в верхнем регистре
     * @throw cipher_error Если ключ пустой, содержит недопустимые символы или слишком слабый
     */
    std::wstring getValidKey(const std::wstring& s);

    /**
     * @brief Валидация открытого текста
     * @param s Открытый текст
     * @return Валидированный текст в верхнем регистре
     * @throw cipher_error Если текст пустой или не содержит русских букв
     */
    std::wstring getValidOpenText(const std::wstring& s);

    /**
     * @brief Валидация зашифрованного текста
     * @param s Зашифрованный текст
     * @return Валидированный зашифрованный текст
     * @throw cipher_error Если текст пустой или содержит недопустимые символы
     */
    std::wstring getValidCipherText(const std::wstring& s);

public:
    /**
     * @brief Запрет конструктора без параметров
     */
    modAlphaCipher() = delete;

    /**
     * @brief Конструктор с установкой ключа
     * @param skey Ключ шифрования в виде строки
     * @throw cipher_error Если ключ невалиден
     */
    modAlphaCipher(const std::wstring& skey);

    /**
     * @brief Метод зашифровывания
     * @param open_text Открытый текст для шифрования
     * @return Зашифрованная строка
     * @throw cipher_error Если текст пустой или не содержит русских букв
     */
    std::wstring encrypt(const std::wstring& open_text);

    /**
     * @brief Метод расшифровывания
     * @param cipher_text Зашифрованный текст для расшифрования
     * @return Расшифрованная строка
     * @throw cipher_error Если текст пустой или содержит недопустимые символы
     */
    std::wstring decrypt(const std::wstring& cipher_text);
};
