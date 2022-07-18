#ifndef _PARSER_LEXER_H
#define _PARSER_LEXER_H

#include <memory>

namespace Parser {

class Token;
class Scanner;

class Lexer {
public:
    explicit Lexer(Scanner& scanner);
    ~Lexer();

Token* Tokenize();

private:
    class Impl;
    std::shared_ptr<Impl> impl_;
}; // namespace Lexer

} // namespace Parser

#endif // _PARSER_LEXER_H