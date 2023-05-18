#pragma once

#include <Core/Renderer.h>
#include <Core/Lights/PointLight.h>
#include <Core/Materials/BasicPhongMaterial.h>
#include <InstancedMesh.h>
#include <Core/Lights/DirectionalLight.h>
#include <Core/Materials/PhysicalMaterial.h>

const unsigned int WIDTH = 1200;
const  unsigned int HEIGHT = 900;
const unsigned int INSTANCES = 100;

int main()
{
	Renderer* r = new Renderer("Test", WIDTH, HEIGHT, AntialiasingType::MSAAx16);
	Scene* sc = new Scene("mainScene");


	//SETUP SCENE
	//------------------------------------
	PointLight* l = new PointLight(glm::vec3(5.0, 3.0, -4.0), glm::vec3(1.0, 0.8, 0.8), 1.5, 1);
	sc->add(l);


	PhysicalMaterial* templeFloor_m = new PhysicalMaterial();
	templeFloor_m->setAlbedoText(new Texture("floor_albedo.png"));
	templeFloor_m->setNormalText(new Texture("floor_normal.png"));
	templeFloor_m->setRoughnessText(new Texture("floor_MOA.png"));
	PhysicalMaterial* wall_m = new PhysicalMaterial();
	wall_m->setAlbedoText(new Texture("walls_albedo.png"));
	wall_m->setNormalText(new Texture("walls_normal.png"));
	wall_m->setRoughnessText(new Texture("walls_MOA.png"));
	PhysicalMaterial* column_m = new PhysicalMaterial();
	column_m->setAlbedoText(new Texture("columns_albedo.png"));
	column_m->setNormalText(new Texture("columns_normal.png"));
	column_m->setRoughnessText(new Texture("columns_MOA.png"));
	PhysicalMaterial* roof_m = new PhysicalMaterial();
	roof_m->setAlbedoText(new Texture("roofs_albedo.png"));
	roof_m->setNormalText(new Texture("roofs_normal.png"));
	roof_m->setRoughnessText(new Texture("roofs_MOA.png"));



	Model* temple = new	Model("templeM.obj", column_m);
	sc->add(temple);
	temple->setPosition(glm::vec3(0.0, 0.0, -25.0));
	temple->setMaterial(templeFloor_m, 1);
	temple->setMaterial(roof_m, 2);
	temple->setMaterial(wall_m, 3);

	//Texture* boxColorTex = new Texture("SeamlessWood-Diffuse.jpg");
	//Texture* boxNormalTex = new Texture("SeamlessWood-NormalMap.tif");
	//BasicPhongMaterial* box_m = new BasicPhongMaterial();
	//box_m->addColorTex(boxColorTex);
	//box_m->addNormalTex(boxNormalTex);
	////box_m->setOpacity(0.2);
	//box_m->setTransparency(true);

	//Model* box = new Model("pablo", "box.obj", box_m);
	//box->setPosition(glm::vec3(4.0, 1.0, 0.0));
	//box->setScale(glm::vec3(2.0, 2, 2));
	////box->setRotation(glm::vec3(90, 60, 0));
	//sc->add(box);
	//Model* box2 = box->clone();
	//box2->setPosition(glm::vec3(2.0, 0.5, 2.0));

	//sc->add(box2);
	//Model* box3 = box->clone();
	//box3->setPosition(glm::vec3(2.0, 0.5, -2.0));
	//sc->add(box3);


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
	////sc->add(new PointLight(glm::vec3(18.0f, 3.0, 3.5f), glm::vec3(1.0, 0.7, 0.5), 0.6, 1));
	corner1->setPosition(glm::vec3(-18.0f, .0f, 3.5f));
	////sc->add(new PointLight(glm::vec3(-18.0f, 3.0, 3.5f), glm::vec3(1.0, 0.7, 0.5), 0.4, 1));

	corner->setMaterial(templeFloor_m, 2);
	corner->setMaterial(roof_m, 1);
	sc->add(corner);
	corner1->setMaterial(templeFloor_m, 2);
	corner1->setMaterial(roof_m, 1);
	sc->add(corner1);



	Texture* floorAlbedoTex = new Texture("floorUniformAlbedo.png");
	Texture* floorNormalTex = new Texture("floorUniformNormal.png");
	PhysicalMaterial* floor_m = new PhysicalMaterial();
	floor_m->setAlbedoText(floorAlbedoTex);
	floor_m->setNormalText(floorNormalTex);
	floor_m->setRoughnessText(new Texture("floorUniformMOA.png"));
	floor_m->setTileU(60);
	floor_m->setTileV(60);
	floor_m->setRoughness(0.6);
	//floor_m->setReceiveShadows(false);
	//Mesh* planeMesh = new Mesh("plane.obj");
	Model* plane = new Model("plane.obj", floor_m);
	plane->setScale(glm::vec3(3.0f, 1.0f, 3.0f));
	sc->add(plane);

	BasicPhongMaterial* alpha_m = new BasicPhongMaterial();
	alpha_m->setShininess(10);
	alpha_m->setSpecularity(1);
	alpha_m->setBaseColor(glm::vec3(0.0, 0.5, 0.0));
	alpha_m->addOpacityTex(new Texture("alphaTest.jpg"));
	alpha_m->setTransparency(true);
	alpha_m->setFaceVisibility(BOTH);
	Model* alphaShadowTest = plane->clone();
	alphaShadowTest->setMaterial(alpha_m);
	alphaShadowTest->setPosition(glm::vec3(-5.0f, 1.0f, 0.0f));
	alphaShadowTest->setScale(glm::vec3(0.02, 1.0, 0.02));
	alphaShadowTest->setRotation(glm::vec3(1.57, 0.0, -1.57));
	//alphaShadowTest->getMesh()->setCastShadows(false);
	sc->add(alphaShadowTest);




	/*Texture* tenguColorTex = new Texture("tengu-color.png");
	Texture* tenguNormalTex = new Texture("tenguNormal.png");
	BasicPhongMaterial* tengu_m = new BasicPhongMaterial();
	tengu_m->addColorTex(tenguColorTex);
	tengu_m->addNormalTex(tenguNormalTex);*/


	//Model* demon = new Model("tengu.obj", tengu_m);
	////demon->setActive(false);
	//sc->add(demon);


	//PhysicalMaterial* pbrMat = new PhysicalMaterial();
	//pbrMat->setRoughness(0.5);
	//pbrMat->setMetalness(0.75);
	//
	//pbrMat->setAlbedoText(new Texture("tenguAlbedo.png"));
	//pbrMat->setNormalText(tenguNormalTex);
	////pbrMat->setOpacityMask(new Texture("tenguOP.png"));
	////pbrMat->setRoughnessText(new Texture("tenguORM.png"));

	//pbrMat->setAlbedoColor(glm::vec3(1.0, 0.0, 0.0));
	//Model* demonPBR = demon->clone();
	//demonPBR->setMaterial(pbrMat);
	//demonPBR->setPosition(glm::vec3(2.0f, 0.0f, 0.0f));
	//sc->add(demonPBR);

	//r->setGammaCorrection(false);

	/*DirectionalLight* dl = new DirectionalLight(glm::vec3(5.0, 5.0, 5.0),glm::vec3(1.0, 0.8, 0.8), 0.25f);
	dl->setPosition(glm::vec3(50.0, 50.0, 50.0));
	sc->add(dl);*/

	/*CubeMapFaces skyFaces("night-sky/px.png",
		"night-sky/nx.png",
		"night-sky/py.png",
		"night-sky/ny.png",
		"night-sky/pz.png",
		"night-sky/nz.png");


	CubeMapTexture* skyText = new CubeMapTexture(skyFaces);
	SkyboxMesh* skybox = new SkyboxMesh(new SkyboxMaterial(skyText));
	sc->setSkybox(skybox);*/

	sc->getActiveCamera()->setFOV(60.0f);
	sc->getActiveCamera()->setProj(WIDTH, HEIGHT);
	sc->getActiveCamera()->setFar(500);
	sc->setAmbientStrength(0.03f);
	r->addScene(sc);
	r->setCurrentScene("mainScene");
	r->setPostProcessPass(true);

	r->run();


}