/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta type
 */

#pragma once

#include <string_view>
#include <typeindex>

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
    struct alignas_double_cacheline Descriptor
    {
        /** @brief Opaque instance type */
        using Instance = void * const;


        /** @brief Signature of a destructor */
        using DestructorFunc = void(*)(Instance instance);

        /** @brief Signature of a default constructor */
        using DefaultConstructorFunc = void(*)(Instance instance);

        /** @brief Signature of a copy constructor */
        using CopyConstructorFunc = void(*)(Instance instance, const Instance other);

        /** @brief Signature of a move constructor */
        using MoveConstructorFunc = void(*)(Instance instance, Instance other);

        /** @brief Signature of a copy assignment */
        using CopyAssignmentFunc = void(*)(Instance instance, const Instance other);

        /** @brief Signature of a move assignment */
        using MoveAssignmentFunc = void(*)(Instance instance, Instance other);


        /** @brief Signature of an unary operator */
        using UnaryFunc = Var(*)(const Instance instance);

        /** @brief Signature of an unary assignment operator */
        using UnaryAssignmentFunc = Var(*)(const Instance instance);

        /** @brief Signature of an unary comparison operator */
        using UnaryComparisonFunc = bool(*)(const Instance instance);


        /** @brief Signature of a binary operator */
        using BinaryFunc = Var(*)(const Instance lhs, const Instance rhs);

        /** @brief Signature of a binary assignment */
        using BinaryAssignmentFunc = void(*)(Instance instance, const Instance other);

        /** @brief Signature of a binary comparison operator */
        using BinaryComparisonFunc = bool(*)(const Instance lhs, const Instance rhs);


        /** @brief Signature of a dereference function */
        using DereferenceFunc = Var(*)(Instance instance);

        /** @brief Signature of a const dereference function */
        using ConstDereferenceFunc = Var(*)(const Instance instance);


        /** @brief Signature of a subscript function */
        using SubscriptFunc = Var(*)(Instance instance, const Instance index);

        /** @brief Signature of a const subscript function */
        using ConstSubscriptFunc = Var(*)(const Instance instance, const Instance index);


        /** @brief Signature of an iterator function (begin / end) */
        using IteratorFunc = Var(*)(Instance instance);

        /** @brief Signature of a const iterator function (begin / end) */
        using ConstIteratorFunc = Var(*)(const Instance instance);


        // Runtime registerable data
        TypeId typeId;                                          // Identifier of the type
        MetaTinyFlatVector<TypeId> baseIds {};                  // Base classes
        MetaTinyFlatVector<FunctionName> functionNames {};      // Functions names
        MetaTinyFlatVector<EnumName> enumNames {};              // Enumeration names
        std::string_view name;                                  // Literal name of the type

        // Compile-time Type meta-data
        const std::type_index typeIndex;                        // Index of the type
        const std::uint32_t typeSize;                           // Size of the type
        const std::uint32_t typeAlignment;                      // Alignment of the type

        // Destructor function
        const DestructorFunc destructorFunc;                    // ~Type(void)

        // Constructor functions
        const DefaultConstructorFunc defaultConstructorFunc;    // Type(void)
        const CopyConstructorFunc copyConstructorFunc;          // Type(const Type &other)
        const MoveConstructorFunc moveConstructorFunc;          // Type(Type &&other)

        // Assignments functions
        const CopyAssignmentFunc copyAssignmentFunc;            // Type &operator=(const Type &other)
        const MoveAssignmentFunc moveAssignmentFunc;            // Type &operator=(Type &&other)

        // Unary functions
        const UnaryFunc bitwiseNotUnaryFunc;                    // Type operator~(void) const
        const UnaryFunc minusUnaryFunc;                         // Type operator-(void) const
        const UnaryFunc incrementSufixUnaryFunc;                // Type operator++(int) const
        const UnaryFunc decrementSufixUnaryFunc;                // Type operator--(int) const

        // Unary Assignment functions
        const UnaryAssignmentFunc incrementPrefixUnaryFunc;     // Type &operator++(void)
        const UnaryAssignmentFunc decrementPrefixUnaryFunc;     // Type &operator--(void)

        // Unary Comparison functions
        const UnaryComparisonFunc boolUnaryFunc;                // operator bool(void) const
        const UnaryComparisonFunc notUnaryFunc;                 // bool operator!(void) const

        // Binary functions
        const BinaryFunc additionBinaryFunc;                    // Type operator+(const Type &other) const
        const BinaryFunc substractionBinaryFunc;                // Type operator-(const Type &other) const
        const BinaryFunc multiplyBinaryFunc;                    // Type operator*(const Type &other) const
        const BinaryFunc divideBinaryFunc;                      // Type operator/(const Type &other) const
        const BinaryFunc moduloBinaryFunc;                      // Type operator%(const Type &other) const
        const BinaryFunc bitwiseAndBinaryFunc;                  // Type operator&(const Type &other) const
        const BinaryFunc bitwiseOrBinaryFunc;                   // Type operator|(const Type &other) const
        const BinaryFunc bitwiseXorBinaryFunc;                  // Type operator^(const Type &other) const
        const BinaryFunc leftShiftBinaryFunc;                   // Type operator<<(const Type &other) const
        const BinaryFunc rightShiftBinaryFunc;                  // Type operator>>(const Type &other) const

        // Binary Assignments functions
        const BinaryAssignmentFunc additionAssignmentFunc;      // Type &operator+=(const Type &other)
        const BinaryAssignmentFunc substractionAssignmentFunc;  // Type &operator-=(const Type &other)
        const BinaryAssignmentFunc divideAssignmentFunc;        // Type &operator/=(const Type &other)
        const BinaryAssignmentFunc moduloAssignmentFunc;        // Type &operator%=(const Type &other)
        const BinaryAssignmentFunc bitwiseAndAssignmentFunc;    // Type &operator&=(const Type &other)
        const BinaryAssignmentFunc bitwiseOrAssignmentFunc;     // Type &operator|=(const Type &other)
        const BinaryAssignmentFunc bitwiseXorAssignmentFunc;    // Type &operator^=(const Type &other)
        const BinaryAssignmentFunc leftShiftAssignmentFunc;     // Type &operator<<=(const Type &other)
        const BinaryAssignmentFunc rightShiftAssignmentFunc;    // Type &operator>>=(const Type &other)

        // Binary Comparison functions
        const BinaryComparisonFunc equalBinaryFunc;             // bool operator==(const Type &other) const
        const BinaryComparisonFunc nequalBinaryFunc;            // bool operator!=(const Type &other) const
        const BinaryComparisonFunc lessBinaryFunc;              // bool operator<(const Type &other) const
        const BinaryComparisonFunc lessEqualBinaryFunc;         // bool operator<=(const Type &other) const
        const BinaryComparisonFunc greaterBinaryFunc;           // bool operator>(const Type &other) const
        const BinaryComparisonFunc greaterEqualBinaryFunc;      // bool operator>=(const Type &other) const

        // Pointer dereference / subscript
        const DereferenceFunc dereferenceFunc;                  // Type &operator*(void)
        const DereferenceFunc constDereferenceFunc;             // const Type &operator*(void) const
        const SubscriptFunc subscriptFunc;                      // Type &operator[](const std::integral index)
        const SubscriptFunc constSubscriptFunc;                 // const Type &operator[](const std::integral index) const

        // Begin / End functions
        const IteratorFunc beginFunc;                           // Iterator begin(void)
        const IteratorFunc endFunc;                             // Iterator end(void)
        const IteratorFunc constBeginFunc;                      // const Iterator begin(void) const
        const IteratorFunc constEndFunc;                        // const Iterator end(void) const


        /** @brief Create a descriptor of a given type */
        template<typename Type>
        static Descriptor Make(void) noexcept;
    };

    static_assert_alignof_double_cacheline(Descriptor);


    /** @brief Destructor */
    ~Type(void) noexcept = default;

    /** @brief Default constructor */
    Type(void) noexcept = default;

    /** @brief Instance constructor */
    Type(const Descriptor * const descriptor) noexcept : _descriptor(descriptor) {}

    /** @brief Copy constructor */
    Type(const Type &other) noexcept = default;

    /** @brief Copy assignment */
    Type &operator=(const Type &other) noexcept = default;


    /** @brief Equality operator */
    [[nodiscard]] bool operator==(const Type &other) const noexcept { return _descriptor == other._descriptor; }

    /** @brief Inequality operator */
    [[nodiscard]] bool operator!=(const Type &other) const noexcept { return _descriptor != other._descriptor; }


    /** @brief Descriptor dereference getter */
    [[nodiscard]] const Descriptor &operator*(void) const noexcept { return *_descriptor; };

    /** @brief Descriptor access getter */
    [[nodiscard]] const Descriptor *operator->(void) const noexcept { return _descriptor; };


    /** @brief Descriptor getter */
    [[nodiscard]] const Descriptor *descriptor(void) const noexcept { return _descriptor; }


private:
    const Descriptor *_descriptor { nullptr };
};

static_assert_fit_eighth_cacheline(kF::Meta::Type);

#include "Type.ipp"