#include "shaderprogram.h"

ShaderProgram::ShaderProgram()
    : _shaderProgramId(0), _linkageDuration(0)
{
}

ShaderProgram::ShaderProgram(const ShaderProgram& shaderProgram)
    : _shaderProgramId(0), _linkageDuration(0), _shaderIdVector(shaderProgram._shaderIdVector)
{
}

ShaderProgram::~ShaderProgram()
{
    deleteShaderProgram();
}

ShaderProgram& ShaderProgram::operator = (const ShaderProgram& shaderProgram)
{

}

void ShaderProgram::addShader(const Shader& shader)
{

}

bool ShaderProgram::hasShader(const Shader& shader) const
{

}

bool ShaderProgram::removeShader(const Shader& shader)
{

}

void ShaderProgram::removeAllShaders()
{

}

bool ShaderProgram::link()
{

}

void ShaderProgram::deleteShaderProgram()
{

}
