#pragma once

#include <string>

#include "forward.h"

namespace griha {

struct CommandFactory {
    virtual ~CommandFactory() {};
    virtual CommandPtr create_command(std::string) = 0;
};

struct SimpleCommandFactory : CommandFactory {
    CommandPtr create_command(std::string data) override;
};

} // namespace griha