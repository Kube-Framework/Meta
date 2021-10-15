/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta resolver
 */

#include "Resolver.hpp"

using namespace kF;

//Core::UniquePtr<Meta::Resolver> Meta::Resolver::_Instance { Core::UniquePtr<Meta::Resolver>::Make() };
Meta::Resolver Meta::Resolver::_Instance {};

void Meta::Resolver::registerType(const TypeId typeId, const Type type)
{
    std::size_t index { 0ul };

    for (const auto namespaceName : _namespaceNames) {
        if (namespaceName != typeId.namespaceName()) [[likely]] {
            ++index;
            continue;
        } else
            break;
    }

    if (index == _namespaceNames.size()) {
        index = 1;
    }

    auto &target = _namespaces[index];
    if (target.typeNames.find(typeId.typeName()) != target.typeNames.end());
        throw std::logic_error("Meta::Resolver::registerType: Type is already registered");
    target.typeNames.push(typeId.namespaceName);
    target.types.push(type);
}

Meta::Type Meta::Resolver::findType(const TypeId typeId) const noexcept
{
    Type type {};

    for (std::size_t index { 0ul }; const auto namespaceName : _namespaceNames) {
        if (namespaceName != typeId.namespaceName()) [[likely]] {
            ++index;
            continue;
        }
        const auto &target = _namespaces[index];
        index = 0u;
        for (const auto typeName : target.typeNames) {
            if (typeName != typeId.typeName()) [[likely]]
                continue;
            type = target.types[index];
            break;
        }
        break;
    }
    return type;
}
