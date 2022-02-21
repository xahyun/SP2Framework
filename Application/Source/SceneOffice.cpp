#include "SceneOffice.h"
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


SceneOffice::SceneOffice()
{
	fontData = getNumberValues("Image//Fonts//FontData.csv");
	phoneState = P_PHONEOFF; pageCounter = 0; enableLight = true;
	computerState = C_COMOFF; goNextScene = false;
}

SceneOffice::~SceneOffice()
{
}

void SceneOffice::Init()
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

	camera.Init(Vector3(-50, 15, 0), Vector3(0, 10, -10), Vector3(0, 1, 0));
	camera.canMove = false;

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

	//office 
	{
		//note for myself: other woker use "desk"
		//for player use "desk corner"
		meshList[GEO_OFFICEFLOOR] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 0), 1.f, 1.f, 5);
		meshList[GEO_OFFICEFLOOR]->textureID = LoadTGA("Image//Office//Patterncarpet.tga");
		meshList[GEO_OFFICEFLOOR]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_OFFICEFLOOR]->material.kDiffuse.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_OFFICEFLOOR]->material.kSpecular.Set(0.3f, 0.3f, 0.3f);
		meshList[GEO_OFFICEFLOOR]->material.kShininess = 1.f;

		meshList[GEO_OFFICETABLE] = MeshBuilder::GenerateOBJMTL("Officetable", "OBJ//furniture//deskCorner.obj", "OBJ//furniture//deskCorner.mtl");
		meshList[GEO_COMPUTER] = MeshBuilder::GenerateOBJMTL("Computer", "OBJ//furniture//computerScreen.obj", "OBJ//furniture//computerScreen.mtl");
		meshList[GEO_KEYBOARD] = MeshBuilder::GenerateOBJMTL("Keyboard", "OBJ//furniture//computerKeyboard.obj", "OBJ//furniture//computerKeyboard.mtl");
		meshList[GEO_OFFICECHAIR] = MeshBuilder::GenerateOBJMTL("OfficeChair", "OBJ//furniture//chairDesk.obj", "OBJ//furniture//chairDesk.mtl");
		meshList[GEO_PLANT1] = MeshBuilder::GenerateOBJMTL("Plant", "OBJ//furniture//pottedPlant.obj", "OBJ//furniture//pottedPlant.mtl");
		meshList[GEO_PLANT2] = MeshBuilder::GenerateOBJMTL("PlantSmall", "OBJ//furniture//plantSmall3.obj", "OBJ//furniture//plantSmall3.mtl");
		meshList[GEO_OFFICEDOOR] = MeshBuilder::GenerateOBJMTL("OfficeDoor", "OBJ//furniture//doorwayFront.obj", "OBJ//furniture//doorwayFront.mtl");
		meshList[GEO_BOOKS] = MeshBuilder::GenerateOBJMTL("Books", "OBJ//furniture//books.obj", "OBJ//furniture//books.mtl");


		meshList[GEO_WALL] = MeshBuilder::GenerateOBJMTL("Wall", "OBJ//furniture//wall.obj", "OBJ//furniture//wall.mtl");
	}

	//computer UI
	{
		meshList[GEO_DEFAULT_WALLPAPER] = MeshBuilder::GenerateQuad("wallpaper", Color(1, 1, 1), 1, 1);
		meshList[GEO_DEFAULT_WALLPAPER]->textureID = LoadTGA("Image//computerUI//white.tga");
		meshList[GEO_EMAIL_ICON] = MeshBuilder::GenerateQuad("email", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_EMAIL_ICON]->textureID = LoadTGA("Image//computerUI//email_icon.tga");
		meshList[GEO_POWER_BUTTON] = MeshBuilder::GenerateQuad("tasks", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_POWER_BUTTON]->textureID = LoadTGA("Image//computerUI//powerButton.tga");
		meshList[GEO_WORK_ICON] = MeshBuilder::GenerateQuad("tasks", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_WORK_ICON]->textureID = LoadTGA("Image//computerUI//work_icon.tga");
		meshList[GEO_SHOPPING_ICON] = MeshBuilder::GenerateQuad("tasks", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_SHOPPING_ICON]->textureID = LoadTGA("Image//computerUI//shopping_icon.tga");
		meshList[GEO_USER_ICON] = MeshBuilder::GenerateQuad("user", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_USER_ICON]->textureID = LoadTGA("Image//computerUI//user_icon.tga");
		meshList[GEO_BUTTON_PRESSED] = MeshBuilder::GenerateQuad("buttonpressed", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_BUTTON_PRESSED]->textureID = LoadTGA("Image//computerUI//buttonPressed.tga");
		meshList[GEO_BUTTON_NOT_PRESSED] = MeshBuilder::GenerateQuad("buttonnotpressed", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_BUTTON_NOT_PRESSED]->textureID = LoadTGA("Image//computerUI//button.tga");

		meshList[GEO_SHOP_LOGO] = MeshBuilder::GenerateQuad("shop_logo", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_SHOP_LOGO]->textureID = LoadTGA("Image//computerUI//shopping//shop_logo.tga");
		meshList[GEO_SHOP_MENU] = MeshBuilder::GenerateQuad("shop_logo", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_SHOP_MENU]->textureID = LoadTGA("Image//computerUI//shopping//shop_menu.tga");
		meshList[GEO_ITEM_CABLE] = MeshBuilder::GenerateQuad("shop_logo", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_ITEM_CABLE]->textureID = LoadTGA("Image//computerUI//shopping//shopping_cable.tga");
		meshList[GEO_ITEM_PHONE] = MeshBuilder::GenerateQuad("shop_logo", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_ITEM_PHONE]->textureID = LoadTGA("Image//computerUI//shopping//shopping_phone.tga");
		meshList[GEO_ITEM_WATCH] = MeshBuilder::GenerateQuad("shop_logo", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_ITEM_WATCH]->textureID = LoadTGA("Image//computerUI//shopping//shopping_watch.tga");
		meshList[GEO_ITEM_NOIMAGE] = MeshBuilder::GenerateQuad("shop_logo", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_ITEM_NOIMAGE]->textureID = LoadTGA("Image//computerUI//shopping//shopping_noimage.tga");
		meshList[GEO_STAR] = MeshBuilder::GenerateQuad("star", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_STAR]->textureID = LoadTGA("Image//computerUI//shopping//star.tga");
		meshList[GEO_LIMITED_EDITION] = MeshBuilder::GenerateQuad("star", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_LIMITED_EDITION]->textureID = LoadTGA("Image//computerUI//shopping//saleIcon.tga");

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
		meshList[GEO_NOTIF_BOX] = MeshBuilder::GenerateQuad("notifBox", Color(1, 1, 1), 1, 1, 1);
		meshList[GEO_NOTIF_BOX]->textureID = LoadTGA("Image//phoneUI//notif_box.tga");
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

	initialiseEmails("otherItems//scamEmails.txt");
	initialiseShop("otherItems//shopItems.txt");
	numUnreadEmails = emails.size();
}

