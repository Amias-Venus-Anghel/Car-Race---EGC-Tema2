#pragma once

#include "components/simple_scene.h"
#include "simpleobject3D.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;

class EnemyCar
{
public:
	/* creates an enemy car given a list of points and a distance from those points, for its route */
	EnemyCar(std::vector<glm::vec3> movePoints,  float moveDist, glm::vec3 color);
	~EnemyCar();

	/* returns mesh and model matrix for car */
	Mesh* GetMesh();
	glm::mat4 GetModelMatrix();
	/* enemy car position */
	glm::vec3 position;
	glm::vec3 color;

private:
	/* given 2 points A and B finds the point C at distance d as such that AC perpedicular on AB */
	glm::vec3 getNewPoint(glm::vec3 p1, glm::vec3 p2, float d);
	/* generates points between 2 given points in a straight line */
	vector<glm::vec3> generateOnStraightLine(glm::vec3 p1, glm::vec3 p2);
	/* for 2 points generates a route portion */
	vector<glm::vec3> createSegment(glm::vec3 p1, glm::vec3 p2);
	/* generates the route for the car */
	void CreateRoute(vector<glm::vec3> p);
	/* removes a set of points where overlapping occurs */
	void RemoveOverlabRoute(vector< glm::vec3>& segment);

	Mesh* mesh; /* car mesh */
	vector<glm::vec3> route; /* car route to be followed*/

	int counterF, counterS; /* counters for model matrix calculus */
	float rotation; /* car rotation */
	float dist; /* route distance to generate at */
};