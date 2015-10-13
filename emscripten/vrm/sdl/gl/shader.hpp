// Copyright (c) 2015-2016 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0
// http://vittorioromeo.info | vittorio.romeo@outlook.com

#pragma once

#include <fstream>
#include <vrm/sdl/common.hpp>
#include <vrm/sdl/context.hpp>
#include <vrm/sdl/gl/check.hpp>

namespace vrm
{
    namespace sdl
    {
        enum class shader_t
        {
            vertex,
            fragment
        };

        namespace impl
        {
            template <shader_t TV>
            constexpr GLenum shader_t_value{GL_VERTEX_SHADER};

            template <>
            constexpr GLenum shader_t_value<shader_t::fragment>{
                GL_FRAGMENT_SHADER};
        }

        template <shader_t TS>
        auto make_shader(const char* src) noexcept
        {
            constexpr std::size_t shaderLogBufferSize{512};
            static char logBuffer[shaderLogBufferSize];
            static std::string logString;

            GLuint id;

            VRM_SDL_GLCHECK(id = glCreateShader(impl::shader_t_value<TS>););
            VRM_SDL_GLCHECK(glShaderSource(id, 1, &src, nullptr));
            VRM_SDL_GLCHECK(glCompileShader(id));

            GLint status;
            VRM_SDL_GLCHECK(glGetShaderiv(id, GL_COMPILE_STATUS, &status));

            VRM_SDL_GLCHECK(glGetShaderInfoLog(
                id, shaderLogBufferSize, nullptr, logBuffer));

            logString = logBuffer;

            if(!logString.empty())
            {
                std::cout << "Shader compilation log" << logString << std::endl;
                int temp;
                std::cin >> temp;
            }

            return impl::unique_shader{id};
        }

        template <shader_t TS>
        auto make_shader_from_file(const std::string& path)
        {
            std::ifstream ifs{path.c_str(), std::ios_base::binary};
            assert(!ifs.fail());

            ifs.seekg(0, std::ios::end);
            auto size(ifs.tellg());
            ifs.seekg(0, std::ios::beg);

            auto buffer(std::make_unique<char[]>(static_cast<sz_t>(size) + 1));
            ifs.read(&buffer[0], size);
            buffer[size] = '\0';

            return make_shader<TS>(buffer.get());
        }
    }
}