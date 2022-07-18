#include "gtest/gtest.h"
#include "scanner.h"
#include "common.h"
#include <stdio.h>

using Scanner = Parser::Scanner;

TEST(ScannerTest, ScanTest1) {
    {
        Scanner scanner(NULL);
        EXPECT_EQ(FILE_ERROR, scanner.ScanTest());
    }

    FILE* fp = fopen("test.cc", "r");
    Scanner scanner(fp);
    ASSERT_TRUE(fp != NULL);
    char ch;
    while ((ch = scanner.ScanTest()) != -1) {
        printf("%c", ch);
    }
    printf("\n");
    EXPECT_EQ(FILE_END, ch);
    fclose(fp);
}

TEST(ScannerTest, ScanTest2) {
    {
        Scanner scanner(NULL);
        EXPECT_EQ(FILE_ERROR, scanner.Scan());
    }

    FILE* fp = fopen("test.cc", "r");
    ASSERT_TRUE(fp != NULL);
    Scanner scanner(fp);
    char ch;
    while ((ch = scanner.ScanTest()) != -1) {
        printf("%c", ch);
    }
    printf("\n");
    EXPECT_EQ(FILE_END, ch);
    fclose(fp);
}

TEST(ScannerTest, ScanTest3) {
    FILE* fp1 = fopen("test.cc", "r");
    FILE* fp2 = fopen("test.cc", "r");
    ASSERT_TRUE(fp1 != NULL);
    ASSERT_TRUE(fp2 != NULL);
    Scanner scanner1(fp1);
    Scanner scanner2(fp2);

    while (true) {
        char ch1 = scanner1.ScanTest();
        char ch2 = scanner2.Scan();
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