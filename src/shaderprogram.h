#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include "GL/glew.h"

#include "shader.h"

class ShaderProgram
{
public:
    ShaderProgram();
    ShaderProgram(const ShaderProgram& shaderProgram);
    ~ShaderProgram();

    ShaderProgram& operator = (const ShaderProgram& shaderProgram);

    bool attach(const Shader& shader);

    bool has(const Shader& shader) const;

    bool detach(const Shader& shader);

    void detachAllShaders();

    bool link();

    inline const std::string& getLog() const
    {
        return _log;
    }

    inline GLuint getId() const
    {
        return _shaderProgramId;
    }

    inline bool isValid() const
    {
        return _shaderProgramId != 0;
    }

    inline unsigned long getLinkageDuration() const
    {
        return _linkageDuration;
    }

private:
    void attachShaderFrom(const ShaderProgram& shaderProgram);
    void extractInfoLog();
    void deleteShaderProgram();
    GLuint* getAttachedShaders()const;

    GLuint _shaderProgramId;
    unsigned long _linkageDuration;
    std::string _log;
};

#endif // SHADERPROGRAM_H
