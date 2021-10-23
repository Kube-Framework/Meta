/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta base types and definitions
 */

#pragma once

#include <memory_resource>

#include <Kube/Core/Utils.hpp>
#include <Kube/Core/Hash.hpp>
#include <Kube/Core/UnsafeAllocator.hpp>
#include <Kube/Core/AllocatedFlatVector.hpp>

namespace kF::Meta
{
    /** @brief Allocator of the meta library, used to provide a good cache locality between allocations*/
    using MetaAllocator = Core::UnsafeAllocator<Core::Hash("Meta")>;

    /** @brief A flat vector that is allocated using the meta allocator */
    template<typename Type, std::integral Range>
    using MetaFlatVector = Core::AllocatedFlatVector<Type, &MetaAllocator::Allocate, &MetaAllocator::Deallocate, Range>;

    /** @brief A flat vector that is allocated using the meta allocator */
    template<typename Type>
    using MetaTinyFlatVector = Core::AllocatedTinyFlatVector<Type, &MetaAllocator::Allocate, &MetaAllocator::Deallocate>;


    /** @brief Name of a namespace */
    using NamespaceName = Core::HashedName;

    /** @brief Name of a type (class / struct) */
    using TypeName = Core::HashedName;

    /** @brief Name of a function (static / member) */
    using FunctionName = Core::HashedName;

    /** @brief Name of an enumeration */
    using EnumName = Core::HashedName;

    class TypeId;
}

/** @brief Identifier of a type */
class kF::Meta::TypeId
{
public:
    /** @brief Destructor */
    ~TypeId(void) noexcept = default;

    /** @brief Constructor */
    TypeId(void) noexcept = default;

    /** @brief Copy constructor */
    TypeId(const TypeId &other) noexcept = default;

    /** @brief Initialization constructor */
    TypeId(const NamespaceName namespaceName, const TypeName typeName) noexcept
        : _namespaceName(namespaceName), _typeName(typeName) {}

    /** @brief Copy assignment */
    TypeId &operator=(const TypeId &other) noexcept = default;


    /** @brief Equality operator */
    [[nodiscard]] bool operator==(const TypeId &other) const noexcept { return value() == other.value(); }

    /** @brief Inequality operator */
    [[nodiscard]] bool operator!=(const TypeId &other) const noexcept { return value() != other.value(); }


    /** @brief Namespace name getter */
    [[nodiscard]] NamespaceName namespaceName(void) const noexcept { return _namespaceName; }

    /** @brief Type name getter */
    [[nodiscard]] TypeName typeName(void) const noexcept { return _typeName; }

    /** @brief Integral value representation getter */
    [[nodiscard]] const std::uint64_t &value(void) const noexcept { return *reinterpret_cast<const std::uint64_t *>(this); }

private:
    NamespaceName _namespaceName;
    TypeName _typeName;
};