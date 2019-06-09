#include "execution_logger.h"

#include <chrono>

namespace griha {

void ExecutionLogger::start() {
    using namespace std;

    const auto now = chrono::system_clock::now();
    filename_ = "bulk"s +
        to_string(chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch()).count());

    next_->start();
}

void ExecutionLogger::stop() {
    output_.flush();
    output_.close();

    next_->stop();
}

inline bool ExecutionLogger::open_file() {
    if (!output_.is_open()) {
        output_.open(filename_);
        output_ << "bulk:";
        return true;
    }
    return false;
}

void ExecutionLogger::execute(const AnyCommand& command) {
    if (!open_file())
        output_ << ',';
    output_ << " " << command.command();

    next_->execute(command);
}

void ExecutionLogger::execute(const BlankLine& command) {
    next_->execute(command);    
}

} // namespace griha