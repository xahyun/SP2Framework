#ifndef SCENE_HOME_H
#define SCENE_HOME_H

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

class SceneHome : public Scene
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

		//home items
		GEO_HOMEFLOOR,
		GEO_HOMEDOOR,
		GEO_BED,
		GEO_SIDETABLE,
		GEO_FLOORLAMP,
		GEO_SOFA,
		GEO_RUG,
		GEO_PLATFORM,
		GEO_TVTABLE,
		GEO_TV,
		GEO_COFFEETABLE,
		GEO_LOUNGECHAIR,
		GEO_BOOKCASE,
		GEO_BOOKS,
		GEO_RUG2,
		GEO_BENCH,
		GEO_COATRACK,
		GEO_PLANT2,
		GEO_WALL,

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
	SceneHome();
	~SceneHome();

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

	//in-game stuff
	Clock* clock;
	player* Player;
	Phone* phone;
	std::string clockTime = "";
	int money = int(DEFAULTMONEY), socialMeter = int(DEFAULTSOCIALMETER);

	//distance and angle from door
	float distanceFromDoor = 0;
	float angleFromDoor = 0;

	//tasks !!
	bool hasReceivedTasks = false;
	Notification notifs = Notification::N_OFF;

	//phone ??
	PhoneScreen phoneState;
	int chatCount;
	bool isPhoneOpen;
	bool hoverAnimation[9] = { false, false, false, false, false, false, false, false, false };
	float NotificationTimer = 3;
	//task list
	int pageCounter;
	int taskNumber = 1;
};




#endif
