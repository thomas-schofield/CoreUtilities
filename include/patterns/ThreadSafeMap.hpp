#pragma once

#include <map>
#include <mutex>

namespace core_utilities
{
    template <
        class Key,
        class T, 
        class Compare = std::less<Key>,
        class Allocator = std::allocator<std::pair<const Key, T>>
    >
    class ThreadSafeMap
    {
        public:
            ThreadSafeMap() = default;
            virtual ~ThreadSafeMap() = default;

            // Capacity
            bool empty() const
            {
                std::lock_guard<std::mutex> lock(internal_map_mutex);
                return internal_map.empty();
            }

            const std::size_t size() const
            {
                std::lock_guard<std::mutex> lock(internal_map_mutex);
                return internal_map.size();
            }

            // Modifiers
            void clear()
            {
                std::lock_guard<std::mutex> lock(internal_map_mutex);
                internal_map.clear();
            }

            template<class Args>
            std::pair<iterator, bool> emplace(Args&& args)
            {
                std::lock_guard<std::mutex> lock(internal_map_mutex);
                return internal_map.emplace(std::forward<Args(args);)
            }

        private:
            std::map<Key, T, Compare, Allocator> internal_map;
            mutable std::mutex internal_map_mutex;
    };
}
