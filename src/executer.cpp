#include "executer.h"

namespace griha {

void StreamExecuter::start() {
    new_block_ = true;
} 

void StreamExecuter::stop() {
    output_ << std::endl;
}

void StreamExecuter::execute(const AnyCommand& command) {
    if (new_block_) {
        output_ << "bulk: ";
        new_block_ = false;
    } else {
        output_ << ", ";
    }

    output_ << command.command();
}

void StreamExecuter::execute([[maybe_unused]] const BlankLine& command) {
    // do nothing
}

} // namespace griha