#include "SceneMenu.h"
#include "Application.h"
#include "Mtx44.h"
#include "GL\glew.h"
#include <iostream>

#include "shader.hpp"
#include "Utility.h"
#include <vector>
#include <fstream>
#include <sstream>
#include "clock.h"


SceneMenu::SceneMenu()
{
	fontData = getNumberValues("Image//Fonts//FontData.csv");
	goNextScene = false;
}

SceneMenu::~SceneMenu()
{
}

void SceneMenu::Init()
{
	// Init VBO here
	// set background color to dark blue
	glClearColor(1.f, 1.f, 1.f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//load parameters
	{
		m_programID = LoadShaders("Shader//Texture.vertexshader", "Shader//Text.fragmentshader");
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
		m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
		m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
		m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
		m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
		m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
		m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
		m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
		m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
		m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
		m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
		m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
		m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
		m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
		m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
		m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
		m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
		m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

		m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
		m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
		m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
		m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
		m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
		m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
		m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
		m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
		m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
		m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
		m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

		m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");
		m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
		m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

		m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
		m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");
		//use our shader
		glUseProgram(m_programID);
		glUniform1i(m_parameters[U_NUMLIGHTS], 2);
	}

	//light parameters
	{
		//get a handle for our "MVP" uniform
		m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
		light[0].type = Light::LIGHT_DIRECTIONAL;
		light[0].position.Set(0, 40, 0);
		light[0].color.Set(0.9f, 0.8f, 0.5);
		light[0].power = 0.6f;
		light[0].kC = 1.f;
		light[0].kL = 0.01f;
		light[0].kQ = 0.001f;
		light[0].cosCutoff = cos(Math::DegreeToRadian(45));
		light[0].cosInner = cos(Math::DegreeToRadian(30));
		light[0].exponent = 3.f;
		light[0].spotDirection.Set(0.f, 0.f, 0.f);

		glUniform1i(m_parameters[U_LIGHT0_TYPE], light[0].type);
		glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &light[0].color.r);
		glUniform1f(m_parameters[U_LIGHT0_POWER], light[0].power);
		glUniform1f(m_parameters[U_LIGHT0_KC], light[0].kC);
		glUniform1f(m_parameters[U_LIGHT0_KL], light[0].kL);
		glUniform1f(m_parameters[U_LIGHT0_KQ], light[0].kQ);
		glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], light[0].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT0_COSINNER], light[0].cosInner);
		glUniform1f(m_parameters[U_LIGHT0_EXPONENT], light[0].exponent);

		light[1].type = Light::LIGHT_POINT;
		light[1].position.Set(0, 15, 0);
		light[1].color.Set(0.9f, 0.8f, 0.5);
		light[1].power = 0.0f;
		light[1].kC = 1.f;
		light[1].kL = 0.01f;
		light[1].kQ = 0.001f;
		light[1].cosCutoff = cos(Math::DegreeToRadian(45));
		light[1].cosInner = cos(Math::DegreeToRadian(30));
		light[1].exponent = 3.f;
		light[1].spotDirection.Set(0.f, 1.f, 0.f);

		glUniform1i(m_parameters[U_LIGHT1_TYPE], light[1].type);
		glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &light[1].color.r);
		glUniform1f(m_parameters[U_LIGHT1_POWER], light[1].power);
		glUniform1f(m_parameters[U_LIGHT1_KC], light[1].kC);
		glUniform1f(m_parameters[U_LIGHT1_KL], light[1].kL);
		glUniform1f(m_parameters[U_LIGHT1_KQ], light[1].kQ);
		glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], light[1].cosCutoff);
		glUniform1f(m_parameters[U_LIGHT1_COSINNER], light[1].cosInner);
		glUniform1f(m_parameters[U_LIGHT1_EXPONENT], light[1].exponent);
	}

	camera.Init(Vector3(0, 15, 0), Vector3(0, 18, -10), Vector3(0, 1, 0));

	Mesh::SetMaterialLoc(m_parameters[U_MATERIAL_AMBIENT], m_parameters[U_MATERIAL_DIFFUSE], m_parameters[U_MATERIAL_SPECULAR], m_parameters[U_MATERIAL_SHININESS]);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//essentials
	{
		meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 100, 100, 100);
		meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("Lightball", Color(0.2f, 0.5f, 0.6f), 1, 10, 18);
	}

	//skybox
	{
		meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
		meshList[GEO_FRONT]->textureID = LoadTGA("Image//ICA2Skybox//Back.tga");
		meshList[GEO_BACK] = MeshBuilder::GenerateQuad("back", Color(1, 1, 1), 1.f);
		meshList[GEO_BACK]->textureID = LoadTGA("Image//ICA2Skybox//Front.tga");
		meshList[GEO_TOP] = MeshBuilder::GenerateQuad("top", Color(1, 1, 1), 1.f);
		meshList[GEO_TOP]->textureID = LoadTGA("Image//ICA2Skybox//Top.tga");
		meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("bottom", Color(1, 1, 1), 1.f);
		meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//ICA2Skybox//Bottom.tga");
		meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("left", Color(1, 1, 1), 1.f);
		meshList[GEO_LEFT]->textureID = LoadTGA("Image//ICA2Skybox//Left.tga");
		meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("right", Color(1, 1, 1), 1.f);
		meshList[GEO_RIGHT]->textureID = LoadTGA("Image//ICA2Skybox//Right.tga");
	}

	//others
	meshList[GEO_TEMP_QUAD] = MeshBuilder::GenerateQuad("Diag", Color(1, 1, 1), 1, 1.5, 1);
	meshList[GEO_TEMP_QUAD]->textureID = LoadTGA("Image//cursor.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//fonts//SegoeUI.tga");
	meshList[GEO_DIALOG_BOX] = MeshBuilder::GenerateQuad("Diag", Color(1, 1, 1), 80, 30, 1);
	meshList[GEO_DIALOG_BOX]->textureID = LoadTGA("Image//dialogBox.tga");

	//city
	{
		//can u name properly wtf
		meshList[GEO_HOUSE1] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//MainHouse.obj", "OBJ//City//MainHouse.mtl");
		meshList[GEO_ROAD] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Road.obj", "OBJ//City//Road.mtl");
		meshList[GEO_HOUSE2] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//house2.obj", "OBJ//City//house2.mtl");
		meshList[GEO_HOUSE5] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//house5.obj", "OBJ//City//house5.mtl");

		meshList[GEO_HOUSECAR] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//HouseCar.obj", "OBJ//City//HouseCar.mtl");
		meshList[GEO_BIGHOUSE] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//BigHouse.obj", "OBJ//City//BigHouse.mtl");


		meshList[GEO_GRASS] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Grass.obj", "OBJ//City//Grass.mtl");
		meshList[GEO_FENCE] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//fence.obj", "OBJ//City//fence.mtl");

		meshList[GEO_CROSSWALK] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Crosswalk.obj", "OBJ//City//Crosswalk.mtl");
		meshList[GEO_HOUSE3] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//house3.obj", "OBJ//City//house3.mtl");
		meshList[GEO_HOUSE4] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//house4.obj", "OBJ//City//house4.mtl");
		meshList[GEO_TREE] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Tree.obj", "OBJ//City//Tree.mtl");
		meshList[GEO_BIGTREE] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//BigTree.obj", "OBJ//City//BigTree.mtl");

		meshList[GEO_CURVE] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//RoadCurve.obj", "OBJ//City//RoadCurve.mtl");
		meshList[GEO_SKYSCRAPER] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Skyscraper.obj", "OBJ//City//Skyscraper.mtl");
		meshList[GEO_SKYSCRAPER2] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Skyscraper2.obj", "OBJ//City//Skyscraper2.mtl");
		meshList[GEO_SKYSCRAPER3] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Skyscraper3.obj", "OBJ//City//Skyscraper3.mtl");
		meshList[GEO_SKYSCRAPER4] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Skyscraper4.obj", "OBJ//City//Skyscraper4.mtl");
		meshList[GEO_SKYSCRAPER5] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Skyscraper5.obj", "OBJ//City//Skyscraper5.mtl");
		meshList[GEO_SKYSCRAPER6] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Skyscraper6.obj", "OBJ//City//Skyscraper6.mtl");
		meshList[GEO_SKYSCRAPER7] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Skyscraper7.obj", "OBJ//City//Skyscraper7.mtl");

		meshList[GEO_BIGAPARTMENT] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//BigApartment.obj", "OBJ//City//BigApartment.mtl");
		meshList[GEO_BIGAPARTMENT2] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//BigApartment2.obj", "OBJ//City//BigApartment2.mtl");
		meshList[GEO_BIGAPARTMENT3] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//BigApartment3.obj", "OBJ//City//BigApartment3.mtl");

		meshList[GEO_BUILDING] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Building.obj", "OBJ//City//Building.mtl");


		meshList[GEO_CONDO] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Condo.obj", "OBJ//City//Condo.mtl");
		meshList[GEO_CONDO2] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Condo2.obj", "OBJ//City//Condo2.mtl");

		meshList[GEO_CAR] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//CAR.obj", "OBJ//City//CAR.mtl");
		meshList[GEO_AMBULANCE] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Ambulance.obj", "OBJ//City//Ambulance.mtl");
		meshList[GEO_VAN] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Van.obj", "OBJ//City//Van.mtl");
		meshList[GEO_TAXI] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Taxi.obj", "OBJ//City//Taxi.mtl");
		meshList[GEO_FIRETRUCK] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Firetruck.obj", "OBJ//City//Firetruck.mtl");
		meshList[GEO_POLICE] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//Police.obj", "OBJ//City//Police.mtl");


		meshList[GEO_FARBUILDING] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//FarBuilding.obj", "OBJ//City//FarBuilding.mtl");
		meshList[GEO_FARBUILDING2] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//FarBuilding2.obj", "OBJ//City//FarBuilding2.mtl");
		meshList[GEO_FARBUILDING3] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//FarBuilding3.obj", "OBJ//City//FarBuilding3.mtl");
		meshList[GEO_FARBUILDING4] = MeshBuilder::GenerateOBJMTL("model7", "OBJ//City//FarBuilding4.obj", "OBJ//City//FarBuilding4.mtl");

		meshList[GEO_MARKER] = MeshBuilder::GenerateCylinder("marker", Color(0.9f, 0.3f, 0.3f), 5);
	}

	//menu
	{
		meshList[GEO_TITLE] = MeshBuilder::GenerateText("title", 16, 16);
		meshList[GEO_TITLE]->textureID = LoadTGA("Image//fonts//ArialRegularBoldItalic.tga");
		meshList[GEO_TITLEIMAGE] = MeshBuilder::GenerateQuad("titleimage", Color(1, 1, 1), 1.f);
		meshList[GEO_TITLEIMAGE]->textureID = LoadTGA("Image//titleimage.tga");
		meshList[GEO_SCAMIMAGE] = MeshBuilder::GenerateQuad("scamimage", Color(1, 1, 1), 1.f);
		meshList[GEO_SCAMIMAGE]->textureID = LoadTGA("Image//isitscam.tga");
		meshList[GEO_START] = MeshBuilder::GenerateText("start", 16, 16);
		meshList[GEO_START]->textureID = LoadTGA("Image//fonts//SegoeUI.tga");
		meshList[GEO_STARTBUTTON] = MeshBuilder::GenerateQuad("startbutton", Color(1, 1, 1), 1.f);
		meshList[GEO_STARTBUTTON]->textureID = LoadTGA("Image//startButton.tga");
		meshList[GEO_QUIT] = MeshBuilder::GenerateText("quit", 16, 16);
		meshList[GEO_QUIT]->textureID = LoadTGA("Image//fonts//SegoeUI.tga");
		meshList[GEO_QUITBUTTON] = MeshBuilder::GenerateQuad("quitbutton", Color(1, 1, 1), 1.f);
		meshList[GEO_QUITBUTTON]->textureID = LoadTGA("Image//startButton.tga");
	}

	camera.canLookAround = false;
	camera.canMove = false;

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
}

