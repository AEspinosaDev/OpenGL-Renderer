#include "Quad.h"

Quad::Quad() : Mesh()
{

	m_MeshNumber = 1;

	m_Vaos = new VAO[1];
	m_BufferData = new MeshBufferData[1];


	m_BufferData[0].triangles = 6;
	m_BufferData[0].numFaces = 2;
	m_BufferData[0].numVertices = 4;

	m_BufferData[0].vertexArray = new float[12] {
		-0.5, -0.5, 0.0,
			0.5, -0.5, 0.0,
			-0.5, 0.5, 0.0,
			0.5, 0.5, 0.0,
	};
	m_BufferData[0].normalArray = new float[12] {
		0.0, 0.0, 1.0,
			0.0, 0.0, 1.0,
			0.0, 0.0, 1.0,
			0.0, 0.0, 1.0,
	};
	m_BufferData[0].tangentsArray = new float[12] {
		1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
			1.0, 0.0, 0.0,
	};
	m_BufferData[0].textCoordArray = new float[8] {
		0.0, 0.0,
			1.0, 0.0,
			0.0, 1.0,
			1.0, 1.0
	};
	m_BufferData[0].faceArray = new unsigned int[6] { 0, 1, 2, 1, 2, 3 };

}


