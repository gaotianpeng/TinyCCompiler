#include "token.h"
#include <sstream>

namespace Lexer {

using namespace std;

const char* token_name[] = {
	"error",                        // 错误，异常，结束标记等
	"文件结尾",                      // 文件结束
	"标识符",                        // 标识符
	"int","char","void",            // 数据类型
	"extern",                       // extern
	"数字","字符","字符串",           // 字面量
	"!","&",                        // 单目运算 ! - & *
	"+","-","*","/","%",            // 算术运算符
	"++","--",                      // 自增自减
	">",">=","<","<=","==","!=",    // 比较运算符
	"&&","||",                      // 逻辑运算
	"(",")",                        // ()
	"[","]",                        // []
	"{","}",                        // {}
	",",":",";",                    // 逗号,冒号,分号
	"=",                            // 赋值
	"if","else",                    // if else
	"switch","case","default",      // swicth case deault
	"while","do","for",             // 循环
	"break","continue","return"     // break,continue,return
};

static int TagToInt(Tag t) {
    return static_cast<int>(t);
}

Token::Token(Tag t):tag(t)  {
}

Token::~Token() {
}

std::string Token::ToString() {
    return token_name[TagToInt(tag)];
}

Id::Id(std::string n):Token(Tag::ID), name(n) {
}

std::string Id::ToString() {
    return Token::ToString() + name;
}

Num::Num(int v):Token(Tag::NUM), val(v) {
}

Char::Char(char c):Token(Tag::CH), ch(c){
}

std::string Char::ToString()  {
    stringstream ss;
    ss << ch;
    return string("[") + Token::ToString() + "]:" + ss.str();
}

std::string Num::ToString() {
    std::stringstream ss;
    ss << val;
    return string("[") + Token::ToString() + "]:" + ss.str();
}

Str::Str(std::string s):Token(Tag::STR), str(s) {
}

std::string Str::ToString() {
    return string("[") + Token::ToString() + "]:" + str;
}

Keywords::Keywords() {
    keywords["int"] =       Tag::KW_INT;
    keywords["char"] =      Tag::KW_CHAR;
    keywords["void"] =      Tag::KW_VOID;
    keywords["extern"] =    Tag::KW_EXTERN;
    keywords["if"] =        Tag::KW_IF;
    keywords["else"] =      Tag::KW_ELSE;
    keywords["case"] =      Tag::KW_CASE;
    keywords["default"] =   Tag::KW_DEFAULT;
    keywords["while"] =     Tag::KW_WHILE;
    keywords["do"] =        Tag::KW_DO;
    keywords["for"] =       Tag::KW_FOR;
    keywords["break"] =     Tag::KW_BREAK;
    keywords["continue"] =  Tag::KW_CONTINUE;
    keywords["return"]   =  Tag::KW_RETURN;
}

Tag Keywords::getTag(std::string name) {
    return keywords.find(name) != keywords.end() ? keywords[name] : Tag::ID;
}

} // namespace Lexer