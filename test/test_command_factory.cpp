#include <catch2/catch.hpp>

#include <memory>

#include <command.h>
#include <command_factory.h>

#include "utils.h"

using namespace std;
using namespace griha;
using namespace Catch;
using namespace Catch::Matchers;

TEST_CASE("SimpleCommandFactory", "[command]") {
    SimpleCommandFactory factory;

    auto command = factory.create_command("");
    REQUIRE(dynamic_pointer_cast<BlankLine>(command));

    command = factory.create_command("cmd1");
    REQUIRE(dynamic_pointer_cast<AnyCommand>(command));
}