void SceneOffice::TransferGameInfo(Game* game)
{
	this->clock = game->gameClock;
	this->Player = game->gamePlayer;
	this->phone = game->playerPhone;
}


void SceneOffice::Update(double dt)
{
	camera.Update(static_cast<float>(dt));
	Application::getCursorPosition(xpos, ypos);
	Application::worldSpaceToScreenSpace(xpos, ypos);
	Application::hideCursorWhenInScreen();

	if (NotificationTimer > 0) {
		//TODO: add animation
		NotificationTimer -= static_cast<float>(dt);
	}
	else {
		notif = N_OFF;
	}
	
	if (hasReceivedTasks == false && NotificationTimer <= 0 && !hasClockedIn) {
		phone->markTaskAsCompleted("Go To Work");
		phone->addTaskToList("Clock In");
		NotificationTimer = 3;
		notif = N_TASK;
		hasReceivedTasks = true;
	}

	if (hasReceivedTasks == false && hasClockedIn && NotificationTimer <= 0) {
		phone->addTaskToList("Read Emails");
		phone->addTaskToList("Purchase HDMI cable");
		NotificationTimer = 3;
		notif = N_TASK;
		hasReceivedTasks = true;
	}

	std::cout << xpos << " " << ypos << "\n";

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


	if (isUsingComputer) {
		if (8 < xpos && xpos < 10 && 10 < ypos && ypos < 12) { comHoverAnimation[0] = true; }
		else { comHoverAnimation[0] = false; }

		if (7 < xpos && xpos < 11 && 10 < ypos && ypos < 12 && mouseToggle && Application::isMouseButtonPressed(0)) {
			mouseToggle = false;
			isUsingComputer = false;
			camera.canLookAround = true;
			computerState = C_COMOFF;
			//TODO: add animation;
			camera.position = camera.defaultPosition;
			camera.target = camera.defaultTarget;
			camera.lookUp(static_cast<float>(dt));
		}

		if (computerState != C_HOME && computerState != C_CLOCKIN) {
			if (68 < xpos && xpos < 72 && 10 < ypos && ypos < 12) { comHoverAnimation[5] = true; }
			else { comHoverAnimation[5] = false; }
			if (68 < xpos && xpos < 72 && 10 < ypos && ypos < 12 && mouseToggle && Application::isMouseButtonPressed(0)) {
				computerState = C_HOME;
			}
		}

		switch (computerState)
		{
		case SceneOffice::C_LOADING:
			break;
		case SceneOffice::C_HOME:
			//hover animations
			if (14 < xpos && xpos < 22 && 26 < ypos && ypos < 33) { comHoverAnimation[1] = true; }
			else { comHoverAnimation[1] = false; }
			if (30 < xpos && xpos < 38 && 26 < ypos && ypos < 33) { comHoverAnimation[2] = true; }
			else { comHoverAnimation[2] = false; }
			if (43 < xpos && xpos < 52 && 26 < ypos && ypos < 33) { comHoverAnimation[3] = true; }
			else { comHoverAnimation[3] = false; }
			if (60 < xpos && xpos < 68 && 26 < ypos && ypos < 33) { comHoverAnimation[4] = true; }
			else { comHoverAnimation[4] = false; }

			//click
			if (14 < xpos && xpos < 22 && 26 < ypos && ypos < 33 && mouseToggle && Application::isMouseButtonPressed(0)) { mouseToggle = false; computerState = C_EMAIL_EXPLAINING; workTimer = 5; }
			if (30 < xpos && xpos < 38 && 26 < ypos && ypos < 33 && mouseToggle && Application::isMouseButtonPressed(0)) { mouseToggle = false; computerState = C_WORK; }
			if (43 < xpos && xpos < 52 && 26 < ypos && ypos < 33 && mouseToggle && Application::isMouseButtonPressed(0)) { mouseToggle = false; computerState = C_SHOPPINGPAGE_MENU; }
			break;
		case SceneOffice::C_EMAIL:
			if (numUnreadEmails > 0) {
				if (14 < xpos && xpos < 33 && 14 < ypos && ypos < 21 && mouseToggle && Application::isMouseButtonPressed(0)) {
					mouseToggle = false;
					//if not scam email and press report
					if (!isScamEmails[numUnreadEmails - 1]) { Player->addSocialMeter(-10); }
					numUnreadEmails--;
				}
				if (45 < xpos && xpos < 64 && 14 < ypos && ypos < 21 && mouseToggle && Application::isMouseButtonPressed(0)) {
					mouseToggle = false;
					//if scam email and press report
					if (isScamEmails[numUnreadEmails - 1]) { Player->addMoney(-2000); Player->addSocialMeter(-10); }
					else {
						Player->addSocialMeter(+10);
					}
					numUnreadEmails--;
				}
				if (numUnreadEmails == 0) {
					phone->markTaskAsCompleted("Read Emails");
					NotificationTimer = 3;
					notif = N_TASK_DONE;
				}
			}
			break;
		case SceneOffice::C_EMAIL_EXPLAINING:
			if (workTimer > 0) {
				workTimer -= static_cast<float>(dt);
			}
			else {
				workTimer = 0;
				computerState = C_EMAIL;
			}
			break;
		case SceneOffice::C_SHOPPINGPAGE_MENU:
			//stuff no hover animation though
			if (17 < xpos && xpos < 30 && 14 < ypos && ypos < 25 && mouseToggle && Application::isMouseButtonPressed(0)) { mouseToggle = false; computerState = C_SHOPPINGPAGE; pageCounter = 0; }
			if (34 < xpos && xpos < 47 && 14 < ypos && ypos < 25 && mouseToggle && Application::isMouseButtonPressed(0)) { mouseToggle = false; computerState = C_SHOPPINGPAGE; pageCounter = 2; }
			if (50 < xpos && xpos < 64 && 14 < ypos && ypos < 25 && mouseToggle && Application::isMouseButtonPressed(0)) { mouseToggle = false; computerState = C_SHOPPINGPAGE; pageCounter = 1; }
			break;
		case SceneOffice::C_SHOPPINGPAGE:
			if (shopList[pageCounter].first.size() >= 1) {
				if (8 < xpos && xpos < 21 && 20 < ypos && ypos < 30 && mouseToggle && Application::isMouseButtonPressed(0)) { 
					mouseToggle = false;
					if (shopList[pageCounter].first[0].isScam) {
						Player->addMoney(-2000);
					}
					else {
						Player->addMoney(-shopList[pageCounter].first[0].productPrice);
					}
					//add reaction
				}
			}
			if (shopList[pageCounter].first.size() >= 2) {
				if (24 < xpos && xpos < 37 && 20 < ypos && ypos < 30 && mouseToggle && Application::isMouseButtonPressed(0)) {
					mouseToggle = false;
					if (shopList[pageCounter].first[1].isScam) {
						Player->addMoney(-2000);
					}
					else {
						Player->addMoney(-shopList[pageCounter].first[1].productPrice);
					}
				}
			}
			if (shopList[pageCounter].first.size() >= 3) {
				if (41 < xpos && xpos < 55 && 20 < ypos && ypos < 30 && mouseToggle && Application::isMouseButtonPressed(0)) {
					mouseToggle = false;
					if (shopList[pageCounter].first[2].isScam) {
						Player->addMoney(-2000);
					}
					else {
						Player->addMoney(-shopList[pageCounter].first[2].productPrice);
					}
				}
			}
			if (shopList[pageCounter].first.size() >= 4) {
				if (58 < xpos && xpos < 72 && 20 < ypos && ypos < 30 && mouseToggle && Application::isMouseButtonPressed(0)) {
					mouseToggle = false;
					if (shopList[pageCounter].first[3].isScam) {
						Player->addMoney(-2000);
					}
					else {
						Player->addMoney(-shopList[pageCounter].first[3].productPrice);
					}
				}
			}
			break;
		case SceneOffice::C_TASKS:
			break;
		case SceneOffice::C_CLOCKIN:
			if (!hasClockedIn) {
				//press and check if late
				if (32 < xpos && xpos < 50 && 19 < ypos && ypos < 25 && mouseToggle && Application::isMouseButtonPressed(0)) {
					std::size_t pos = clockTime.find(":");
					if (std::stoi(clockTime.substr(0, pos)) >= 10 && std::stoi(clockTime.substr(pos + 1)) > 0) {
						Player->addSocialMeter(-10);
					}
					else {
						Player->addSocialMeter(10);
					}
					hasClockedIn = true;
					phone->markTaskAsCompleted("Clock In");
					NotificationTimer = 3;
					notif = N_TASK_DONE;
				}
			}
			else {
				computerState = C_HOME;
			}
			break;
		}
	}

	if (isPhoneOpen && isUsingComputer) {
		camera.canLookAround = false;
	}
	else {
		if (0 >= xpos && xpos >= 15 && 0 >= ypos && ypos >= 15) {
			camera.canLookAround = true;
		}
	}

	//phone hover animation to make things more pussy popping
	if (!isPhoneOpen) {
		if (65 <= xpos && xpos <= 75 && 45 <= ypos && ypos <= 55 && mouseToggle && Application::isMouseButtonPressed(0)) {
			mouseToggle = false;
			isPhoneOpen = true;
			phoneState = P_HOME;
		}

		if (65 <= xpos && xpos <= 75 && 45 <= ypos && ypos <= 55) { hoverAnimation[2] = true; }
		else { hoverAnimation[2] = false; }

		if (0 <= xpos && xpos <= 15 && 0 <= ypos && ypos <= 15) { hoverAnimation[0] = true; camera.canLookAround = false; }
		else if (!isUsingComputer) { hoverAnimation[0] = false; camera.canLookAround = true; }
	}
	else {
		if (35 <= xpos && xpos <= 45 && 38 <= ypos && ypos <= 48) { hoverAnimation[1] = true; }
		else { hoverAnimation[1] = false; }
		if (35 <= xpos && xpos <= 45 && 28 <= ypos && ypos <= 38) { hoverAnimation[8] = true; }
		else { hoverAnimation[8] = false; }

		if (35 <= xpos && xpos <= 45 && 38 <= ypos && ypos <= 48 && mouseToggle && Application::isMouseButtonPressed(0)) {
			mouseToggle = false;
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
		case SceneOffice::P_HOME:
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
		case SceneOffice::P_CHAT:
			break;
		case SceneOffice::P_HELP:
			break;
		case SceneOffice::P_TASKS:
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

	angleBetweenComputer = camera.angleDifference(Vector3(-55, camera.position.y, 14));
	if (angleBetweenComputer > 5 && angleBetweenComputer < 60 && !isPhoneOpen && !isUsingComputer) {
		//allow user to use computer
		if (Application::isMouseButtonPressed(0) && mouseToggle) {
			isUsingComputer = true;
			camera.canLookAround = false;
			computerState = C_CLOCKIN;
			//TODO: add animation;
			camera.position = Vector3(-55, camera.defaultPosition.y + 2, 2);
			camera.target = Vector3(-55.48f, 16.9f, 2.86f);
			camera.lookUp(static_cast<float>(dt));
		}
	}
	
}


void SceneOffice::Render()
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

	//render office space 
	{
		modelStack.PushMatrix();
		modelStack.Rotate(-90, 1, 0, 0);
		modelStack.Scale(200, 200, 200);
		RenderMesh(meshList[GEO_OFFICEFLOOR], false);
		modelStack.PopMatrix();



		//modelStack.PushMatrix();
		//modelStack.Scale(30, 25, 30);
		//RenderMesh(meshList[GEO_OFFICETABLE], true);
		//modelStack.PopMatrix();



		//player table
		modelStack.PushMatrix();
		modelStack.Translate(-40, 0, -10);
		modelStack.Scale(30, 25, 30);
		RenderMesh(meshList[GEO_OFFICETABLE], true);
		modelStack.PopMatrix();

		//TODO: ADD DEFAULT WALLPAPER for polish !!

		if (angleBetweenComputer > 5 && angleBetweenComputer < 60 && !isPhoneOpen && !isUsingComputer) {
			modelStack.PushMatrix();
			modelStack.Translate(-55, 10, 14);
			modelStack.Rotate(-30, 0, 1, 0);
			modelStack.Scale(32, 32, 32);
			RenderMesh(meshList[GEO_COMPUTER], enableLight);
			modelStack.PopMatrix();
		}
		else {
			modelStack.PushMatrix();
			modelStack.Translate(-55, 10, 14);
			modelStack.Rotate(-30, 0, 1, 0);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_COMPUTER], enableLight);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(-45, 10, 8);
		/*modelStack.Rotate(-30, 0, 1, 0);*/
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_KEYBOARD], enableLight);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-30, 0, 20);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_PLANT1], enableLight);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-60, 10, -5);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_BOOKS], enableLight);
		modelStack.PopMatrix();

		modelStack.PushMatrix();
		modelStack.Translate(-65, 10, 5);
		modelStack.Rotate(-90, 0, 1, 0);
		modelStack.Scale(30, 30, 30);
		RenderMesh(meshList[GEO_PLANT2], enableLight);
		modelStack.PopMatrix();



		//background stuff
		{
			modelStack.PushMatrix();
			modelStack.Translate(70, 0, 50);
			modelStack.Scale(30, 25, 30);
			RenderMesh(meshList[GEO_OFFICETABLE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(70, 0, -10);
			modelStack.Scale(30, 25, 30);
			RenderMesh(meshList[GEO_OFFICETABLE], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(70, 0, -70);
			modelStack.Scale(30, 25, 30);
			RenderMesh(meshList[GEO_OFFICETABLE], true);
			modelStack.PopMatrix();

			//middle table com
			modelStack.PushMatrix();
			modelStack.Translate(55, 10, 14);
			modelStack.Rotate(-30, 0, 1, 0);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_COMPUTER], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(65, 10, 8);
			/*modelStack.Rotate(-30, 0, 1, 0);*/
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_KEYBOARD], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(55, 0, -5);
			modelStack.Rotate(105, 0, 1, 0);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_OFFICECHAIR], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(40, 0, 20);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_PLANT1], true);
			modelStack.PopMatrix();

			//back table com
			modelStack.PushMatrix();
			modelStack.Translate(55, 10, -48);
			modelStack.Rotate(-30, 0, 1, 0);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_COMPUTER], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(65, 10, -53);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_KEYBOARD], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(55, 0, -65);
			modelStack.Rotate(105, 0, 1, 0);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_OFFICECHAIR], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(40, 0, -40);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_PLANT1], true);
			modelStack.PopMatrix();


			//front table com
			modelStack.PushMatrix();
			modelStack.Translate(54, 10, 73);
			modelStack.Rotate(-30, 0, 1, 0);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_COMPUTER], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(65, 10, 70);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_KEYBOARD], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(55, 0, 55);
			modelStack.Rotate(105, 0, 1, 0);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_OFFICECHAIR], true);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(40, 0, 80);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_PLANT1], true);
			modelStack.PopMatrix();



			//door
			modelStack.PushMatrix();
			modelStack.Translate(10, 0, 95);
			modelStack.Scale(30, 30, 30);
			RenderMesh(meshList[GEO_OFFICEDOOR], true);
			modelStack.PopMatrix();

		}



		//modelStack.PushMatrix();
		//modelStack.Translate(-40, 0, 70);
		//modelStack.Scale(30, 25, 30);
		//RenderMesh(meshList[GEO_OFFICETABLE2], true);
		//modelStack.PopMatrix();

		//cubical
		{
			modelStack.PushMatrix();
			modelStack.Translate(-25, 0, 30);
			modelStack.Scale(60, 20, 60);
			RenderMesh(meshList[GEO_WALL], enableLight);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-25, 0, -30);
			modelStack.Scale(60, 20, 60);
			RenderMesh(meshList[GEO_WALL], enableLight);
			modelStack.PopMatrix();


			modelStack.PushMatrix();
			modelStack.Translate(85, 0, 30);
			modelStack.Scale(60, 20, 60);
			RenderMesh(meshList[GEO_WALL], enableLight);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(85, 0, -30);
			modelStack.Scale(60, 20, 60);
			RenderMesh(meshList[GEO_WALL], enableLight);
			modelStack.PopMatrix();
		}


		{

			modelStack.PushMatrix();
			modelStack.Translate(100, 0, 100);
			modelStack.Scale(300, 50, 300);
			RenderMesh(meshList[GEO_WALL], enableLight);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(100, 0, -100);
			/*modelStack.Rotate(90, 1, 0, 0);*/
			modelStack.Scale(300, 50, 300);
			RenderMesh(meshList[GEO_WALL], enableLight);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(85, 0, -100);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(300, 50, 300);
			RenderMesh(meshList[GEO_WALL], enableLight);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(-100, 0, -100);
			modelStack.Rotate(90, 0, 1, 0);
			modelStack.Scale(300, 50, 300);
			RenderMesh(meshList[GEO_WALL], enableLight);
			modelStack.PopMatrix();

			modelStack.PushMatrix();
			modelStack.Translate(300, 170, -300);
			modelStack.Rotate(90, 1, 0, 0);
			modelStack.Scale(1500, 1500, 1500);
			RenderMesh(meshList[GEO_WALL], enableLight);
			modelStack.PopMatrix();
		}
	}


	//regular phone UI
	{
		if (!isPhoneOpen) {
			if (!isUsingComputer) {
				if (hoverAnimation[2]) { RenderImageOnScreen(meshList[GEO_PHONE_ICON], Color(1, 1, 1), 9, 9, 70, 50, 0, 20); }
				else { RenderImageOnScreen(meshList[GEO_PHONE_ICON], Color(1, 1, 1), 8, 8, 70, 50, 0, 0); }

				if (hoverAnimation[0]) { RenderImageOnScreen(meshList[GEO_WATCH], Color(1, 1, 1), 30, 40, 10, 10, 0, 180); }
				else { RenderImageOnScreen(meshList[GEO_WATCH], Color(1, 1, 1), 30, 40, 8, 6, 0, 215); }

				//render last because it needs to be top
				if (hoverAnimation[0]) { RenderTextOnScreen(meshList[GEO_TEXT], clockTime, Color(1, 1, 1), 5, 4, 7); }
				else { RenderTextOnScreen(meshList[GEO_TEXT], clockTime, Color(1, 1, 1), 5, 5, 0, 0, 35); }
			}
		}
		else {
			RenderPhoneUI();
		}
	}

	//renderComputerUI 
	{
		if (isUsingComputer) {
			RenderComputerUI();
		}
	}

	//notification
	if (notif != N_OFF) {
		RenderNotifications();
	}

	//render cursor last because it has to show up on top
	{
		modelStack.PushMatrix();
		RenderImageOnScreen(meshList[GEO_TEMP_QUAD], Color(1, 1, 1), 5, 5, static_cast<float>(xpos), static_cast<float>(ypos - 3), 0, 180);
		modelStack.PopMatrix();
	}

}

