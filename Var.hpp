/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta variable
 */

#pragma once

#include "Base.hpp"
#include "Type.hpp"

namespace kF::Meta
{
    class Var;
}

/** @brief An opaque meta variable */
class alignas_half_cacheline kF::Meta::Var
{
public:
    struct alignas_quarter_cacheline Cache
    {
        std::uint64_t reg0 { 0ull };
        std::uint64_t reg1 { 0ull };

        /** @brief Destructor */
        constexpr ~Cache(void) noexcept = default;

        /** @brief Default constructor */
        constexpr Cache(void) noexcept = default;

        /** @brief Copy constructor */
        constexpr Cache(const Cache &other) noexcept : reg0(other.reg0), reg1(other.reg1) {}

        /** @brief Copy assignment */
        constexpr Cache &operator=(const Cache &other) noexcept { reg0 = other.reg0; reg1 = other.reg1; }


        /** @brief Get data cache pointer (only valid when the cache contains an optimized type) */
        [[nodiscard]] constexpr void *cacheData(void) noexcept { return reinterpret_cast<void *&>(reg0); }

        /** @brief Get constant data cache pointer (only valid when the cache contains an optimized type) */
        [[nodiscard]] constexpr const void *cacheData(void) const noexcept { return reinterpret_cast<void * const &>(reg0); }


        /** @brief Get data allocation pointer (only valid when the cache contains an allocation pointer in reg0) */
        [[nodiscard]] constexpr void *allocationData(void) noexcept { return reinterpret_cast<void *>(&reg0); }

        /** @brief Get constant data allocation pointer (only valid when the cache contains an allocation pointer in reg0) */
        [[nodiscard]] constexpr const void *allocationData(void) const noexcept { return reinterpret_cast<const void *>(&reg0); }

        /** @brief Get allocation size (only valid when the cache contains an allocation pointer in reg0) */
        [[nodiscard]] constexpr std::uint64_t allocationSize(void) const noexcept { return reg1 & 0xFFFFFFFF; }

        /** @brief Get allocation size (only valid when the cache contains an allocation pointer in reg0) */
        [[nodiscard]] constexpr std::uint64_t allocationAlignment(void) const noexcept { return reg1 >> 32ull; }


        /** @brief Set the allocation meta data */
        constexpr void setAllocation(const void * const data, const std::size_t size, const std::size_t alignment) noexcept
            { reinterpret_cast<const void *&>(reg0) = data; reg1 = size & (alignment << 32ull); }
    };

    static_assert_fit_quarter_cacheline(Cache);

    /** @brief Optimized size usable for instances of variables */
    static constexpr auto OptimizedSize = sizeof(Cache);


    /** @brief State of a variable */
    enum class State : std::uint32_t
    {
        Undefined = 0,
        Value,
        TrivialValue,
        Allocation,
        TrivialAllocation,
        Reference,
        ConstantReference
    };


    /** @brief Reset members template option */
    enum class ResetMembers : bool { No = false, Yes = true };

    /** @brief Destroy instance template option */
    enum class DestroyInstance : bool { No = false, Yes = true };

    /** @brief Instance optimized template option */
    enum class InstanceOptimized : bool { No = false, Yes = true };

    /** @brief Instance trivial template option */
    enum class InstanceTrivial : bool { No = false, Yes = true };

    /** @brief Instance trivial template option */
    enum class DeallocateInstance : bool { No = false, Yes = true };


    /** @brief Check if a type is movable when emplaced into a Var */
    template<typename Type>
    static constexpr bool IsEmplaceMovable = !std::is_lvalue_reference_v<Type>;

    /** @brief Check if a type is nothrow emplacable into a Var */
    template<typename Type>
    static constexpr bool IsEmplaceNothrow = IsEmplaceMovable<Type> ? nothrow_move_constructible(Type) : nothrow_copy_constructible(Type);

