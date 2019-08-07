#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <numeric>
#include <optional>

// We need a type to represent a `Cache` which maps pairs of (hashed) objects
// to stored value (e.g. the coassembly index between the objects.
template <typename Store>
using Cache = std::map<std::pair<std::size_t, std::size_t>, Store>;

// The `Context<T>` class represents a caching context withing which to compute
// the assembly and coassembly index of objects.
//
// The caching API is protected so that derived classes can inherit and
// ultimately override the functionality. The only public component of the
// caching API is the `cache_size` method which just returns the number of
// cached objects. At the moment, it is good, but if we want to, for example,
// have a `DatabaseContext<T>` class which is backed by a database on disk,
// then we'd need want to create a more generic, abstract "Context" type from
// which to inherit.
//
// Two other public methods are provided (beyond the boilerplate constructors
// and assignment operators): `assembly_index` and `coassembly_index`. These
// methods are (almost) mutually recursive[1]. The details of the
// `coassembly_index` still need to be worked out, but it looks like a pretty
// reasonable approximation based on a very trivial example system (binary
// strings).
//
// # Example Usage
// ```cpp
// /**
//  * filename: src/main.cpp
//  * g++-9 -std=c++17 -Wall -Wextra -pedantic -Iinclude -O3 -o bin/pathways src/string.cpp src/main.cpp
//  **/
// #include "pathways.h"
// #include "mystring.h"
//
// auto main() -> int {
//     Context<MyString> ctx;
//     std::cout << "c ~ " << ctx.assembly_index({ "011101" }) << std::endl;
//     std::cout << "Cache size: " << ctx.cache_size() << std::endl;
// }
// ```
// ```shell
// $ ./bin/pathways
// c ~ 4
// Cache size: 23
// ```
//
// [1] There is a protected and overloaded `coassembly_index` method which
// accepts a pair of objects rather than the objects as separate objects. That
// function then calls the public `coassembly_index` accordingly.
template <typename T>
class Context {
    protected:
        // The `_cache` maps pairs of hashes (of type `std::size`) to `int32_t` values
        // representing (co)assembly indicies.
        Cache<int32_t> _cache;

        // Get the cached value for a given pair of object hashes. The `std::nullopt_t`
        // value is returned if the key is not found.
        auto cached(std::pair<std::size_t, std::size_t> const& key) const noexcept -> std::optional<int32_t> {
            auto const iter = this->_cache.find(key);
            if (iter != std::end(this->_cache)) {
                return iter->second;
            }
            return {};
        }

        // Get the cached value of two given object hashes. The `std::nullopt_t` value
        // is returned if the key is not found.
        auto cached(std::size_t const& x_hash, std::size_t const& y_hash) const noexcept -> std::optional<int32_t> {
            return this->cached(std::make_pair(x_hash, y_hash));
        }

        // Get the cached value of two given object. The `std::nullopt_t` value is
        // returned if the key is not found.
        auto cached(T const& x, T const& y) const noexcept -> std::optional<int32_t> {
            auto const x_hash = std::hash<T>{}(x);
            auto const y_hash = std::hash<T>{}(y);

            return this->cached(x_hash, y_hash);
        }

        // Get the cached value of single given object, stored in the cache as a pair
        // `(x,x)`. The `std::nullopt_t` value is returned if the key is not found.
        auto cached(T const& x) const noexcept -> std::optional<int32_t> {
            auto const x_hash = std::hash<T>{}(x);

            return this->cached(x_hash, x_hash);
        }

        // Set the cached value for a pair of object hashes, returning the stored
        // value.
        auto cache(std::pair<std::size_t, std::size_t> const& key, int32_t store) noexcept -> int32_t {
            return this->_cache[key] = store;
        }

        // Set the cached value for two given object haches, returning the stored
        // value.
        auto cache(std::size_t x_hash, std::size_t y_hash, int32_t store) noexcept -> int32_t {
            return this->cache(std::make_pair(x_hash, y_hash), store);
        }

        // Set the cached value for two given objects, returning the stored value.
        auto cache(T const& x, T const& y, int32_t store) noexcept-> int32_t {
            auto const x_hash = std::hash<T>{}(x);
            auto const y_hash = std::hash<T>{}(y);

            return this->cache(x_hash, y_hash, store);
        }