void SceneOffice::Exit()
{
	// Cleanup VBO here
	delete meshList[GEO_AXES];
	glDeleteVertexArrays(1, &m_vertexArrayID);

	glDeleteProgram(m_programID);
}

void SceneOffice::RenderMesh(Mesh* mesh, bool enableLight)
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

void SceneOffice::RenderSkybox()
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

void SceneOffice::RenderPhoneUI()
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
	case SceneOffice::P_HOME:
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

	case SceneOffice::P_CHATLIST:
		break;

	case SceneOffice::P_MONEY:
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

	case SceneOffice::P_SOCIALSCORE:
		RenderImageOnScreen(meshList[GEO_SOCIALSCORE_BAR], Color(1, 1, 1), (16 * (static_cast<float>(socialMeter)/100)), 10, (52 + (8 * (static_cast<float>(socialMeter) / 100))), 35);
		RenderImageOnScreen(meshList[GEO_SOCIALSCORE_UI], Color(1, 1, 1), 20, 20, 60, 30);

		RenderTextOnScreen(meshList[GEO_TEXT], "SOCIAL METER", Color(0.6f, 0.3f, 0.3f), 3, 50, 18);
		break;

	case SceneOffice::P_HELP:
		break;

	case SceneOffice::P_TASKS:
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

