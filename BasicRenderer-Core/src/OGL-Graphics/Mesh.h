#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <string>
#include <vector>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h> 
#include <VAO.h>
#include <IBO.h>

struct MeshBufferData {

	unsigned int numFaces{0};
	unsigned int numVertices{0};
	unsigned int* faceArray{nullptr};

	float* vertexArray{nullptr};
	float* normalArray{ nullptr };
	float* colorArray{ nullptr };
	float* textCoordArray{ nullptr };
	float* tangentsArray{ nullptr };

};

class Mesh
{
protected:
	VAO m_Vao;
	unsigned int m_Triangles;

	std::string  m_FileRoute;

	MeshBufferData m_BufferData;

	bool castShadows;

public:
	Mesh() :m_Triangles(-1),castShadows(true), m_FileRoute("") {}

	Mesh(const std::string r) : m_Triangles(-1),  castShadows(true), m_FileRoute(r) { importFile(); }

	~Mesh() {}

	inline void setCastShadows(bool c) { castShadows = c; }

	inline bool getCastShadows() {  return castShadows; }

	virtual void generateBuffers();
	
	virtual void draw();

	void importFile();

};


