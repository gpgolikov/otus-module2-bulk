#include "execution_logger.h"

#include <chrono>

namespace griha {

void ExecutionLogger::start() {
    using namespace std;

    const auto now = chrono::system_clock::now();
    filename_ = "bulk"s +
        to_string(chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch()).count()) +
        ".log"s;

    next_->start();
}

void ExecutionLogger::stop() {
    output_.flush();
    output_.close();

    next_->stop();
}

inline void ExecutionLogger::open_file() {
    if (!output_.is_open())
        output_.open(filename_);
}

void ExecutionLogger::execute(const AnyCommand& command) {
    open_file(); // maybe this need states to increase performance
    output_ << "AnyCommand received with command name: " 
            << command.command() << std::endl;

    next_->execute(command);
}

void ExecutionLogger::execute(const BlankLine& command) {
    output_ << "BlankLine command" << std::endl;

    next_->execute(command);    
}

} // namespace griha