#include "shaderprogram.h"
#include "glerror.h"

ShaderProgram::ShaderProgram()
    : _shaderProgramId(glCreateProgram()), _linkageDuration(0)
{
}

ShaderProgram::ShaderProgram(const ShaderProgram& shaderProgram)
    : _shaderProgramId(glCreateProgram()), _linkageDuration(0)
{
}

ShaderProgram::~ShaderProgram()
{
    deleteShaderProgram();
}

ShaderProgram& ShaderProgram::operator = (const ShaderProgram& shaderProgram)
{
    return *this;
}

bool ShaderProgram::attach(const Shader& shader)
{
    if (!shader.isValid())
    {
        return false;
    }
    GlError error;
    glAttachShader(_shaderProgramId, shader.getId());
    return !error.hasOccured();
}

bool ShaderProgram::has(const Shader& shader) const
{
    GLuint* shaders = getAttachedShaders();
    bool found = false;

    for(int i = 0; !found && shaders[i] != 0; ++i)
    {
        found = shaders[i] == shader.getId();
    }
    delete[] shaders;

    return found;
}

bool ShaderProgram::detach(const Shader& shader)
{
    if (!shader.isValid())
    {
        return false;
    }
    GlError error;
    glDetachShader(_shaderProgramId, shader.getId());
    return !error.hasOccured();
}

void ShaderProgram::detachAllShaders()
{
    GLuint* shaders = getAttachedShaders();
    for(int i = 0; shaders[i] != 0; ++i)
    {
        glDetachShader(_shaderProgramId, shaders[i]);
    }
    delete[] shaders;
}

bool ShaderProgram::link()
{
    return false;
}

void ShaderProgram::deleteShaderProgram()
{
    if (_shaderProgramId != 0)
    {
        glDeleteProgram(_shaderProgramId);
        _shaderProgramId = 0;
    }
}

GLuint* ShaderProgram::getAttachedShaders()const
{
    GLint nbShaders = 0;
    glGetProgramiv(_shaderProgramId, GL_ATTACHED_SHADERS, &nbShaders);

    GLuint* shaders = new GLuint[nbShaders + 1];

    GLsizei count = 0;
    glGetAttachedShaders(_shaderProgramId, nbShaders, &count, shaders);
    shaders[count] = 0;

    return shaders;
}


