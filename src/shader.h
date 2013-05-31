#ifndef SHADER_H
#define SHADER_H

#include <string>
#include "GL/glew.h"

class Shader
{
public:
    enum ShaderType {VERTEX_SHADER, GEOMETRY_SHADER, FRAGMENT_SHADER};

    Shader(ShaderType type);
    Shader(const Shader& shader);
    ~Shader();
    Shader& operator = (const Shader& shader);

    inline GLuint getId() const
    {
        return _shaderId;
    }

    inline bool isValid() const
    {
        return _shaderId != 0;
    }

    inline ShaderType getType() const
    {
        return _type;
    }

    void extractSource(std::string& source) const;

    inline unsigned int getCompilationDuration() const
    {
        return _compilationDuration;
    }

    bool compile(const char* source);

    inline const std::string& getLog() const
    {
        return _log;
    }

private:
    void deleteShaderId();
    void extractInfoLog();
    void createShader();

    GLuint _shaderId;
    const ShaderType _type;
    unsigned int _compilationDuration;
    std::string _log;
};

#endif // SHADER_H
