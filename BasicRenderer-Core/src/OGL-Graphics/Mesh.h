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

	unsigned int triangles;
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

	std::string  m_FileRoute;
	unsigned int m_MeshNumber;

	VAO* m_Vaos;
	MeshBufferData* m_BufferData;

	bool isInstancedMesh;
	bool castShadows;

	bool isDirty;

public:
	Mesh() :m_MeshNumber(0), m_Vaos(nullptr), m_BufferData(nullptr), castShadows(true), isInstancedMesh(false), isDirty(true), m_FileRoute("") {}

	Mesh(const std::string r) :m_MeshNumber(0) ,m_Vaos(nullptr), m_BufferData(nullptr) , castShadows(true), isInstancedMesh(false), isDirty(true), m_FileRoute(r) { importFile(); }

	~Mesh() {
		delete[] m_Vaos;
		delete[] m_BufferData;
	}

	inline std::string getFileRoute() const { return  modelPath + m_FileRoute; }

	inline void setCastShadows(bool c) { castShadows = c; }

	inline bool getCastShadows() {  return castShadows; }

	inline bool isInstanced() { return isInstancedMesh; }

	inline unsigned int getNumberOfMeshes() { return m_MeshNumber; }

	inline MeshBufferData getMeshBuffertData(unsigned int idx = 0) { return m_BufferData[idx]; }

	inline void setDirty() { isDirty = true; }

	virtual void generateBuffers();
	
	virtual void draw();

	virtual void draw(int meshIdx);

	void importFile();

};


