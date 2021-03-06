// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <vrm/gl/common.hpp>
#include <vrm/gl/check.hpp>
#include <vrm/gl/shader.hpp>
#include <vrm/gl/attribute_helpers.hpp>

VRM_SDL_NAMESPACE
{
    class attribute
    {
    private:
        GLuint _location;

    public:
        attribute() = default;
        attribute(GLuint location) noexcept : _location{location} {}

        auto& enable(int size = 1) noexcept
        {
            for(auto i(0); i < size; ++i)
            {
                VRM_SDL_GLCHECK(glEnableVertexAttribArray(_location + i));
            }
            return *this;
        }

        auto& disable() noexcept
        {
            VRM_SDL_GLCHECK(glDisableVertexAttribArray(_location));
            return *this;
        }

        auto& vertex_attrib_pointer(sz_t n_components, GLenum type,
            bool normalized = true, sz_t stride = 0,
            const GLvoid* first_element = nullptr,
            sz_t layout_offset = 0) noexcept
        {
            VRM_CORE_ASSERT(n_components > 0 && n_components < 5);

            VRM_SDL_GLCHECK(glVertexAttribPointer(_location + layout_offset,
                n_components, type, normalized, stride, first_element));

            return *this;
        }

        template <typename T>
        auto& vertex_attrib_pointer_in(sz_t n_components, GLenum type,
            bool normalized = true, sz_t offset = 0,
            sz_t layout_offset = 0) noexcept
        {
            VRM_CORE_STATIC_ASSERT(std::is_standard_layout<T>{}, "");

            return vertex_attrib_pointer(n_components, type, normalized,
                sizeof(T), vrmc::num_to_void_ptr(offset), layout_offset);
        }

        template <typename T, typename TValue>
        auto& vertex_attrib_pointer_in(bool normalized = true, sz_t offset = 0,
            sz_t layout_offset = 0) noexcept
        {
            return vertex_attrib_pointer_in<T>(impl::n_components_for<TValue>,
                impl::attrib_type_for<TValue>, normalized, offset,
                layout_offset);
        }

        auto location() const noexcept { return _location; }
    };
}
VRM_SDL_NAMESPACE_END