void SceneOffice::RenderComputerUI()
{
	//default background
	RenderImageOnScreen(meshList[GEO_DEFAULT_WALLPAPER], Color(1, 1, 1), 68, 42, 41, 28.5f);
	if (comHoverAnimation[0]) {
		RenderImageOnScreen(meshList[GEO_POWER_BUTTON], Color(1, 1, 1), 4, 4, 10, 12);
	}
	else {
		RenderImageOnScreen(meshList[GEO_POWER_BUTTON], Color(1, 1, 1), 3, 3, 10, 12);
	}
	if (computerState != C_HOME && computerState != C_CLOCKIN) {
		if (comHoverAnimation[5]) {
			RenderImageOnScreen(meshList[GEO_HOME_ICON], Color(1, 1, 1), 6, 6, 70, 12, 0, 20);
		}
		else {
			RenderImageOnScreen(meshList[GEO_HOME_ICON], Color(1, 1, 1), 5, 5, 70, 12);
		}
	}

	Mesh* tempMesh = meshList[GEO_ITEM_NOIMAGE];
	switch (computerState)
	{
	case SceneOffice::C_LOADING:
		break;
	case SceneOffice::C_HOME:
		if (comHoverAnimation[1]) {
			RenderImageOnScreen(meshList[GEO_EMAIL_ICON], Color(1, 1, 1), 9, 9, 18, 30, 0, 20);
		}
		else {
			RenderImageOnScreen(meshList[GEO_EMAIL_ICON], Color(1, 1, 1), 8, 8, 18, 30);
		}
		RenderTextOnScreen(meshList[GEO_TEXT], "Email", Color(0.6f, 0.3f, 0.3f), 2, 16, 23);

		if (comHoverAnimation[2]) {
			RenderImageOnScreen(meshList[GEO_WORK_ICON], Color(1, 1, 1), 9, 9, 33, 30, 0, 20);
		}
		else {
			RenderImageOnScreen(meshList[GEO_WORK_ICON], Color(1, 1, 1), 8, 8, 33, 30);
		}
		RenderTextOnScreen(meshList[GEO_TEXT], "Work", Color(0.6f, 0.3f, 0.3f), 2, 31, 23);

		if (comHoverAnimation[3]) {
			RenderImageOnScreen(meshList[GEO_SHOPPING_ICON], Color(1, 1, 1), 9, 9, 48, 30, 0, 20);
		}
		else {
			RenderImageOnScreen(meshList[GEO_SHOPPING_ICON], Color(1, 1, 1), 8, 8, 48, 30);
		}
		RenderTextOnScreen(meshList[GEO_TEXT], "Shop", Color(0.6f, 0.3f, 0.3f), 2, 46, 23);

		if (comHoverAnimation[4]) {
			RenderImageOnScreen(meshList[GEO_TASKS_ICON], Color(1, 1, 1), 9, 9, 63, 30, 0, 20);
		}
		else {
			RenderImageOnScreen(meshList[GEO_TASKS_ICON], Color(1, 1, 1), 8, 8, 63, 30);
		}
		RenderTextOnScreen(meshList[GEO_TEXT], "Tasks", Color(0.6f, 0.3f, 0.3f), 2, 61, 23);
		break;
	case SceneOffice::C_EMAIL:
		if (numUnreadEmails == 0) {
			//no tasks left
			//add graphic somewhere its vv plain rn
			RenderTextOnScreen(meshList[GEO_TEXT], "NO MORE UNREAD EMAILS", Color(0.6f, 0.3f, 0.3f), 3, 20, 30);
		}
		else {
			std::size_t pos = emails[numUnreadEmails - 1].second.find("(");
			RenderTextOnScreen(meshList[GEO_TEXT], emails[numUnreadEmails - 1].second.substr(0, pos - 1), Color(0.6f, 0.3f, 0.3f), 3, 20, 40);
			RenderTextOnScreen(meshList[GEO_TEXT], emails[numUnreadEmails - 1].second.substr(pos), Color(0.6f, 0.3f, 0.3f), 2.5f, 20, 38);
			RenderImageOnScreen(meshList[GEO_USER_ICON], Color(1, 1, 1), 5, 5, 15, 40);

			for (unsigned i = 0; i < emails[numUnreadEmails - 1].first.size(); i++) {
				RenderTextOnScreen(meshList[GEO_TEXT], emails[numUnreadEmails - 1].first[i], Color(0.6f, 0.3f, 0.3f), 2.5f, 12, static_cast<float>(35 - (i * 3)));
			}

			//render buttons
			if (14 < xpos && xpos < 33 && 14 < ypos && ypos < 21 && Application::isMouseButtonPressed(0)) {
				RenderImageOnScreen(meshList[GEO_BUTTON_PRESSED], Color(1, 1, 1), 20, 7, 25, 18);
				RenderTextOnScreen(meshList[GEO_TEXT], "REPORT", Color(1, 1, 1), 3.5f, 18, 15);
			}
			else {
				RenderImageOnScreen(meshList[GEO_BUTTON_NOT_PRESSED], Color(1, 1, 1), 20, 7, 25, 18);
				RenderTextOnScreen(meshList[GEO_TEXT], "REPORT", Color(1, 1, 1), 3.5f, 18, 17);
			}
			if (45 < xpos && xpos < 64 && 14 < ypos && ypos < 21 && Application::isMouseButtonPressed(0)) {
				RenderImageOnScreen(meshList[GEO_BUTTON_PRESSED], Color(1, 1, 1), 20, 7, 55, 18);
				RenderTextOnScreen(meshList[GEO_TEXT], "TAKE ACTION", Color(1, 1, 1), 3, 46, 15);
			}
			else {
				RenderImageOnScreen(meshList[GEO_BUTTON_NOT_PRESSED], Color(1, 1, 1), 20, 7, 55, 18);
				RenderTextOnScreen(meshList[GEO_TEXT], "TAKE ACTION", Color(1, 1, 1), 3, 46, 17);
			}
		}
		break;
	case SceneOffice::C_SHOPPINGPAGE_MENU:
		RenderImageOnScreen(meshList[GEO_SOCIALSCORE_BAR], Color(1, 1, 1), 68, 18, 41, 38);
		RenderImageOnScreen(meshList[GEO_SHOP_LOGO], Color(1, 1, 1), 24, 15, 41, 38);
		RenderImageOnScreen(meshList[GEO_SHOP_MENU], Color(1, 1, 1), 50, 15, 41, 20);
		break;
	case SceneOffice::C_SHOPPINGPAGE:
		RenderImageOnScreen(meshList[GEO_SOCIALSCORE_BAR], Color(1, 1, 1), 68, 13, 41, 43);
		RenderTextOnScreen(meshList[GEO_TEXT], shopList[pageCounter].second, Color(1, 1, 1), 6, (40 - static_cast<float>(shopList[pageCounter].second.length() * 1.5)), 38);
		for (unsigned i = 0; i < shopList[pageCounter].first.size(); i++) {
			switch (pageCounter)
			{
			case 0:
				tempMesh = meshList[GEO_ITEM_CABLE];
				break;
			case 1:
				tempMesh = meshList[GEO_ITEM_PHONE];
				break;
			case 2:
				tempMesh = meshList[GEO_ITEM_WATCH];
				break;
			}
			if (shopList[pageCounter].first[i].hasPicture) {
				RenderImageOnScreen(tempMesh, Color(1, 1, 1), 15, 15, 15 + (i * 17), 27);
			}
			else {
				RenderImageOnScreen(meshList[GEO_ITEM_NOIMAGE], Color(1, 1, 1), 15, 15, 15 + (i * 17), 27);
			}
			RenderTextOnScreen(meshList[GEO_TEXT], std::to_string(shopList[pageCounter].first[i].productPrice) + ".00", Color(0.6f, 0.3f, 0.3f), 2, 16 + (i * 17), 29);
			RenderTextOnScreen(meshList[GEO_TEXT], shopList[pageCounter].first[i].productName, Color(0.6f, 0.3f, 0.3f), 1.5f, 9 + (i * 17), 24);
			if (shopList[pageCounter].first[i].isLimitedEdition) {
				RenderImageOnScreen(meshList[GEO_LIMITED_EDITION], Color(1, 1, 1), 3, 3, 20 + (i * 17), 18);
			}
			for (int j = 0; j < shopList[pageCounter].first[i].reviewStars; j++) {
				RenderImageOnScreen(meshList[GEO_STAR], Color(1, 1, 1), 1.5f, 1.5f, 10 + (i * 17) + (j * 2.5f), 22);
			}
			
		}
		break;
	case SceneOffice::C_CLOCKIN:
		if (!hasClockedIn) {
			RenderTextOnScreen(meshList[GEO_TEXT], "Click button to clock in:", Color(0.6f, 0.3f, 0.3f), 3, 27, 30);
			if (32 < xpos && xpos < 50 && 19 < ypos && ypos < 25 && Application::isMouseButtonPressed(0)) {
				RenderImageOnScreen(meshList[GEO_BUTTON_PRESSED], Color(1, 1, 1), 20, 7, 42, 23);
				RenderTextOnScreen(meshList[GEO_TEXT], "CLOCK IN", Color(1, 1, 1), 3.5f, 35, 20);
			}
			else {
				RenderImageOnScreen(meshList[GEO_BUTTON_NOT_PRESSED], Color(1, 1, 1), 20, 7, 42, 23);
				RenderTextOnScreen(meshList[GEO_TEXT], "CLOCK IN", Color(1, 1, 1), 3.5f, 35, 22);
			}
		}
		break;
	case SceneOffice::C_EMAIL_EXPLAINING:
		//TODO: MAKE THIS BETTER
		RenderTextOnScreen(meshList[GEO_TEXT], "LET'S GO PHISHING!!", Color(0.6f, 0.3f, 0.3f), 5, 12, 40);
		RenderTextOnScreen(meshList[GEO_TEXT], "Read and detect scams in emails!", Color(0.6f, 0.3f, 0.3f), 3, 12, 32);
		RenderTextOnScreen(meshList[GEO_TEXT], "Report the fake scam emails,", Color(0.6f, 0.3f, 0.3f), 3, 12, 28);
		RenderTextOnScreen(meshList[GEO_TEXT], "and take action on the real ones!", Color(0.6f, 0.3f, 0.3f), 3, 12, 24);
		RenderTextOnScreen(meshList[GEO_TEXT], "Hint: watch out for any inconsistencies in the email", Color(0.5f, 0.5f, 0.5f), 2.5f, 12, 20);
		RenderTextOnScreen(meshList[GEO_TEXT], "as well as be vigilant for any impersonation.", Color(0.5f, 0.5f, 0.5f), 2.5f, 12, 17);
		break;
	}

	RenderTextOnScreen(meshList[GEO_TEXT], clockTime, Color(0.6f, 0.3f, 0.3f), 2, 38, 47);
}

