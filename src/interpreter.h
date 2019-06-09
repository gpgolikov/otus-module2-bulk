#pragma once

#include <iostream>

#include "forward.h"

namespace griha {

class Interpreter {
public:
    struct Context {
        ExecuterPtr executer;
        CommandFactoryPtr command_factory;
        size_t block_size;
    };

public:
    inline explicit Interpreter(Context context)
        : context_(std::move(context)) {}

    void run(std::istream& input);

private:
    Context context_;
};

} // namespace griha