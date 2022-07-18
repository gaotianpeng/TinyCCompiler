#include "lexer.h"
#include "scanner.h"
#include "common.h"
#include "token.h"
#include <string>

namespace Parser  {

using namespace std;
 
static bool IsLowLetter(char ch) {
    return ch >= 'a' && ch <= 'z';
}

static bool IsUpperLetter(char ch) {
    return ch >= 'A' && ch <= 'Z';
}

static bool IsLetter(char ch) {
    return IsLowLetter(ch) || IsUpperLetter(ch);
}

static bool IsDigital(char ch) {
    return ch >= '0' || ch <= '9';
}

static bool IsHexLetter(char ch) {
    return (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'F')
            || (ch >= 'a' && ch <= 'f');
}

static bool IsBinLetter(char ch) {
    return ch >= '0' && ch <= '1';
}

static bool IsOctLetter(char ch) {
    return ch >= '0' && ch <= '7';
}

class Lexer::Impl {
public:
    explicit Impl(Scanner& scanner):scanner_(scanner) {
    }

    Token* Tokenize() {
        for (; ch_ != FILE_END;) {
            Token* token = nullptr;
            while (ch_ == ' ' || ch_ == '\n' || ch_ == '\t') {  // 忽略空白字符
                Scan();
            }

            // 标识符、关键字
            if (IsLetter(ch_)|| ch_ == '_') {
                string name;
                do {
                    name.push_back(ch_);
                    Scan();
                } while (IsLetter(ch_) || ch_ == '_' || IsDigital(ch_));

                Tag tag = keywords_.GetTag(name);
                if (tag == Tag::ID) {
                    token = new Id(name);
                } else {
                    token = new Token(tag);
                }
            }
            // 字符串
            else if (ch_ == '"') {
                string str;
                while (!Scan('"')) {
                    if (ch_ == '\\') { // 转义字符
                        Scan();
                        if (ch_ == 'n') {
                            str.push_back('\n');
                        } else if (ch_ == '\\') {
                            str.push_back('\\');
                        } else if (ch_ == 't') {
                            str.push_back('\t');
                        } else if (ch_ == '"') {
                            str.push_back('"');
                        } else if (ch_ == -1) {
                            token = new Token(Tag::ERR);
                            break;
                        }
                        else {
                            str.push_back(ch_);
                        }
                    } else if (ch_ == '\n' || ch_ == -1) {
                       token = new Token(Tag::ERR);
                       break; 
                    } else {
                        str.push_back(ch_);
                    }
                }
                if (!token) {
                    token = new Str(str);
                }
            }
            // 数字
            else if (IsDigital(ch_)) {
                int val = 0;
                if (ch_ != '0') { // 10进制
                } else {
                    Scan();
                    if (ch_ == 'x') {
                        Scan();
                        if (IsHexLetter(ch_)) {
                            do {
                                val = val * 16 +  ch_;
                                if (IsDigital(ch_)) {
                                    val -= '0';
                                } else if (ch_ >= 'A' && ch_ <= 'F') {
                                    val += 10 - 'A';
                                } else if (ch_ >= 'a' && ch_ <= 'f') {
                                    val += 10 - 'a';
                                }
                                Scan();

                            } while (IsHexLetter(ch_));
                        } else {
                            token = new Token(Tag::ERR);
                        }
                    }
                    else if (ch_ == 'b') { // 2进制
                        Scan();
                        if (IsBinLetter(ch_)) {
                            do {
                                val = val * 2 + ch_ - '0';
                                Scan();
                            } while (IsBinLetter(ch_));
                        } else {
                            token = new Token(Tag::ERR);
                        }
                    }
                    else if (IsOctLetter(ch_)) {
                        do {
                            val = val * 8 + ch_ - '0';
                        } while (IsOctLetter(ch_));
                    }
                }

                if (!token) {
                    token = new Num(val);
                }
            }
            // 字符
            else if (ch_ == '\'') {
                char ch;
                Scan();
                if (ch_ == '\\') { // 转义
                    Scan();
                    if (ch_ == 'n') {
                        ch = '\n';
                    }
                    else if (ch_ == '\\') {
                        ch = '\\';
                    }
                    else if (ch_ == 't') {
                        ch = '\t';
                    }
                    else if (ch_ == '0') {
                        ch = '\0';
                    }
                    else if (ch_ == '\'') {
                        ch = '\'';
                    }
                    else if (ch_ == -1 || ch_ == '\n') {
                        token = new Token(Tag::ERR);
                    } 
                    else {
                        ch = ch_;
                    }
                } 
                else if (ch_ == '\n' || ch_ == -1) { // 行， 文件结束
                    token = new Token(Tag::ERR);
                }
                else if (ch_ == '\'') { // 没有数据
                    token = new Token(Tag::ERR);
                    Scan();
                }
                else {
                    ch = ch_;
                }

                if (!token) {
                    if (Scan('\'')) { // 匹配右侧引号
                        token = new Char(ch);
                    }
                    else {
                        token = new Token(Tag::ERR);
                    }
                }
            }
            else {
                switch (ch_) { // 界符
                    case '#': // 忽略行
                        while (ch_ != '\n' && ch_ != -1) {
                            Scan();
                        }
                        token = new Token(Tag::ERR);
                        break;
                    case '+':
                        token = new Token(Scan('+') ? Tag::INC : Tag::ADD);
                        break;
                    case '-':
                        token = new Token(Scan('-') ? Tag::DEC : Tag::SUB);
                        break;
                    case '*':
                        token = new Token(Tag::MUL);
                        Scan();
                        break;
                    case '/':
                        Scan();
                        if (ch_ == '/') { // 单行注释
                            while (ch_ != '\n' && ch_ != -1) {
                                Scan();
                            }
                            token = new Token(Tag::ERR);
                        }
                        else if (ch_ == '*') { // 多行注释
                            while (!Scan(-1)) {
                                if (ch_ == '*') {
                                    if (Scan('/')) {
                                        break;
                                    }
                                }
                            }
                            
                            if (ch_ == -1) { // 没有正常结束注释
                                // Error
                            }

                            token = new Token(Tag::ERR);
                        } else {
                            token = new Token(Tag::DIV);
                        }
                        break;
                    case '%':
                        token = new Token(Tag::MOD);
                        Scan();
                        break;
                    case '>':
                        token = new Token(Scan('=') ? Tag::GE : Tag::GT);
                        break;
                    case '<':
                        token = new Token(Scan('=') ? Tag::LE : Tag::LT);
                        break;
                    case '=':
                        token = new Token(Scan('=') ? Tag::EQU : Tag::ASSIGN); 
                        break;
                    case '&':
                        token = new Token(Scan('&') ? Tag::ADD : Tag::LEA);
                        break;
                    case '|':
                        token = new Token(Scan('|') ? Tag::OR : Tag::ERR);
                        if (token->tag == Tag::ERR) {
                            // Errro
                        }
                        break;
                    case '!':
                        token = new Token(Scan('=') ? Tag::NEQU : Tag::NOT);
                        break;
                    case ',':
                        token = new Token(Tag::COMMA);
                        break;
                    case ':':
                        token = new Token(Tag::COLON);
                        break;
                    case ';':
                        token = new Token(Tag::SEMICON);
                        break;
                    case '(':
                        token = new Token(Tag::LPAREN);
                        break;
                    case ')':
                        token = new Token(Tag::RPAREN);
                        break;
                    case '[':
                        token = new Token(Tag::LBRACK); 
                        break;
                    case ']':
                        token = new Token(Tag::RBRACK);
                        break;
                    case '{':
                        token = new Token(Tag::LBRACE);
                        break;
                    case '}':
                        token = new Token(Tag::RBRACE);
                        break;
                    case -1:
                        Scan();
                        break;
                    default:
                        token = new Token(Tag::ERR);
                        Scan();
                        break;
                } 
                
            }

            if (token_) {
                delete token_;
                token_ = nullptr;
            }
        
            token_ = token;
            if (token_ && token_->tag != Tag::ERR)  {
                return token_;
            } else {
                continue;
            }
        }


        // 文件结束
        if (token_) {
            delete token_;
        }

        return new Token(Tag::END);
    }

private:
    bool Scan(char need = 0) {
        ch_ = scanner_.Scan();
        if (need) {
            if (ch_ != need) {
                return false;
            }
            ch_ = scanner_.Scan();
            return true;
        }

        return true;
    }

private:
    Scanner& scanner_;
    char ch_ = 0;
    static Keywords keywords_;
    Token* token_ = nullptr;
};

Keywords Lexer::Impl::keywords_;

Lexer::Lexer(Scanner& scanner) {
    impl_.reset(new Impl(scanner));
}

Lexer::~Lexer() {
}

Token* Lexer::Tokenize() {
    return impl_->Tokenize();
}

} // namespace Parser