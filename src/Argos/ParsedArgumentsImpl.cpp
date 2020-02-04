//****************************************************************************
// Copyright © 2020 Jan Erik Breimo. All rights reserved.
// Created by Jan Erik Breimo on 2020-01-07.
//
// This file is distributed under the BSD License.
// License text is included with the source distribution.
//****************************************************************************
#include "ParsedArgumentsImpl.hpp"

#include "Argos/ArgosException.hpp"

namespace Argos
{
    namespace
    {
        template <typename It, typename Value, typename IsLess>
        It lowerBound(It begin, It end, Value&& v, IsLess isLess)
        {
            while (begin != end)
            {
                auto offset = std::distance(begin, end) / 2;
                auto mid = begin + offset;
                if (isLess(*mid, v))
                    begin = mid + 1;
                else
                    end = mid;
            }
            return begin;
        }

    }

    ParsedArgumentsImpl::ParsedArgumentsImpl(std::shared_ptr<ParserData> data)
        : m_Data(move(data))
    {
        assert(m_Data);
        for (auto& a : m_Data->arguments)
        {
            m_ValueIds.emplace_back(a->name, a->valueId_);
            if (!a->valueName.empty())
                m_ValueIds.emplace_back(a->valueName, a->valueId_);
        }
        for (auto& o : m_Data->options)
        {
            for (auto& f : o->flags)
                m_ValueIds.emplace_back(f, o->valueId_);
            if (!o->valueName.empty())
                m_ValueIds.emplace_back(o->valueName, o->valueId_);
        }
        sort(m_ValueIds.begin(), m_ValueIds.end());
        m_ValueIds.erase(unique(m_ValueIds.begin(), m_ValueIds.end()),
                         m_ValueIds.end());
    }

    bool ParsedArgumentsImpl::has(int valueId) const
    {
        return m_Values.find(valueId) != m_Values.end();
    }

    const std::vector<std::string>& ParsedArgumentsImpl::arguments() const
    {
        return m_Arguments;
    }

    void ParsedArgumentsImpl::addArgument(const std::string& arg)
    {
        m_Arguments.push_back(arg);
    }

    void ParsedArgumentsImpl::addUnprocessedArgument(const std::string& arg)
    {
        m_UnprocessedArguments.push_back(arg);
    }

    void ParsedArgumentsImpl::assignValue(int valueId, const std::string& value)
    {
        auto it = m_Values.lower_bound(valueId);
        if (it == m_Values.end() || it->first != valueId)
        {
            m_Values.emplace(valueId, value);
            return;
        }

        it->second = value;
        ++it;
        while (it != m_Values.end() && it->first == valueId)
            m_Values.erase(it++);
    }

    void ParsedArgumentsImpl::appendValue(int valueId, const std::string& value)
    {
        m_Values.emplace(valueId, value);
    }

    void ParsedArgumentsImpl::clearValue(int valueId)
    {
        m_Values.erase(valueId);
    }

    int ParsedArgumentsImpl::getValueId(std::string_view valueName) const
    {
        auto idIt = lowerBound(m_ValueIds.begin(), m_ValueIds.end(),
                               valueName,
                               [](auto& p, auto& s) {return p.first < s;});
        if (idIt == m_ValueIds.end() || idIt->first != valueName)
            ARGOS_THROW("Unknown value: " + std::string(valueName));
        return idIt->second;
    }

    std::optional<std::string_view> ParsedArgumentsImpl::getValue(int valueId) const
    {
        auto it = m_Values.lower_bound(valueId);
        if (it == m_Values.end() || it->first != valueId)
            return {};
        auto nx = next(it);
        if (nx != m_Values.end() && nx->first == valueId)
            ARGOS_THROW("Attempt to read multiple values as a single value.");
        return it->second;
    }

    std::vector<std::string_view> ParsedArgumentsImpl::getValues(int valueId) const
    {
        std::vector<std::string_view> result;
        auto it = m_Values.lower_bound(valueId);
        for (; it != m_Values.end() && it->first == valueId; ++it)
            result.emplace_back(it->second);
        return result;
    }

    const std::shared_ptr<ParserData>& ParsedArgumentsImpl::parserData() const
    {
        return m_Data;
    }

    ParserResultCode ParsedArgumentsImpl::resultCode() const
    {
        return m_ResultCode;
    }

    void ParsedArgumentsImpl::setResultCode(ParserResultCode resultCode)
    {
        m_ResultCode = resultCode;
    }

    const OptionData* ParsedArgumentsImpl::specialOption() const
    {
        return m_SpecialOption;
    }

    void ParsedArgumentsImpl::setSpecialOption(const OptionData* option)
    {
        m_ResultCode = ParserResultCode::SPECIAL_OPTION;
        m_SpecialOption = option;
    }
}
