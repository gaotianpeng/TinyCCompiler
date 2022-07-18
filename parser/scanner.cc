#include "scanner.h"
#include "common.h"

namespace Parser {

const int BUF_LEN = 80;

class Scanner::Impl {
public:
    Impl(FILE* fp):fp_(fp) {
    }

    char Scan() {
        if (!fp_) {
            return FILE_ERROR;
        }
        
        if (read_pos == line_len - 1) {
            line_len = fread(buffer_, 1, BUF_LEN, fp_);
            if (line_len == 0) {
                return FILE_END;
            }
            read_pos = -1;
        }

        ++read_pos;
        return buffer_[read_pos];
    }

    /*
        for test
    */
    char ScanTest() {
        if (!fp_) {
            return FILE_ERROR;
        }

        char ch = 0;
        if (fscanf(fp_, "%c", &ch) != EOF) {
            return ch;
        }

        return FILE_END;
    }

private:
    char buffer_[BUF_LEN];
    int line_len = 0;
    int read_pos = -1;
    FILE* fp_;
};

Scanner::Scanner(FILE* fp)  {
    impl_.reset(new Impl(fp));
}

char Scanner::Scan() {
    return impl_->Scan();
}

char Scanner::ScanTest() {
    return impl_->ScanTest();
}

} // namespace Parser