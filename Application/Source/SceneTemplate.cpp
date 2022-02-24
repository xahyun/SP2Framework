#include "SceneTemplate.h"
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


SceneTemplate::SceneTemplate()
{
	fontData = getNumberValues("Image//Fonts//FontData.csv");
	phoneState = P_PHONEOFF; pageCounter = 0;
}

SceneTemplate::~SceneTemplate()
{
}

void SceneTemplate::Init()
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

	camera.Init(Vector3(0, 15, 0), Vector3(0, 10, -10), Vector3(0, 1, 0));

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

	meshList[GEO_WATCH] = MeshBuilder::GenerateQuad("watch", Color(1, 1, 1), 1, 1, 1);
	meshList[GEO_WATCH]->textureID = LoadTGA("Image//watchInterface.tga");

	//phone UI
	{
		meshList[GEO_PHONE_ICON] = MeshBuilder::GenerateQuad("phoneIcon", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_PHONE_ICON]->textureID = LoadTGA("Image//phoneUI//phoneIcon.tga");
		meshList[GEO_PHONE_UI] = MeshBuilder::GenerateQuad("outsideBox", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_PHONE_UI]->textureID = LoadTGA("Image//phoneUI//outside box.tga");
		meshList[GEO_HOME_ICON] = MeshBuilder::GenerateQuad("home", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_HOME_ICON]->textureID = LoadTGA("Image//phoneUI//home_icon.tga");
		meshList[GEO_CHAT_ICON] = MeshBuilder::GenerateQuad("chat", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_CHAT_ICON]->textureID = LoadTGA("Image//phoneUI//chat_icon.tga");
		meshList[GEO_TASKS_ICON] = MeshBuilder::GenerateQuad("tasks", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_TASKS_ICON]->textureID = LoadTGA("Image//phoneUI//tasks_icon.tga");
		meshList[GEO_MONEY_ICON] = MeshBuilder::GenerateQuad("outsideBox", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_MONEY_ICON]->textureID = LoadTGA("Image//phoneUI//money_icon.tga");
		meshList[GEO_SOCIALSCORE_ICON] = MeshBuilder::GenerateQuad("outsideBox", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_SOCIALSCORE_ICON]->textureID = LoadTGA("Image//phoneUI//ss_icon.tga");
		meshList[GEO_SOCIALSCORE_UI] = MeshBuilder::GenerateQuad("socialScoreUI", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_SOCIALSCORE_UI]->textureID = LoadTGA("Image//phoneUI//socialScore_UI.tga");
		meshList[GEO_SOCIALSCORE_BAR] = MeshBuilder::GenerateQuad("socialScoreUI", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_SOCIALSCORE_BAR]->textureID = LoadTGA("Image//phoneUI//socialScore_bar.tga");
		meshList[GEO_HELP_ICON] = MeshBuilder::GenerateQuad("help", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_HELP_ICON]->textureID = LoadTGA("Image//phoneUI//help_icon.tga");
	}

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 1000.f);
	projectionStack.LoadMatrix(projection);
}

void SceneTemplate::TransferGameInfo(Game* game)
{
	this->clock = game->gameClock;
	this->Player = game->gamePlayer;
	this->phone = game->playerPhone;
}


