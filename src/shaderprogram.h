#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <string>
#include <vector>
#include <memory>
#include "GL/glew.h"

#include "shader.h"

class UniformInfo
{
public:
    UniformInfo(GLuint index, GLint size, GLenum type, const char* name);
    UniformInfo(const UniformInfo& activeUniformInfo);

    UniformInfo& operator = (const UniformInfo& uniformInfo);

    bool operator == (const UniformInfo& uniformInfo) const;

    inline bool operator != (const UniformInfo& uniformInfo) const
    {
        return !(*this == uniformInfo);
    }

    inline const std::string& getName() const
    {
        return _name;
    }

    inline GLuint getIndex() const
    {
        return _index;
    }

    inline GLint getSize() const
    {
        return _size;
    }

    inline GLenum getType() const
    {
        return _type;
    }

    inline bool isArray() const
    {
        return _size > 1;
    }
private:
    void normalizeArrayName();
    GLuint _index;
    GLint _size;
    GLenum _type;
    std::string _name;
};

typedef std::vector<UniformInfo> UniformInfoVector;

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

    bool validate();

    void extractActiveUniformInfo(UniformInfoVector& vector);

    inline const std::string& getLastLinkLog() const
    {
        return _lastLinkLog;
    }

    inline const std::string& getLastValidationLog() const
    {
        return _lastValidationLog;
    }

    inline GLuint getId() const
    {
        return _shaderProgramId;
    }

    bool exists() const;

    inline unsigned long getLinkageDuration() const
    {
        return _linkageDuration;
    }

private:
    void attachShadersFrom(const ShaderProgram& shaderProgram);
    void extractInfoLog(std::string &log);
    void deleteShaderProgram();
    GLuint getNbAttachedShaders()const;
    GLuint* getAttachedShaders()const;

    GLuint _shaderProgramId;
    unsigned long _linkageDuration;
    std::string _lastLinkLog;
    std::string _lastValidationLog;
};

#endif // SHADERPROGRAM_H
