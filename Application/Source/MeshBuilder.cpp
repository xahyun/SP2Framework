#include "MeshBuilder.h"
#include <GL\glew.h>
#include "Vertex.h"
#include "MyMath.h"
#include <vector>

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	v.pos.Set(-lengthX, 0.f, 0.f); v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(lengthX, 0.f, 0.f); v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.f, -lengthY, 0.f); v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.f, lengthY, 0.f); v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.f, 0.f, -lengthX); v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.f, 0.f, lengthX); v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_LINES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float lengthX, float lengthY, int texcoord)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.f); v.color = color; v.normal.Set(0, 0, 1); v.texCoord.Set(texcoord, texcoord);
	vertex_buffer_data.push_back(v); 
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.f); v.color = color; v.normal.Set(0, 0, 1); v.texCoord.Set(0, texcoord);
	vertex_buffer_data.push_back(v); 
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.f); v.color = color; v.normal.Set(0, 0, 1); v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v); 
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.f); v.color = color; v.normal.Set(0, 0, 1); v.texCoord.Set(texcoord, 0);
	vertex_buffer_data.push_back(v);
	
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);

	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);


	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}



/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param Radius - radius of circle
\param numSegments - number of segments circle has (default 36)

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCircle(const std::string& meshName, Color color, float Radius, int numSegments)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	for (int i = 0; i <= numSegments; i ++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
		v.pos.Set(Radius * cosf(theta), 0.f, Radius * sinf(theta)); v.color = color; v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.f, 0.f, 0.f); v.color = color; v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}
	for (unsigned int i = 0; i < vertex_buffer_data.size(); i ++) {
		index_buffer_data.push_back(i);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}


/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param outerRadius - radius of circle
\param innterRadius - radius of middle portion
\param numSegments - number of segments circle has (default 36)

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateRing(const std::string& meshName, Color color, float outerRadius, float innerRadius, int numSegments)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<unsigned> index_buffer_data;
	for (int i = 0; i <= numSegments; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
		v.pos.Set(outerRadius * cosf(theta), 0.f, outerRadius * sinf(theta)); v.color = color; v.normal.Set(0, 1, 0);
		
		vertex_buffer_data.push_back(v);
		v.pos.Set(innerRadius * cosf(theta), 0.f, innerRadius * sinf(theta)); v.color = color; v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}
	for (unsigned int i = 0; i < vertex_buffer_data.size(); i++) {
		index_buffer_data.push_back(i);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}



/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	//TODO: figure out normals
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(-1, 0, 0);  v.texCoord.Set(0, 0);	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	v.normal.Set(-1, 0, 0);  v.texCoord.Set(0, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	 v.color = color;	    v.normal.Set(-1, 0, 0);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(-1, 0, 0);  v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(-1, 0, 0);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(-1, 0, 0);  v.texCoord.Set(1, 0);  vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	 v.color = color;	    v.normal.Set(0, 0, -1);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(0, 0, -1);  v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(0, 0, -1);  v.texCoord.Set(0, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, 0, -1);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(0, 0, -1);  v.texCoord.Set(1, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(0, 0, -1);  v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, -1, 0);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(0, -1, 0);  v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(0, -1, 0);  v.texCoord.Set(1, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, -1, 0);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	v.normal.Set(0, -1, 0);  v.texCoord.Set(0, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(0, -1, 0);  v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);

	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, 0, 1);  v.texCoord.Set(0, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	v.normal.Set(0, 0, 1);  v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, 0, 1);  v.texCoord.Set(1, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	v.color = color;     	v.normal.Set(0, 0, 1);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, 0, 1);  v.texCoord.Set(0, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, 0, 1);  v.texCoord.Set(1, 0);  vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(1, 0, 0);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(1, 0, 0);  v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	    v.normal.Set(1, 0, 0);  v.texCoord.Set(1, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(1, 0, 0);  v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(1, 0, 0);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(1, 0, 0);  v.texCoord.Set(0, 1);  vertex_buffer_data.push_back(v);

	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, 1, 0);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, 1, 0);  v.texCoord.Set(1, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(0, 1, 0);  v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, 1, 0);  v.texCoord.Set(1, 1);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, -0.5f * lengthZ);	v.color = color;	v.normal.Set(0, 1, 0);  v.texCoord.Set(0, 0);  vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.5f * lengthZ);	v.color = color;	    v.normal.Set(0, 1, 0);  v.texCoord.Set(0, 1);  vertex_buffer_data.push_back(v);

	for (unsigned i = 0; i < 36; ++i)
	{
		index_buffer_data.push_back(i);
	}

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}


