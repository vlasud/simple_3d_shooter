#include <render/shaders.h>

#include <fstream>
#include <filesystem>
#include <core/filesystem.h>
#include <cassert>


static int determine_shader_type(const std::string& shader_path)
{
    std::string fileExtension;
    filesystem::get_file_extension(shader_path, fileExtension);
    return fileExtension == ".vert" ? GL_VERTEX_SHADER : fileExtension == ".frag" ? GL_FRAGMENT_SHADER : -1;
}

ShaderProgram::ShaderProgram(const std::string& path_to_shaders) :
    id(0), error(0)
{
    id = glCreateProgram();

    for (auto& i : std::filesystem::directory_iterator(path_to_shaders))
    {
        if (i.is_directory())
            continue;

        Shader shader(i.path().string());
        glAttachShader(id, shader.getId());
        shaders.push_back(shader);
    }

    glLinkProgram(id);
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(id);
}

void ShaderProgram::addShader(const std::string& path)
{
    Shader shader(path);
    shaders.push_back(shader);

    glAttachShader(id, shader.getId());
}


Shader::Shader(const std::string& path) :
    id(0)
{
    int shaderType = determine_shader_type(path);
    assert(shaderType == GL_VERTEX_SHADER || shaderType == GL_FRAGMENT_SHADER);

    id = glCreateShader(shaderType);

    std::string shaderContent;
    filesystem::read_file_content(path, shaderContent);

    const char* shaderContent_C = shaderContent.c_str();
    glShaderSource(id, 1, &shaderContent_C, nullptr);
    glCompileShader(id);
}

Shader::~Shader()
{
    glDeleteShader(id);
}