#include "Shader.h"

void Shader::initShader(const char* _vertexPath, const char* _fragPath)
{
	shaderProgram = 0;
    //shader代码
    std::string  _vertexCode("");
    std::string  _fragCode("");
    //shader读取器
    std::ifstream	_vShaderFile;
    std::ifstream	_fShaderFile;

    //增加try catch的扩展功能
    _vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    _fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        _vShaderFile.open(_vertexPath);
        _fShaderFile.open(_fragPath);

        // 从读取器里读取shader
        std::stringstream	_vShaderStream, _fShaderStream;
        _vShaderStream << _vShaderFile.rdbuf();
        _fShaderStream << _fShaderFile.rdbuf();

        _vShaderFile.close();
        _fShaderFile.close();

        //得到代码
        _vertexCode = _vShaderStream.str();
        _fragCode = _fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::string  errStr = "ffShader constructor fail to read filePath";
        std::cout << errStr << std::endl;
        return;
    }

    const char* _vShaderStr = _vertexCode.c_str();
    const char* _fShaderStr = _fragCode.c_str();

    //shader的编译链接、记录编译bug信息
    unsigned int	_vertexID = 0, _fragID = 0;
    char	_infoLog[512];
    int		_successFlag;

    //创建vertex shader、传入代码、编译shader、获取编译信息
    _vertexID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(_vertexID, 1, &_vShaderStr, NULL);
    glCompileShader(_vertexID);
    glGetShaderiv(_vertexID, GL_COMPILE_STATUS, &_successFlag);
    if (!_successFlag)
    {
        glGetShaderInfoLog(_vertexID, 512, NULL, _infoLog);
        std::string errStr(_infoLog);
        std::cout << errStr << std::endl;
    }

    //创建fragment shader、传入代码、编译shader、获取编译信息
    _fragID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(_fragID, 1, &_fShaderStr, NULL);
    glCompileShader(_fragID);
    glGetShaderiv(_fragID, GL_COMPILE_STATUS, &_successFlag);
    if (!_successFlag)
    {
        glGetShaderInfoLog(_fragID, 512, NULL, _infoLog);
        std::string errStr(_infoLog);
        std::cout << errStr << std::endl;
    }

    //创建shader程序、链接shader程序、链接编译完的程序、获取编译信息
    m_shaderProgram = glCreateProgram();
    glAttachShader(m_shaderProgram, _vertexID);
    glAttachShader(m_shaderProgram, _fragID);
    glLinkProgram(m_shaderProgram);
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &_successFlag);
    if (!_successFlag)
    {
        glGetProgramInfoLog(m_shaderProgram, 512, NULL, _infoLog);
        std::string errStr(_infoLog);
        std::cout << errStr << std::endl;
    }

    //shader结束 删除中间状态
    glDeleteShader(_vertexID);
    glDeleteShader(_fragID);
}

void Shader::setMatrix(const std::string& _name, glm::mat4 _matrix) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, _name.c_str()), 1, GL_FALSE, glm::value_ptr(_matrix));
}

void Shader::setVec3(const std::string& _name, glm::vec3 _vec3) const
{
	glUniform3fv(glGetUniformLocation(m_shaderProgram, _name.c_str()), 1, glm::value_ptr(_vec3));
}

void Shader::setFloat(const std::string& _name, float _f) const
{
	glUniform1f(glGetUniformLocation(m_shaderProgram, _name.c_str()), _f);
}

void Shader::setInt(const std::string& _name, int _i) const
{
	glUniform1i(glGetUniformLocation(m_shaderProgram, _name.c_str()), _i);
}

