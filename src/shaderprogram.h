#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <vector>
#include "GL/glew.h"

#include "shader.h"

class ShaderProgram
{
public:
    ShaderProgram();
    ShaderProgram(const ShaderProgram& shaderProgram);
    ~ShaderProgram();

    ShaderProgram& operator = (const ShaderProgram& shaderProgram);

    void addShader(const Shader& shader);

    bool hasShader(const Shader& shader) const;

    bool removeShader(const Shader& shader);

    void removeAllShaders();

    bool link();

    inline const std::string& getLog() const
    {
        return _log;
    }

    inline GLuint getId() const
    {
        return _shaderProgramId;
    }

    inline unsigned long getLinkageDuration() const
    {
        return _linkageDuration;
    }

private:

    void deleteShaderProgram();

    GLuint _shaderProgramId;
    unsigned long _linkageDuration;
    std::vector<GLuint> _shaderIdVector;
    std::string _log;
};

#endif // SHADERPROGRAM_H
