#ifndef _PARSER_TOKEN_H
#define _PARSER_TOKEN_H

#include <string>
#include <unordered_map>

namespace Parser {

enum class Tag:int {
    ERR,                                // 错误，异常
    END,                                // 文件结束标记
    ID,                                 // 标识符
    KW_INT, KW_CHAR, KW_VOID,           // 数据类型
    KW_EXTERN,                          // extern
    NUM, CH, STR,                       // 常量
    NOT, LEA,                           // 单目运算符 ! & - *
    ADD, SUB, MUL, DIV, MOD,            // 算术运算符
    INC, DEC,                           // 自增自减
    GT, GE, LT, LE, EQU, NEQU,          // 比较运算符
    AND, OR,                            // 逻辑运算
    LPAREN, RPAREN,                     // ()
    LBRACK, RBRACK,                     // []
    LBRACE, RBRACE,                     // {}
    COMMA, COLON, SEMICON,              // 逗号, 冒号, 分号
    ASSIGN,                             // 赋值
    KW_IF, KW_ELSE,                     // if  else 
    KW_SWITCH, KW_CASE, KW_DEFAULT,     // switch case default
    KW_WHILE, KW_DO, KW_FOR,            // 循环
    KW_BREAK, KW_CONTINUE, KW_RETURN    // break, continue, return
};

class Token {
public:
    explicit Token(Tag t);
    virtual std::string ToString();
    virtual ~Token();
    
    Tag tag;
};

class Id: public Token {
public:
    explicit Id(std::string n);
    virtual std::string ToString();

private:
    std::string name;
};

class Num: public Token {
public:
    Num(int v);
    virtual std::string ToString();

private:
    int val;
};

class Char: public Token {
public:
    Char(char ch);
    virtual std::string ToString();

private:
    char ch;
};

class Str: public Token  {
public:
    Str(std::string s);
    virtual std::string ToString();

private:
    std::string str;
};

class Keywords
{
public:
	//hash函数
	struct string_hash {
		size_t operator()(const std::string& str) const{
            std::hash<std::string> hasher;
			return hasher(str);
		}
	};
	std::unordered_map<std::string, Tag, string_hash> keywords;

public:
	Keywords();//关键字列表初始化
	Tag GetTag(std::string name);//测试是否是关键字
};

} // namespace Parser

#endif // _PARSER_TOKEN_H