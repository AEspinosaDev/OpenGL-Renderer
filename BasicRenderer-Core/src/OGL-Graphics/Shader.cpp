#include "Shader.h"

Shader::Shader(const std::string& filename) :m_filePath(shaderPath + filename), m_RendererID(0) {
	ShaderProgramSource src = parseShader();
	m_RendererID = createShader(src.VertexSource, src.FragmentSource);

}
Shader:: ~Shader() {
	GLcall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const {
	GLcall(glUseProgram(m_RendererID));
}

void Shader::unbind() const {
	GLcall(glUseProgram(0));

}


void Shader::setBool(const std::string& name, bool value) const
{
	GLcall(glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), (int)value));
}

void Shader::setInt(const std::string& name, int value) const
{
	GLcall(glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value));
}

void Shader::setFloat(const std::string& name, float value)
{
	GLcall(glUniform1f(getUniformLocation(name), value));
}

void Shader::setMat4(const std::string& name, glm::mat4 value)
{
	GLcall(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE,
		&(value[0][0])));
}
void Shader::setVec3(const std::string& name, glm::vec3 value) {

	GLcall(glUniform3fv(getUniformLocation(name), 1, &value[0]));
}
void Shader::setVec4(const std::string& name, glm::vec4 value) {

	glUniform4fv(getUniformLocation(name), 1, &value[0]);
}



unsigned int Shader::getUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	GLcall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
		std::cout << "Warning: Uniform name doesnt exist\n";

	else
		m_UniformLocationCache[name] = location;


	return location;
}
unsigned int Shader::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();

	GLcall(glShaderSource(id, 1, &src, nullptr));
	GLcall(glCompileShader(id));

	int result;
	GLcall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile shader" <<
			(type == GL_VERTEX_SHADER ? " vertex" : "fragment") << std::endl;

		std::cout << message << std::endl;

		GLcall(glDeleteShader(id));
		return 0;
	}

	return id;
}
unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {



	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLcall(glAttachShader(program, vs));
	GLcall(glAttachShader(program, fs));

	GLcall(glLinkProgram(program));
	GLcall(glValidateProgram(program));


	GLcall(glDeleteShader(vs));
	GLcall(glDeleteShader(fs));

	return program;


}
ShaderProgramSource Shader::parseShader() {

	std::ifstream stream(m_filePath);

	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1


	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;

			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;

			}
		}
		else {
			ss[(int)type] << line << '\n';

		}
	}
	return { ss[0].str(), ss[1].str() };
}
