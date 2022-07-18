#ifndef __PARSER_SCANNER_H
#define __PARSER_SCANNER_H

/*
    scanner(扫描器)：属于词法分析部分
        scanner读取源文件, 按序返回文件内的字符，直到文件结束
*/
#include <stdio.h>
#include <memory>
namespace Lexer {
class Scanner {
public:
    Scanner();
    char Scan(FILE* fp);

    /*
        for test
    */
    char ScanTest(FILE* fp);

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
}; // class Scanner
} // namesapce Lexer

#endif // __PARSER_SCANNER_H