void SceneTemplate::Update(double dt)
{
	camera.Update(static_cast<float>(dt));
	Application::getCursorPosition(xpos, ypos);
	Application::worldSpaceToScreenSpace(xpos, ypos);
	Application::hideCursorWhenInScreen();

	clock->UpdateClock(dt);
	phone->Update(float(dt));
	socialMeter = static_cast<int>(Player->getSocialMeter());
	money = static_cast<int>(Player->getMoney());

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

	clockTime = clock->getHourMinute();
	if (!Application::isMouseButtonPressed(0) && !Application::isMouseButtonPressed(1) && !Application::isMouseButtonPressed(2)) {
		mouseToggle = true;
	}


	//phone hover animation to make things more pussy popping
	if (!isPhoneOpen) {
		if (65 <= xpos && xpos <= 75 && 45 <= ypos && ypos <= 55 && mouseToggle && Application::isMouseButtonPressed(0)) {
			mouseToggle = false;
			camera.canLookAround = false;
			camera.canMove = false;
			isPhoneOpen = true;
			phoneState = P_HOME;
		}

		if (65 <= xpos && xpos <= 75 && 45 <= ypos && ypos <= 55) { hoverAnimation[2] = true; }
		else { hoverAnimation[2] = false; }

		if (0 <= xpos && xpos <= 15 && 0 <= ypos && ypos <= 15) { hoverAnimation[0] = true; }
		else { hoverAnimation[0] = false; }
	}
	else {
		if (35 <= xpos && xpos <= 45 && 38 <= ypos && ypos <= 48) { hoverAnimation[1] = true; }
		else { hoverAnimation[1] = false; }
		if (35 <= xpos && xpos <= 45 && 28 <= ypos && ypos <= 38) { hoverAnimation[8] = true; }
		else { hoverAnimation[8] = false; }

		if (35 <= xpos && xpos <= 45 && 38 <= ypos && ypos <= 48 && mouseToggle && Application::isMouseButtonPressed(0)) {
			mouseToggle = false;
			camera.canLookAround = true;
			camera.canMove = true;
			isPhoneOpen = false;
			phoneState = P_PHONEOFF;
		}

		if (phoneState != P_HOME) {
			if (35 <= xpos && xpos <= 45 && 28 <= ypos && ypos <= 38 && mouseToggle && Application::isMouseButtonPressed(0)) {
				mouseToggle = false;
				phoneState = P_HOME;
			}
		}

		switch (phoneState)
		{
		case SceneTemplate::P_HOME:
			//hover animation
			if (50 < xpos && xpos < 60 && 30 < ypos && ypos < 40) { hoverAnimation[3] = true; }
			else { hoverAnimation[3] = false; }
			if (60 < xpos && xpos < 70 && 30 < ypos && ypos < 40) { hoverAnimation[4] = true; }
			else { hoverAnimation[4] = false; }
			if (50 < xpos && xpos < 60 && 20 < ypos && ypos < 30) { hoverAnimation[5] = true; }
			else { hoverAnimation[5] = false; }
			if (60 < xpos && xpos < 70 && 20 < ypos && ypos < 30) { hoverAnimation[6] = true; }
			else { hoverAnimation[6] = false; }
			if (50 < xpos && xpos < 60 && 10 < ypos && ypos < 20) { hoverAnimation[7] = true; }
			else { hoverAnimation[7] = false; }

			//mouse press things <3
			if (50 < xpos && xpos < 60 && 30 < ypos && ypos < 40 && mouseToggle && Application::isMouseButtonPressed(0)) {
				mouseToggle = false;
				phoneState = P_CHATLIST;
			}
			if (60 < xpos && xpos < 70 && 30 < ypos && ypos < 40 && mouseToggle && Application::isMouseButtonPressed(0)) {
				mouseToggle = false;
				pageCounter = 0;
				phoneState = P_TASKS;
			}
			if (50 < xpos && xpos < 60 && 20 < ypos && ypos < 30 && mouseToggle && Application::isMouseButtonPressed(0)) {
				mouseToggle = false;
				phoneState = P_MONEY;
			}
			if (60 < xpos && xpos < 70 && 20 < ypos && ypos < 30 && mouseToggle && Application::isMouseButtonPressed(0)) {
				mouseToggle = false;
				phoneState = P_SOCIALSCORE;
			}
			break;
		case SceneTemplate::P_CHAT:
			break;
		case SceneTemplate::P_CHATLIST:
			break;
		case SceneTemplate::P_MONEY:
			break;
		case SceneTemplate::P_SOCIALSCORE:
			break;
		case SceneTemplate::P_HELP:
			break;
		case SceneTemplate::P_TASKS:
			if (56 < xpos && xpos < 60 && 31 < ypos && ypos < 35 && mouseToggle && Application::isMouseButtonPressed(0)) {
				mouseToggle = false;
				if (pageCounter > 0) {
					pageCounter--;
				}
			}
			if (56 < xpos && xpos < 60 && 11 < ypos && ypos < 15 && mouseToggle && Application::isMouseButtonPressed(0)) {
				mouseToggle = false;
				if (unsigned(pageCounter) < phone->taskList.size() - 5) {
					pageCounter++;
				}
			}
			break;
		}
	}
	
}


