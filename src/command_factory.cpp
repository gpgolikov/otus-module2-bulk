#include "command_factory.h"

#include "command.h"

namespace griha {

CommandPtr SimpleCommandFactory::create_command(std::string data) {
    if (data.empty())
        return std::make_shared<BlankLine>();
    else
        return std::make_shared<AnyCommand>(std::move(data));
}

} // namespace griha