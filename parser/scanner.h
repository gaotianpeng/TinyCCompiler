#ifndef __PARSER_SCANNER_H
#define __PARSER_SCANNER_H

/*
    scanner(扫描器)：属于词法分析部分
        scanner读取源文件, 按序返回文件内的字符，直到文件结束
*/
#include <stdio.h>

namespace Lexer {
class Scanner {
public:
    static char Scan(FILE* fp);
protected:

private:

}; // class Scanner
} // namesapce Lexer

#endif // __PARSER_SCANNER_H