void SceneMenu::TransferGameInfo(Game* game)
{
}


void SceneMenu::Update(double dt)
{
	camera.Update(static_cast<float>(dt));
	Application::getCursorPosition(xpos, ypos);
	Application::worldSpaceToScreenSpace(xpos, ypos);
	Application::hideCursorWhenInScreen();

	camera.lookRight(float(dt / 10));

	//debug controls xx
	{
		if (Application::IsKeyPressed('1')) {
			glEnable(GL_CULL_FACE);
		}
		if (Application::IsKeyPressed('2')) {
			glDisable(GL_CULL_FACE);
		}

		if (Application::IsKeyPressed('3')) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
		}
		if (Application::IsKeyPressed('4')) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //line mode
		}

		if (Application::IsKeyPressed('5')) {
			enableLight = false;
		}
		if (Application::IsKeyPressed('6')) {
			enableLight = true;
		}
	}

	if (!Application::isMouseButtonPressed(0) && !Application::isMouseButtonPressed(1) && !Application::isMouseButtonPressed(2)) {
		mouseToggle = true;
	}

	//Mouse Inputs
	if (xpos > 26 && xpos < 56.5 && ypos > 16.5 && ypos < 21.5 && mouseToggle && Application::isMouseButtonPressed(0))
	{
		goNextScene = true;
	}
	else if (xpos > 26 && xpos < 56.5 && ypos > 10 && ypos < 15 && mouseToggle && Application::isMouseButtonPressed(0)) {
		goNextScene2 = true;
	}
	rotate += (float)(dt) * 10;
	
}


