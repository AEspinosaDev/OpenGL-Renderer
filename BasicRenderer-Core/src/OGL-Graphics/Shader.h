#pragma once
#include "Graphics_Core.h"
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>

	struct ShaderProgramSource
	{
		std::string VertexSource;
		std::string FragmentSource;


	};
	enum ShaderType {
		lit,
		unlit
	};

	class Shader {
		const std::string& m_filePath;
		unsigned int m_RendererID;
		std::unordered_map<std::string, int> m_UniformLocationCache;
		ShaderType type;

	public:
		Shader(const std::string& filename, ShaderType t);
		~Shader();

		void bind() const;
		void unbind() const;

		inline ShaderType getType() { return type; }


		void setBool(const std::string& name, bool value) const;
		
		void setInt(const std::string& name, int value) const;
		

		void setFloat(const std::string& name, float value);
		
		void setMat4(const std::string& name, glm::mat4 value);
		
		void setVec3(const std::string& name, glm::vec3 value);

		void setVec4(const std::string& name, glm::vec4 value);


	private:
		unsigned int getUniformLocation(const std::string& name);

		unsigned int compileShader(unsigned int type, const std::string& source);

		unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

		ShaderProgramSource parseShader();

	};

