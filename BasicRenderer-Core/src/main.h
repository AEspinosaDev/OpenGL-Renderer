#pragma once

#include <Core/Renderer.h>
#include <Core/Lights/PointLight.h>
#include <Core/Materials/BasicPhongMaterial.h>

const unsigned int WIDTH = 1200;
const  unsigned int HEIGHT = 900;

int main()
{
	Renderer* r = new Renderer('test', WIDTH, HEIGHT, AntialiasingType::MSAAx16);

	r->createScene("mainScene");
	Scene* sc = r->getScene("mainScene");

	//SETUP SCENE
	//------------------------------------
	auto m_Shaders = r->getShaders();

	Texture* boxColorTex = new Texture("SeamlessWood-Diffuse.jpg");
	Texture* boxNormalTex = new Texture("SeamlessWood-NormalMap.tif");
	BasicPhongMaterial* box_m = new BasicPhongMaterial(m_Shaders);
	box_m->addColorTex(boxColorTex);
	box_m->addNormalTex(boxNormalTex);
	//box_m->setOpacity(0.9);
	box_m->setTransparency(true);

	Model* box = new Model("pablo");
	box->loadMesh("box.obj");
	box->loadMaterial(box_m);
	box->setPosition(glm::vec3(2.0, 0.5, 0.0));
	sc->add(box);
	Model* box2 = box->clone();
	box2->setPosition(glm::vec3(2.0, 0.5, 2.0));
	sc->add(box2);
	Model* box3 = box->clone();
	box3->setPosition(glm::vec3(2.0, 0.5, -2.0));
	sc->add(box3);
	

	Texture* floorAlbedoTex = new Texture("floor.jpg");
	Texture* floorNormalTex = new Texture("floor-normal.jpg");
	BasicPhongMaterial* floor_m = new BasicPhongMaterial(m_Shaders);
	floor_m->addColorTex(floorAlbedoTex);
	floor_m->addNormalTex(floorNormalTex);
	floor_m->setTileU(20);
	floor_m->setTileV(20);
	//floor_m->setReceiveShadows(false);

	Model* plane = new Model();
	plane->loadMesh("plane.obj");
	plane->loadMaterial(floor_m);
	sc->add(plane);


	Texture* tenguColorTex = new Texture("tengu-color.png");
	Texture* tenguNormalTex = new Texture("tengu-normal.png");
	BasicPhongMaterial* tengu_m = new BasicPhongMaterial(m_Shaders);
	tengu_m->addColorTex(tenguColorTex);
	tengu_m->addNormalTex(tenguNormalTex);


	Model* demon = new Model();
	demon->loadMesh("tengu.obj");
	demon->loadMaterial(tengu_m);
	//demon->setActive(false);
	sc->add(demon);

	sc->getActiveCamera()->setProj(45.0, WIDTH, HEIGHT);

	PointLight* l = new PointLight(glm::vec3(5.0, 3.0, 4.0), glm::vec3(1.0, 0.8, 0.8), 1.5, 1);
	sc->add(l);
	sc->add(new PointLight(glm::vec3(-4.0, 1.0, 2.0), glm::vec3(1.0, 0.5, 0.5), 1, 1));

	CubeMapFaces skyFaces("night-sky/px.png",
		"night-sky/nx.png",
		"night-sky/py.png",
		"night-sky/ny.png",
		"night-sky/pz.png",
		"night-sky/nz.png");


	CubeMapTexture* skyText = new CubeMapTexture(skyFaces);
	SkyboxMesh* skybox = new SkyboxMesh(new SkyboxMaterial(skyText, m_Shaders));
	sc->setSkybox(skybox);

	r->setCurrentScene("mainScene");

	r->run();


}