void SceneMenu::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//projection.SetToOrtho(-600, 600, -400, 400, -10, 10); //note: -40, 40, -30, 30, -10, 10 normally

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]); //update the shader with new MVP

	Position lightPosition_cameraspace = viewStack.Top() * light[0].position;
	glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);

	//render skybox, axes and lightball
	{
		RenderMesh(meshList[GEO_AXES], false);
		RenderSkybox();
	}

	//render city
	{
		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(60, 60, 60);
		modelStack.Translate(-0.5f, 0, 0.89f);
		RenderMesh(meshList[GEO_HOUSE1], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Translate(-2, 0, 0.89f);
		RenderMesh(meshList[GEO_HOUSECAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(20, 20, 20);
		modelStack.Translate(-7.15f, 0, 2.59f);
		RenderMesh(meshList[GEO_CAR], true);
		modelStack.PopMatrix();

		//roads
		modelStack.PushMatrix();//
		modelStack.Scale(60, 60, 60);
		modelStack.Translate(0.5f, 0, -0.3f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Translate(1.5f, 0, -0.3f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Translate(-0.5f, 0, -0.3f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Translate(-1.5f, 0, -0.3f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Translate(3.0f, 0, 0.2f);
		RenderMesh(meshList[GEO_CURVE], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Translate(1.7f, 0, -3.5f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Translate(2.7f, 0, -3.5f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Translate(3.7f, 0, -3.5f);

		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Translate(5.2f, 0, -3.0f);
		RenderMesh(meshList[GEO_CURVE], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(-1.5f, 0, -5.7f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(-0.5f, 0, -5.7f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(0.5f, 0, -5.7f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(1.5f, 0, -5.7f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(2.9f, 0, -5.2f);
		RenderMesh(meshList[GEO_CURVE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-270, 0, 1, 0);
		modelStack.Translate(-3.7f, 0, -3.4f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-270, 0, 1, 0);
		modelStack.Translate(-2.7f, 0, -3.4f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-270, 0, 1, 0);
		modelStack.Translate(-1.7f, 0, -3.4f);
		RenderMesh(meshList[GEO_ROAD], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-270, 0, 1, 0);
		modelStack.Translate(-0.2f, 0, -2.9f);
		RenderMesh(meshList[GEO_CURVE], true);
		modelStack.PopMatrix();
		//.. 

		modelStack.PushMatrix();
		modelStack.Scale(60, 60, 60);
		modelStack.Translate(1.0f, 0, 0.89f);
		RenderMesh(meshList[GEO_HOUSE2], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Scale(75, 75, 75);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Translate(1.7f, 0, -1.5f);
		RenderMesh(meshList[GEO_BIGHOUSE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(75, 75, 75);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(-1.7f, 0, -1.5f);
		RenderMesh(meshList[GEO_BIGHOUSE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(30, 30, 30);
		modelStack.Translate(0.7f, 0, 0.55f);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(30, 30, 30);
		modelStack.Translate(0.7f, 0, 2);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(30, 30, 30);
		modelStack.Translate(0.7f, 0, -2.75f);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Scale(30, 30, 30);
		modelStack.Translate(0.7f, 0, -4.2f);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(50, 30, 50);
		modelStack.Translate(3.2f, 0, 1.15f);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(50, 30, 50);
		modelStack.Translate(2.2f, 0, 0.15f);
		RenderMesh(meshList[GEO_FENCE], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(30, 30, 30);
		modelStack.Translate(3.5f, 0, 2);
		RenderMesh(meshList[GEO_BIGTREE], true);
		modelStack.PopMatrix();

		/*modelStack.PushMatrix();
		modelStack.Scale(30,30,30);
		modelStack.Translate(0, 0.1, 0);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_CROSSWALK], true);
		modelStack.PopMatrix();*/

		modelStack.PushMatrix();//floor
		//scale, translate, rotate
		modelStack.Scale(1500, 1500, 1500);
		modelStack.Translate(0, 0, -0.1f);
		RenderMesh(meshList[GEO_GRASS], true);
		modelStack.PopMatrix();




		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(60, 60, 60);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(0.5f, 0, 1.5f);
		RenderMesh(meshList[GEO_HOUSE1], true);
		modelStack.PopMatrix();//yes

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(60, 60, 60);
		modelStack.Translate(-2, 0, -1.5f);
		RenderMesh(meshList[GEO_HOUSECAR], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(60, 60, 60);
		modelStack.Translate(-3.3f, 0, -1.5f);
		RenderMesh(meshList[GEO_HOUSE2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(20, 20, 20);
		modelStack.Translate(-7.15f, 0, -4.59f);
		RenderMesh(meshList[GEO_CAR], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(60, 60, 60);
		//modelStack.Rotate(0, -45, 0, 0);
		modelStack.Translate(1, 0, -1.5f);
		RenderMesh(meshList[GEO_HOUSE2], true);
		modelStack.PopMatrix();//otherside

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(60, 60, 60);
		//modelStack.Rotate(0, -45, 0, 0);
		modelStack.Translate(2.2f, 0, -1.8f);
		RenderMesh(meshList[GEO_HOUSE5], true);
		modelStack.PopMatrix();//otherside

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(60, 60, 60);
		//modelStack.Rotate(0, -45, 0, 0);
		modelStack.Translate(3.5f, 0, -1.8f);
		RenderMesh(meshList[GEO_CONDO2], true);
		modelStack.PopMatrix();//otherside



		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(30, 30, 30);
		modelStack.Translate(0.7f, 0, 1.55f);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(20, 20, 20);
		modelStack.Rotate(-270, 0, 1, 0);
		modelStack.Translate(-17.7f, 0, -2.55f);
		RenderMesh(meshList[GEO_TAXI], true);
		modelStack.PopMatrix();//taxi


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(20, 20, 20);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Translate(-1.5f, 0, -1.55f);
		RenderMesh(meshList[GEO_VAN], true);
		modelStack.PopMatrix();//van


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(20, 20, 20);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(-11, 0, -3.55f);
		RenderMesh(meshList[GEO_FIRETRUCK], true);
		modelStack.PopMatrix();//firetruck



		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(20, 20, 20);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(9.5f, 0, -3.55f);
		RenderMesh(meshList[GEO_POLICE], true);
		modelStack.PopMatrix();//police



		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(30, 30, 30);
		modelStack.Translate(0.7f, 0, -2.55f);
		RenderMesh(meshList[GEO_TREE], true);
		modelStack.PopMatrix();



		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(-3, 0, 4);
		RenderMesh(meshList[GEO_SKYSCRAPER], true);
		modelStack.PopMatrix();





		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(-0.5f, 0, 4.3f);
		RenderMesh(meshList[GEO_BUILDING], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(20, 20, 20);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(-6, 0, 15);
		RenderMesh(meshList[GEO_AMBULANCE], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(1.4f, 0, 4.3f);
		RenderMesh(meshList[GEO_BIGAPARTMENT3], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(-1, 0, -3);
		RenderMesh(meshList[GEO_SKYSCRAPER2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Translate(2, 0, 6);
		RenderMesh(meshList[GEO_SKYSCRAPER3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Translate(0.5f, 0, 6);
		RenderMesh(meshList[GEO_SKYSCRAPER4], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Translate(-1, 0, 6);
		RenderMesh(meshList[GEO_SKYSCRAPER5], true);
		modelStack.PopMatrix();



		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Translate(-4, 0, 3);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_SKYSCRAPER7], true);
		modelStack.PopMatrix();//right side

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Translate(-4, 0, 1.5f);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_SKYSCRAPER3], true);//apartment right 
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Translate(-4, 0, -0.5f);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_BUILDING], true);//apartment right 
		modelStack.PopMatrix();



		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Rotate(-45, 0, 1, 0);
		modelStack.Translate(1.5f, 0, 6);
		RenderMesh(meshList[GEO_BIGAPARTMENT], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Rotate(45, 0, 1, 0);
		modelStack.Translate(-0.8f, 0, 6);
		RenderMesh(meshList[GEO_BIGAPARTMENT2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(0.25f, 0, -3.2f);
		RenderMesh(meshList[GEO_CONDO], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(75, 75, 75);
		modelStack.Rotate(-180, 0, 1, 0);
		modelStack.Translate(1.5f, 0, -3);
		RenderMesh(meshList[GEO_CONDO2], true);
		modelStack.PopMatrix();




		//faraway building

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(175, 300, 175);
		//modelStack.Rotate(45, 0, 1, 0);
		modelStack.Translate(-1, 0, 3.2f);
		RenderMesh(meshList[GEO_FARBUILDING], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(300, 300, 300);
		//modelStack.Rotate(45, 0, 1, 0);
		modelStack.Translate(0, 0, 2);
		RenderMesh(meshList[GEO_FARBUILDING2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(300, 300, 300);
		//modelStack.Rotate(45, 0, 1, 0);
		modelStack.Translate(0.5f, 0, 2);
		RenderMesh(meshList[GEO_FARBUILDING3], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(300, 300, 300);
		modelStack.Rotate(-45, 0, 1, 0);
		modelStack.Translate(1.8f, 0, 0.2f);
		RenderMesh(meshList[GEO_FARBUILDING4], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(300, 300, 300);
		modelStack.Rotate(-45, 0, 1, 0);
		modelStack.Translate(-0.1f, 0, 2);
		RenderMesh(meshList[GEO_FARBUILDING4], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(175, 300, 175);
		//modelStack.Rotate(45, 0, 1, 0);
		modelStack.Translate(-1, 0, -1);
		RenderMesh(meshList[GEO_FARBUILDING], true);
		modelStack.PopMatrix();


		//pther side


		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(175, 300, 175);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(-0.8f, 0, 2.5f);
		RenderMesh(meshList[GEO_FARBUILDING2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(175, 300, 175);
		modelStack.Rotate(90, 0, 1, 0);
		modelStack.Translate(0.3f, 0, 2.5f);
		RenderMesh(meshList[GEO_FARBUILDING3], true);
		modelStack.PopMatrix();

		////right side

		modelStack.PushMatrix();
		//scale, translate, rotate
		modelStack.Scale(175, 300, 175);
		modelStack.Translate(-2.5f, 0, 1);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_FARBUILDING3], true);
		modelStack.PopMatrix();



		modelStack.PushMatrix();
		modelStack.Scale(175, 300, 175);
		modelStack.Translate(-2.5f, 0, 0.2f);
		modelStack.Rotate(-90, 0, 1, 0);
		RenderMesh(meshList[GEO_FARBUILDING4], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Scale(175, 300, 175);
		modelStack.Translate(-0.3f, 0, -1);
		RenderMesh(meshList[GEO_FARBUILDING2], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Scale(175, 300, 175);
		modelStack.Translate(0.3f, 0, -1);
		RenderMesh(meshList[GEO_FARBUILDING3], true);
		modelStack.PopMatrix();


		modelStack.PushMatrix();
		modelStack.Scale(175, 300, 175);
		modelStack.Translate(1.5f, 0, -1.5f);
		RenderMesh(meshList[GEO_FARBUILDING4], true);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-195, -0.4f, -42);
		modelStack.Scale(2, 1, 2);
		RenderMesh(meshList[GEO_MARKER], false);
		modelStack.PopMatrix();
	}

	//render mainmenu
	{
		RenderImageOnScreen(meshList[GEO_TITLEIMAGE], Color(0, 0, 0), 85, 40, 40, 40);
		RenderImageOnScreen(meshList[GEO_SCAMIMAGE], Color(0, 0, 0), 30, 15, 65, 30, 0, 180);
		RenderTextOnScreen(meshList[GEO_TITLE], "SCAMULATION", Color(0, 0, 0), 8, 14.6, 35.6);
		RenderTextOnScreen(meshList[GEO_TITLE], "SCAMULATION", Color(1, 0, 0), 8, 15, 36);
		RenderImageOnScreen(meshList[GEO_STARTBUTTON], Color(0, 0, 0), 30, 5, 41.5, 19);
		RenderTextOnScreen(meshList[GEO_START], "START", Color(0, 0, 0), 5, 34, 16.5);
		RenderImageOnScreen(meshList[GEO_QUITBUTTON], Color(0, 0, 0), 30, 5, 41.5, 13);
		RenderTextOnScreen(meshList[GEO_QUIT], "QUIT", Color(0, 0, 0), 5, 36, 10.5);
	}

	//render cursor last because it has to show up on top
	{
		modelStack.PushMatrix();
		RenderImageOnScreen(meshList[GEO_TEMP_QUAD], Color(1, 1, 1), 5, 5, static_cast<float>(xpos), static_cast<float>(ypos - 3), 0, 180);
		modelStack.PopMatrix();
	}
}

void SceneMenu::Exit()
{
	// Cleanup VBO here
	delete meshList[GEO_AXES];
	glDeleteVertexArrays(1, &m_vertexArrayID);

	glDeleteProgram(m_programID);
}

void SceneMenu::RenderMesh(Mesh* mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	if (mesh->textureID > 0) {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else {
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureID > 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SceneMenu::RenderSkybox()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 0, camera.position.z);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Translate(0, -0.499f, 0);
	modelStack.Rotate(-90, 1, 0, 0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 0, camera.position.z);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Translate(0, 0.499f, 0);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 0, camera.position.z);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Translate(0, 0, -0.499f);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 0, camera.position.z);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Translate(0, 0, 0.499f);
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Rotate(180, 0, 0, 1);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 0, camera.position.z);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Translate(-0.499f, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, 0, camera.position.z);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	modelStack.Translate(0.499f, 0, 0);
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Rotate(180, 1, 0, 0);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();
}

void SceneMenu::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	//glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	float offset = 0;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.6f + offset, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		offset += (fontData[(int)(text[i])] / (float)64);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() *
			modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE,
			&MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//glEnable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
}

void SceneMenu::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float xRotation, float zRotation)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen

	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Rotate(xRotation, 1, 0, 0);
	modelStack.Rotate(zRotation, 0, 0, 1);
	modelStack.Scale(size, size, size);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	float offset = 0;
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(0.6f + offset, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		offset += (fontData[(int)(text[i])] / (float)64);
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
}

std::vector<float> SceneMenu::getNumberValues(std::string filename)
{
	std::vector<float> ret;
	int x = 8;
	std::string str;
	std::fstream file(filename, std::ios::in);
	if (file.is_open())
	{
		while (std::getline(file, str))
		{
			if (x > 0) {
				x--;

			}
			else {
				std::size_t it = str.find(",");
				if (it != std::string::npos) {
					ret.push_back(std::stof(str.substr(it + 1)));
				}
			}
		}
	}
	else
		std::cout << "Could not open the file\n";

	return ret;
}

void SceneMenu::RenderImageOnScreen(Mesh* mesh, Color color, float sizex, float sizey, float x, float y, float xRotation, float zRotation)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	glDisable(GL_CULL_FACE);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Translate(x, y, 0);
	modelStack.Rotate(xRotation, 1, 0, 0);
	modelStack.Rotate(zRotation, 0, 0, 1);
	modelStack.Scale(sizex, sizey, 1);

	Mtx44 characterSpacing;
	characterSpacing.SetToTranslation(0.5f, 0.5f, 0);
	Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	glUniform1i(m_parameters[U_LIGHTENABLED], 1);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	RenderMesh(mesh, false);
	glBindTexture(GL_TEXTURE_2D, 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();
	glEnable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
	glEnable (GL_CULL_FACE);

}

