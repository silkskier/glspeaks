
// Copyright (c) 2010-2023 niXman (github dot nixman at pm dot me). All
// rights reserved.
//
// This file is part of YAS(https://github.com/niXman/yas) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#ifndef __yas__detail__tools__limit_hpp
#define __yas__detail__tools__limit_hpp

#include <yas/detail/tools/cast.hpp>
#include <yas/detail/type_traits/type_traits.hpp>
#include <yas/detail/tools/limit_exceptions.hpp>

namespace yas {

/***************************************************************************/

template<typename T>
struct limit_base {
    using type = typename std::conditional<
         std::is_array<typename std::remove_reference<T>::type>::value
        ,typename std::remove_cv<T>::type
        ,typename std::conditional<
             std::is_lvalue_reference<T>::value
            ,T
            ,typename std::decay<T>::type
        >::type
    >::type;
};

template<typename T>
struct max_limit {
    using type = typename limit_base<T>::type;

    max_limit(const max_limit &) = delete;
    max_limit& operator=(const max_limit &) = delete;

    constexpr max_limit(T &&v, std::uint64_t lmax)
        :val(std::forward<type>(v))
        ,lmax(lmax)
    {}
    constexpr max_limit(max_limit &&r)
        :val(std::forward<type>(r.val))
        ,lmax(r.lmax)
    {}

    template<typename L, typename V>
    bool check(const L l, const V &) const {
        return __YAS_SCAST(std::uint64_t, l) <= lmax;
    }

    type val;
    const std::uint64_t lmax;
};

template<typename T>
struct minmax_limit {
    using type = typename limit_base<T>::type;

    minmax_limit(const minmax_limit &) = delete;
    minmax_limit& operator=(const minmax_limit &) = delete;

    constexpr minmax_limit(T &&v, std::uint64_t lmin, std::uint64_t lmax)
        :val(std::forward<type>(v))
        ,lmin(lmin)
        ,lmax(lmax)
    {}
    constexpr minmax_limit(minmax_limit &&r)
        :val(std::forward<type>(r.val))
        ,lmin(r.lmin)
        ,lmax(r.lmax)
    {}

    template<typename L, typename V>
    bool check(const L l, const V &) const {
        return (lmin <= __YAS_SCAST(std::uint64_t, l) && __YAS_SCAST(std::uint64_t, l) <= lmax);
    }

    type val;
    const std::uint64_t lmin;
    const std::uint64_t lmax;
};

/***************************************************************************/

template<typename T>
constexpr max_limit<T> limit(T &&v, std::uint64_t lmax) {
    return {std::forward<T>(v), lmax};
}

template<typename T>
constexpr minmax_limit<T> limit(T &&v, std::uint64_t lmin, std::uint64_t lmax) {
    return {std::forward<T>(v), lmin, lmax};
}

/***************************************************************************/

} // ns yas

#endif // __yas__detail__tools__limit_hpp
