#include "Mesh.h"



void Mesh::generateBuffers() {

	if (!isDirty) return;

	for (size_t n = 0; n < m_MeshNumber; n++)
	{

		m_BufferData[n].triangles = m_BufferData[n].numFaces * 3;
		m_Vaos[n].bind();

		if (m_BufferData[n].vertexArray != NULL) {
			VBO pos_b(m_BufferData[n].vertexArray, m_BufferData[n].numVertices * sizeof(float) * 3);
			VBLayout layout0;
			layout0.push<float>(3);
			m_Vaos[n].addBuffer(pos_b, layout0);
		}


		if (m_BufferData[n].normalArray != NULL) {
			VBO normal_b(m_BufferData[n].normalArray, m_BufferData[n].numVertices * sizeof(float) * 3);
			VBLayout layout2;
			layout2.push<float>(3);
			m_Vaos[n].addBuffer(normal_b, layout2);
		}

		if (m_BufferData[n].textCoordArray != NULL) {
			VBO texCoord_b(m_BufferData[n].textCoordArray, m_BufferData[n].numVertices * sizeof(float) * 2);
			VBLayout layout3;
			layout3.push<float>(2);
			m_Vaos[n].addBuffer(texCoord_b, layout3);
		}

		if (m_BufferData[n].tangentsArray != NULL) {
			VBO tangent_b(m_BufferData[n].tangentsArray, m_BufferData[n].numVertices * sizeof(float) * 3);
			VBLayout layout4;
			layout4.push<float>(3);
			m_Vaos[n].addBuffer(tangent_b, layout4);
		}
		if (m_BufferData[n].colorArray != NULL) {
			VBO color_b(m_BufferData[n].colorArray, m_BufferData[n].numVertices * sizeof(float) * 3);
			VBLayout layout1;
			layout1.push<float>(3);
			m_Vaos[n].addBuffer(color_b, layout1);
		}

		IBO trisIndex(m_BufferData[n].faceArray, m_BufferData[n].triangles);

		m_Vaos[n].unbind();
	}

	isDirty = false;
}

void Mesh::draw()
{
	for (size_t n = 0; n < m_MeshNumber; n++)
	{
		m_Vaos[n].bind();
		GLcall(glDrawElements(GL_TRIANGLES, m_BufferData[n].triangles, GL_UNSIGNED_INT, (void*)0));
	}
}

void Mesh::draw(int meshIdx)
{
	m_Vaos[meshIdx].bind();
	GLcall(glDrawElements(GL_TRIANGLES, m_BufferData[meshIdx].triangles, GL_UNSIGNED_INT, (void*)0));
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

	m_MeshNumber = sc->mNumMeshes;
	m_Vaos = new VAO[sc->mNumMeshes];
	m_BufferData = new MeshBufferData[sc->mNumMeshes];

	for (unsigned int n = 0; n < sc->mNumMeshes; ++n)
	{

		const aiMesh* mesh = sc->mMeshes[n];

		std::cout << m_FileRoute << " " << mesh->mMaterialIndex << std::endl;

		unsigned int faceIndex = 0;
		unsigned int vertexIndex = 0;
		unsigned int normalIndex = 0;
		unsigned int tangentsIndex = 0;

		m_BufferData[n].faceArray = (unsigned int*)malloc(sizeof(unsigned int) * mesh->mNumFaces * 3);

		if (mesh->HasFaces()) {
			for (unsigned int t = 0; t < mesh->mNumFaces; ++t) {
				const aiFace* face = &mesh->mFaces[t];
				memcpy(&m_BufferData[n].faceArray[faceIndex], face->mIndices, 3 * sizeof(unsigned int));
				faceIndex += 3;
			}
		}

		if (mesh->HasPositions()) {  //Idem: por defecto siempre tiene.

			m_BufferData[n].vertexArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);


			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				const aiVector3D* vertex = &mesh->mVertices[k];
				memcpy(&m_BufferData[n].vertexArray[vertexIndex], &(vertex->x), sizeof(float));
				memcpy(&m_BufferData[n].vertexArray[vertexIndex + 1], &(vertex->y), sizeof(float));
				memcpy(&m_BufferData[n].vertexArray[vertexIndex + 2], &(vertex->z), sizeof(float));
				vertexIndex += 3;
			}
		}
		if (mesh->HasNormals()) {
			m_BufferData[n].normalArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);

			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				const aiVector3D* a_normal = &mesh->mNormals[k];
				memcpy(&m_BufferData[n].normalArray[normalIndex], &(a_normal->x), sizeof(float));
				memcpy(&m_BufferData[n].normalArray[normalIndex + 1], &(a_normal->y), sizeof(float));
				memcpy(&m_BufferData[n].normalArray[normalIndex + 2], &(a_normal->z), sizeof(float));
				normalIndex += 3;
			}
		}
		if (mesh->mTextureCoords != NULL) {
			m_BufferData[n].textCoordArray = (float*)malloc(sizeof(float) * 2 * mesh->mNumVertices);

			for (int k = 0; k < mesh->mNumVertices; k++) {
				const aiVector3D* a_textCoord = &mesh->mTextureCoords[0][k];

				memcpy(&m_BufferData[n].textCoordArray[k * 2], &(a_textCoord->x), sizeof(float));
				memcpy(&m_BufferData[n].textCoordArray[k * 2 + 1], &(a_textCoord->y), sizeof(float));

			}

		}
		if (mesh->HasTangentsAndBitangents()) {
			m_BufferData[n].tangentsArray = (float*)malloc(sizeof(float) * mesh->mNumVertices * 3);
			for (unsigned int k = 0; k < mesh->mNumVertices; ++k) {
				const aiVector3D* a_tangent = &mesh->mTangents[k];
				memcpy(&m_BufferData[n].tangentsArray[tangentsIndex], &(a_tangent->x), sizeof(float));
				memcpy(&m_BufferData[n].tangentsArray[tangentsIndex + 1], &(a_tangent->y), sizeof(float));
				memcpy(&m_BufferData[n].tangentsArray[tangentsIndex + 2], &(a_tangent->z), sizeof(float));
				normalIndex += 3;
			}
		}

		m_BufferData[n].numFaces = mesh->mNumFaces;
		m_BufferData[n].numVertices = mesh->mNumVertices;

		

	}
}
