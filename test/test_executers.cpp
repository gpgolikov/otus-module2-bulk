#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>
#include <memory>

#include <command.h>
#include <executer.h>

#include "utils.h"

using namespace std;
using namespace griha;
using namespace Catch;
using namespace Catch::Matchers;

TEST_CASE("StreamExecuter", "[executers]") {
    ostringstream os;
    StreamExecuter executer(os);

    executer.start();
    REQUIRE(os.str().empty());

    executer.execute(AnyCommand("cmd1"));
    REQUIRE_THAT(os.str(), Equals("bulk: cmd1"));

    executer.execute(AnyCommand("cmd2"));
    REQUIRE_THAT(os.str(), Equals("bulk: cmd1, cmd2"));

    executer.execute(BlankLine());
    REQUIRE_THAT(os.str(), Equals("bulk: cmd1, cmd2"));

    executer.execute(AnyCommand("cmd3"));
    REQUIRE_THAT(os.str(), Equals("bulk: cmd1, cmd2, cmd3"));

    executer.stop();
    REQUIRE_THAT(os.str(), Equals("bulk: cmd1, cmd2, cmd3\n"));
}