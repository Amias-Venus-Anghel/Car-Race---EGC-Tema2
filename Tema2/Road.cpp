#include "Road.h"

using namespace std;

Road::Road(vector<glm::vec3> points, float distance):
	color(glm::vec3(0, 0, 0))
{
	/* Create the mesh for the road */
	mesh = CreateRoad(points, distance);
}

Road::~Road()
{
	delete mesh;
}

glm::vec3 Road::PointAtDistance(glm::vec3 p1, glm::vec3 p2, float d) {
	/* given 2 points A and B finds the point C at distance d as such that AC perpedicular on AB */
	glm::vec3 D = glm::normalize(p2 - p1);
	glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
	return p1 + d * P;
}

vector<glm::vec3> Road::generateOnStraightLine(glm::vec3 p1, glm::vec3 p2) {
	/* generate points on line from p1 to p2 formula from: 
	https://math.stackexchange.com/questions/175896/finding-a-point-along-a-line-a-certain-distance-away-from-another-point */
	vector<glm::vec3> points;
	glm::vec3 v = p2 - p1;
	glm::vec3 u = glm::normalize(v);
	float d = glm::distance(p2, p1);
	for (float i = 0.1; i < d; i += 0.1) {
		glm::vec3 newP = p1 + i * u;
		points.push_back(newP);
	}

	return points;
}


void Road::createSegment(glm::vec3 p1, glm::vec3 p2, int& i, float d, std::vector<VertexFormat>& vertices, std::vector<unsigned int>& indices) {
	/* generates and adds to given vectors the corespoinding values for a mesh segment between p1 and p2 */
	vector< glm::vec3> points = generateOnStraightLine(p1, p2);

	/* after generating a line between p1 and p2, generates for each point an interior and exterior point
	for the mesh */
	for (int it = 0; it < points.size() - 1; it++) {

		glm::vec3 inPoint = PointAtDistance(points[it], points[it + 1], d);
		glm::vec3 outPoint = PointAtDistance(points[it], points[it + 1], -d);

		vertices.push_back(VertexFormat(inPoint, color));
		vertices.push_back(VertexFormat(outPoint, color));
	}

	/* adds coresponding vertices
	i is given as parameter so no triangle is added twice*/
	for (; i < vertices.size() - 1; i++) {

		indices.push_back(i - 1);
		indices.push_back(i);
		indices.push_back(i + 1);
	}

	/* add the initial generated point to the center points list */
	pointsCenter.insert(pointsCenter.end(), points.begin(), points.end());
}

Mesh* Road::CreateRoad(vector<glm::vec3> p, float d)
{
	/* generates the vertices and indices and creates the mesh */
	std::vector<VertexFormat> vertices = {};
	std::vector<unsigned int> indices = {};
	int i = 1;

	/* for each pair if points adds a mesh segment */
	for (int it = 0; it < p.size() - 1; it++) {
		createSegment(p[it], p[it + 1], i, d, vertices, indices);
	}

	/* creates a mesh segment between last and first points so the road conects itsef */
	createSegment(p[p.size() - 1], p[0], i, d, vertices, indices);

	/* conects last remaining points */
	indices.push_back(vertices.size() - 2);
	indices.push_back(vertices.size() - 1);
	indices.push_back(0);

	indices.push_back(vertices.size() - 1);
	indices.push_back(0);
	indices.push_back(1);

	/* creates and returns mesh */
	Mesh* road = new Mesh("road");

	road->SetDrawMode(GL_TRIANGLES);
	road->InitFromData(vertices, indices);
	return road;
	return road;
}
