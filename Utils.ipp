/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta utility definitions
 */

template<typename Type>
inline bool kF::Meta::Utils::ToBool(const Type &instance) noexcept
{
    if constexpr (std::is_convertible_v<Type, bool>)
        return static_cast<bool>(instance);
    else
        return instance.operator bool();
}