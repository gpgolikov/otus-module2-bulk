#include "command.h"

#include "executer.h"

namespace griha {

void AnyCommand::execute(Executer& ex_ctx) {
    ex_ctx.execute(*this);
}

void BlankLine::execute(Executer& ex_ctx) {
    ex_ctx.execute(*this);
}

} // namespace griha