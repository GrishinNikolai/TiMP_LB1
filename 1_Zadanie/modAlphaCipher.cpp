#include "modAlphaCipher.h"
#include <algorithm>

modAlphaCipher::modAlphaCipher(const std::wstring& skey)
{
    for (size_t i = 0; i < numAlpha.size(); i++) {
        alphaNum[numAlpha[i]] = i;
    }
    key = convert(toUpper(skey));
}

std::wstring modAlphaCipher::encrypt(const std::wstring& open_text)
{
    std::wstring text = toUpper(open_text);
    std::vector<int> work = convert(text);
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

std::wstring modAlphaCipher::decrypt(const std::wstring& cipher_text)
{
    std::vector<int> work = convert(cipher_text);
    for (size_t i = 0; i < work.size(); i++) {
        work[i] = (work[i] + alphaNum.size() - key[i % key.size()]) % alphaNum.size();
    }
    return convert(work);
}

std::vector<int> modAlphaCipher::convert(const std::wstring& s)
{
    std::vector<int> result;
    for (wchar_t c : s) {
        result.push_back(alphaNum[c]);
    }
    return result;
}

std::wstring modAlphaCipher::convert(const std::vector<int>& v)
{
    std::wstring result;
    for (int i : v) {
        result.push_back(numAlpha[i]);
    }
    return result;
}

std::wstring modAlphaCipher::toUpper(const std::wstring& s)
{
    std::wstring result;
    std::locale loc("ru_RU.UTF-8");
    
    for (wchar_t c : s) {
        wchar_t upper_c = std::toupper(c, loc);
        bool found = false;
        for (wchar_t alpha_char : numAlpha) {
            if (upper_c == alpha_char) {
                found = true;
                break;
            }
        }
        if (found) {
            result.push_back(upper_c);
        }
    }
    return result;
}
