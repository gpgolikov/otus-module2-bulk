#pragma once

#include "forward.h"

namespace griha {

struct ReaderSubscriber {
    virtual ~ReaderSubscriber() {}
    virtual void on_block_begin() = 0;
    virtual void on_block_end() = 0;
    virtual void on_new_statement(StatementPtr stm) = 0;
};

} // namespace griha