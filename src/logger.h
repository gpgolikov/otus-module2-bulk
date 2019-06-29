#pragma once

#include <fstream>
#include <string>

#include "forward.h"

namespace griha {

class Logger {

public:
    void start_new_log(const std::string& prefix);
    void log(StatementPtr stm);

private:
    std::ofstream output_;
};

} // namespace griha