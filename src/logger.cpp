#include "logger.h"

#include <chrono>

#include "statement.h"

namespace griha {

namespace {

struct LoggerExecuter : Executer {

    std::ostream& output;

    LoggerExecuter(std::ostream& os) : output(os) {}

    void execute(const SomeStatement& stm) override {
        output << stm.value() << std::endl;
    }
};

} // unnamed namespace

void Logger::start_new_log(const std::string& prefix) {
    using namespace std;

    if (output_.is_open()) {
        output_.flush();
        output_.close();
    }

    const auto now = chrono::system_clock::now();
    const auto now_ns = chrono::duration_cast<chrono::nanoseconds>(now.time_since_epoch());
    const auto filename = prefix + to_string(now_ns.count()) + ".log"s;

    output_.open(filename);
}

void Logger::log(StatementPtr stm) {
    LoggerExecuter executer(output_);
    stm->execute(executer);
}

} // namespace griha