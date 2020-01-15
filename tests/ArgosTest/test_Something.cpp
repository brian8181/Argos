//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-01-07.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include <catch2/catch.hpp>
#include "Argos/Argos.hpp"

#include <string>
#include <vector>

struct Argv
{
    Argv(std::initializer_list<std::string> args) : strings(args)
    {
        for (auto& arg : strings)
            argv.push_back(arg.data());
    }

    size_t size() const
    {
        return argv.size();
    }

    char** data()
    {
        return argv.data();
    }

    std::vector<std::string> strings;
    std::vector<char*> argv;
};

TEST_CASE("Test basics")
{
    Argos::Argos argos("test");
    argos.setAutoExit(false);
    REQUIRE(argos.programName() == "test");

    argos.addOption({"-h", "--help"})
            .type(Argos::OptionType::HELP)
            .text("Show help message.");
    Argv argv{"test", "--help"};
    auto result = argos.parse(argv.size(), argv.data());
}
