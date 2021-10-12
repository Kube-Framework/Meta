/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta variable
 */

#pragma once

namespace kF::Meta
{
    class Var;
}

/** @brief An opaque meta variable */
class alignas_half_cacheline kF::Meta::Var
{
public:


private:

};

static_assert_fit_half_cacheline(kF::Meta::Var);
