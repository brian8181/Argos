//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-01-29.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#pragma once
#include <string>
#include <optional>
#include "IArgumentView.hpp"

namespace Argos
{
    class ParsedArgumentsImpl;

    class ParsedArgumentsBuilder
    {
    public:
        explicit ParsedArgumentsBuilder(ParsedArgumentsImpl* impl);

        ParsedArgumentsBuilder& append(const std::string& name,
                                       const std::string& value);

        ParsedArgumentsBuilder& append(const IArgumentView& arg,
                                       const std::string& value);

        ParsedArgumentsBuilder& assign(const std::string& name,
                                       const std::string& value);

        ParsedArgumentsBuilder& assign(const IArgumentView& arg,
                                       const std::string& value);

        ParsedArgumentsBuilder& clear(const std::string& name);

        ParsedArgumentsBuilder& clear(const IArgumentView& arg);

        std::optional<std::string> get(const std::string& name);

        std::optional<std::string> get(const IArgumentView& arg);

        std::vector<std::string> getVector(const std::string& name);

        std::vector<std::string> getVector(const IArgumentView& arg);

        bool has(const std::string& name);

        bool has(const IArgumentView& arg);

        void error(const std::string& errorMessage);
    private:
        ParsedArgumentsImpl* m_Impl;
    };
}
