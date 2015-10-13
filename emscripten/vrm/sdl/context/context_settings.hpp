// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <chrono>
#include <vrm/sdl/math.hpp>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/utils.hpp>
#include <vrm/sdl/resource.hpp>
#include <vrm/sdl/elements.hpp>
#include <vrm/sdl/context/unique_sdl_resources.hpp>

namespace vrm
{
    namespace sdl
    {
        template <typename TGameState>
        struct context_settings
        {
            using state_type = TGameState;

            using update_fn_type =
                std::function<state_type(const state_type&, ft)>;

            using draw_fn_type = std::function<void(const state_type&)>;

            using interpolate_fn_type = std::function<state_type(
                const state_type&, const state_type&, float)>;

            static auto& null_update_fn() noexcept
            {
                static update_fn_type result([](const auto& state, auto)
                    {
                        return state;
                    });
                return result;
            }

            static auto& null_draw_fn() noexcept
            {
                static draw_fn_type result([](const auto&)
                    {
                    });
                return result;
            }

            static auto& null_interpolate_fn() noexcept
            {
                static interpolate_fn_type result(
                    [](const auto& state, const auto&, float)
                    {
                        return state;
                    });

                return result;
            }
        };
    }
}