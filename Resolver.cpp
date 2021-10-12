/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Meta resolver
 */

#include "Resolver.hpp"

using namespace kF;

inline static Meta::Resolver *Meta::Resolver::_Instance { std::make_unique<Meta::Resolver>() };
