#pragma once

#include <memory>

namespace griha {

struct Command;
using CommandPtr = std::shared_ptr<Command>;

struct CommandFactory;
using CommandFactoryPtr = std::shared_ptr<CommandFactory>;

struct Executer;
using ExecuterPtr = std::shared_ptr<Executer>;

} // namespace griha