#pragma once

#include <Core/Renderer.h>
#include <Core/SceneObjects/Lights/PointLight.h>
#include <Core/Materials/BasicPhongMaterial.h>
#include <InstancedMesh.h>
#include <Core/SceneObjects/Lights/DirectionalLight.h>
#include <Core/Materials/PhysicalMaterial.h>


const unsigned int INSTANCES = 100;


int main()
{
	Renderer* r = Renderer::getInstance();
	Scene* sc = new Scene("mainScene");


	//SETUP SCENE
	//------------------------------------
	PointLight* l = new PointLight(glm::vec3(5.0, 3.0, -4.0), glm::vec3(1.0, 0.8, 0.8), 1.5, 10);
	sc->add(l);


	PhysicalMaterial* templeFloor_m = new PhysicalMaterial();
	templeFloor_m->setAlbedoText(new Texture("floor_albedo.png"));
	templeFloor_m->setNormalText(new Texture("floor_normal.png"));
	templeFloor_m->setMaskMap(new Texture("floor_MOA.png"), UNITY_HDRP);
	PhysicalMaterial* wall_m = new PhysicalMaterial();
	wall_m->setAlbedoText(new Texture("walls_albedo.png"));
	wall_m->setNormalText(new Texture("walls_normal.png"));
	wall_m->setMaskMap(new Texture("walls_MOA.png"), UNITY_HDRP);
	PhysicalMaterial* column_m = new PhysicalMaterial();
	column_m->setAlbedoText(new Texture("columns_albedo.png"));
	column_m->setNormalText(new Texture("columns_normal.png"));
	column_m->setMaskMap(new Texture("columns_MOA.png"), UNITY_HDRP);
	PhysicalMaterial* roof_m = new PhysicalMaterial();
	roof_m->setAlbedoText(new Texture("roofs_albedo.png"));
	roof_m->setNormalText(new Texture("roofs_normal.png"));
	roof_m->setMaskMap(new Texture("roofs_MOA.png"), UNITY_HDRP);



	Model* temple = new	Model("templeM.obj", column_m);
	sc->add(temple);
	temple->setPosition(glm::vec3(0.0, 0.0, -25.0));
	temple->setMaterial(templeFloor_m, 1);
	temple->setMaterial(roof_m, 2);
	temple->setMaterial(wall_m, 3);

	PhysicalMaterial* box_m = new PhysicalMaterial();
	box_m->setAlbedoText(new Texture("SeamlessWood-Diffuse.jpg"));
	box_m->setNormalText(new Texture("SeamlessWood-NormalMap.tif"));
	box_m->setRoughnessText(new Texture("SeamlessWood-RoughnessMap.tif"));
	//box_m->setOpacity(0.2);
	//box_m->setTransparency(true);


	PhysicalMaterial* mosaic_m = new PhysicalMaterial();
	mosaic_m->setAlbedoText(new Texture("mosaicAlbedo.jpg"));
	mosaic_m->setNormalText(new Texture("mosaicNormal.jpg"));
	mosaic_m->setAOText(new Texture("mosaicAO.jpg"));
	mosaic_m->setRoughnessText(new Texture("mosaicRough.jpg"));
	mosaic_m->setTileU(2);
	mosaic_m->setTileV(2);
	/*PhysicalMaterial* stoneFloor_m = new PhysicalMaterial();
	stoneFloor_m->setAlbedoText(new Texture("stoneFloorAlbedo.jpg"));
	stoneFloor_m->setNormalText(new Texture("stoneFloorNormal.jpg"));
	stoneFloor_m->setAOText(new Texture("stoneFloorAO.jpg"));
	stoneFloor_m->setRoughnessText(new Texture("stoneFloorRough.jpg"));
	stoneFloor_m->setTileU(20);
	stoneFloor_m->setTileV(20);*/

	Model* box = new Model("cajaTest", "box.obj", box_m);
	box->setPosition(glm::vec3(6.0, 0.5, 0.0));
	sc->add(box);
	Model* box2 = box->clone();
	box2->setPosition(glm::vec3(6.5, 0.5, 1.0));
	sc->add(box2);
	Model* mosaic = box->clone();
	mosaic->setMaterial(mosaic_m);
	mosaic->setPosition(glm::vec3(0.0f, -4.97f, 0.0f));
	mosaic->setScale(glm::uvec3(10.0f));
	sc->add(mosaic);
	Model* mainPlatform = box->clone();
	/*mainPlatform->setMaterial(stoneFloor_m);
	mainPlatform->setPosition(glm::vec3(0.0f, -0.5f, -20.0f));
	mainPlatform->setScale(glm::uvec3(70.0f,1.0f,90.0f));
	sc->add(mainPlatform);*/

	InstancedMesh* iColumn = new InstancedMesh("column.obj", INSTANCES);
	glm::mat4* matrices = new glm::mat4[INSTANCES];

	for (size_t i = 0; i < INSTANCES; i++)
	{
		float rotation = 0.0f;
		glm::vec3 pos = glm::vec3(-25.0f, 0.0, -23.0);
		pos.x += i * 2;


		if (i >= 25) {
			pos = glm::vec3(25.0f, 0.0, -25.0);
			pos.z += (i - 25) * 2;
			rotation = 1.57f;
		}
		if (i >= 50) {
			pos = glm::vec3(-25.0f, 0.0, 25.0);
			pos.x += (i - 50) * 2;
			rotation = 0.0f;
		}
		if (i >= 75) {
			pos = glm::vec3(-25.0f, 0.0, 25.0);
			pos.z -= (i - 75) * 2;
			rotation = 1.57f;
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, pos);
		model = glm::rotate(model, rotation, glm::vec3(0.0, 1.0, 0.0));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		matrices[i] = model;


	}
	
	iColumn->setWorldMatrices(matrices);
	Model* column = new Model(iColumn, column_m);
	column->setPosition(glm::vec3(.0f, .0f, -15.0f));
	sc->add(column);


	Model* corner = new Model("corner.obj", column_m);
	corner->setPosition(glm::vec3(18.0f, .0f, 3.5f));
	Model* corner1 = corner->clone();
	corner1->setPosition(glm::vec3(-18.0f, .0f, 3.5f));


	corner->setMaterial(templeFloor_m, 2);
	corner->setMaterial(roof_m, 1);
	sc->add(corner);
	corner1->setMaterial(templeFloor_m, 2);
	corner1->setMaterial(roof_m, 1);
	sc->add(corner1);
	
	sc->add(new PointLight(glm::vec3(18.0f, 3.0, 3.5f), glm::vec3(1.0, 0.7, 0.5), 0.6, 10));
	sc->add(new PointLight(glm::vec3(-18.0f, 3.0, 3.5f), glm::vec3(1.0, 0.7, 0.5), 0.4, 10));


	PhysicalMaterial* floor_m = new PhysicalMaterial();
	floor_m->setAlbedoText(new Texture("floorAlbedo.jpg"));
	floor_m->setNormalText(new Texture("floorNormal.jpg"));
	//floor_m->setMaskMap(new Texture("floorUniformMOA.png"),UNITY_HDRP);
	floor_m->setRoughnessText(new Texture("floorRough.jpg"));
	floor_m->setTileU(60);
	floor_m->setTileV(60);
	//floor_m->setReceiveShadows(false);
	Model* plane = new Model("plane.obj", floor_m);
	plane->setScale(glm::vec3(3.0f, 1.0f, 3.0f));
	//plane->setPosition(glm::vec3(0.0f, -0.5f, 0.0f));
	sc->add(plane);

	PhysicalMaterial* gold = new PhysicalMaterial();
	gold->setAlbedoColor(glm::vec3(1.0f, .84f, .0f));
	gold->setRoughness(0.15f);
	gold->setMetalness(0.85f);
	//gold->setFaceVisibility(BOTH);
	Model* ball = new Model("highSphere.obj", gold);
	sc->add(ball);
	ball->setPosition(glm::vec3(0, 1, 0));
	ball->setScale(glm::vec3(0.4, 0.4, 0.4));


	//BasicPhongMaterial* alpha_m = new BasicPhongMaterial();
	//alpha_m->setShininess(10);
	//alpha_m->setSpecularity(1);
	//alpha_m->setBaseColor(glm::vec3(0.0, 0.5, 0.0));
	//alpha_m->addOpacityTex(new Texture("alphaTest.jpg"));
	//alpha_m->setTransparency(true);
	//alpha_m->setFaceVisibility(BOTH);
	//Model* alphaShadowTest = plane->clone();
	//alphaShadowTest->setMaterial(alpha_m);
	//alphaShadowTest->setPosition(glm::vec3(-5.0f, 1.0f, 0.0f));
	//alphaShadowTest->setScale(glm::vec3(0.02, 1.0, 0.02));
	//alphaShadowTest->setRotation(glm::vec3(1.57, 0.0, -1.57));
	////alphaShadowTest->getMesh()->setCastShadows(false);
	//sc->add(alphaShadowTest);


	//r->setGammaCorrection(false);

	DirectionalLight* dl = new DirectionalLight(glm::vec3(-5.0, -5.0, 5.0),glm::vec3(1.0, 0.8, 0.8),2.0f);
	dl->setPosition(glm::vec3(50.0, 50.0, 50.0));
	sc->add(dl);

	/*CubeMapFaces skyFaces("night-sky/px.png",
		"night-sky/nx.png",
		"night-sky/py.png",
		"night-sky/ny.png",
		"night-sky/pz.png",
		"night-sky/nz.png");


	CubeMapTexture* skyText = new CubeMapTexture(skyFaces);
	SkyboxMesh* skybox = new SkyboxMesh(new SkyboxMaterial(skyText));
	sc->setSkybox(skybox);*/

	sc->add(new Camera());

	sc->getActiveCamera()->setFOV(60.0f);
	sc->getActiveCamera()->setFar(150);
	//sc->setAmbientStrength(0.05f);
	r->addScene(sc);
	r->setCurrentScene("mainScene");
	r->setPostProcessPass(true);


	r->run();


}