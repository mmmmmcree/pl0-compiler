#pragma once

#include "Token.h"
#include "Definition.h"

namespace pl0 {
    class Identifier
    {
    public:
        Identifier() = default;
        Identifier(IdentifierType type, std::string_view name);
        bool is(IdentifierType type) const;
        const std::string &getName() const;
        int getValue() const;
        bool initialize(int value);
        bool initialize(std::string_view value);
        bool assign(int value);
        bool assign(std::string_view value);
    private:
        IdentifierType m_type;
        std::string m_name;
        int m_value;
    };

    using IdentifierMap = std::unordered_map<std::string, Identifier>;
}