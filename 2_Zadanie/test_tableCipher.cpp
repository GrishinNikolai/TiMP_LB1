#include <UnitTest++/UnitTest++.h>
#include "tableCipher.h"
#include <iostream>
#include <locale>
#include <codecvt>

// Макрос для сравнения wstring с правильной конвертацией в string для вывода ошибок
#define CHECK_EQUAL_WSTR(expected, actual) \
    do { \
        std::wstring exp__(expected); \
        std::wstring act__(actual); \
        if (exp__ != act__) { \
            UnitTest::TestResults* testResults__ = UnitTest::CurrentTest::Results(); \
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter; \
            std::string expStr = converter.to_bytes(exp__); \
            std::string actStr = converter.to_bytes(act__); \
            UnitTest::MemoryOutStream stream__; \
            stream__ << "Expected " << expStr << " but was " << actStr; \
            testResults__->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), stream__.GetText()); \
        } \
    } while(0)

// Тестовый сценарий для конструктора (KeyTest)
SUITE(KeyTest) {
    TEST(ValidKey) {
        CHECK_EQUAL_WSTR(L"ИТРРЕИПВМ", tableCipher(3).encrypt(L"ПРИВЕТМИР")); // Минимальный валидный ключ
    }

    TEST(ValidKey2) {
        CHECK_EQUAL_WSTR(L"ЕВРИИРМПТ", tableCipher(5).encrypt(L"ПРИВЕТМИР")); // Валидный ключ
    }

    TEST(NegativeKey) {
        CHECK_THROW(tableCipher cipher(-1), tableCipher_error);
    }

    TEST(ZeroKey) {
        CHECK_THROW(tableCipher cipher(0), tableCipher_error);
    }

    TEST(KeyOne) {
        CHECK_THROW(tableCipher cipher(1), tableCipher_error); // Слишком слабый ключ
    }

    TEST(KeyTwo) {
        CHECK_THROW(tableCipher cipher(2), tableCipher_error); // Слишком слабый ключ
    }
}

// Фикстура для тестов с ключом 3
struct Key3_fixture {
    tableCipher* p;
    Key3_fixture() {
        p = new tableCipher(3);
    }
    ~Key3_fixture() {
        delete p;
    }
};

// Тестовый сценарий для метода encrypt
SUITE(EncryptTest) {
    TEST_FIXTURE(Key3_fixture, BasicEncryption) {
        CHECK_EQUAL_WSTR(L"ИТРРЕИПВМА", p->encrypt(L"ПРИВЕТМИРА"));
    }

    TEST_FIXTURE(Key3_fixture, TextWithSpaces) {
        CHECK_EQUAL_WSTR(L"ИТРРЕИПВМ", p->encrypt(L"ПРИВЕТ МИР"));
    }

    TEST_FIXTURE(Key3_fixture, MixedCaseText) {
        CHECK_EQUAL_WSTR(L"ИТРРЕИПВМ", p->encrypt(L"Привет Мир"));
    }

    TEST_FIXTURE(Key3_fixture, TextEquallyThanKey) { // Длина 2, ключ 3
        CHECK_THROW(p->encrypt(L"ПР"), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, TextShorterThanKey) { // Длина 3, ключ 3
        CHECK_THROW(p->encrypt(L"ПРИ"), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, EmptyText) {
        CHECK_THROW(p->encrypt(L""), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, TextWithOnlySpaces) {
        CHECK_THROW(p->encrypt(L"   "), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, TextWithInvalidCharacters) {
        CHECK_THROW(p->encrypt(L"ПРИВЕТ123"), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, TextWithInvalidCharacters2) {
        CHECK_THROW(p->encrypt(L"ПРИВЕТ,МИР"), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, LongText) {
        CHECK_EQUAL_WSTR(L"ФВИЕСНКЭИЕСИОНПЕАВЙОТЕОШРАЕРТООТНРН", p->encrypt(L"ШИФРОВАНИЕПЕРЕСТАНОВКОЙЭТОИНТЕРЕСНО"));
    }

    TEST(EncryptWithKey4) {
        tableCipher cipher(4);
        CHECK_EQUAL_WSTR(L"ПЕЯНМТАХАОЮНЕККЬРТИ", cipher.encrypt(L"КОМПЬЮТЕРНАЯТЕХНИКА"));
    }

    TEST(EncryptWithKey5) {
        tableCipher cipher(8);
        CHECK_EQUAL_WSTR(L"МЕМИАНРАГВООРРПИ", cipher.encrypt(L"ПРОГРАММИРОВАНИЕ"));
    }
}

// Тестовый сценарий для метода decrypt
SUITE(DecryptTest) {
    TEST_FIXTURE(Key3_fixture, BasicDecryption) {
        CHECK_EQUAL_WSTR(L"ПРИВЕТМИРА", p->decrypt(L"ИТРРЕИПВМА"));
    }

    TEST_FIXTURE(Key3_fixture, DecryptWithInvalidCharacters) {
        CHECK_THROW(p->decrypt(L"ИТР123РЕИПВМ"), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, DecryptWithInvalidCharacters2) {
        CHECK_THROW(p->decrypt(L"ИТР,РЕИПВМ"), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, DecryptWithSpaces) {
        CHECK_EQUAL_WSTR(L"ПРИВЕТМИР", p->decrypt(L"ИТР РЕИ ПВМ"));
    }

    TEST_FIXTURE(Key3_fixture, DecryptWithLowercase) {
        CHECK_EQUAL_WSTR(L"ПРИВЕТМИР", p->decrypt(L"итрреипвм"));
    }

    TEST_FIXTURE(Key3_fixture, EmptyCipherText) {
        CHECK_THROW(p->decrypt(L""), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, TextWithOnlySpaces) {
        CHECK_THROW(p->encrypt(L"   "), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, CipherTextShorterThanKey) {
        CHECK_THROW(p->decrypt(L"ИТ"), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, CipherTextEquallyThanKey) {
        CHECK_THROW(p->decrypt(L"ИТР"), tableCipher_error);
    }

    TEST_FIXTURE(Key3_fixture, DecryptLongText) {
        CHECK_EQUAL_WSTR(L"ШИФРОВАНИЕПЕРЕСТАНОВКОЙЭТОИНТЕРЕСНО", p->decrypt(L"ФВИЕСНКЭИЕСИОНПЕАВЙОТЕОШРАЕРТООТНРН"));
    }

    TEST(DecryptWithKey4) {
        tableCipher cipher(4);
        CHECK_EQUAL_WSTR(L"КОМПЬЮТЕРНАЯТЕХНИКА", cipher.decrypt(L"ПЕЯНМТАХАОЮНЕККЬРТИ"));
    }

    TEST(DecryptWithKey5) {
        tableCipher cipher(8);
        CHECK_EQUAL_WSTR(L"ПРОГРАММИРОВАНИЕ", cipher.decrypt(L"МЕМИАНРАГВООРРПИ"));
    }
}

int main(int argc, char** argv) {
    return UnitTest::RunAllTests();
}
