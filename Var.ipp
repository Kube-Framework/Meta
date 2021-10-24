/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta variable
 */

#include "Var.hpp"

inline kF::Meta::~Var(void)
{
    destroy<ResetMembers::No>();
}

inline kF::Meta::Var(const Var &other)
{
    emplace<DestroyInstance::No>(other);
}

inline kF::Meta::Var(Var &&other)
{
    emplace<DestroyInstance::No>(std::move(other));
}

inline const void *kF::Meta::Var::data(void) const noexcept
{
    const auto s = static_cast<std::uint32_t>(_state);
    if (s != State::Undefined && s < static_cast<std::uint32_t>(State::Allocation)) [[likely]]
        return data<InstanceOptimized::Yes>();
    else
        return data<InstanceOptimized::No>();
}

template<kF::Meta::Var::InstanceOptimized IsInstanceOptimized>
inline const void *kF::Meta::Var::data(void) const noexcept
{
    if constexpr (IsInstanceOptimized == IsInstanceOptimized::Yes) {
        return reinterpret_cast<const void *>(&_cache);
    } else {
        return reinterpret_cast<const void *&>(_cache);
    }
}

template<kF::Meta::Var::ResetMembers ShouldResetMembers>
inline void kF::Meta::Var::destroy(void)
{
    switch (_state) {
    case State::Value:
        return destroy<ShouldResetMembers, InstanceTrivial::No, InstanceAllocated::No>();
    case State::Allocation:
        return destroy<ShouldResetMembers, InstanceTrivial::No, InstanceAllocated::Yes>();
    case State::TrivialAllocation:
        return destroy<ShouldResetMembers, InstanceTrivial::Yes, InstanceAllocated::Yes>();
    default:
        return destroy<ShouldResetMembers, InstanceTrivial::Yes, InstanceAllocated::No>();
    }
}

template<kF::Meta::Var::ResetMembers ShouldResetMembers, kF::Meta::Var::InstanceTrivial IsInstanceTrivial, kF::Meta::Var::InstanceAllocated IsInstanceAllocated>
inline void kF::Meta::Var::destroy(void) noexcept(IsInstanceTrivial == kF::Meta::Var::InstanceTrivial::Yes)
{
    // If the type is a non-trivial value, destroy it
    if constexpr (IsInstanceAllocated == InstanceAllocated::No) {
        if constexpr (IsInstanceTrivial == InstanceTrivial::No) {
            const auto desc = _type.descriptor();
            const auto instance = data<VarState>();
            if (desc->destructorFunc)
                desc->destructorFunc(instance);
        }
    // If the type is an allocation, destroy (only non-trivial types) and deallocate it
    } else {
        const auto desc = _type.descriptor();
        const auto instance = data<VarState>();
        if constexpr (IsInstanceTrivial == InstanceTrivial::No) {
            if (desc->destructorFunc)
                desc->destructorFunc(instance);
        }
        Deallocate(instance, desc->typeSize, desc->typeAlignment);
    }

    if constexpr (ShouldResetMembers == ResetMembers::Yes) {
        _type = Type {};
        _state = State::Undefined;
    }
}

template<typename Type, kF::Meta::Var::DestroyInstance ShouldDestroyInstance>
inline void kF::Meta::Var::emplace(Type &&other) noexcept(kF::Meta::Var::IsEmplaceNothrow<Type>)
{
    using ConcreteType = std::remove_cvref_t<Type>;

    // The type fits into the optimized cache
    if constexpr (IsOptimized<Type>) {
        if constexpr (ShouldDestroyInstance == ShouldDestroyInstance::Yes) {
            destroy<ResetMembers::No>();
        }
        std::construct_at<Type>(&_cache, std::forward<Type>(other));
        if constexpr (std::is_trivial_v<Type>)
            _state = State::TrivialValue;
        else
            _state = State::Value;
    // The type needs to be allocated
    } else {
        if ((_state == State::Allocation || _state == State::TrivialAllocation) && _cache.reg1) [[likely]] {
            if (_cache.allocationSize() >= sizeof(ConcreteType) && _cache.allocationAlignment() >= alignof(ConcreteType)) {

            }
        }

        switch (_state) {
        case State::Allocation:
            destroy<ResetMembers::No, InstanceTrivial::No, InstanceAllocated::Yes>();
        case State::TrivialAllocation:
            destroy<ResetMembers::No, InstanceTrivial::No, InstanceAllocated::Yes>();
        default:

        }
        std::construct_at<Type>(reinterpret_cast<void *&>(_cache), std::forward<Type>(other));
        if constexpr (std::is_trivial_v<Type>)
            _state = State::TrivialAllocation;
        else
            _state = State::Allocation;
    }

    // @todo: if we check for movable we can force move !! std::forward doesn't move implicitly by-value arguments !!

    // if constexpr (IsEmplaceMovable<Type>) {
    // } else {

    // }
}