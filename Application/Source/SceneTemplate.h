#ifndef SCENE_TEMPLATE_H
#define SCENE_TEMPLATE_H

#include "Scene.h"
#include "FirstPersonCamera.h"
#include "Mesh.h"
#include "MeshBuilder.h"
#include "Vertex.h"
#include "MatrixStack.h"
#include "Light.h"
#include "LoadTGA.h"
#include "Material.h"

#define SKYBOXSIZE 1000
#define PLAYERSPEED 2.0f

class SceneTemplate : public Scene
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

		GEO_CAR,
		GEO_AMBULANCE,
		GEO_VAN,
		GEO_TAXI,
		GEO_FIRETRUCK,
		GEO_POLICE,



		GEO_FARBUILDING,
		GEO_FARBUILDING2,
		GEO_FARBUILDING3,
		GEO_FARBUILDING4,
		//GEO_FARBUILDING5,



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

public:
	SceneTemplate();
	~SceneTemplate();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

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
	void RenderText(Mesh* mesh, std::string text, Color color);

	//use screen points
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderImageOnScreen(Mesh* mesh, Color color, float sizex = 1, float sizey = 1, float x = 0, float y = 0);
	std::vector<float> getNumberValues(std::string filename);
	std::vector<float> fontData;

	bool enableLight;
};




#endif
