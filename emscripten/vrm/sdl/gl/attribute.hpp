// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/macros.hpp>
#include <vrm/sdl/gl/shader.hpp>

namespace vrm
{
    namespace sdl
    {
        class attribute
        {
        private:
            GLuint _location;

        public:
            attribute(GLuint location) noexcept : _location{location} {}

            void enable() noexcept
            {
                VRM_SDL_GLCHECK(glEnableVertexAttribArray(_location));
            }
            void disable() noexcept
            {
                VRM_SDL_GLCHECK(glDisableVertexAttribArray(_location));
            }

            void vertex_attrib_pointer(sz_t n_components, GLenum type,
                bool normalized = true, sz_t stride = 0,
                const GLvoid* first_element = nullptr)
            {
                assert(n_components > 0 && n_components < 5);

                VRM_SDL_GLCHECK(glVertexAttribPointer(_location, n_components,
                    type, normalized, stride, first_element));
            }
        };
    }
}