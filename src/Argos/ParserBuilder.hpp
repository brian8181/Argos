//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-01-13.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include "ArgumentBuilder.hpp"
#include "ArgumentParser.hpp"
#include "OptionBuilder.hpp"

namespace Argos
{
    class ParserBuilder
    {
    public:
        ArgumentBuilder addArgument(const std::string& name);

        OptionBuilder addOption(const std::string& flag);

        ArgumentParser makeParser() const;
    private:
        std::vector<Argument> m_Arguments;
        std::vector<Option> m_Options;
    };
}