        // Set the cached value for a single object — treated as a pair `(x,x)` —
        // returning the stored value.
        auto cache(T const& x, int32_t store) noexcept -> int32_t {
            auto const x_hash = std::hash<T>{}(x);

            return this->cache(x_hash, x_hash, store);
        }

        // Compute the coassembly index for a pair of objects, optionally caching
        // intermediate results.
        auto coassembly_index(std::pair<T const&, T const&> const& objs, bool cache = true) noexcept -> int32_t {
            return this->coassembly_index(std::get<0>(objs), std::get<1>(objs), cache);
        }

    public:
        Context() = default;
        Context(Context<T> const&) = delete;
        Context(Context<T>&&) = default;

        auto operator=(Context<T> const&) -> Context<T>& = delete;
        auto operator=(Context<T>&&) -> Context<T>& = default;

        // Get the number of pairs stored in the cache.
        auto cache_size() const noexcept -> std::size_t {
            return std::size(this->_cache);
        }

        // Compute the assembly index of an object. Optionally, you can turn on or off
        // caching with the `cache` argument.
        auto assembly_index(T const& x, bool cache = true) noexcept -> int32_t {
            if (x.is_basic()) {
                // If `x` is a basic object, it's assembly index is 0 by definition.
                return 0;
            } else if (cache) {
                // If `cache` is true, we try to find the object `x` in the cache. If that's
                // successful, we return the cached assembly index.
                auto const c = this->cached(x);
                if (c) {
                    return c.value();
                }
            }

            // In the case we have not yet computed the assembly index for the object, or
            // we've decided to ignore the cached value, we start with the largest possible
            // assembly index that we can store ($2^31-1$).
            auto c = std::numeric_limits<int32_t>::max();

            // We then disassembly the object into pairs of objects which can be joined
            // together to produce the original. We then compute the smallest coassembly
            // index of each pair — plus 1 to account for the final joinging operation
            // which yields the original object.
            for (auto const& parts: x.disassembly()) {
                auto const cc = this->coassembly_index(parts, cache);
                c = std::min(c, cc + 1);
            }

            // Cache and return the result if we want, otherwise just return it.
            if (cache) {
                return this->cache(x, c);
            } else {
                return c;
            }
        }

        // *Estimate* the coassembly index of two objects. As with the
        // `assembly_index`, you can optionally turn on or off caching with the `cache`
        // argument.
        auto coassembly_index(T const& x, T const& y, bool cache = true) noexcept -> int32_t {
            if (x.is_basic()) {
                // If the *first* object is basic, return the *second* object's assembly index.
                return this->assembly_index(y, cache);
            } else if (y.is_basic()) {
                // If the *second* object is basic, return the *first* object's assembly index.
                return this->assembly_index(x, cache);
            } else if (cache) {
                // If `cache` is true, we try to find the pair of objects `(x, y)` in the
                // cache. If that's successful, we return the cached assembly index.
                auto const cc = this->cached(x, y);
                if (cc) {
                    return cc.value();
                }
            }

            // In the case we have not yet computed the coassembly index for the object, or
            // we've decided to ignore the cached value, we start with the largest possible
            // assembly index that we can store ($2^31-1$).
            auto cc = std::numeric_limits<int32_t>::max();

            // The following are simple approximations which *should* be replaced with
            // a more robust algorithm. However, it seems the approximation is pretty
            // reasonable give a toy system (binary string).
            if (x <= y) {
                // If the *first* object is less than or equal to the *second* object,
                // approximate the coassembly index as the assembly index of the *second*
                // object.
                cc = this->assembly_index(y, cache);
            } else if (y <= x) {
                // If the *second* object is less than or equal to the *first* object,
                // approximate the coassembly index as the assembly index of the *first*
                // object.
                cc = this->assembly_index(x, cache);
            } else {
                // If the objects are incomparable, assume they share no substructures in
                // common, and approximate the coassembly index as the sum of the assembly
                // indices of each object.
                cc = this->assembly_index(x, cache) + this->assembly_index(y, cache);
            }

            // Cache and return the result if we want, otherwise just return it.
            if (cache) {
                return this->cache(x, y, cc);
            } else {
                return cc;
            }
        }
};
