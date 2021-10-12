/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta type
 */

#pragma once

#include <Kube/Core/Utils.hpp>
#include <Kube/Core/SmallVector.hpp>

#include "Base.hpp"
#include "Var.hpp"

namespace kF::Meta
{
    class Type;
    class TypeRef;
}

/** @brief A meta type is an opaque type descriptor, registered for runtime operations */
class alignas_eighth_cacheline kF::Meta::Type
{
public:
    /** @brief Descriptor structure of a type */
    struct Descriptor
    {
        using DestructorFunc = void(*)(void * const instance);
        using ConstructorFunc = void(*)(void * const instance, void * const arguments, const std::size_t argumentCount);
        using CopyConstructorFunc = void(*)(void * const instance, void * const other);
        using MoveConstructorFunc = void(*)(void * const instance, void * const other);

        const TypeId typeId;
        const DestructorFunc destructorFunc;
        const ConstructorFunc constructorFunc;
        const CopyConstructorFunc copyConstructorFunc;
        const MoveConstructorFunc moveConstructorFunc;

        MetaTinyFlatVector<TypeId> bases {};
        MetaTinyFlatVector<TypeId> constructors {};
        MetaTinyFlatVector<TypeId> functions {};
        MetaTinyFlatVector<TypeId> enums {};
    };


    /** @brief Default constructor */
    Type(void) noexcept = default;

    /** @brief Instance constructor */
    Type(const Descriptor * const descriptor) noexcept : _descriptor(descriptor) {}

    /** @brief Copy constructor */
    Type(const Type &other) noexcept = default;

    /** @brief Move constructor */
    Type(Type &&other) noexcept = default;

    /** @brief Destructor */
    ~Type(void) noexcept = default;

    /** @brief Copy assignment */
    Type &operator=(const Type &other) noexcept = default;

    /** @brief Move assignment */
    Type &operator=(Type &&other) noexcept = default;


private:
    const Descriptor *_descriptor { nullptr };
};

static_assert_fit_eighth_cacheline(kF::Meta::Type);