void SceneTemplate::Render()
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

	//regular game UI
	if (!isPhoneOpen) {
		if (hoverAnimation[2]) { RenderImageOnScreen(meshList[GEO_PHONE_ICON], Color(1, 1, 1), 9, 9, 70, 50, 0, 20); }
		else { RenderImageOnScreen(meshList[GEO_PHONE_ICON], Color(1, 1, 1), 8, 8, 70, 50, 0, 0); }

		if (hoverAnimation[0]) { RenderImageOnScreen(meshList[GEO_WATCH], Color(1, 1, 1), 30, 40, 10, 10, 0, 180); }
		else { RenderImageOnScreen(meshList[GEO_WATCH], Color(1, 1, 1), 30, 40, 8, 6, 0, 215); }

		//render last because it needs to be top
		if (hoverAnimation[0]) { RenderTextOnScreen(meshList[GEO_TEXT], clockTime, Color(1, 1, 1), 5, 4, 7); }
		else { RenderTextOnScreen(meshList[GEO_TEXT], clockTime, Color(1, 1, 1), 5, 5, 0, 0, 35); }

	}
	else {
		RenderPhoneUI();
	}

	//render cursor last because it has to show up on top
	{
		modelStack.PushMatrix();
		RenderImageOnScreen(meshList[GEO_TEMP_QUAD], Color(1, 1, 1), 5, 5, static_cast<float>(xpos), static_cast<float>(ypos - 3), 0, 180);
		modelStack.PopMatrix();
	}
}

void SceneTemplate::Reset()
{
}

void SceneTemplate::Exit()
{
	// Cleanup VBO here
	delete meshList[GEO_AXES];
	glDeleteVertexArrays(1, &m_vertexArrayID);

	glDeleteProgram(m_programID);
}

void SceneTemplate::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneTemplate::RenderSkybox()
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

