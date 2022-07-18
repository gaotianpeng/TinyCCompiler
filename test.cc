#include "gtest/gtest.h"
#include "scanner.h"
#include <stdio.h>

using Scanner = Lexer::Scanner;

TEST(FORTEST, For_Test) {
    EXPECT_EQ(-1, Scanner::Scan(NULL));
    FILE* fp = fopen("test.cc", "r");
    ASSERT_TRUE(fp != NULL);
    char ch;
    while ((ch = Scanner::Scan(fp)) != -1) {
        printf("%c", ch);
    }
    printf("\n");
    EXPECT_EQ(-1, ch);   
}