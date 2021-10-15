/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta utility definitions
 */

#pragma once

namespace kF::Meta::Utils
{
    /** @brief Template type to boolean */
    template<typename Type>
    [[nodiscard]] bool ToBool(const Type &instance) noexcept;
}

#include "Utils.ipp"