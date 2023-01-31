#include "EnemyCar.h"


EnemyCar::EnemyCar(vector<glm::vec3> movePoints,  float moveDist, glm::vec3 color_) :
	dist(moveDist), rotation(0), color(color_)
{
	/* Create the mesh for the road*/
	mesh = simpleobject3D::CreateCar("enemy", color_);
	/* creates route for car */
	CreateRoute(movePoints);
	/* counterF (first) is set to 1 and counterS (second) is set to 0 for model matrix calculus */
	counterS = (rand() % (route.size() - 3));
	counterF = counterS + 1;
}

EnemyCar::~EnemyCar()
{
	delete mesh;
}

Mesh* EnemyCar::GetMesh()
{
	return mesh;
}

glm::mat4 EnemyCar::GetModelMatrix() {
	glm::mat4 modelMatrix = glm::mat4(1);
	
	/* if one of the countes is too big, reset it */
	if (counterF >= route.size()) {
		counterF = 0;
	}
	if (counterS >= route.size()) {
		counterS = 0;
	}
	/* get points on positions given by first and second counters */
	glm::vec3 point1 = route[counterF];
	glm::vec3 point2 = route[counterS];
	
	/* increment counters */
	counterF++;
	counterS++;

	/* calculate direction vector using the 2 points */
	glm::vec3 dirVec = point2 - point1;
	/* calculate car rotation using direction vector */
	rotation = atan(dirVec.x / dirVec.z); 
	position = glm::vec3(point1.x, 0.38, point1.z);
	
	/* calculate model matrix
	car moves following route points, the route contains enough points for a smoth movement */
	modelMatrix = glm::translate(modelMatrix, position); /* also translate car up a bit */
	modelMatrix = glm::rotate(modelMatrix, rotation, glm::vec3(0, 1, 0));
	//modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));

	return modelMatrix;
}

glm::vec3 EnemyCar::getNewPoint(glm::vec3 p1, glm::vec3 p2, float d) {
	/* given 2 points A and B finds the point C at distance d as such that AC perpedicular on AB */
	glm::vec3 D = glm::normalize(p2 - p1);
	glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
	return p1 + d * P;
}

vector<glm::vec3> EnemyCar::generateOnStraightLine(glm::vec3 p1, glm::vec3 p2) {
	/* generate points on line from p1 to p2 */
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

vector<glm::vec3> EnemyCar::createSegment(glm::vec3 p1, glm::vec3 p2) {
	/* generates a route segment and returns it */
	/* generate points on straight line from p1 to p2 */
	vector< glm::vec3> points = generateOnStraightLine(p1, p2);
	vector< glm::vec3> segment;
	
	/* for each point calculate the route point at given ditance */
	for (int it = 0; it < points.size() - 1; it++) {
		glm::vec3 point = getNewPoint(points[it], points[it + 1], dist);
		segment.push_back(point);
	}
	return segment;
}

void EnemyCar::RemoveOverlabRoute(vector<glm::vec3>& segment) {
	/* in case of overlap remove points from route and segment */
	
	float dot = -1;
	while (dot < 0 && segment.size() > 0) {
		/* remove last point of route */
		route.pop_back();
		/* remove first point of segment */
		if (segment.size() > 1) {
			vector< glm::vec3>::iterator it = next(segment.begin());
			segment.erase(segment.begin(), it);
		}
		else {
			/* if segment has only one point left */
			segment.pop_back();
			break;
		}

		/* check next pair of points for overlap */
		glm::vec3 p1 = *prev(route.end(), 2) - *prev(route.end());
		glm::vec3 p2 = *prev(route.end()) - *segment.begin();
		dot = glm::dot(p1, p2);
	}
	
}

void EnemyCar::CreateRoute(vector<glm::vec3> p)
{
	/* given referance point, creates a route for car */;
	/* for each 2 points generates a route segment */
	for (int it = 0; it < p.size() - 1; it++) {

		vector<glm::vec3> segment = createSegment(p[it], p[it + 1]);
		float dot = 0; /* used to check overlap */
		/* unless itterator it_p is at the begining check if intermediar segment is needed
		or if overlap occurs */
		if (it != 0) {
			/* check if overlap occurs
			context: we check if direction given by last 2 points of route and the direction given by
			the last point of route and the first point of the segment are the same. if directions corespond
			there is no overlap but an intermediar segment is needed, if the directions are oposite then
			an overlap occurs and there is no need for an intermediar segment 
			sourse: https://stackoverflow.com/questions/49535295/how-to-check-if-vectors-are-facing-same-direction*/
			glm::vec3 p1 = *prev(route.end(), 2) - *prev(route.end());
			glm::vec3 p2 = *prev(route.end()) - *segment.begin();
			dot = glm::dot(p1, p2);
			if (dot < 0) {
				RemoveOverlabRoute(segment);
			}
			
			if (segment.size() > 0) {
				/* generate intermediar segment and add it to the route */
				vector<glm::vec3> intermediar = generateOnStraightLine(*prev(route.end()), *segment.begin());
				route.insert(route.end(), intermediar.begin(), intermediar.end());
			}	
		}
		
		if (!segment.empty()) {
			/* if after the overlap removal there are any points left in the segment, they are added to the route */
			route.insert(route.end(), segment.begin(), segment.end());
		}
	}

	/* finish route by connecting the last and first points in the same manner as before */
	vector<glm::vec3> segment = createSegment(*prev(p.end()), *p.begin());
	
	glm::vec3 p1 = *prev(route.end(), 2) - *prev(route.end());
	glm::vec3 p2 = *prev(route.end()) - *segment.begin();

	float dot = glm::dot(p1, p2);
	if (dot < 0) {
		RemoveOverlabRoute(segment);
	}
	
	if (segment.size() > 0) {
		/* generate intermediar segment and add it to the route */
		vector<glm::vec3> intermediar = generateOnStraightLine(*prev(route.end()), *segment.begin());
		route.insert(route.end(), intermediar.begin(), intermediar.end());
	}

	if (!segment.empty()) {
		route.insert(route.end(), segment.begin(), segment.end());

		vector<glm::vec3> intermediar = generateOnStraightLine(*prev(route.end()), *route.begin());
		route.insert(route.end(), intermediar.begin(), intermediar.end());
	}

}
