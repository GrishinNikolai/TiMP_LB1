#include <UnitTest++/UnitTest++.h>
#include "modAlphaCipher.h"
#include <iostream>
#include <locale>

#define CHECK_EQUAL_WSTR(expected, actual) \
    do { \
        std::wstring exp__(expected); \
        std::wstring act__(actual); \
        if (exp__ != act__) { \
            UnitTest::TestResults* testResults__ = UnitTest::CurrentTest::Results(); \
            std::string expStr(exp__.begin(), exp__.end()); \
            std::string actStr(act__.begin(), act__.end()); \
            UnitTest::MemoryOutStream stream__; \
            stream__ << "Expected " << expStr << " but was " << actStr; \
            testResults__->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), stream__.GetText()); \
        } \
    } while(0)

SUITE(KeyTest) {
    TEST(ValidKey) {
        CHECK_EQUAL_WSTR(L"БСДБС", modAlphaCipher(L"БСД").encrypt(L"ААААА"));
    }

    TEST(LongKey) {
        CHECK_EQUAL_WSTR(L"БСДДС", modAlphaCipher(L"БСДДСТВГЫТ").encrypt(L"ААААА"));
    }

    TEST(LowCaseKey) {
        CHECK_EQUAL_WSTR(L"БСДБС", modAlphaCipher(L"бсд").encrypt(L"ААААА"));
    }

    TEST(DigitsInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"Б1"), cipher_error);
    }

    TEST(PunctuationInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"Б,С"), cipher_error);
    }

    TEST(WhitespaceInKey) {
        CHECK_THROW(modAlphaCipher cipher(L"Б С"), cipher_error);
    }

    TEST(EmptyKey) {
        CHECK_THROW(modAlphaCipher cipher(L""), cipher_error);
    }

    TEST(WeakKey) {
        CHECK_THROW(modAlphaCipher cipher(L"ААА"), cipher_error);
    }
}

struct KeyB_fixture {
    modAlphaCipher* p;
    KeyB_fixture() {
        p = new modAlphaCipher(L"Б");
    }
    ~KeyB_fixture() {
        delete p;
    }
};

SUITE(EncryptTest) {
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK_EQUAL_WSTR(L"УЁТУПГПЁТППВЪЁОЙЁЕМАРСПГЁСЛЙ",
                   p->encrypt(L"ТЕСТОВОЕСООБЩЕНИЕДЛЯПРОВЕРКИ"));
    }

    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_EQUAL_WSTR(L"УЁТУПГПЁТППВЪЁОЙЁЕМАРСПГЁСЛЙ",
                   p->encrypt(L"тестовоесообщениедляпроверки"));
    }

    TEST_FIXTURE(KeyB_fixture, StringWithWhitespaceAndPunct) {
        CHECK_EQUAL_WSTR(L"УЁТУПГПЁТППВЪЁОЙЁЕМАРСПГЁСЛЙ",
                   p->encrypt(L"Тестовое сообщение для проверки!!!"));
    }

    TEST_FIXTURE(KeyB_fixture, StringWithNumbers) {
        CHECK_EQUAL_WSTR(L"УЁТУПГПЁТППВЪЁОЙЁЕМАРСПГЁСЛЙ",
                   p->encrypt(L"Тестовое123сообщение456для789проверки"));
    }

    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->encrypt(L""), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, NoAlphaString) {
        CHECK_THROW(p->encrypt(L"1234+8765=9999"), cipher_error);
    }

    TEST(MaxShiftRussianText) {
        modAlphaCipher cipher(L"Я");
        std::wstring text = L"СЪЕШЬЖЕЕЩЁЭТИХМЯГКИХФРАНЦУЗСКИХБУЛОК";
        std::wstring encrypted = cipher.encrypt(text);
        CHECK_EQUAL_WSTR(L"РЩДЧЫЁДДШЕЬСЗФЛЮВЙЗФУПЯМХТЖРЙЗФАТКНЙ", encrypted);
    }
}

SUITE(DecryptTest) {
    TEST_FIXTURE(KeyB_fixture, UpCaseString) {
        CHECK_EQUAL_WSTR(L"ТЕСТОВОЕСООБЩЕНИЕДЛЯПРОВЕРКИ",
                   p->decrypt(L"УЁТУПГПЁТППВЪЁОЙЁЕМАРСПГЁСЛЙ"));
    }

    TEST_FIXTURE(KeyB_fixture, LowCaseString) {
        CHECK_THROW(p->decrypt(L"уётупгпётппвъёойёемарспгёслй"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, WhitespaceString) {
        CHECK_THROW(p->decrypt(L"УЁТ УПГ ПЁТ ППВ ЪЁО ЙЁЕ МАР СПГ ЁСЛЙ"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, DigitsString) {
        CHECK_THROW(p->decrypt(L"УЁТУПГПЁТ2025ППВЪЁОЙЁЕМАРСПГЁСЛЙ"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, PunctString) {
        CHECK_THROW(p->decrypt(L"УЁТ,УПГ"), cipher_error);
    }

    TEST_FIXTURE(KeyB_fixture, EmptyString) {
        CHECK_THROW(p->decrypt(L""), cipher_error);
    }

    TEST(MaxShiftRussianText) {
        modAlphaCipher cipher(L"Я");
        std::wstring text = L"РЩДЧЫЁДДШЕЬСЗФЛЮВЙЗФУПЯМХТЖРЙЗФАТКНЙ";
        std::wstring decrypted = cipher.decrypt(text);
        CHECK_EQUAL_WSTR(L"СЪЕШЬЖЕЕЩЁЭТИХМЯГКИХФРАНЦУЗСКИХБУЛОК", decrypted);
    }
}

int main(int argc, char** argv) {
    return UnitTest::RunAllTests();
}
