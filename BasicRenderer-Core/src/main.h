#pragma once

#include <Core/Renderer.h>
#include <Core/Lights/PointLight.h>
#include <Core/Materials/BasicPhongMaterial.h>

const unsigned int WIDTH = 1200;
const  unsigned int HEIGHT = 900;

int main()
{
	Renderer* r = new Renderer("Test", WIDTH, HEIGHT, AntialiasingType::MSAAx16);

	Scene* sc = new Scene("mainScene");

	//SETUP SCENE
	//------------------------------------
	/*BasicPhongMaterial* sphere_m = new BasicPhongMaterial(m_Shaders);
	sphere_m->setShininess(500);
	sphere_m->setSpecularity(3);
	Model* sphere = new Model();
	sphere->loadMesh("box.obj");
	sphere->loadMaterial(sphere_m);
	sphere->setPosition(glm::vec3(-2.0, 2.0, 0.0));
	m_Models["sphere"] = sphere;*/


	Texture* boxColorTex = new Texture("SeamlessWood-Diffuse.jpg");
	Texture* boxNormalTex = new Texture("SeamlessWood-NormalMap.tif");
	BasicPhongMaterial* box_m = new BasicPhongMaterial();
	box_m->addColorTex(boxColorTex);
	box_m->addNormalTex(boxNormalTex);
	//box_m->setOpacity(0.2);
	box_m->setTransparency(true);

	Model* box = new Model("pablo", "box.obj", box_m);
	box->setPosition(glm::vec3(2.0, 1.0, 0.0));
	box->setScale(glm::vec3(2.0, 2, 2));
	//box->setRotation(glm::vec3(90, 60, 0));

	sc->add(box);
	Model* box2 = box->clone();
	box2->setPosition(glm::vec3(2.0, 0.5, 2.0));

	sc->add(box2);
	Model* box3 = box->clone();
	box3->setPosition(glm::vec3(2.0, 0.5, -2.0));
	sc->add(box3);
	

	Texture* floorAlbedoTex = new Texture("floor.jpg");
	Texture* floorNormalTex = new Texture("floor-normal.jpg");
	BasicPhongMaterial* floor_m = new BasicPhongMaterial();
	floor_m->addColorTex(floorAlbedoTex);
	floor_m->addNormalTex(floorNormalTex);
	floor_m->setTileU(20);
	floor_m->setTileV(20);
	//floor_m->setReceiveShadows(false);
	Mesh* planeMesh = new Mesh("plane.obj");
	Model* plane = new Model(planeMesh, floor_m);
	sc->add(plane);


	Texture* tenguColorTex = new Texture("tengu-color.png");
	Texture* tenguNormalTex = new Texture("tengu-normal.png");
	BasicPhongMaterial* tengu_m = new BasicPhongMaterial();
	tengu_m->addColorTex(tenguColorTex);
	tengu_m->addNormalTex(tenguNormalTex);


	Model* demon = new Model("tengu.obj", tengu_m);
	//demon->setActive(false);
	sc->add(demon);

	sc->getActiveCamera()->setProj(WIDTH, HEIGHT);

	PointLight* l = new PointLight(glm::vec3(5.0, 3.0, 4.0), glm::vec3(1.0, 0.8, 0.8), 1.5, 1);
	sc->add(l);
	sc->add(new PointLight(glm::vec3(-4.0, 1.0, 2.0), glm::vec3(1.0, 0.5, 0.5), 1, 1));

	CubeMapFaces skyFaces("night-sky/px.png",
		"night-sky/nx.png",
		"night-sky/py.png",
		"night-sky/ny.png",
		"night-sky/pz.png",
		"night-sky/nz.png");


	/*CubeMapTexture* skyText = new CubeMapTexture(skyFaces);
	SkyboxMesh* skybox = new SkyboxMesh(new SkyboxMaterial(skyText));*/
	//sc->setSkybox(skybox);

	r->addScene(sc);
	r->setCurrentScene("mainScene");
	r->setPostProcessPass(true);

	r->run();


}