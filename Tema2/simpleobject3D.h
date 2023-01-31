#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace simpleobject3D
{
	/* functions generate simple meshes */
	Mesh* CreateSquare(const std::string& name, float length, glm::vec3 color, bool fill);
	Mesh* CreateCar(const std::string& name, glm::vec3 color);
	std::vector<Mesh*> CreateStartGate(const std::string& name, glm::vec3 mainColor, glm::vec3 secundarColor);
}