    /** @brief Check if a type can be optimized */
    template<typename Type>
    static constexpr bool IsOptimized = sizeof(Type) >= sizeof(OptimizedSize);


    /** @brief Destructor */
    ~Var(void);

    /** @brief Default constructor, initialize to undefined state */
    Var(void) noexcept = default;

    /** @brief Copy constructor, deep-copy other value (may be expensive) */
    Var(const Var &other);

    /** @brief Move constructor, move other value */
    Var(Var &&other);


    /** @brief Copy assignment, deep-copy other value (may be expensive) */
    Var &operator=(const Var &other);

    /** @brief Move assignment, move other value */
    Var &operator=(Var &&other);


    /** @brief Fast check if a variable is undefined */
    [[nodiscard]] operator bool(void) const noexcept { return isUndefined(); }


    /** @brief Fast check if a variable is undefined */
    [[nodiscard]] bool isUndefined(void) const noexcept { return _state == State::Undefined; }


    /** @brief Get the underlying constant data pointer of a variable, safe even if state is undefined */
    [[nodiscard]] const void *data(void) const noexcept;

    /** @brief Get the underlying data pointer of a variable, safe even if state is undefined */
    [[nodiscard]] force_inline void *data(void) noexcept { return const_cast<void *>(const_cast<const Var *>(this)->data()); }

    /** @brief Get the underlying constant data pointer of a variable, unsafe when the state is undefined
     *  @tparam InstanceOptimized Compile-time knowledge of instance being optimized or not */
    template<InstanceOptimized IsInstanceOptimized>
    [[nodiscard]] const void *data(void) const noexcept;

    /** @brief Get the underlying data pointer of a variable, unsafe when the state is undefined
     *  @tparam InstanceOptimized Compile-time knowledge of instance being optimized or not */
    template<InstanceOptimized IsInstanceOptimized>
    [[nodiscard]] force_inline void *data(void) noexcept { return const_cast<void *>(const_cast<const Var *>(this)->data<VarState>()); }


    /** @brief Destroy the variable
     *  @tparam ResetMembers If 'Yes', then member variables are reseted, else the variable is unsafe to use */
    template<ResetMembers IsResetMembers = ResetMembers::Yes>
    void destroy(void);

    /** @brief Destroy the variable
     *  @tparam State Compile-time knowledge of the runtime variable state (unsafe if not exactly equal to current runtime state)
     *  @tparam ResetMembers If 'Yes', then member variables are reseted, else the variable is unsafe to use
     *  @tparam InstanceTrivial If 'Yes', then no destructor is called
     *  @tparam InstanceOptimized If 'Yes', then owned instance treated as an optimized value, else as an allocation
     *  @tparam DeallocateInstance If 'Yes', then any allocation will be deallocated */
    template<ResetMembers IsResetMembers = ResetMembers::Yes, InstanceTrivial IsInstanceTrivial,
            InstanceOptimized IsInstanceOptimized, DeallocateInstance IsDeallocateInstance = DeallocateInstance::Yes>
    void destroy(void) noexcept(IsInstanceTrivial == kF::Meta::Var::InstanceTrivial::Yes);


    /** @brief Emplace a value using another existing one */
    template<typename Type, DestroyInstance IsDestroyInstance>
    void emplace(Type &&other) noexcept(IsEmplaceNothrow<Type>);

private:
    Cache _cache {};
    Type _type {};
    State _state { State::Undefined };

    /** @brief Allocate memory */
    static inline void *Allocate(const std::size_t size, const std::size_t alignment) noexcept
        { return Meta::MetaAllocator::Allocate(size, alignment); }

    /** @brief Deallocate memory */
    static inline void Deallocate(void * const instance, const std::size_t size, const std::size_t alignment) noexcept
        { return Meta::MetaAllocator::Deallocate(instance, size, alignment); }
};

static_assert_fit_half_cacheline(kF::Meta::Var);