#pragma once

#include <iostream>
#include <vector>
#include <memory>

#include "forward.h"

namespace griha {

using CommandContainer = std::vector<CommandPtr>;

class Reader;
struct ReaderSubscriber {
    virtual ~ReaderSubscriber() {}
    virtual void on_block_begin(CommandPtr command) = 0;
    virtual void on_block_end(const CommandContainer& commands) = 0;
};
using ReaderSubscriberPtr = std::shared_ptr<ReaderSubscriber>;

class Reader {
public:
    Reader(CommandFactoryPtr command_factory, size_t block_size);

    ~Reader();

    Reader(Reader&&);
    Reader& operator= (Reader&&);

    // delete copy operations to simplify Reader class
    Reader(const Reader&) = delete;
    Reader& operator=(const Reader&) = delete;

    void subscribe(ReaderSubscriberPtr subscriber);

    void run(std::istream& input);

private:
    std::unique_ptr<struct ReaderImpl> priv_;
};

} // namespace griha