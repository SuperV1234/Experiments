// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include "./shared.hpp"

namespace resource
{
    namespace impl
    {
        using shared_counter_type = unsigned int;

        class shared_metadata
        {
        private:
            shared_counter_type _owner_count;
            shared_counter_type _weak_count;

        public:
            shared_metadata(shared_counter_type owner_count,
                shared_counter_type weak_count) noexcept
                : _owner_count{owner_count},
                  _weak_count{weak_count}
            {
            }

            shared_metadata(const shared_metadata&) = delete;
            shared_metadata& operator=(const shared_metadata&) = delete;

            shared_metadata(shared_metadata&&) = delete;
            shared_metadata& operator=(shared_metadata&&) = delete;

            void increment_owner() noexcept
            {
                ++_owner_count;
            }

            void decrement_owner() noexcept
            {
                assert(_owner_count > 0);
                --_owner_count;
            }

            void increment_weak() noexcept
            {
                ++_weak_count;
            }

            void decrement_weak() noexcept
            {
                assert(_weak_count > 0);
                --_weak_count;
            }

            auto owner_count() const noexcept
            {
                return _owner_count;
            }

            auto weak_count() const noexcept
            {
                return _weak_count;
            }

            auto total_count() const noexcept
            {
                return owner_count() + weak_count();
            }

            auto has_any_ref() const noexcept
            {
                return total_count() > 0;
            }
        };
    }
}