#ifndef SCENE_CITY_H
#define SCENE_CITY_H

#include "Scene.h"
#include "FirstPersonCamera.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "Vertex.h"
#include "MatrixStack.h"
#include "Light.h"
#include "LoadTGA.h"
#include "Material.h"
#include "clock.h"
#include "player.h"

#define SKYBOXSIZE 1000
#define PLAYERSPEED 2.0f

class SceneCity : public Scene
{
	enum GEOMETRY_TYPE {
		GEO_AXES,
		GEO_TEXT,

		//temporary items
		GEO_DIALOG_BOX,
		GEO_TEMP_QUAD,

		//otherUI
		GEO_WATCH,

		//phone ui
		GEO_NOTIF_BOX,
		GEO_PHONE_UI,
		GEO_CHAT_ICON,
		GEO_PHONE_ICON,
		GEO_HOME_ICON,
		GEO_MONEY_ICON,
		GEO_SOCIALSCORE_ICON,
		GEO_SOCIALSCORE_UI,
		GEO_SOCIALSCORE_BAR,
		GEO_TASKS_ICON,
		GEO_HELP_ICON,
		GEO_SETTINGS_ICON,

		//skybox and lightball
		GEO_LIGHTBALL,
		GEO_FRONT,
		GEO_BACK,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,

		//city scene
		GEO_HOUSE1,
		GEO_ROAD,
		GEO_HOUSE2,
		GEO_HOUSE5,
		GEO_BIGHOUSE,
		GEO_GRASS,
		GEO_FENCE,
		GEO_CROSSWALK,
		GEO_HOUSE3,
		GEO_HOUSE4,
		GEO_TREE,
		GEO_BIGTREE,
		GEO_CURVE,
		GEO_SKYSCRAPER,
		GEO_SKYSCRAPER2,
		GEO_SKYSCRAPER3,
		GEO_SKYSCRAPER4,
		GEO_SKYSCRAPER5,
		GEO_SKYSCRAPER6,
		GEO_SKYSCRAPER7,
		GEO_HOUSECAR,

		GEO_BIGAPARTMENT,
		GEO_BIGAPARTMENT2,
		GEO_BIGAPARTMENT3,

		GEO_CONDO,
		GEO_CONDO2,

		//vehicles
		GEO_CAR,
		GEO_AMBULANCE,
		GEO_VAN,
		GEO_TAXI,
		GEO_FIRETRUCK,
		GEO_POLICE,

		//far away buildings
		GEO_FARBUILDING,
		GEO_FARBUILDING2,
		GEO_FARBUILDING3,
		GEO_FARBUILDING4,
		//GEO_FARBUILDING5,

		GEO_MARKER,

		GEO_BUILDING,//hospital

		NUM_GEOMETRY,
	};

	enum UNIFORM_TYPE {
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHTENABLED,
		U_LIGHT0_TYPE,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_TYPE,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_NUMLIGHTS,
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,
		U_TOTAL,
	};

	enum PhoneScreen {
		P_PHONEOFF,
		P_HOME,
		P_CHAT,
		P_CHATLIST,
		P_MONEY,
		P_SOCIALSCORE,
		P_HELP,
		P_TASKS
	};

	enum Notification {
		N_OFF,
		N_CHAT,
		N_TASK,
		N_WORK
	};

public:
	SceneCity();
	~SceneCity();

	void Init();
	void TransferGameInfo(Game* game);
	void Update(double dt);
	void Render();
	void Exit();

private:
	unsigned m_vertexArrayID;
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];
	Mesh* meshList[NUM_GEOMETRY];

	FirstPersonCamera camera;
	MS modelStack, viewStack, projectionStack;
	Light light[5];

	//functions to render things
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderPhoneUI();
	void RenderNotifications();
	void RenderText(Mesh* mesh, std::string text, Color color);

	//use screen points
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size = 1, float x = 0, float y = 0, float xRotation = 0, float zRotation = 0);
	void RenderImageOnScreen(Mesh* mesh, Color color, float sizex = 1, float sizey = 1, float x = 0, float y = 0, float xRotation = 0, float zRotation = 0);
	std::vector<float> getNumberValues(std::string filename);
	std::vector<float> fontData;

	double xpos, ypos;
	bool enableLight;
	bool keyToggle;
	bool mouseToggle;
	Notification notifs;

	//in-game stuff
	Clock* clock;
	player* Player;
	Phone* phone;
	std::string clockTime = "";
	int money = int(DEFAULTMONEY), socialMeter = int(DEFAULTSOCIALMETER);

	//phone ??
	PhoneScreen phoneState;
	int taskCount, chatCount;
	bool isPhoneOpen;
	float NotificationTimer = 5;
	bool hoverAnimation[9] = { false, false, false, false, false, false, false, false, false };
	//task list
	int pageCounter;
	int taskNumber = 1;

	//city
	bool hasEnteredOffice = false;
	float distanceToOffice = 0;
};




#endif