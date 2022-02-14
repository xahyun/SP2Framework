#ifndef MESH_BUILDER_H
#define MESH_BUILDER_H
#define BIG_NUMBER INT32_MAX

#include "Mesh.h"
#include "Vertex.h"
#include "LoadOBJ.h"

/******************************************************************************/
/*!
		Class MeshBuilder:
\brief	Provides methods to generate mesh of different shapes
*/
/******************************************************************************/
class MeshBuilder
{
public:
	static Mesh* GenerateAxes(const std::string &meshName, float lengthX = 1.f, float lengthY = 1.f, float lengthZ = 1.f);
	static Mesh* GenerateQuad(const std::string &meshName, Color color, float lengthX = 1.f, float lengthY = 1.f, int texcoord = 1);
	static Mesh* GenerateCircle(const std::string& meshName, Color color, float Radius = 1.f, int numSegments = 36);
	static Mesh* GenerateRing(const std::string& meshName, Color color, float outerRadius = 1.f, float innerRadius = 0.5f, int numSegments = 36);
	static Mesh* GenerateCube(const std::string &meshName, Color color, float lengthX = 1.f, float lengthY = 1.f, float lengthZ = 1.f);
	static Mesh* GenerateHemisphere(const std::string& meshName, Color color, float Radius = 1.f, int numSegments = 18, int numStacks = 36);
	static Mesh* GenerateSphere(const std::string& meshName, Color color, float Radius = 1.f, int numSegments = 18, int numStacks = 36);
	static Mesh* GenerateCylinder(const std::string& meshName, Color color, float Radius = 1.f, float height = 5.0f, int numSegments = 36, int numStacks = 10);
	static Mesh* GenerateHalfCylinder(const std::string& meshName, Color color, float Radius = 1.f, float height = 5.0f, int numSegments = 36, int numStacks = 10);
	static Mesh* GenerateCone(const std::string& meshName, Color color, float Radius = 1.f, float height = 5.0f, int numSegments = 36, int numStacks = 10);
	static Mesh* GenerateTorus(const std::string& meshName, Color color, float outerRadius = 1.f, float innerRadius = 0.5f, int numSegments = 18, int numStacks = 36);

	static Mesh* GenerateOBJ(const std::string& meshName, const std::string& file_path);
	static Mesh* GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path);

	static Mesh* GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol);
};

#endif