/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta resolver
 */

#pragma once

#include <array>

#include <Kube/Core/UniquePtr.hpp>

#include "Type.hpp"

#ifndef KUBE_META_NAMESPACE_COUNT
/** @brief KUBE_META_NAMESPACE_COUNT indicates the maximum number of namespaces available in the meta resolver
 *  You may change this value if the default is not enough */
# define KUBE_META_NAMESPACE_COUNT 8
#endif

namespace kF::Meta
{
    class Resolver;
}

/** @brief Meta resolver allow to query meta data at runtime */
class alignas_double_cacheline kF::Meta::Resolver
{
public:
    /** @brief Indicates the maximum number of namespaces available in the meta resolver
     *  You may change the KUBE_META_NAMESPACE_COUNT this value if the default is not enough */
    static constexpr std::size_t NamespaceCount = KUBE_META_NAMESPACE_COUNT;

    static_assert(NamespaceCount > 1, "KUBE_META_NAMESPACE_COUNT must be higher than 1");

    /** @brief Contains all the data of a namespace */
    struct Namespace
    {
        MetaTinyFlatVector<TypeName> typeNames {};
        MetaTinyFlatVector<Type> types {};
    };


    /** @brief Get the unique Resolver instance */
    [[nodiscard]] static inline Resolver *Get(void) noexcept { return &_Instance; }


    /** @brief Destructor */
    ~Resolver(void) noexcept = default;


    /** @brief Register a type in the resolver */
    void registerType(const TypeId typeId, const Type type);


    /** @brief Find a meta type registered by the resolver using a TypeId */
    [[nodiscard]] Type findType(const TypeId typeId) const noexcept;


private:
    std::array<NamespaceName, NamespaceCount> _namespaceNames {};
    std::array<Namespace, NamespaceCount> _namespaces {};

    static Resolver _Instance;

    /** @brief Constructor */
    Resolver(void) noexcept = default;
};

static_assert_alignof_double_cacheline(kF::Meta::Resolver);