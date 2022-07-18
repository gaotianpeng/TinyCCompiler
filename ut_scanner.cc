#include "gtest/gtest.h"
#include "scanner.h"
#include "common.h"
#include <stdio.h>

using Scanner = Lexer::Scanner;

TEST(FORTEST, For_Test1) {
    Scanner scanner;
    EXPECT_EQ(FILE_ERROR, scanner.ScanTest(NULL));
    FILE* fp = fopen("test.cc", "r");
    ASSERT_TRUE(fp != NULL);
    char ch;
    while ((ch = scanner.ScanTest(fp)) != -1) {
        printf("%c", ch);
    }
    printf("\n");
    EXPECT_EQ(FILE_END, ch);
    fclose(fp);
}

TEST(FORTEST, For_Test2) {
    Scanner scanner;
    EXPECT_EQ(FILE_ERROR, scanner.Scan(NULL));
    FILE* fp = fopen("test.cc", "r");
    ASSERT_TRUE(fp != NULL);
    fclose(fp);
}
TEST(FORTEST, For_Test3) {
    Scanner scanner;
    FILE* fp1 = fopen("test.cc", "r");
    FILE* fp2 = fopen("test.cc", "r");
    ASSERT_TRUE(fp1 != NULL);
    ASSERT_TRUE(fp2 != NULL);

    while (true) {
        char ch1 = scanner.ScanTest(fp1);
        char ch2 = scanner.Scan(fp2);
        if (ch1 != ch2) {
            printf("Error %c, %c\n", ch1, ch2);
            break;
        } else {
            if (ch1 == FILE_END) {
                printf("file end\n");
                break;
            }
        }
        printf("%c", ch1);
    }
    fclose(fp1);
    fclose(fp2);
}