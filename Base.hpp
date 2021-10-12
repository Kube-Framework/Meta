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

    /** @brief Name of a type (can be a class, a struct or an enum) */
    using TypeName = Core::HashedName;

    /** @brief Name of a type */
    using TypeId = std::uint64_t;


    /** @brief Construct a type id using a namespace name and a type name */
    [[nodiscard]] constexpr TypeId MakeTypeId(const NamespaceName namespaceName, const TypeName typeName) noexcept
        { return (static_cast<std::uint64_t>(namespaceName) << 32ull) | static_cast<std::uint64_t>(typeName); }

    /** @brief Construct a type id only using a type name */
    [[nodiscard]] constexpr TypeId MakeTypeId(const TypeName typeName) noexcept
        { return static_cast<TypeId>(typeName); }

    /** @brief Extract type name from type id */
    [[nodiscard]] constexpr TypeName GetTypeName(const TypeId typeId) noexcept
        { return static_cast<TypeName>(typeId); }

    /** @brief Extract namespace name from type id */
    [[nodiscard]] constexpr NamespaceName GetNamespaceName(const TypeId typeId) noexcept
        { return static_cast<NamespaceName>(typeId >> 32ull); }
}