void SceneOffice::RenderNotifications()
{
	RenderImageOnScreen(meshList[GEO_NOTIF_BOX], Color(1, 1, 1), 40, 12, 60, 7);
	switch (notif)
	{
	case SceneOffice::N_CHAT:
		break;
	case SceneOffice::N_TASK:
		RenderImageOnScreen(meshList[GEO_TASKS_ICON], Color(1, 1, 1), 6, 6, 45, 7);
		RenderTextOnScreen(meshList[GEO_TEXT], "New Task Available", Color(0.6f, 0.3f, 0.3f), 3, 49, 7);
		RenderTextOnScreen(meshList[GEO_TEXT], "Check your phone for more details", Color(0.6f, 0.3f, 0.3f), 2
			, 49, 5);
		break;
	case SceneOffice::N_TASK_DONE:
		RenderImageOnScreen(meshList[GEO_TASKS_ICON], Color(1, 1, 1), 6, 6, 45, 7);
		RenderTextOnScreen(meshList[GEO_TEXT], "Task Completed", Color(0.6f, 0.3f, 0.3f), 3, 49, 7);
		RenderTextOnScreen(meshList[GEO_TEXT], "Check your phone for more details", Color(0.6f, 0.3f, 0.3f), 2
			, 49, 5);
		break;
	case SceneOffice::N_WORK:
		break;
	default:
		break;
	}
}

