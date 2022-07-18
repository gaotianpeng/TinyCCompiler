#include "scanner.h"

namespace Lexer {

char Scanner::Scan(FILE* fp) {
    if (!fp) {
        return -1;
    }

    char ch = 0;
    if (fscanf(fp, "%c", &ch) != EOF) {
        return ch;
    }

    return -1;
}

} // namespace Lexer