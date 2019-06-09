#pragma once

#include <iostream>

#include "forward.h"
#include "command.h"

namespace griha {

struct Executer {
    virtual ~Executer() {}
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void execute(const AnyCommand&) = 0;
    virtual void execute(const BlankLine&) = 0;
};

class StreamExecuter : public Executer {
public:
    inline explicit StreamExecuter(std::ostream& output) : output_(output) {}

    void start() override;
    void stop() override;

    void execute(const AnyCommand& command) override;
    void execute(const BlankLine& command) override;

private:
    std::ostream& output_;
    bool new_block_ = { false };
};

} // namespace griha