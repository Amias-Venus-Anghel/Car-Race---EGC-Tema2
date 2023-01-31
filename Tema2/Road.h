#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <string>
#include <iostream>

class Road
{
public:
	/* road gets a list of points and a distance and builds the mesh  using points situated ar the specified
	distance from the center, given by the points */
	Road(std::vector<glm::vec3> points, float distance); 
	~Road();
	
	/* return road mesh for rendering */
	Mesh* mesh;

	/* lists of interior and exterior points of the road */
	std::vector <glm::vec3> pointsCenter;
	glm::vec3 color;

private:
	/* creates the road mesh */
	Mesh* CreateRoad(std:: vector<glm::vec3> points, float distance);
	/* generates points between 2 given points in a straight line */
	std::vector<glm::vec3>  generateOnStraightLine(glm::vec3 p1, glm::vec3 p2);
	/* given 2 points A and B finds the point C at distance d as such that AC perpedicular on AB */
	glm::vec3 PointAtDistance(glm::vec3 p1, glm::vec3 p2, float d);
	/* for 2 points generates a mesh portion */
	void createSegment(glm::vec3 p1, glm::vec3 p2, int& i, float d, std::vector<VertexFormat>& vertices, std::vector<unsigned int>& indices);
};