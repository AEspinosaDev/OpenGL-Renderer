#pragma once
#include "Mesh.h"


class Quad : public Mesh {
public: 
	Quad();

	inline std::string getFileRoute() = delete;
	void importFile() = delete;

};
