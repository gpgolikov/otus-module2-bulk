#pragma once

#include <string>

#include "forward.h"

namespace griha {

struct Command {
    virtual ~Command() {};
    virtual void execute(Executer&) = 0;
};

class AnyCommand : public Command {

public:
    inline explicit AnyCommand(std::string command) : command_(std::move(command)) {}

    void execute(Executer& ex_ctx) override;

    inline const std::string& command() const { return command_; }

private:
    std::string command_;
};

class BlankLine : public Command {

public:
    void execute(Executer& ex_ctx) override;
};

} // namespace griha