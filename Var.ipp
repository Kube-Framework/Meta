/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta variable
 */

#include "Var.hpp"

force_inline kF::Meta::Var::~Var(void)
{
    destroy<ResetMembers::No>();
}

force_inline kF::Meta::Var::Var(const Var &other)
{
    emplace<const Var &, DestroyInstance::No>(other);
}

force_inline kF::Meta::Var::Var(Var &&other)
{
    emplace<Var &&, DestroyInstance::No>(std::move(other));
}

force_inline const void *kF::Meta::Var::data(void) const noexcept
{
    if (_state != State::Undefined && static_cast<std::uint32_t>(_state) < static_cast<std::uint32_t>(State::Allocation)) [[likely]]
        return data<InstanceOptimized::Yes>();
    else
        return data<InstanceOptimized::No>();
}

template<kF::Meta::Var::InstanceOptimized IsInstanceOptimized>
force_inline const void *kF::Meta::Var::data(void) const noexcept
{
    if constexpr (IsInstanceOptimized == IsInstanceOptimized::Yes)
        return _cache.cacheData();
    else
        return _cache.allocationData();
}

template<kF::Meta::Var::ResetMembers IsResetMembers>
inline void kF::Meta::Var::destroy(void)
{
    switch (_state) {
    case State::Value:
        return destroy<IsResetMembers, InstanceTrivial::No, InstanceOptimized::Yes>();
    case State::Allocation:
        return destroy<IsResetMembers, InstanceTrivial::No, InstanceOptimized::No>();
    case State::TrivialAllocation:
        return destroy<IsResetMembers, InstanceTrivial::Yes, InstanceOptimized::No>();
    default:
        return destroy<IsResetMembers, InstanceTrivial::Yes, InstanceOptimized::Yes>();
    }
}

template<kF::Meta::Var::ResetMembers IsResetMembers, kF::Meta::Var::InstanceTrivial IsInstanceTrivial,
        kF::Meta::Var::InstanceOptimized IsInstanceOptimized, kF::Meta::Var::DeallocateInstance IsDeallocateInstance>
inline void kF::Meta::Var::destroy(void) noexcept(IsInstanceTrivial == kF::Meta::Var::InstanceTrivial::Yes)
{
    static_assert(!(IsDeallocateInstance == DeallocateInstance::No && IsResetMembers == ResetMembers::Yes),
            "Destroy cannot reset members if allocation is not deallocated, it will induce leaks");

    // If the type is optimized, destroy it when its a non-trivial value
    if constexpr (IsInstanceOptimized == InstanceOptimized::Yes) {
        if constexpr (IsInstanceTrivial == InstanceTrivial::No) {
            const auto desc = _type.descriptor();
            const auto instance = data<InstanceOptimized::Yes>();
            if (desc->destructorFunc)
                desc->destructorFunc(instance);
        }
    // Else the type is an allocation, destroy non-trivial types and deallocate if allowed
    } else {
        [[maybe_unused]] const auto instance = data<InstanceOptimized::No>();
        if constexpr (IsInstanceTrivial == InstanceTrivial::No) {
            const auto desc = _type.descriptor();
            if (desc->destructorFunc)
                desc->destructorFunc(instance);
        }
        if constexpr (IsDeallocateInstance == DeallocateInstance::Yes) {
            Deallocate(instance, _cache.allocationSize(), _cache.allocationAlignment());
        }
    }

    if constexpr (IsResetMembers == ResetMembers::Yes) {
        _cache = Cache {};
        _type = Type {};
        _state = State::Undefined;
    }
}

template<typename Type, kF::Meta::Var::DestroyInstance IsDestroyInstance>
inline void kF::Meta::Var::emplace(Type &&other) noexcept(kF::Meta::Var::IsEmplaceNothrow<Type>)
{
    using ConcreteType = std::remove_cvref_t<Type>;

    // The type fits into the optimized cache
    if constexpr (IsOptimized<ConcreteType>) {
        if constexpr (IsDestroyInstance == DestroyInstance::Yes) {
            destroy<ResetMembers::No>();
        }
        std::construct_at<Type>(&_cache, std::forward<Type>(other));
        if constexpr (std::is_trivial_v<Type>) {
            _state = State::TrivialValue;
        } else {
            _state = State::Value;
        }
    // The type needs to be allocated
    } else {
        switch (_state) {
        case State::Undefined:
            break;
        case State::Value:
            destroy<ResetMembers::No, InstanceTrivial::No, InstanceOptimized::Yes>();
            break;
        case State::Allocation:
            destroy<ResetMembers::No, InstanceTrivial::No, InstanceOptimized::No, DeallocateInstance::No>();
            [[fallthrough]];
        case State::TrivialAllocation:
            if (_cache.allocationSize() >= sizeof(ConcreteType) && _cache.allocationAlignment() >= alignof(ConcreteType)) {

            } else
                Deallocate(data<InstanceOptimized::No>)
            break;
        default:
            destroy<ResetMembers::No, InstanceTrivial::Yes, InstanceOptimized::Yes>();
            break;
        }
        switch
        if ((_state == State::Allocation || _state == State::TrivialAllocation) && _cache.allocationSize() >= sizeof(ConcreteType)
                && _cache.allocationAlignment() >= alignof(ConcreteType)) {
        } else
            destroy<ResetMembers::No, InstanceTrivial::

        switch (_state) {
        case State::Allocation:
            destroy<ResetMembers::No, InstanceTrivial::No, InstanceOptimized::No>();
        case State::TrivialAllocation:
            destroy<ResetMembers::No, InstanceTrivial::No, InstanceOptimized::No>();
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