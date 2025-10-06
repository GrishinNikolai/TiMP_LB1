#include "tableCipher.h"
#include <algorithm>
#include <sstream>
#include <string>

tableCipher::tableCipher(int k)
{
    if (k <= 0) {
        throw std::invalid_argument("Ключ должен быть положительным числом");
    }
    key = k;
}

std::wstring tableCipher::encrypt(const std::wstring& open_text)
{
    if (open_text.empty()) {
        return L"";
    }
    
    std::wstring text = toUpper(open_text);
    int text_len = text.length();
    
    // Вычисляем количество строк
    int rows = (text_len + key - 1) / key;
    
    // Вычисляем общее количество ячеек и количество пустых ячеек
    
    // Создаем таблицу для записи
    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(key));
    
    // ЗАПИСЬ: по горизонтали слева направо, сверху вниз
    int index = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            if (index < text.length()) {
                table[i][j] = text[index++];
            }
        }
    }
    
    // СЧИТЫВАНИЕ: по вертикалям, начиная с верхнего правого угла, 
    // поочередно сверху вниз (справа налево по столбцам)
    std::wstring result;
    for (int j = key - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            result += table[i][j];
        }
    }
    
    return result;
}

std::wstring tableCipher::decrypt(const std::wstring& cipher_text)
{
    if (cipher_text.empty()) {
        return L"";
    }
    
    std::wstring text = toUpper(cipher_text);
    int text_len = text.length();
    
    // Вычисляем количество строк
    int rows = (text_len + key - 1) / key;
    
    // Вычисляем общее количество ячеек и количество пустых ячеек
    int total_cells = rows * key;
    int empty_cells = total_cells - text_len;
    
    // Добавляем пробелы в конец текста, чтобы заполнить пустые ячейки
    if (empty_cells > 0) {
        for(int i = 1; i <= empty_cells; i++) {
            text.insert((i*(rows - 1)+(i-1)), L" ");
        }
    }
    
    // Создаем таблицу для заполнения
    std::vector<std::vector<wchar_t>> table(rows, std::vector<wchar_t>(key));
    
    // ЗАПОЛНЕНИЕ: по вертикалям, начиная с верхнего правого угла,
    // поочередно сверху вниз (справа налево по столбцам)
    int index = 0;
    for (int j = key - 1; j >= 0; j--) {
        for (int i = 0; i < rows; i++) {
            if (index < text.length()) {
                table[i][j] = text[index++];
            }
        }
    }
    
    // ЧТЕНИЕ: по горизонтали слева направо, сверху вниз
    std::wstring result;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < key; j++) {
            result += table[i][j];
        }
    }
    
    // Удаляем добавленные пробелы из результата
    if (empty_cells > 0) {
        result.erase(result.length() - empty_cells);
    }
    
    return result;
}

std::wstring tableCipher::toUpper(const std::wstring& s)
{
    std::wstring result = s;
    std::locale loc("ru_RU.UTF-8");
    for (wchar_t& c : result) {
        c = std::toupper(c, loc);
    }
    return result;
}