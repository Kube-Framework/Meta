/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta variable
 */

#include "Var.hpp"

inline const void *kF::Meta::Var::data(void) const noexcept
{
    if (_state == State::Undefined) [[unlikely]]
        return nullptr;
    else if (static_cast<std::uint32_t>(_state) < static_cast<std::uint32_t>(State::Allocation)) [[likely]]
        return data<InstanceOptimized::Yes>();
    else
        return data<InstanceOptimized::No>();

    switch (_state) {
    case State::Value:
    case State::TrivialValue:
        return data<InstanceOptimized::Yes>();
    case State::Allocation:
    case State::TrivialAllocation:
    case State::Reference:
    case State::ConstantReference:
        return data<InstanceOptimized::No>();
    default:
        return nullptr;
    }
}

template<kF::Meta::Var::InstanceOptimized IsInstanceOptimized>
inline const void *kF::Meta::Var::data(void) const noexcept
{
    if constexpr (IsInstanceOptimized == IsInstanceOptimized::Yes) {
        return reinterpret_cast<const void *>(&_storage);
    } else {
        return *reinterpret_cast<const void **>(&_storage);
    }
}

template<kF::Meta::Var::ResetMembers ShouldResetMembers>
inline void kF::Meta::Var::destroy(void)
{
    switch (_state) {
    case State::Undefined:
        return destroy<State::Undefined, ShouldResetMembers>();
    case State::Value:
        return destroy<State::Value, ShouldResetMembers>();
    case State::TrivialValue:
        return destroy<State::TrivialValue, ShouldResetMembers>();
    case State::Allocation:
        return destroy<State::Allocation, ShouldResetMembers>();
    case State::TrivialAllocation:
        return destroy<State::TrivialAllocation, ShouldResetMembers>();
    case State::Reference:
        return destroy<State::Reference, ShouldResetMembers>();
    case State::ConstantReference:
        return destroy<State::ConstantReference, ShouldResetMembers>();
    default:
        return;
    }
}

template<kF::Meta::Var::State VarState, kF::Meta::Var::ResetMembers ShouldResetMembers>
inline void kF::Meta::Var::destroy(void)
{
    // If the type is a non-trivial value, destroy it
    if constexpr (VarState == State::Value) {
        const auto desc = _type.descriptor();
        const auto instance = data<VarState>();
        if (desc->destructorFunc)
            desc->destructorFunc(instance);
    // If the type is an allocation, destroy (only non-trivial types) and deallocate it
    } else if constexpr (VarState == State::Allocation || VarState == State::TrivialAllocation) {
        const auto desc = _type.descriptor();
        const auto instance = data<VarState>();
        if constexpr (VarState == State::Allocation) {
            if (desc->destructorFunc)
                desc->destructorFunc(instance);
        }
        Meta::MetaAllocator::Deallocate(instance, desc->typeSize, desc->typeAlignment);
    }

    if constexpr (ShouldResetMembers == ResetMembers::Yes) {
        _type = Type {};
        _state = State::Undefined;
    }
}


template<typename Type, kF::Meta::Var::DestroyInstance ShouldDestroyInstance>
inline void kF::Meta::Var::emplace(Type &&other) noexcept(kF::Meta::Var::IsEmplaceNothrow<Type>)
{
    if constexpr (ShouldDestroyInstance) {
        destroy<ResetMembers::No>();
    }

    // @todo: if we check for movable we can force move !! std::forward doesn't move implicitly by-value arguments !!

    // if constexpr (IsEmplaceMovable<Type>) {
        if constexpr (IsOptimized<Type>) {
            std::construct_at<Type>(&_storage, std::forward<Type>(other));
            if constexpr (std::is_trivial_v<Type>)
                _state = State::TrivialValue;
            else
                _state = State::Value;
        } else {
            std::construct_at<Type>(reinterpret_cast<void *&>(_storage), std::forward<Type>(other));
            if constexpr (std::is_trivial_v<Type>)
                _state = State::TrivialAllocation;
            else
                _state = State::Allocation;
        }
    // } else {

    // }
}