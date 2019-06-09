#include <catch2/catch.hpp>

#include <iostream>
#include <sstream>
#include <memory>

#include <command.h>
#include <command_factory.h>
#include <reader.h>

#include "utils.h"

using namespace std;
using namespace griha;
using namespace Catch;
using namespace Catch::Matchers;

struct ReaderMonitor : ReaderSubscriber {

    CommandPtr first_command;
    std::vector<std::pair<CommandPtr, CommandContainer>> blocks;

    void clear() {
        blocks.clear();
    }

    void on_block_begin(CommandPtr command) override {
        first_command = command;
    }

    void on_block_end(const CommandContainer& commands) override {
        blocks.emplace_back(first_command, commands);
    }
};

TEST_CASE("Reader", "[reader]") {
    
    Reader reader(make_shared<SimpleCommandFactory>(), 3);
    auto monitor = make_shared<ReaderMonitor>();
    reader.subscribe(monitor);

    SECTION("Fixed blocks") {
        istringstream is;
        is.str(
            "cmd1\n"
            "cmd2\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5\n"
            "cmd6\n"s);

        reader.run(is);

        REQUIRE_THAT(monitor->blocks.size(), Equals(2));
        // check first block
        REQUIRE(monitor->blocks.front().first);
        auto first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.front().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd1"));
        auto& block1 = monitor->blocks.front().second;
        REQUIRE_THAT(block1.size(), Equals(3));
        REQUIRE(block1[0] == first_command);
        auto command = dynamic_pointer_cast<AnyCommand>(block1[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd1"));
        command = dynamic_pointer_cast<AnyCommand>(block1[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd2"));
        command = dynamic_pointer_cast<AnyCommand>(block1[2]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd3"));
        // check second block
        REQUIRE(monitor->blocks.back().first);
        first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.back().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd4"));
        auto& block2 = monitor->blocks.back().second;
        REQUIRE_THAT(block2.size(), Equals(3));
        REQUIRE(block2[0] == first_command);
        command = dynamic_pointer_cast<AnyCommand>(block2[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd4"));
        command = dynamic_pointer_cast<AnyCommand>(block2[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd5"));
        command = dynamic_pointer_cast<AnyCommand>(block2[2]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd6"));
    }

    SECTION("Fixed blocks - EOF before block has been ended") {
        istringstream is;
        is.str(
            "cmd1\n"
            "cmd2\n"
            "cmd3\n"
            "cmd4\n"
            "cmd5"s);

        reader.run(is);

        REQUIRE_THAT(monitor->blocks.size(), Equals(2));
        // check first block
        REQUIRE(monitor->blocks.front().first);
        auto first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.front().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd1"));
        auto& block1 = monitor->blocks.front().second;
        REQUIRE_THAT(block1.size(), Equals(3));
        REQUIRE(block1[0] == first_command);
        auto command = dynamic_pointer_cast<AnyCommand>(block1[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd1"));
        command = dynamic_pointer_cast<AnyCommand>(block1[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd2"));
        command = dynamic_pointer_cast<AnyCommand>(block1[2]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd3"));
        // check second block
        REQUIRE(monitor->blocks.back().first);
        first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.back().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd4"));
        auto& block2 = monitor->blocks.back().second;
        REQUIRE_THAT(block2.size(), Equals(2));
        REQUIRE(block2[0] == first_command);
        command = dynamic_pointer_cast<AnyCommand>(block2[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd4"));
        command = dynamic_pointer_cast<AnyCommand>(block2[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd5"));
    }

    SECTION("Explicit blocks") {
        istringstream is;
        is.str(
            "cmd1\n"
            "cmd2\n"
            "cmd3\n"
            "{\n"
            "cmd4\n"
            "cmd5\n"
            "cmd6\n"
            "cmd7\n"
            "}"s);

        reader.run(is);

        REQUIRE_THAT(monitor->blocks.size(), Equals(2));
        // check first block
        REQUIRE(monitor->blocks.front().first);
        auto first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.front().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd1"));
        auto& block1 = monitor->blocks.front().second;
        REQUIRE_THAT(block1.size(), Equals(3));
        REQUIRE(block1[0] == first_command);
        auto command = dynamic_pointer_cast<AnyCommand>(block1[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd1"));
        command = dynamic_pointer_cast<AnyCommand>(block1[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd2"));
        command = dynamic_pointer_cast<AnyCommand>(block1[2]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd3"));
        // check second block
        REQUIRE(monitor->blocks.back().first);
        first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.back().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd4"));
        auto& block2 = monitor->blocks.back().second;
        REQUIRE_THAT(block2.size(), Equals(4));
        REQUIRE(block2[0] == first_command);
        command = dynamic_pointer_cast<AnyCommand>(block2[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd4"));
        command = dynamic_pointer_cast<AnyCommand>(block2[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd5"));
        command = dynamic_pointer_cast<AnyCommand>(block2[2]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd6"));
        command = dynamic_pointer_cast<AnyCommand>(block2[3]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd7"));
    }

    SECTION("Explicit blocks - fixed block after explicit") {
        istringstream is;
        is.str(
            "cmd1\n"
            "cmd2\n"
            "cmd3\n"
            "{\n"
            "cmd4\n"
            "cmd5\n"
            "cmd6\n"
            "cmd7\n"
            "}\n"
            "cmd8\n"
            "cmd9\n"
            "cmd10"s);

        reader.run(is);

        REQUIRE_THAT(monitor->blocks.size(), Equals(3));
        // check first block
        REQUIRE(monitor->blocks.front().first);
        auto first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.front().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd1"));
        auto& block1 = monitor->blocks.front().second;
        REQUIRE_THAT(block1.size(), Equals(3));
        REQUIRE(block1[0] == first_command);
        auto command = dynamic_pointer_cast<AnyCommand>(block1[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd1"));
        command = dynamic_pointer_cast<AnyCommand>(block1[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd2"));
        command = dynamic_pointer_cast<AnyCommand>(block1[2]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd3"));
        // check second block
        REQUIRE(monitor->blocks[1].first);
        first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks[1].first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd4"));
        auto& block2 = monitor->blocks[1].second;
        REQUIRE_THAT(block2.size(), Equals(4));
        REQUIRE(block2[0] == first_command);
        command = dynamic_pointer_cast<AnyCommand>(block2[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd4"));
        command = dynamic_pointer_cast<AnyCommand>(block2[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd5"));
        command = dynamic_pointer_cast<AnyCommand>(block2[2]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd6"));
        command = dynamic_pointer_cast<AnyCommand>(block2[3]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd7"));
        // check third block
        REQUIRE(monitor->blocks.back().first);
        first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.back().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd8"));
        auto& block3 = monitor->blocks.back().second;
        REQUIRE_THAT(block3.size(), Equals(3));
        REQUIRE(block3[0] == first_command);
        command = dynamic_pointer_cast<AnyCommand>(block3[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd8"));
        command = dynamic_pointer_cast<AnyCommand>(block3[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd9"));
        command = dynamic_pointer_cast<AnyCommand>(block3[2]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd10"));
    }

    SECTION("Explicit blocks - explicit block before block has been ended") {
        istringstream is;
        is.str(
            "cmd1\n"
            "cmd2\n"
            "{\n"
            "cmd4\n"
            "cmd5\n"
            "cmd6\n"
            "cmd7\n"
            "}"s);

        reader.run(is);

        REQUIRE_THAT(monitor->blocks.size(), Equals(2));
        // check first block
        REQUIRE(monitor->blocks.front().first);
        auto first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.front().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd1"));
        auto& block1 = monitor->blocks.front().second;
        REQUIRE_THAT(block1.size(), Equals(2));
        REQUIRE(block1[0] == first_command);
        auto command = dynamic_pointer_cast<AnyCommand>(block1[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd1"));
        command = dynamic_pointer_cast<AnyCommand>(block1[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd2"));
        // check second block
        REQUIRE(monitor->blocks.back().first);
        first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.back().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd4"));
        auto& block2 = monitor->blocks.back().second;
        REQUIRE_THAT(block2.size(), Equals(4));
        REQUIRE(block2[0] == first_command);
        command = dynamic_pointer_cast<AnyCommand>(block2[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd4"));
        command = dynamic_pointer_cast<AnyCommand>(block2[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd5"));
        command = dynamic_pointer_cast<AnyCommand>(block2[2]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd6"));
        command = dynamic_pointer_cast<AnyCommand>(block2[3]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd7"));
    }

    SECTION("Explicit blocks - nested blocks") {
        istringstream is;
        is.str(
            "cmd1\n"
            "cmd2\n"
            "{\n"
            "cmd4\n"
            "cmd5\n"
            "{\n"
            "cmd6\n"
            "cmd7\n"
            "cmd8\n"
            "}\n"
            "cmd9\n"
            "}"s);

        reader.run(is);

        REQUIRE_THAT(monitor->blocks.size(), Equals(2));
        // check first block
        REQUIRE(monitor->blocks.front().first);
        auto first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.front().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd1"));
        auto& block1 = monitor->blocks.front().second;
        REQUIRE_THAT(block1.size(), Equals(2));
        REQUIRE(block1[0] == first_command);
        auto command = dynamic_pointer_cast<AnyCommand>(block1[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd1"));
        command = dynamic_pointer_cast<AnyCommand>(block1[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd2"));
        // check second block
        REQUIRE(monitor->blocks.back().first);
        first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.back().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd4"));
        auto& block2 = monitor->blocks.back().second;
        REQUIRE_THAT(block2.size(), Equals(6));
        REQUIRE(block2[0] == first_command);
        command = dynamic_pointer_cast<AnyCommand>(block2[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd4"));
        command = dynamic_pointer_cast<AnyCommand>(block2[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd5"));
        command = dynamic_pointer_cast<AnyCommand>(block2[2]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd6"));
        command = dynamic_pointer_cast<AnyCommand>(block2[3]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd7"));
        command = dynamic_pointer_cast<AnyCommand>(block2[4]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd8"));
        command = dynamic_pointer_cast<AnyCommand>(block2[5]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd9"));
    }

    SECTION("Explicit blocks - EOF before explicit block has been ended") {
        istringstream is;
        is.str(
            "cmd1\n"
            "cmd2\n"
            "{\n"
            "cmd4\n"
            "cmd5\n"
            "cmd6\n"
            "cmd7"s);

        reader.run(is);

        REQUIRE_THAT(monitor->blocks.size(), Equals(1));
        // check first block
        REQUIRE(monitor->blocks.front().first);
        auto first_command = dynamic_pointer_cast<AnyCommand>(monitor->blocks.front().first);
        REQUIRE(first_command);
        REQUIRE_THAT(first_command->command(), Equals("cmd1"));
        auto& block1 = monitor->blocks.front().second;
        REQUIRE_THAT(block1.size(), Equals(2));
        REQUIRE(block1[0] == first_command);
        auto command = dynamic_pointer_cast<AnyCommand>(block1[0]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd1"));
        command = dynamic_pointer_cast<AnyCommand>(block1[1]);
        REQUIRE(command);
        REQUIRE_THAT(command->command(), Equals("cmd2"));
    }
}