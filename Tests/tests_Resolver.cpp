/**
 * @ Author: Matthieu Moinvaziri
 * @ Description: Resolver
 */

#include <gtest/gtest.h>

#include <Kube/Meta/Meta.hpp>

using namespace kF;

TEST(Resolver, Basics)
{
    ASSERT_NE(Meta::Resolver::Get(), nullptr);
}