void SceneOffice::RenderText(Mesh* mesh, std::string text, Color color)
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

void SceneOffice::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y, float xRotation, float zRotation)
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

std::vector<float> SceneOffice::getNumberValues(std::string filename)
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

void SceneOffice::initialiseShop(std::string textFile)
{
	std::string str;
	std::fstream file(textFile, std::ios::in);
	std::vector<products> productCategory;
	std::string categoryName;
	products product;
	if (file.is_open())
	{
		while (std::getline(file, str))
		{
			if (str == "//") {
				shopList.push_back(std::make_pair(productCategory, categoryName));
				productCategory.clear();
			}
			else if (str.substr(0, 2) == "c:") {
				categoryName = str.substr(2);
			}
			else if (str.substr(0, 2) == "p:") {
				product.productPrice = std::stoi(str.substr(2));
			}
			else if (str.substr(0, 2) == "l:") {
				product.isLimitedEdition = std::stoi(str.substr(2));
			}
			else if (str.substr(0, 2) == "b:") {
				product.reviewStars = std::stoi(str.substr(2));
			}
			else if (str.substr(0, 2) == "n:") {
				product.productName = str.substr(2);
			}
			else if (str.substr(0, 2) == "i:") {
				product.hasPicture = std::stoi(str.substr(2));
			}
			else if (str.substr(0, 2) == "s:") {
				product.isScam = std::stoi(str.substr(2));
			}
			else {
				productCategory.push_back(product);
			}
		}
	}
	else
		std::cout << "Could not open the file\n";
}

void SceneOffice::initialiseEmails(std::string textFile)
{
	std::string str;
	std::fstream file(textFile, std::ios::in);
	std::string speaker;
	bool isScam;
	std::vector<std::string> email;
	if (file.is_open())
	{
		while (std::getline(file, str))
		{
			if (str == " ") {
				emails.push_back(std::make_pair(email, speaker));
				isScamEmails.push_back(isScam);
				email.clear();
			}
			if (str.substr(0, 2) == "s:") {
				speaker = str.substr(2);
			}
			else if (str.substr(0, 2) == "b:") {
				isScam = std::stoi(str.substr(2));
			}
			else {
				email.push_back(str);
			}
		}
	}
	else
		std::cout << "Could not open the file\n";
}

void SceneOffice::RenderImageOnScreen(Mesh* mesh, Color color, float sizex, float sizey, float x, float y, float xRotation, float zRotation)
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

