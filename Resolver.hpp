/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta resolver
 */

#pragma once

namespace kF::Meta
{
    class Resolver;
}

/** @brief Meta resolver allow to query meta data at runtime */
class kF::Meta::Resolver
{
public:


private:
    inline static Resolver *_Instance;
};