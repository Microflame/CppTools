#pragma once

#include "exception.hpp"

namespace vtools
{
namespace detail
{

#define VTOOLS_DEFER ::vtools::detail::DeferImpl VTOOLS_CONCAT(defer_, __LINE__) = [&]()
template <typename F> 
struct DeferImpl {
    DeferImpl(F f): fn(f) {}
    ~DeferImpl() noexcept(false) { fn(); }
    F fn;
};

} // namespace detail
} // namespace vtools
