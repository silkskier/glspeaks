
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

#ifndef __yas__types__std__std_array_serializers_hpp
#define __yas__types__std__std_array_serializers_hpp

#include <yas/detail/type_traits/type_traits.hpp>
#include <yas/detail/type_traits/serializer.hpp>
#include <yas/detail/io/serialization_exceptions.hpp>

#include <yas/types/concepts/const_sized_array.hpp>

#include <array>

namespace yas {
namespace detail {

/***************************************************************************/

template<std::size_t F, typename T, std::size_t N>
struct serializer<
	type_prop::not_a_fundamental,
	ser_case::use_internal_serializer,
	F,
	std::array<T, N>
> {
	template<typename Archive>
	static Archive& save(Archive& ar, const std::array<T, N>& array) {
        return concepts::const_sized_array::save<N, F>(ar, array.data(), array.data()+N);
	}

	template<typename Archive>
	static Archive& load(Archive& ar, std::array<T, N>& array) {
        return concepts::const_sized_array::load<N, F>(ar, array.data(), array.data()+N);
	}
};

/***************************************************************************/

} // namespace detail
} // namespace yas

#endif // __yas__types__std__std_array_serializers_hpp
