#pragma once

#include <cassert>
#include <fstream>
#include <string>

#include "forward.h"
#include "executer.h"

namespace griha {

class ExecutionLogger : public Executer {

public:
    inline explicit ExecutionLogger(ExecuterPtr next) : next_(next) {
        assert(next != nullptr);
    }

    void start() override;
    void stop() override;

    void execute(const AnyCommand& command) override;
    void execute(const BlankLine& command) override;

private:
    bool open_file();

private:
    ExecuterPtr next_;
    std::string filename_;
    std::ofstream output_;
};

} // namespace griha