#include "interpreter.h"

#include <memory>

#include "reader.h"
#include "executer.h"

namespace griha {

template <typename FnBlockBegin, typename FnBlockEnd>
struct InterpreterSubscriber : ReaderSubscriber {

    FnBlockBegin fn_block_begin;
    FnBlockEnd fn_block_end;

    InterpreterSubscriber(FnBlockBegin fn_bb, FnBlockEnd fn_be)
        : fn_block_begin(std::move(fn_bb))
        , fn_block_end(std::move(fn_be)) {}

    void on_block_begin(CommandPtr command) override {
        fn_block_begin(command);
    }

    void on_block_end(const CommandContainer& commands) override {
        fn_block_end(commands);
    }
};

void Interpreter::run(std::istream& input) {
    Reader reader(context_.command_factory, context_.block_size);

    InterpreterSubscriber subscr {
        [&]([[maybe_unused]] CommandPtr command) {
            context_.executer->start();            
        },
        [&](const CommandContainer& commands) {
            for (auto& cmd : commands) {
                cmd->execute(*context_.executer);
            }
            context_.executer->stop();
        }
    };
    reader.subscribe(std::make_shared<decltype(subscr)>(std::move(subscr)));

    reader.run(input);
}

} // namespace griha