#pragma once
#include <vector>
#include <string>
#include <stdexcept>
#include <locale>
#include <codecvt>

class tableCipher_error : public std::runtime_error {
private:
    std::string message;
public:
    explicit tableCipher_error(const std::string& what_arg) : std::runtime_error(what_arg), message(what_arg) {}
    explicit tableCipher_error(const char* what_arg) : std::runtime_error(what_arg), message(what_arg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

class tableCipher
{
private:
    int key;
    std::wstring toUpper(const std::wstring& s);
    bool isValidRussianText(const std::wstring& text);
    std::wstring prepareText(const std::wstring& s);
    void validateKey(int k);
    void validateTextLength(const std::wstring& text, const std::string& operation);

public:
    tableCipher() = delete;
    tableCipher(int k);
    std::wstring encrypt(const std::wstring& open_text);
    std::wstring decrypt(const std::wstring& cipher_text);
};
