#include "SceneLoading.h"
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


SceneLoading::SceneLoading()
{
	fontData = getNumberValues("Image//Fonts//FontData.csv");
	goNextScene = false;
}

SceneLoading::~SceneLoading()
{
}

void SceneLoading::Init()
{
	// Init VBO here
	// set background color to dark blue
	glClearColor(1.f, 1.f, 1.f, 0.0f);
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

	meshList[GEO_LOADING_BAR] = MeshBuilder::GenerateQuad("loading bar", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOADING_BAR]->textureID = LoadTGA("Image//phoneUI//socialScore_bar.tga");
	meshList[GEO_LOADING_SCREEN] = MeshBuilder::GenerateQuad("loading bar", Color(1, 1, 1), 1, 1);
	meshList[GEO_LOADING_SCREEN]->textureID = LoadTGA("Image//computerUI//work//loadingBar.tga");

	camera.canLookAround = false;
	camera.canMove = false;

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
}

void SceneLoading::TransferGameInfo(Game* game)
{
}


void SceneLoading::Update(double dt)
{
	camera.Update(static_cast<float>(dt));
	Application::getCursorPosition(xpos, ypos);
	Application::worldSpaceToScreenSpace(xpos, ypos);
	Application::hideCursorWhenInScreen();

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


	//update
	timer += static_cast<float>(dt);
	if (timer >= 3) {
		goNextScene = true;
		timer = 0;
	}
	else {
		goNextScene = false;
	}
	timer2 += static_cast<float>(dt * 3);
	if (timer2 >= 4) {
		timer2 = 0;
	}
}


void SceneLoading::Render()
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
	}

	//render loading bar
	{
		RenderImageOnScreen(meshList[GEO_LOADING_BAR], Color(1, 1, 1), (timer) * 20, 5, 10 + (timer) * 10, 29);
		RenderImageOnScreen(meshList[GEO_LOADING_SCREEN], Color(1, 1, 1), 60, 5, 40, 29);
		RenderTextOnScreen(meshList[GEO_TEXT], "Loading", Color(0.6f, 0.3f, 0.3f), 3, 33, 32);
		if (timer2 > 1) {
			RenderTextOnScreen(meshList[GEO_TEXT], ".", Color(0.6f, 0.3f, 0.3f), 4, 45, 32);
		}
		if (timer2 > 2) {
			RenderTextOnScreen(meshList[GEO_TEXT], ".", Color(0.6f, 0.3f, 0.3f), 4, 47, 32);
		}
		if (timer2 > 3) {
			RenderTextOnScreen(meshList[GEO_TEXT], ".", Color(0.6f, 0.3f, 0.3f), 4, 49, 32);
		}
	}

	//render cursor last because it has to show up on top
	{
		modelStack.PushMatrix();
		RenderImageOnScreen(meshList[GEO_TEMP_QUAD], Color(1, 1, 1), 5, 5, static_cast<float>(xpos), static_cast<float>(ypos - 3), 0, 180);
		modelStack.PopMatrix();
	}
}

void SceneLoading::Reset()
{
}

void SceneLoading::Exit()
{
	// Cleanup VBO here
	delete meshList[GEO_AXES];
	glDeleteVertexArrays(1, &m_vertexArrayID);

	glDeleteProgram(m_programID);
}

void SceneLoading::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneLoading::RenderSkybox()
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

void SceneLoading::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneLoading::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float xRotation, float zRotation)
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

std::vector<float> SceneLoading::getNumberValues(std::string filename)
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

void SceneLoading::RenderImageOnScreen(Mesh* mesh, Color color, float sizex, float sizey, float x, float y, float xRotation, float zRotation)
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

