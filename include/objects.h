#pragma once

#include <utility>

namespace pathways {

// This file defines the basic interface required of any given *object* in
// order for the pathway assembly algorithms to do their thing. This interface
// is designed to allow you to implement it for any type, regardless of whether
// or not you have direct access to it's internals.
//
// Each of the following functions have default definitions which use methods
// defined on their objects. If you are using a type which you cannot directly
// modify, these functions can be specialized for your type.
//
// First, you need to be able to determine if the object is "basic". This is
// something of a terminating condition for the various algorithms.
//
// If you are creating a custom type, then implement a
// ```cpp
// auto is_basic() const -> bool;
// ```
// method and you are covered.
template <typename T>
auto is_basic(T const& x) -> bool {
	return x.is_basic();
}

// Second, you need to be able to tell if one object is "below" another. That
// is to say, is on object a substructure of another. This is used to determine
// how the coassembly recursion will progress.
//
// If youare creating a custom type, then implement a
// ```cpp
// // `this` is below `other`
// auto is_below(T const& other) -> bool;
// ```
// method and you are covered.
template <typename T>
auto is_below(T const& x, T const& y) -> bool {
	return x.is_below(y);
}

// Third, objects are disassembled into pairs of components. The result of this
// disassembly process is anything that is iterable and whose iterator can be
// dereferenced to a `Components<T>` defined below. For example, this function
// could return an array or vector containing `Components<T>` instances, or it
// could be an object which can lazily construct those pairs.In order for the
// algorithms to generalize over this you'll need to tell it what type that
// will by. Something like
//
// ```cpp
// template<>
// struct disassembly_type<YourType> {
//     using value = YourDisassemblyType;
// };
// ```
//
// If you are creating a custom type, you need only provide a public typedef such as
// ```cpp
// using value = YourDisassemblyType;
// ```
// within our class/struct.
template <typename T>
struct disassembly_type {
	using value = typename T::disassembly_type;
};

// Fourth, you need to actually specify how your object can be disassembled.
//
// If you are creating a custom type, implement a
// ```cpp
// auto disassemble() const -> disassembly_type;
// ```
// method and you are covered.
template <typename T, typename Disassembly = typename disassembly_type<T>::value>
auto disassemble(T const& x) -> Disassembly {
	return x.disassemble();
}

// For convience in writing some of the algorithms, we've provided the
// following template type.
template <typename T>
using Components = std::pair<T, T>;

// Finally, your type should specialize the `std::hash` function. This might look something like
// namespace std {
//     template <> struct hash<YourType> {
//         auto operator()(YourType const& arg) const noexcept -> std::size_t {
//             // your logic
//         }
//     };
// }


}