/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta type
 */

#include "Type.hpp"
#include "Utils.hpp"

#define ConstexprDetect(IsDetected, Functor) ConstexprTernary(IsDetected, Functor, nullptr)
#define ConstexprUnaryDetect(Operator, Functor) ConstexprTernary((std::is_invocable_v<Operator, Type>), Functor, nullptr)
#define ConstexprBinaryDetect(Operator, Functor) ConstexprTernary((std::is_invocable_v<Operator, Type, Type>), Functor, nullptr)

// @todo: Detect operators, check -> operator and see xstddef


template<typename Type>
inline kF::Meta::Type::Descriptor kF::Meta::Type::Descriptor::Make(void) noexcept
{
    return Descriptor {};
}
//     constexpr auto As = [](Instance ptr) { return *reinterpret_cast<Type *>(ptr); }
//     constexpr auto AsConst = [](const Instance ptr) { return *reinterpret_cast<const Type *>(ptr); }

//     return Descriptor {
//         // Type meta-data
//         typeIndex: typeid(Type),
//         typeSize: sizeof(Type),
//         typeAlignment: alignof(Type),

//         // Destructor function
//         destructorFunc: ConstexprDetect(std::is_destructible_v<Type>, [](Instance instance) { As(instance).~Type(); }),

//         // Constructor functions
//         defaultConstructorFunc: ConstexprDetect(std::is_default_constructible_v<Type>, [](Instance instance) { new (instance) Type(); }),
//         copyConstructorFunc: ConstexprDetect(std::is_copy_constructible_v<Type>, [](Instance instance, const Instance other) { new (instance) Type(AsConst(other)); }),
//         moveConstructorFunc: ConstexprDetect(std::is_move_constructible_v<Type>, [](Instance instance, Instance other) { new (instance) Type(std::move(As(other))); }),

//         // Assignments functions
//         copyAssignmentFunc: ConstexprDetect(std::is_copy_assignable_v<Type>, [](Instance instance, const Instance other) { As(instance) = AsConst(other); }),
//         moveAssignmentFunc: ConstexprDetect(std::is_move_assignable_v<Type>, [](Instance instance, Instance other) { As(instance) = std::move(As(other)); }),

//         // Unary functions
//         bitwiseNotUnaryFunc: [](const Instance instance) { return ~AsConst(instance); },
//         minusUnaryFunc: [](const Instance instance) { return -AsConst(instance); },
//         incrementSufixUnaryFunc: [](const Instance instance) { return AsConst(instance)++; },
//         decrementSufixUnaryFunc: [](const Instance instance) { return AsConst(instance)--; },

//         // Unary Assignment functions
//         incrementPrefixUnaryFunc: [](Instance instance) { return ++As(instance); },
//         decrementPrefixUnaryFunc: [](Instance instance) { return --As(instance); },

//         // Unary Comparison functions
//         boolUnaryFunc: [](const Instance instance) -> bool { return Utils::ToBool(AsConst(instance)); },
//         notUnaryFunc: [](const Instance instance) -> bool { return !Utils::ToBool(AsConst(instance)); },

//         // Binary functions
//         additionBinaryFunc: /*[](const Instance rhs, const Instance lhs)*/ {},
//         substractionBinaryFunc: /*[](const Instance rhs, const Instance lhs)*/ {},
//         multiplyBinaryFunc: /*[](const Instance rhs, const Instance lhs)*/ {},
//         divideBinaryFunc: /*[](const Instance rhs, const Instance lhs)*/ {},
//         moduloBinaryFunc: /*[](const Instance rhs, const Instance lhs)*/ {},
//         bitwiseAndBinaryFunc: /*[](const Instance rhs, const Instance lhs)*/ {},
//         bitwiseOrBinaryFunc: /*[](const Instance rhs, const Instance lhs)*/ {},
//         bitwiseXorBinaryFunc: /*[](const Instance rhs, const Instance lhs)*/ {},
//         leftShiftBinaryFunc: /*[](const Instance rhs, const Instance lhs)*/ {},
//         rightShiftBinaryFunc: /*[](const Instance rhs, const Instance lhs)*/ {},

//         // Binary Assignments functions
//         additionAssignmentFunc: [](Instance instance, const Instance other) { As(instance) += AsConst(other); },
//         substractionAssignmentFunc: [](Instance instance, const Instance other) { As(instance) -= AsConst(other); },
//         multiplyAssignmentFunc: [](Instance instance, const Instance other) { As(instance) *= AsConst(other); },
//         divideAssignmentFunc: [](Instance instance, const Instance other) { As(instance) /= AsConst(other); },
//         moduloAssignmentFunc: [](Instance instance, const Instance other) { As(instance) %= AsConst(other); },
//         bitwiseAndAssignmentFunc: [](Instance instance, const Instance other) { As(instance) &= AsConst(other); },
//         bitwiseOrAssignmentFunc: [](Instance instance, const Instance other) { As(instance) |= AsConst(other); },
//         bitwiseXorAssignmentFunc: [](Instance instance, const Instance other) { As(instance) ^= AsConst(other); },
//         leftShiftAssignmentFunc: [](Instance instance, const Instance other) { As(instance) <<= AsConst(other); },
//         rightShiftAssignmentFunc: [](Instance instance, const Instance other) { As(instance) >>= AsConst(other); },

//         // Binary Comparison functions
//         equalBinaryFunc: [](const Instance lhs, const Instance rhs) -> bool { return AsConst(lhs) == AsConst(rhs); },
//         nequalBinaryFunc: [](const Instance lhs, const Instance rhs) -> bool { return AsConst(lhs) != AsConst(rhs); },
//         lessBinaryFunc: [](const Instance lhs, const Instance rhs) -> bool { return AsConst(lhs) < AsConst(rhs); },
//         lessEqualBinaryFunc: [](const Instance lhs, const Instance rhs) -> bool { return AsConst(lhs) <= AsConst(rhs); },
//         greaterBinaryFunc: [](const Instance lhs, const Instance rhs) -> bool { return AsConst(lhs) > AsConst(rhs); },
//         greaterEqualBinaryFunc: [](const Instance lhs, const Instance rhs) -> bool { return AsConst(lhs) >= AsConst(rhs); },

//         // Pointer dereference / subscript
//         dereferenceFunc: /*[](Instance instance)*/ {},
//         constDereferenceFunc: /*[](const Instance instance)*/ {},
//         subscriptFunc: /*[](Instance instance)*/ {},
//         constSubscriptFunc: /*[](const Instance instance)*/ {},

//         // Begin / End functions
//         beginFunc: [](Instance instance) { return std::begin(As(instance)); },
//         endFunc: [](Instance instance) { return std::end(As(instance)); },
//         constBeginFunc: [](const Instance instance) { return std::begin(AsConst(instance)); },
//         constEndFunc: [](const Instance instance) { return std::end(AsConst(instance)); }
//     };
// }

#undef ConstexprDetect