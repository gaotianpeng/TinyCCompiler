#include "lexer.h"
#include "scanner.h"
#include "token.h"

namespace Parser  {

class Lexer::Impl {
public:
    explicit Impl(Scanner& scanner):scanner_(scanner) {
    }

Token* Tokenize() {
    return nullptr;
}

private:
    bool Scan(char need) {
        ch_ = scanner_.Scan();

        return true;
    }

private:
    Scanner& scanner_;
    char ch_;
    static Keywords keywords_;
};


Lexer::Lexer(Scanner& scanner) {
    impl_.reset(new Impl(scanner));
}

Lexer::~Lexer() {
}

Token* Lexer::Tokenize() {
    return impl_->Tokenize();
}

} // namespace Parser