/******************************************************************************/
/*!
\brief
Generate the vertices of a hemisphere; Use Color to generate colour for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param Radius - radius of the hemisphere
\param numSegments - number of "slices" generated
\param numStacks - number of "stacks" generated

\return Pointer to mesh storing VBO/IBO of hemisphere
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateHemisphere(const std::string& meshName, Color color, float Radius, int numSegments, int numStacks)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 180.f / numStacks;
	float degreePerSlice = 360.f / numSegments;

	for (unsigned stack = 0; stack < unsigned int(numStacks / 2) + 1 ; ++stack)
	{
		float phi = -90.f + stack * degreePerStack;
		for (unsigned slice = 0; slice < unsigned int(numSegments + 1); ++slice)
		{
			float theta = slice * degreePerSlice;
			float x = cosf(Math::DegreeToRadian(phi)) * cosf(Math::DegreeToRadian(theta));
			float y = sinf(Math::DegreeToRadian(phi));
			float z = cosf(Math::DegreeToRadian(phi)) * sinf(Math::DegreeToRadian(theta));

			v.pos.Set(x * Radius, y * Radius, z * Radius);		v.color = color; 	v.normal.Set(x, y, z);  vertex_buffer_data.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < unsigned int(numStacks); ++stack)
	{
		for (unsigned slice = 0; slice < unsigned int(numSegments + 1); ++slice)
		{
			index_buffer_data.push_back(stack * (numSegments + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSegments + 1) + slice);
		}
	}

	int x = index_buffer_data.size();
	for (int i = 0; i <= numSegments; i++) {
		float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);
		v.pos.Set(Radius * cosf(theta), 0.f, Radius * sinf(theta)); v.color = color; v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0.f, 0.f, 0.f); v.color = color; v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
	}

	for (unsigned int i = x; i < vertex_buffer_data.size(); i++) {
		index_buffer_data.push_back(i);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}


/******************************************************************************/
/*!
\brief
Generate the vertices of a Sphere; Use Color to generate colour for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param Radius - radius of the sphere
\param numSegments - number of "slices" generated
\param numStacks - number of "stacks" generated

\return Pointer to mesh storing VBO/IBO of Sphere
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateSphere(const std::string& meshName, Color color, float Radius, int numSegments, int numStacks)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float degreePerStack = 180.f / numStacks;
	float degreePerSlice = 360.f / numSegments;

	for (int stack = 0; stack < numStacks + 1; ++stack)
	{
		float phi = -90.f + stack * degreePerStack;
		for (int slice = 0; slice < numSegments + 1; ++slice)
		{
			float theta = slice * degreePerSlice;
			float x = Radius * cosf(Math::DegreeToRadian(phi)) * cosf(Math::DegreeToRadian(theta));
			float y = Radius * sinf(Math::DegreeToRadian(phi));
			float z = Radius * cosf(Math::DegreeToRadian(phi)) * sinf(Math::DegreeToRadian(theta));
			float s = (float (stack) / float(numStacks));
			float t = 1 - (float (slice) / float(numSegments));

			
			v.normal.Set(cosf(Math::DegreeToRadian(phi)) * cosf(Math::DegreeToRadian(theta)), sinf(Math::DegreeToRadian(phi)), cosf(Math::DegreeToRadian(phi)) * sinf(Math::DegreeToRadian(theta)));
			v.pos.Set(x, y, z);		v.color = color;    v.texCoord.Set(t, s); 	vertex_buffer_data.push_back(v);
		}
	}
	for (int stack = 0; stack < numStacks; ++stack)
	{
		for (int slice = 0; slice < numSegments + 1; ++slice)
		{
			index_buffer_data.push_back(stack * (numSegments + 1) + slice);
			index_buffer_data.push_back((stack + 1) * (numSegments + 1) + slice);
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}


/******************************************************************************/
/*!
\brief
Generate the vertices of a Cylinder; Use Color to generate colour for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param Radius - radius of the cylinder
\param numSegments - number of "slices" generated
\param height - height of cylinder
\param numStacks - number of "stacks" generated

\return Pointer to mesh storing VBO/IBO of cylinder
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCylinder(const std::string& meshName, Color color, float Radius, float height, int numSegments, int numStacks)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float radianPerSlice = Math::TWO_PI / numSegments;

	//cylinder top
	for (int slice = 0; slice <= numSegments; ++slice)
	{
		v.pos.Set(Radius * cosf(slice * radianPerSlice), height * 0.5f, Radius * sinf(slice * radianPerSlice));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0, height * 0.5f, 0);
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}
	for (int slice = 0; slice <= numSegments; ++slice)
	{
		index_buffer_data.push_back(slice * 2 + 0);
		index_buffer_data.push_back(slice * 2 + 1);
	}

	//cylinder side
	unsigned offset = index_buffer_data.size(); //offset because index does not start from 0, starts from where top circle ends
	for (int slice = 0; slice <= numSegments; ++slice)
	{
		v.pos.Set(Radius * cosf(slice * radianPerSlice), -height * 0.5f, Radius * sinf(slice * radianPerSlice));
		v.color = color;
		v.normal.Set(cosf(slice * radianPerSlice), 0, sinf(radianPerSlice));
		vertex_buffer_data.push_back(v);
		v.pos.Set(Radius * cosf(slice * radianPerSlice), height * 0.5f, Radius * sinf(slice * radianPerSlice)); //going bottom to top!
		v.color = color;
		v.normal.Set(cosf(slice * radianPerSlice), 0, sinf(radianPerSlice));
		vertex_buffer_data.push_back(v);
	}
	for (int slice = 0; slice <= numSegments; ++slice)
	{
		index_buffer_data.push_back(offset + slice * 2 + 0);
		index_buffer_data.push_back(offset + slice * 2 + 1);
	}

	//cylinder bottom
	offset = index_buffer_data.size();

	for (int slice = 0; slice <= numSegments; ++slice)
	{
		v.pos.Set(0, -height * 0.5f, 0);
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(Radius * cosf(slice * radianPerSlice), -height * 0.5f, Radius * sinf(slice * radianPerSlice));
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
	}
	for (int slice = 0; slice <= numSegments; ++slice)
	{
		index_buffer_data.push_back(offset + slice * 2 + 0);
		index_buffer_data.push_back(offset + slice * 2 + 1);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}



/******************************************************************************/
/*!
\brief
Generate the vertices of a Cylinder; Use Color to generate colour for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param Radius - radius of the cylinder
\param numSegments - number of "slices" generated
\param height - height of cylinder
\param numStacks - number of "stacks" generated

\return Pointer to mesh storing VBO/IBO of cylinder
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateHalfCylinder(const std::string& meshName, Color color, float Radius, float height, int numSegments, int numStacks)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float radianPerSlice = Math::TWO_PI / numSegments;

	//cylinder top
	for (int slice = 0; slice <= numSegments / 2; ++slice)
	{
		v.pos.Set(Radius * cosf(slice * radianPerSlice), height * 0.5f, Radius * sinf(slice * radianPerSlice));
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(0, height * 0.5f, 0);
		v.color = color;
		v.normal.Set(0, 1, 0);
		vertex_buffer_data.push_back(v);
	}
	for (int slice = 0; slice <= numSegments / 2; ++slice)
	{
		index_buffer_data.push_back(slice * 2 + 0);
		index_buffer_data.push_back(slice * 2 + 1);
	}

	//cylinder side
	unsigned offset = index_buffer_data.size(); //offset because index does not start from 0, starts from where top circle ends
	for (int slice = 0; slice <= numSegments / 2; ++slice)
	{
		v.pos.Set(Radius * cosf(slice * radianPerSlice), -height * 0.5f, Radius * sinf(slice * radianPerSlice));
		v.color = color;
		v.normal.Set(cosf(slice * radianPerSlice), 0, sinf(radianPerSlice));
		vertex_buffer_data.push_back(v);
		v.pos.Set(Radius * cosf(slice * radianPerSlice), height * 0.5f, Radius * sinf(slice * radianPerSlice)); //going bottom to top!
		v.color = color;
		v.normal.Set(cosf(slice * radianPerSlice), 0, sinf(radianPerSlice));
		vertex_buffer_data.push_back(v);
	}
	for (int slice = 0; slice <= numSegments / 2; ++slice)
	{
		index_buffer_data.push_back(offset + slice * 2 + 0);
		index_buffer_data.push_back(offset + slice * 2 + 1);
	}

	//cylinder bottom
	offset = index_buffer_data.size();

	for (int slice = 0; slice <= numSegments / 2; ++slice)
	{
		v.pos.Set(0, -height * 0.5f, 0);
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
		v.pos.Set(Radius * cosf(slice * radianPerSlice), -height * 0.5f, Radius * sinf(slice * radianPerSlice));
		v.color = color;
		v.normal.Set(0, -1, 0);
		vertex_buffer_data.push_back(v);
	}
	for (int slice = 0; slice <= numSegments / 2; ++slice)
	{
		index_buffer_data.push_back(offset + slice * 2 + 0);
		index_buffer_data.push_back(offset + slice * 2 + 1);
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}




/******************************************************************************/
/*!
\brief
Generate the vertices of a Cone; Use Color to generate colour for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param Radius - radius of the cone
\param numSegments - number of "slices" generated
\param height - height of cone
\param numStacks - number of "stacks" generated

\return Pointer to mesh storing VBO/IBO of cone
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCone(const std::string& meshName, Color color, float Radius, float height, int numSegments, int numStacks)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float stackHeight = float(height / numStacks);
	float difference = numStacks / Radius;

	int x = index_buffer_data.size();
	for (int i = 0; i <= numSegments; i++) {
		float theta = Math::TWO_PI * float(i) / float(numSegments);
		v.pos.Set(0.f, -height / 2, 0.f); v.color = color;
		vertex_buffer_data.push_back(v);
		v.pos.Set(Radius * cosf(theta), -height / 2, Radius * sinf(theta)); v.color = color;
		vertex_buffer_data.push_back(v);
	}

	for (unsigned i = x; i < vertex_buffer_data.size(); i++) {
		index_buffer_data.push_back(i);
	}

	for (int stack = 0; stack < Radius; stack++) {
		for (int segment = 0; segment <= numSegments; segment++) {
			//its basically just the same code but like abit diff
			float theta = Math::TWO_PI * float(segment) / float(numSegments); 
			
			
		}
	}

	for (unsigned int i = 0; i < vertex_buffer_data.size(); i++) {
		index_buffer_data.push_back(i);
	}

	

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateTorus(const std::string& meshName, Color color, float outerRadius, float innerRadius, int numSegments, int numStacks)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string& meshName, const std::string& file_path)
{
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if (!success)
		return NULL;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJMTL(const std::string& meshName, const std::string& file_path, const std::string& mtl_path)
{
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	std::vector<Material> materials;
	bool success = LoadOBJMTL(file_path.c_str(), mtl_path.c_str(), vertices, uvs, normals, materials);
	if (!success)
		return NULL;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh* mesh = new Mesh(meshName);

	for (Material& material : materials) {
		mesh->materials.push_back(material);
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();

	return mesh;

}

Mesh* MeshBuilder::GenerateText(const std::string& meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	float width = 1.f / numCol;
	float height = 1.f / numRow;

	unsigned offset = 0;
	
	for (unsigned row = 0; row < numRow; ++row)
	{
		for (unsigned col = 0; col < numCol; ++col)
		{
			//Task: Add 4 vertices into vertex_buffer_data
			//Task: Add 6 indices into index_buffer_data
			v.pos.Set(0.5f, 0.5f, 0.f);  v.normal.Set(0, 0, 1);  v.texCoord.Set((col + 1) * width, (numRow - row) * height);       vertex_buffer_data.push_back(v); //TR
			v.pos.Set(-0.5f, 0.5f, 0.f);  v.normal.Set(0, 0, 1); v.texCoord.Set(col * width, (numRow - row) * height);       vertex_buffer_data.push_back(v); //TL
			v.pos.Set(-0.5f, -0.5f, 0.f);  v.normal.Set(0, 0, 1);  v.texCoord.Set(col * width, (numRow - row - 1) * height);       vertex_buffer_data.push_back(v); //BL
			v.pos.Set(0.5f, -0.5f, 0.f);   v.normal.Set(0, 0, 1); v.texCoord.Set((col + 1) * width, (numRow - row - 1) * height);      vertex_buffer_data.push_back(v); //BR

			//tri1
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			//tri2
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);

			offset += 4;
		}
	}

	Mesh* mesh = new Mesh(meshName);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() *
		sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size()
		* sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	mesh->mode = Mesh::DRAW_TRIANGLES;
	mesh->indexSize = index_buffer_data.size();
	return mesh;
}

Mesh* MeshBuilder::GenerateOfficefloor(const std::string& meshName, Color color, float lengthX, float lengthY, int texcoord)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	v.pos.Set(0.5f * lengthX, 0.5f * lengthY, 0.f);    v.color = color; v.normal.Set(0, 0, 1);  vertex_buffer_data.push_back(v); v.texCoord.Set(0, 5); //v0
	v.pos.Set(-0.5f * lengthX, 0.5f * lengthY, 0.f);    v.color = color; v.normal.Set(0, 0, 1);  vertex_buffer_data.push_back(v); v.texCoord.Set(5, 5); //v1
	v.pos.Set(-0.5f * lengthX, -0.5f * lengthY, 0.f);    v.color = color; v.normal.Set(0, 0, 1);  vertex_buffer_data.push_back(v); v.texCoord.Set(5, 0); //v2
	v.pos.Set(0.5f * lengthX, -0.5f * lengthY, 0.f);    v.color = color; v.normal.Set(0, 0, 1); vertex_buffer_data.push_back(v); v.texCoord.Set(0, 0);//v3

	//tri1
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	//tri2
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh* mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}