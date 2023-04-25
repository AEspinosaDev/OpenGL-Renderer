#include "Mesh.h"



void Mesh::generateBuffers() {

	m_Triangles = m_BufferData.numFaces * 3;
	m_Vao.bind();

	if (m_BufferData.vertexArray != NULL) {
		VBO pos_b(m_BufferData.vertexArray, m_BufferData.numVertices * sizeof(float) * 3);
		VBLayout layout0;
		layout0.push<float>(3);
		m_Vao.addBuffer(pos_b, layout0);
	}


	if (m_BufferData.normalArray != NULL) {
		VBO normal_b(m_BufferData.normalArray, m_BufferData.numVertices * sizeof(float) * 3);
		VBLayout layout2;
		layout2.push<float>(3);
		m_Vao.addBuffer(normal_b, layout2);
	}

	if (m_BufferData.textCoordArray != NULL) {
		VBO texCoord_b(m_BufferData.textCoordArray, m_BufferData.numVertices * sizeof(float) * 2);
		VBLayout layout3;
		layout3.push<float>(2);
		m_Vao.addBuffer(texCoord_b, layout3);
	}

	if (m_BufferData.tangentsArray != NULL) {
		VBO tangent_b(m_BufferData.tangentsArray, m_BufferData.numVertices * sizeof(float) * 3);
		VBLayout layout4;
		layout4.push<float>(3);
		m_Vao.addBuffer(tangent_b, layout4);
	}
	if (m_BufferData.colorArray != NULL) {
		VBO color_b(m_BufferData.colorArray, m_BufferData.numVertices * sizeof(float) * 3);
		VBLayout layout1;
		layout1.push<float>(3);
		m_Vao.addBuffer(color_b, layout1);
	}

	IBO trisIndex(m_BufferData.faceArray, m_Triangles);

}

void Mesh::draw()
{
	m_Vao.bind();
	GLcall(glDrawElements(GL_TRIANGLES, m_Triangles, GL_UNSIGNED_INT, (void*)0));
}



void Mesh::importFile() {

	const std::string pFile = modelPath + m_FileRoute;
	Assimp::Importer importer;

	const aiScene* sc;
	sc = importer.ReadFile(pFile,
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	if (!sc) {

		std::cout << "Error al importar" << std::endl;
	}

	std::cout << "Se han cargado " << sc->mNumMeshes << " meshes" << std::endl;

	for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
	{
		const aiMesh* mesh = sc->mMeshes[n];

		unsigned int faceIndex = 0;
		unsigned int vertexIndex = 0;
		unsigned int normalIndex = 0;
		unsigned int tangentsIndex = 0;

		m_BufferData.faceArray = (unsigned int*)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);

		if (mesh->HasFaces()) {
			for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
				const aiFace* face = &mesh->mFaces[t];
				memcpy(&m_BufferData.faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
				faceIndex += 3;
			}
		}

		if (mesh->HasPositions()) {  //Idem: por defecto siempre tiene.

			m_BufferData.vertexArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);


			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				const aiVector3D* vertex = &mesh->mVertices[k];
				memcpy(&m_BufferData.vertexArray[vertexIndex], &(vertex->x), sizeof(float));
				memcpy(&m_BufferData.vertexArray[vertexIndex + 1], &(vertex->y), sizeof(float));
				memcpy(&m_BufferData.vertexArray[vertexIndex + 2], &(vertex->z), sizeof(float));
				vertexIndex += 3;
			}
		}
		if (mesh->HasNormals()) {
			m_BufferData.normalArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);

			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				const aiVector3D* a_normal = &mesh->mNormals[k];
				memcpy(&m_BufferData.normalArray[normalIndex], &(a_normal->x), sizeof(float));
				memcpy(&m_BufferData.normalArray[normalIndex + 1], &(a_normal->y), sizeof(float));
				memcpy(&m_BufferData.normalArray[normalIndex + 2], &(a_normal->z), sizeof(float));
				normalIndex += 3;
			}
		}
		if (mesh->mTextureCoords != NULL) {
			m_BufferData.textCoordArray = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);

			for (int k = 0; k < mesh->mNumVertices; k++) {
				const aiVector3D* a_textCoord = &mesh->mTextureCoords[0][k];

				memcpy(&m_BufferData.textCoordArray[k * 2], &(a_textCoord->x), sizeof(float));
				memcpy(&m_BufferData.textCoordArray[k * 2 + 1], &(a_textCoord->y), sizeof(float));

			}

		}
		if (mesh->HasTangentsAndBitangents()) {
			m_BufferData.tangentsArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				const aiVector3D* a_tangent = &mesh->mTangents[k];
				memcpy(&m_BufferData.tangentsArray[tangentsIndex], &(a_tangent->x), sizeof(float));
				memcpy(&m_BufferData.tangentsArray[tangentsIndex + 1], &(a_tangent->y), sizeof(float));
				memcpy(&m_BufferData.tangentsArray[tangentsIndex + 2], &(a_tangent->z), sizeof(float));
				normalIndex += 3;
			}
		}

		m_BufferData.numFaces = mesh->mNumFaces;
		m_BufferData.numVertices = mesh->mNumVertices;
		
	}
}
