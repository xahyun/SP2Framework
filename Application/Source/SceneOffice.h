#ifndef SCENE_OFFICE_H
#define SCENE_OFFICE_H

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
#include <utility>
#include <string>
#include <vector>
#include <tuple>

#define SKYBOXSIZE 1000
#define PLAYERSPEED 2.0f

struct products {
	bool hasPicture;
	bool isLimitedEdition;
	int reviewStars;
	std::string productName;
	int productPrice;
	bool isScam;
};

class SceneOffice : public Scene
{
	enum GEOMETRY_TYPE {
		GEO_AXES,
		GEO_TEXT,

		//temporary items
		GEO_DIALOG_BOX,
		GEO_TEMP_QUAD,

		//skybox and lightball
		GEO_LIGHTBALL,
		GEO_FRONT,
		GEO_BACK,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,

		//otherUI
		GEO_WATCH,
		GEO_HAPPY_FACE,
		GEO_ANGRY_FACE,

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

		//computer UI,
		GEO_DEFAULT_WALLPAPER,
		GEO_POWER_BUTTON,
		GEO_EMAIL_ICON,
		GEO_SHOPPING_ICON,
		GEO_WORK_ICON,
		GEO_USER_ICON,
		GEO_BUTTON_PRESSED,
		GEO_BUTTON_NOT_PRESSED,

		//shopping thing UI
		GEO_SHOP_LOGO,
		GEO_SHOP_MENU,
		GEO_ITEM_NOIMAGE,
		GEO_ITEM_PHONE,
		GEO_ITEM_WATCH,
		GEO_ITEM_CABLE,
		GEO_STAR,
		GEO_LIMITED_EDITION,

		//office items
		GEO_OFFICETABLE,
		GEO_COMPUTER,
		GEO_KEYBOARD,
		GEO_OFFICECHAIR,
		GEO_PLANT1,
		GEO_PLANT2,
		GEO_OFFICEDOOR,
		GEO_BOOKS,
		GEO_WALL,
		GEO_OFFICEFLOOR,

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

	enum computerScreen {
		C_COMOFF,
		C_LOADING,
		C_HOME,
		C_EMAIL,
		C_EMAIL_EXPLAINING,
		C_SHOPPINGPAGE_MENU,
		C_SHOPPINGPAGE,
		C_CLOCKIN,
		C_TASKS,
		C_WORK
	};

	enum Notification {
		N_OFF,
		N_CHAT,
		N_TASK,
		N_TASK_DONE,
		N_WORK
	};

public:
	SceneOffice();
	~SceneOffice();

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
	Notification notifs = Notification::N_OFF;

	//functions to render things
	void RenderMesh(Mesh* mesh, bool enableLight);
	void RenderSkybox();
	void RenderPhoneUI();
	void RenderComputerUI();
	void RenderNotifications();
	void RenderText(Mesh* mesh, std::string text, Color color);

	//use screen points
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size = 1, float x = 0, float y = 0, float xRotation = 0, float zRotation = 0);
	void RenderImageOnScreen(Mesh* mesh, Color color, float sizex = 1, float sizey = 1, float x = 0, float y = 0, float xRotation = 0, float zRotation = 0);
	std::vector<float> getNumberValues(std::string filename);
	std::vector<float> fontData;

	bool enableLight;
	bool keyToggle;
	bool mouseToggle;

	//in-game stuff
	Clock* clock;
	player* Player;
	Phone* phone;
	std::string clockTime = "";
	int money = int(DEFAULTMONEY), socialMeter = int(DEFAULTSOCIALMETER);
	double xpos, ypos;
	float NotificationTimer = 2;
	bool hasReceivedTasks = false;
	int taskNumber = 1;
	Notification notif;

	//phone ??
	PhoneScreen phoneState;
	int taskCount, chatCount;
	bool isPhoneOpen = false;
	bool hoverAnimation[9] = { false, false, false, false, false, false, false, false, false };
	//just a general page counter
	int pageCounter;

	//work computer 
	float angleBetweenComputer = 0;
	bool isUsingComputer;
	bool comHoverAnimation[6] = { false, false, false, false, false, false };
	//TODO: if there's time make the emails into a struct thanks
	std::vector<bool> isScamEmails;
	std::vector<std::pair<std::vector<std::string>, std::string>> emails;
	void initialiseEmails(std::string textFile);
	int numUnreadEmails;
	computerScreen computerState;
	float workTimer = 0;
	int workTasksLeft = 10;
	bool hasClockedIn = false;

	//shopping game
	int moneySpent = 0;
	std::vector<std::pair<std::vector<products>, std::string>> shopList;
	void initialiseShop(std::string textFile);
};


#endif
