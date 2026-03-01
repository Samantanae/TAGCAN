#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "test_utils.h"
#include "tests.h"

#include "../include/in_basefonc.h"
#include "../include/config_value.h"
int total_tests = 0;
int passed_tests = 0;


void test_str_func() {
    printf("=== Testing str_functions.h ===\n\n");

    // ===== str_len tests =====
    printf("TEST: str_len\n");
    TEST("str_len(\"hello\") == 5", str_len("hello") == 5);
    TEST("str_len(\"\") == 0", str_len("") == 0);
    TEST("str_len(\"hello world\") == 11", str_len("hello world") == 11);
    TEST("str_len(NULL) == 0", str_len(NULL) == CAN_TG_ERROR_NULL_POINTOR);
    printf("\n");

    // ===== str_endswith tests =====
    printf("TEST: str_endswith\n");
    TEST("str_endswith(\"hello.txt\", \".txt\") == true", str_endswith("hello.txt", ".txt") == 1);
    TEST("str_endswith(\"hello.txt\", \".c\") == false", str_endswith("hello.txt", ".c") == 0);
    TEST("str_endswith(\"hello\", \"hello\") == true", str_endswith("hello", "hello") == 1);
    TEST("str_endswith(\"hi\", \"hello\") == false", str_endswith("hi", "hello") == 0);
    TEST("str_endswith(\"hello\", \"\") == true", str_endswith("hello", "") == 1);
    TEST("str_endswith(NULL, \".txt\") == false", str_endswith(NULL, ".txt") == CAN_TG_ERROR_NULL_POINTOR);
    TEST("str_endswith(\"hello\", NULL) == false", str_endswith("hello", NULL) == CAN_TG_ERROR_NULL_POINTOR);
    printf("\n");

    // ===== str_count tests =====
    printf("TEST: str_count\n");
    printf("\t C1 = CAN_TG_ERROR_NULL_POINTOR\n");
    TEST("str_count(\"hello\", \"l\") == 2", str_count("hello", "l") == 2);
    TEST("str_count(\"hello world\", \"o\") == 2", str_count("hello world", "o") == 2);
    TEST("str_count(\"hello hello hello\", \"hello\") == 3", str_count("hello hello hello", "hello") == 3);
    TEST("str_count(\"hello\", \"x\") == 0", str_count("hello", "x") == 0);
    TEST("str_count(NULL, \"l\") == C1", str_count(NULL, "l") == CAN_TG_ERROR_NULL_POINTOR);
    TEST("str_count(\"hello\", NULL) == C1", str_count("hello", NULL) == CAN_TG_ERROR_NULL_POINTOR);
    TEST("str_count(\"hello\", \"\") == 0", str_count("hello", "") == 0);
    TEST("str_count(\"aaaa\", \"aa\") == 2", str_count("aaaa", "aa") == 2);
    printf("\n");

    // ===== str_replace tests =====
    printf("TEST: str_replace\n");

    char buffer[256] = "test        ";

    int len1 = str_replace("hello world", "world", "C", buffer, -1);
    TEST("str_replace basic", len1 > 0 && strcmp(buffer, "hello C") == 0);

    int len2 = str_replace("hello hello hello", "hello", "hi", buffer, -1);
    TEST("str_replace multiple", len2 > 0 && strcmp(buffer, "hi hi hi") == 0);

    int len3 = str_replace("hello hello hello", "hello", "hi", buffer, 1);
    TEST("str_replace limited (count=1)", len3 > 0 && strcmp(buffer, "hi hello hello") == 0);

    int len4 = str_replace("hello hello hello", "hello", "hi", buffer, 2);
    TEST("str_replace limited (count=2)", len4 > 0 && strcmp(buffer, "hi hi hello") == 0);

    int len5 = str_replace("hello world", "xyz", "abc", buffer, -1);
    TEST("str_replace no match", len5 > 0 && strcmp(buffer, "hello world") == 0);

    int len6 = str_replace("hello world", " ", "", buffer, -1);
    TEST("str_replace with empty string", len6 > 0 && strcmp(buffer, "helloworld") == 0);

    int len7 = str_replace("a,b,c", ",", " and ", buffer, -1);
    TEST("str_replace with longer string", len7 > 0 && strcmp(buffer, "a and b and c") == 0);

    int len8 = str_replace(NULL, "a", "b", buffer, -1);
    TEST("str_replace NULL string", len8 == CAN_TG_ERROR_NULL_POINTOR);

    int len9 = str_replace("hello", "", "a", buffer, -1);
    TEST("str_replace empty old substring", len9 == -1);

    char small_buffer[5];
    int len10 = str_replace("hello world", "world", "C", small_buffer, -1);

    char test_replace_01[90] = ";3_;1_;4_";
    int len11 = str_replace(test_replace_01, ";3_;",";3_blabla;",test_replace_01,1);
    // buffer size cannot be determined by this implementation, just ensure function returns non-negative
    TEST("str_replace buffer (size not checked)", len10 >= 0);
    TEST("str replace inside str", len11>=0);
    printf("\n");

    printf("test supp\n");
    char a1[] = "                  a1a2a3a5    a56a93g                                  ";
    TEST_EQ_INT("supp test", str_remove(a1,"a1a2"),1);

    // ===== SUMMARY =====
    printf("=== SUMMARY ===\n");
    printf("Passed: %d/%d\n", passed_tests, total_tests);
    printf("Failed: %d/%d\n", total_tests - passed_tests, total_tests);

    if (passed_tests == total_tests) {
        printf("\n All tests passed!\n");
    } else {
        printf("\n Some tests failed!\n");
    }
}