void SceneTemplate::RenderPhoneUI()
{
	RenderImageOnScreen(meshList[GEO_PHONE_UI], Color(1, 1, 1), 30, 45, 60, 28);

	if (hoverAnimation[1]) { RenderImageOnScreen(meshList[GEO_PHONE_ICON], Color(1, 1, 1), 9, 9, 40, 43, 0, 20); }
	else { RenderImageOnScreen(meshList[GEO_PHONE_ICON], Color(1, 1, 1), 8, 8, 40, 43); }

	if (phoneState != P_HOME) {
		if (hoverAnimation[8]) { RenderImageOnScreen(meshList[GEO_HOME_ICON], Color(1, 1, 1), 9, 9, 40, 33, 0, 20); }
		else { RenderImageOnScreen(meshList[GEO_HOME_ICON], Color(1, 1, 1), 8, 8, 40, 33); }
	}

	switch (phoneState)
	{
	case SceneTemplate::P_HOME:
		if (hoverAnimation[3]) { RenderImageOnScreen(meshList[GEO_CHAT_ICON], Color(1, 1, 1), 9, 9, 55, 35, 0, 20); }
		else { RenderImageOnScreen(meshList[GEO_CHAT_ICON], Color(1, 1, 1), 8, 8, 55, 35, 0, 0); }
		if (hoverAnimation[4]) { RenderImageOnScreen(meshList[GEO_TASKS_ICON], Color(1, 1, 1), 9, 9, 65, 35, 0, 20); }
		else { RenderImageOnScreen(meshList[GEO_TASKS_ICON], Color(1, 1, 1), 8, 8, 65, 35, 0, 0); }
		if (hoverAnimation[5]) { RenderImageOnScreen(meshList[GEO_MONEY_ICON], Color(1, 1, 1), 9, 9, 55, 25, 0, 20); }
		else { RenderImageOnScreen(meshList[GEO_MONEY_ICON], Color(1, 1, 1), 8, 8, 55, 25, 0, 0); }
		if (hoverAnimation[6]) { RenderImageOnScreen(meshList[GEO_SOCIALSCORE_ICON], Color(1, 1, 1), 9, 9, 65, 25, 0, 20); }
		else { RenderImageOnScreen(meshList[GEO_SOCIALSCORE_ICON], Color(1, 1, 1), 8, 8, 65, 25, 0, 0); }
		if (hoverAnimation[7]) { RenderImageOnScreen(meshList[GEO_HELP_ICON], Color(1, 1, 1), 9, 9, 55, 15, 0, 20); }
		else { RenderImageOnScreen(meshList[GEO_HELP_ICON], Color(1, 1, 1), 8, 8, 55, 15, 0, 0); }

		RenderTextOnScreen(meshList[GEO_TEXT], "my phone <3", Color(0.6f, 0.3f, 0.3f), 3, 52, 41);
		break;

	case SceneTemplate::P_CHATLIST:
		break;

	case SceneTemplate::P_MONEY:
		RenderTextOnScreen(meshList[GEO_TEXT], "TY-RA Bank", Color(0.6f, 0.3f, 0.3f), 4, 50, 40);
		RenderTextOnScreen(meshList[GEO_TEXT], "Account Balance:", Color(0.6f, 0.3f, 0.3f), 3, 49, 37);
		if (money > 5000) { RenderTextOnScreen(meshList[GEO_TEXT], "$ " + std::to_string(money), Color(0.3f, 0.7f, 0.3f), 5, 52, 30); }
		else if (money < 0) { RenderTextOnScreen(meshList[GEO_TEXT], "-$ " + std::to_string(abs(money)), Color(0.7f, 0.3f, 0.3f), 5, 52, 30); }
		else { RenderTextOnScreen(meshList[GEO_TEXT], "$ " + std::to_string(money), Color(0.7f, 0.7f, 0.4f), 5, 52, 30); }
		
		RenderTextOnScreen(meshList[GEO_TEXT], "Bank Rating:", Color(0.6f, 0.3f, 0.3f), 4, 49, 21);
		if (money > 20000) { 
			RenderTextOnScreen(meshList[GEO_TEXT], "Splendid!", Color(0.7f, 0.7f, 0.7f), 3, 52, 17);
			RenderTextOnScreen(meshList[GEO_TEXT], "Here is a", Color(0.3f, 0.3f, 0.3f), 3, 52, 14);
			RenderTextOnScreen(meshList[GEO_TEXT], "medal :)", Color(0.3f, 0.3f, 0.3f), 3, 52, 11);
		}
		else if (money > 9000) { 
			RenderTextOnScreen(meshList[GEO_TEXT], "Model citizen!", Color(0.3f, 0.7f, 0.3f), 3, 52, 17);
			RenderTextOnScreen(meshList[GEO_TEXT], "Keep it up!", Color(0.3f, 0.3f, 0.3f), 3, 52, 14);
		}
		else if (money > 5000) {
			RenderTextOnScreen(meshList[GEO_TEXT], "Meh.", Color(0.3f, 0.3f, 0.3f), 3, 52, 17);
			RenderTextOnScreen(meshList[GEO_TEXT], "you're doing...", Color(0.3f, 0.3f, 0.3f), 3, 52, 14);
			RenderTextOnScreen(meshList[GEO_TEXT], "fine, I guess.", Color(0.3f, 0.3f, 0.3f), 3, 52, 11);
		}
		else if (money < -1000) {
			RenderTextOnScreen(meshList[GEO_TEXT], "In debt", Color(0.7f, 0.3f, 0.3f), 3, 52, 17);
			RenderTextOnScreen(meshList[GEO_TEXT], "please be more", Color(0.3f, 0.3f, 0.3f), 3, 52, 14);
			RenderTextOnScreen(meshList[GEO_TEXT], "careful.", Color(0.3f, 0.3f, 0.3f), 3, 52, 11);
		}
		else { 
			RenderTextOnScreen(meshList[GEO_TEXT], "Mediocre.", Color(0.7f, 0.7f, 0.3f), 3, 52, 17);
			RenderTextOnScreen(meshList[GEO_TEXT], "Do better.", Color(0.3f, 0.3f, 0.3f), 3, 52, 14);
		}
		break;

	case SceneTemplate::P_SOCIALSCORE:
		RenderImageOnScreen(meshList[GEO_SOCIALSCORE_BAR], Color(1, 1, 1), (16 * (static_cast<float>(socialMeter)/100)), 10, (52 + (8 * (static_cast<float>(socialMeter) / 100))), 35);
		RenderImageOnScreen(meshList[GEO_SOCIALSCORE_UI], Color(1, 1, 1), 20, 20, 60, 30);

		RenderTextOnScreen(meshList[GEO_TEXT], "SOCIAL METER", Color(0.6f, 0.3f, 0.3f), 3, 50, 18);
		break;

	case SceneTemplate::P_HELP:
		break;

	case SceneTemplate::P_TASKS:
		RenderTextOnScreen(meshList[GEO_TEXT], "TO DO: ", Color(0.6f, 0.3f, 0.3f), 4, 50, 40);
		RenderTextOnScreen(meshList[GEO_TEXT], "^", Color(0.6f, 0.3f, 0.3f), 4, 58, 33);
		RenderTextOnScreen(meshList[GEO_TEXT], "v", Color(0.6f, 0.3f, 0.3f), 4, 58, 13);
		if (static_cast<int>(phone->taskList.size()) - 1 - pageCounter > 0) { 
			if (!phone->taskList[static_cast<int>(phone->taskList.size()) - 1 - pageCounter].isCompleted) { RenderTextOnScreen(meshList[GEO_TEXT], "-", Color(0.6f, 0.3f, 0.3f), 3, 48, 30); RenderTextOnScreen(meshList[GEO_TEXT], phone->taskList[static_cast<int>(phone->taskList.size()) - 1 - pageCounter].taskName, Color(0.6f, 0.3f, 0.3f), 3, 50, 30); }
			else {
				RenderTextOnScreen(meshList[GEO_TEXT], "-", Color(0.5f, 0.5f, 0.5f), 3, 48, 30);
				RenderTextOnScreen(meshList[GEO_TEXT], phone->taskList[phone->taskList.size() - 1 - int(pageCounter)].taskName, Color(0.5f, 0.5f, 0.5f), 3, 50, 30);
			}
		}
		if (static_cast<int>(phone->taskList.size()) - 2 - pageCounter > 0) {
			if (!phone->taskList[static_cast<int>(phone->taskList.size()) - 2 - pageCounter].isCompleted) { RenderTextOnScreen(meshList[GEO_TEXT], "-", Color(0.6f, 0.3f, 0.3f), 3, 48, 27); RenderTextOnScreen(meshList[GEO_TEXT], phone->taskList[static_cast<int>(phone->taskList.size()) - 2 - pageCounter].taskName, Color(0.6f, 0.3f, 0.3f), 3, 50, 27); }
			else {
				RenderTextOnScreen(meshList[GEO_TEXT], "-", Color(0.5f, 0.5f, 0.5f), 3, 48, 27);
				RenderTextOnScreen(meshList[GEO_TEXT], phone->taskList[static_cast<int>(phone->taskList.size()) - 2 - pageCounter].taskName, Color(0.5f, 0.5f, 0.5f), 3, 50, 27);
			}
		}
		if (static_cast<int>(phone->taskList.size()) - 3 - pageCounter > 0) {
			if (!phone->taskList[static_cast<int>(phone->taskList.size()) - 3 - pageCounter].isCompleted) { RenderTextOnScreen(meshList[GEO_TEXT], "-", Color(0.6f, 0.3f, 0.3f), 3, 48, 24); RenderTextOnScreen(meshList[GEO_TEXT], phone->taskList[static_cast<int>(phone->taskList.size()) - 3 - pageCounter].taskName, Color(0.6f, 0.3f, 0.3f), 3, 50, 24); }
			else {
				RenderTextOnScreen(meshList[GEO_TEXT], "-", Color(0.5f, 0.5f, 0.5f), 3, 48, 24);
				RenderTextOnScreen(meshList[GEO_TEXT], phone->taskList[static_cast<int>(phone->taskList.size()) - 3 - pageCounter].taskName, Color(0.5f, 0.5f, 0.5f), 3, 50, 24);
			}
		}
		if (static_cast<int>(phone->taskList.size()) - 4 - pageCounter > 0) {
			if (!phone->taskList[static_cast<int>(phone->taskList.size()) - 4 - pageCounter].isCompleted) { RenderTextOnScreen(meshList[GEO_TEXT], "-", Color(0.6f, 0.3f, 0.3f), 3, 48, 21); RenderTextOnScreen(meshList[GEO_TEXT], phone->taskList[static_cast<int>(phone->taskList.size()) - 4 - pageCounter].taskName, Color(0.6f, 0.3f, 0.3f), 3, 50, 21); }
			else {
				RenderTextOnScreen(meshList[GEO_TEXT], "-", Color(0.5f, 0.5f, 0.5f), 3, 48, 21);
				RenderTextOnScreen(meshList[GEO_TEXT], phone->taskList[static_cast<int>(phone->taskList.size()) - 4 - pageCounter].taskName, Color(0.5f, 0.5f, 0.5f), 3, 50, 21);
			}
		}
		if (static_cast<int>(phone->taskList.size()) - 5 - pageCounter > 0) {
			if (!phone->taskList[static_cast<int>(phone->taskList.size()) - 5 - pageCounter].isCompleted) { RenderTextOnScreen(meshList[GEO_TEXT], "-", Color(0.6f, 0.3f, 0.3f), 3, 48, 18); RenderTextOnScreen(meshList[GEO_TEXT], phone->taskList[static_cast<int>(phone->taskList.size()) - 5 - pageCounter].taskName, Color(0.6f, 0.3f, 0.3f), 3, 50, 18); }
			else {
				RenderTextOnScreen(meshList[GEO_TEXT], "-", Color(0.5f, 0.5f, 0.5f), 3, 48, 18);
				RenderTextOnScreen(meshList[GEO_TEXT], phone->taskList[static_cast<int>(phone->taskList.size()) - 5 - pageCounter].taskName, Color(0.5f, 0.5f, 0.5f), 3, 50, 18);
			}
		}

		break;
	}

	RenderTextOnScreen(meshList[GEO_TEXT], clockTime, Color(0.6f, 0.3f, 0.3f), 3, 56, 44);
}

void SceneTemplate::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneTemplate::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float xRotation, float zRotation)
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

std::vector<float> SceneTemplate::getNumberValues(std::string filename)
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

void SceneTemplate::RenderImageOnScreen(Mesh* mesh, Color color, float sizex, float sizey, float x, float y, float xRotation, float zRotation)
{
	if (!mesh || mesh->textureID <= 0) //Proper error check
		return;
	glDisable(GL_DEPTH_TEST); //uncomment for RenderTextOnScreen
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
}

