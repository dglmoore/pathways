#pragma once

#include <algorithm>
#include <cstdint>
#include <map>
#include <numeric>
#include <optional>

template <typename Store>
using Cache = std::map<std::pair<std::size_t, std::size_t>, Store>;

template <typename T>
class Context {
    private:
        Cache<int32_t> _cache;

    public:
        auto cached(std::pair<std::size_t, std::size_t> const& key) const noexcept -> std::optional<int32_t> {
            auto const iter = this->_cache.find(key);
            if (iter != std::end(this->_cache)) {
                return iter->second;
            }
            return {};
        }

        auto cached(std::size_t const& x_hash, std::size_t const& y_hash) const noexcept -> std::optional<int32_t> {
            return this->cached(std::make_pair(x_hash, y_hash));
        }

        auto cached(T const& x, T const& y) const noexcept -> std::optional<int32_t> {
            auto const x_hash = std::hash<T>{}(x);
            auto const y_hash = std::hash<T>{}(y);

            return this->cached(x_hash, y_hash);
        }

        auto cached(T const& x) const noexcept -> std::optional<int32_t> {
            auto const x_hash = std::hash<T>{}(x);

            return this->cached(x_hash, x_hash);
        }

        auto cache(std::pair<std::size_t, std::size_t> const& key, int32_t store) noexcept -> int32_t {
            return this->_cache[key] = store;
        }

        auto cache(std::size_t x_hash, std::size_t y_hash, int32_t store) noexcept -> int32_t {
            return this->cache(std::make_pair(x_hash, y_hash), store);
        }

        auto cache(T const& x, T const& y, int32_t store) noexcept-> int32_t {
            auto const x_hash = std::hash<T>{}(x);
            auto const y_hash = std::hash<T>{}(y);

            return this->cache(x_hash, y_hash, store);
        }

        auto cache(T const& x, int32_t store) noexcept -> int32_t {
            auto const x_hash = std::hash<T>{}(x);

            return this->cache(x_hash, x_hash, store);
        }

        auto cache_size() const noexcept -> std::size_t {
            return std::size(this->_cache);
        }

        auto assembly_index(T const& x, bool cache = true) noexcept -> int32_t {
            if (x.is_basic()) {
                return 0;
            } else if (cache) {
                auto const c = this->cached(x);
                if (c) {
                    return c.value();
                }
            }

            auto c = std::numeric_limits<int32_t>::max();
            for (auto const& parts: x.disassembly()) {
                auto const cc = this->coassembly_index(parts, cache);
                c = std::min(c, cc + 1);
            }
            if (cache) {
                return this->cache(x, c);
            } else {
                return c;
            }
        }

        auto coassembly_index(T const& x, T const& y, bool cache = true) noexcept -> int32_t {
            if (x.is_basic()) {
                return this->assembly_index(y, cache);
            } else if (y.is_basic()) {
                return this->assembly_index(x, cache);
            } else if (cache) {
                auto const cc = this->cached(x, y);
                if (cc) {
                    return cc.value();
                }
            }

            auto cc = std::numeric_limits<int32_t>::max();
            if (x < y) {
                cc = this->assembly_index(y, cache);
            } else if (y < x) {
                cc = this->assembly_index(x, cache);
            } else {
                cc = this->assembly_index(x, cache) + this->assembly_index(y, cache);
            }

            if (cache) {
                return this->cache(x, y, cc);
            } else {
                return cc;
            }
        }

        auto coassembly_index(std::pair<T const&, T const&> const& objs, bool cache = true) noexcept -> int32_t {
            return this->coassembly_index(std::get<0>(objs), std::get<1>(objs), cache);
        }
};