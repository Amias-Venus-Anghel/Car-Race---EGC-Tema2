#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <string>
#include <iostream>

class Tree
{
public:
	/* generates tree meshes and their positions on map given the road exterior and interior points 
	and a distance */
	Tree(std::vector<glm::vec3> pointsRoad, float dist);
	~Tree();

	/* returns list of meshes of trees */
	std::vector<glm::vector<Mesh*>> GetMesh();
	/* return list of model matrixes fro trees, given their position on map */
	std::vector<glm::mat4> GetModelMatrix();

	std::vector<glm::vec3> color_leaf;
	std::vector<glm::vec3> color_trunk;

private:
	/* generates model matrix for given position */
	glm::mat4 createModelMatrix(glm::vec3 position);
	/* given 2 points A and B finds the point C at distance d as such that AC perpedicular on AB */
	glm::vec3 getNewPoint(glm::vec3 p1, glm::vec3 p2, float d);
	/* choses and returns a random tree */
	std::vector<Mesh*> GetRandomTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk);
	/* returns random color (from a preset color vector) */
	glm::vec3 GetRandomLeafColor();
	glm::vec3 GetRandomTrunkColor();

	/* creates and returns the mesh for one singular tree */
	std::vector<Mesh*> CreateBasicTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk);
	std::vector<Mesh*> CreateTallTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk);
	std::vector<Mesh*> CreateHatTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk);
	std::vector<Mesh*> CreateHatTallTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk);
	std::vector<Mesh*> CreatePineTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk);
	std::vector<Mesh*> CreateShapedTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk);

	/* calculates trees position */
	void PlaceTrees(std::vector<glm::vec3> points, float dist);

	/* variables to store trees info */
	std::vector<glm::vector<Mesh*>> meshes;
	std::vector<glm::mat4> modelMatrixs;
	std::vector<glm::vec3> positions;

	std::vector<glm::vec3> posibleColorsLeafs;
	std::vector<glm::vec3> posibleColorsTrunks;
};