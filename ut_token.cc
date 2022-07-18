#include "gtest/gtest.h"
#include "token.h"
#include <iostream>

using namespace std;

TEST(TokenTest, Keyword) {
    Parser::Keywords::string_hash hash;
    cout << hash("gtp") << endl;
    cout << hash("zsp") << endl;
    cout << hash("gtp") << endl;
}