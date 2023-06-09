#include "Scene.h"

void Scene::add(SceneObject* obj)
{
	switch (obj->getObjectType())
	{
	case ObjectType::MODEL:
		if (obj->getName() == "") {
			auto newName = "Model" + std::to_string(modelNameIndexCount);
			modelNameIndexCount++;
			obj->setName(newName);
		}
		m_Objects[obj->getName()] = obj;
		m_Models[obj->getName()] = (Model*)obj;
		break;
	case ObjectType::LIGHT:
		if (obj->getName() == "") {
			Light* light = static_cast<Light*>(obj);
			std::string newName;
			switch (light->getType())
			{
			case 0:
				newName = "PointLight" + std::to_string(pointlightNameIndexCount);
				pointlightNameIndexCount++;
				break;
			case 1:
				newName = "DirectionalLight" + std::to_string(directionallightNameIndexCount);
				directionallightNameIndexCount++;
				break;
			case 2:
				newName = "SpotLight" + std::to_string(spotlightNameIndexCount);
				spotlightNameIndexCount++;
				break;
			}
			obj->setName(newName);
		}
		m_Objects[obj->getName()] = obj;
		m_Lights[obj->getName()] = (Light*)obj;
		break;
	case ObjectType::CAMERA:
		if (obj->getName() == "") {
			auto newName = "Camera" + std::to_string(cameraNameIndexCount);
			cameraNameIndexCount++;
			obj->setName(newName);
		}
		m_Objects[obj->getName()] = obj;
		m_Cameras[obj->getName()] = (Camera*)obj;
		break;
	}

}
