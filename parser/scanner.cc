#include "scanner.h"
#include "common.h"

namespace Parser {

const int BUF_LEN = 80;

class Scanner::Impl {
public:
    char Scan(FILE* fp) {
        if (!fp) {
            return FILE_ERROR;
        }
        
        if (read_pos == line_len - 1) {
            line_len = fread(buffer_, 1, BUF_LEN, fp);
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
    char ScanTest(FILE* fp) {
        if (!fp) {
            return FILE_ERROR;
        }

        char ch = 0;
        if (fscanf(fp, "%c", &ch) != EOF) {
            return ch;
        }

        return FILE_END;
    }

private:
    char buffer_[BUF_LEN];
    int line_len = 0;
    int read_pos = -1;
};

Scanner::Scanner()  {
    impl_.reset(new Impl());
}

char Scanner::Scan(FILE* fp) {
    return impl_->Scan(fp);
}

char Scanner::ScanTest(FILE* fp) {
    return impl_->ScanTest(fp);
}

} // namespace Parser