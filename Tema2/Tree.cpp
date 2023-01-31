#include "Tree.h"

using namespace std;

Tree::Tree(vector<glm::vec3> pointsRoad, float dist)
{
    /* add posible colors for trees */
    posibleColorsLeafs.push_back(glm::vec3(0.3, 0.6, 0.1));
    posibleColorsLeafs.push_back(glm::vec3(0.625, 0.039, 0.87));
    posibleColorsLeafs.push_back(glm::vec3(0.7, 0.7, 0));
    posibleColorsLeafs.push_back(glm::vec3(0.9, 0.4, 0.6));
    posibleColorsLeafs.push_back(glm::vec3(0.9, 0.5, 0.1));

    posibleColorsTrunks.push_back(glm::vec3(0.7, 0.5, 0.1));
    posibleColorsTrunks.push_back(glm::vec3(0.9, 0.7, 0.4));
    posibleColorsTrunks.push_back(glm::vec3(0.36, 0.11, 0.06));

    /* creates trees on the left side and on the right side of the road */
    PlaceTrees(pointsRoad, dist);
    PlaceTrees(pointsRoad, -dist);
}

Tree::~Tree()
{
    for (int it = 0; it < meshes.size() - 1; it++) {
        delete meshes[it][0];
        delete meshes[it][1];
    }
}

void Tree::PlaceTrees(vector<glm::vec3> points, float dist) {
    /* create one tree for each given point at given distance */
    for (int it = 0; it < points.size() - 1; ) {
        /* generates posible tree position */
        glm::vec3 pos = getNewPoint(points[it], points[it + 1], dist);
        bool valid = true;
    
       /* check if another tree is too close we dont want 2 trees overlapping */ 
        for (int it_check = 0; it_check < positions.size(); it_check++) {
            if (distance(positions[it_check], pos) < 3) {
                valid = false;
                break;
            }
        }

        /* if position is valid, a tree is generated and it's position is added to the list */
        if (valid) {
            glm::vec3 colLeaf = GetRandomLeafColor();
            glm::vec3 colTrunk = GetRandomTrunkColor();

            std::vector<Mesh*> mesh = GetRandomTree(colLeaf, colTrunk);

            color_leaf.push_back(colLeaf);
            color_trunk.push_back(colTrunk);
            meshes.push_back(mesh);

            modelMatrixs.push_back(createModelMatrix(pos));
            positions.push_back(pos);
        }
        /* number of road points is way to big to creat a tree for each one
        we create one tree per 50 road points -> advance iterator so it does not get bigger than end*/
        if (it + 50 < points.size() - 1) {
            it += 50;
        }
        else {
            it = points.size() - 1;
        }
    }
}

glm::mat4 Tree::createModelMatrix(glm::vec3 position) {
    /* returns model matrix that positions object to given position */
	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix *= glm::translate(modelMatrix, position + glm::vec3(0, 1.5, 0));
	return modelMatrix;
}

vector<glm::vector<Mesh*>> Tree::GetMesh()
{
	return meshes;
}

vector<glm::mat4> Tree::GetModelMatrix()
{
	return modelMatrixs;
}


glm::vec3 Tree::getNewPoint(glm::vec3 p1, glm::vec3 p2, float d) {
	glm::vec3 D = glm::normalize(p2 - p1);
	glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
	return p1 + d * P;
}

std::vector<Mesh*> Tree::GetRandomTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk) {
    int type = std::rand() % 6;
   
    if (type == 0) {
        return CreateBasicTree(colorLeaf, colorTrunk);
    }
    if (type == 1) {
        return CreateTallTree(colorLeaf, colorTrunk);
    }
    if (type == 2) {
        return CreateHatTree(colorLeaf, colorTrunk);
    }
    if (type == 3) {
        return CreateHatTallTree(colorLeaf, colorTrunk);
    }
    if (type == 4) {
        return CreatePineTree(colorLeaf, colorTrunk);
    }
    if (type == 5) {
        return CreateShapedTree(colorLeaf, colorTrunk);
    }
}

glm::vec3 Tree::GetRandomLeafColor() {
    int index = std::rand() % posibleColorsLeafs.size();
    return posibleColorsLeafs[index];
}

glm::vec3 Tree::GetRandomTrunkColor() {
    int index = std::rand() % posibleColorsTrunks.size();
    return posibleColorsTrunks[index];
}

std::vector<Mesh*> Tree::CreateBasicTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk)
{
    /* created leafs mesh */
    std::vector<VertexFormat> vertices_l
    {
        VertexFormat(glm::vec3(-1, 0, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 2, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 0, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 2, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 0, -1), colorLeaf),
        VertexFormat(glm::vec3(-1, 2, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 0, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 2, -1), colorLeaf)
    };

    std::vector<unsigned int> indices_l =
    {
        0, 1, 2,
        1, 3, 2,
        0, 4, 1,
        1, 4 ,5,
        2, 3, 7,
        2, 7, 6,
        7, 5, 4,
        6, 7, 4,
        0, 2, 4,
        2, 6, 4,
        1, 5, 7,
        1, 7, 3
    };

    Mesh* leaf = new Mesh("leafs");
    leaf->InitFromData(vertices_l, indices_l);

    /* created leafs mesh */
    std::vector<VertexFormat> vertices_t
    {
        VertexFormat(glm::vec3(-0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 2, -0.25), colorTrunk)
    };

    std::vector<unsigned int> indices_t =
    {
        0, 1, 2,
        1, 3, 2,
        0, 4, 1,
        1, 4 ,5,
        2, 3, 7,
        2, 7, 6,
        7, 5, 4,
        6, 7, 4,
        0, 2, 4,
        2, 6, 4,
        1, 5, 7,
        1, 7, 3
    };

    Mesh* trunk = new Mesh("trunk");
    trunk->InitFromData(vertices_t, indices_t);

    std::vector<Mesh*> mesh;
    mesh.push_back(leaf);
    mesh.push_back(trunk);

    return mesh;

}

std::vector<Mesh*> Tree::CreateTallTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk)
{
    /* created leafs mesh */
    std::vector<VertexFormat> vertices_l
    {
        VertexFormat(glm::vec3(-1, 1, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 3, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 1, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 3, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 1, -1), colorLeaf),
        VertexFormat(glm::vec3(-1, 3, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 1, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 3, -1), colorLeaf)
    };

    std::vector<unsigned int> indices_l =
    {
        0, 1, 2,
        1, 3, 2,
        0, 4, 1,
        1, 4 ,5,
        2, 3, 7,
        2, 7, 6,
        7, 5, 4,
        6, 7, 4,
        0, 2, 4,
        2, 6, 4,
        1, 5, 7,
        1, 7, 3
    };

    Mesh* leaf = new Mesh("leafs");
    leaf->InitFromData(vertices_l, indices_l);

    /* created leafs mesh */
    std::vector<VertexFormat> vertices_t
    {
        VertexFormat(glm::vec3(-0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 3, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 3, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 3, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 3, -0.25), colorTrunk)
    };

    std::vector<unsigned int> indices_t =
    {
        0, 1, 2,
        1, 3, 2,
        0, 4, 1,
        1, 4 ,5,
        2, 3, 7,
        2, 7, 6,
        7, 5, 4,
        6, 7, 4,
        0, 2, 4,
        2, 6, 4,
        1, 5, 7,
        1, 7, 3
    };

    Mesh* trunk = new Mesh("trunk");
    trunk->InitFromData(vertices_t, indices_t);

    std::vector<Mesh*> mesh;
    mesh.push_back(leaf);
    mesh.push_back(trunk);

    return mesh;

}

std::vector<Mesh*> Tree::CreateHatTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk)
{
    /* created leafs mesh */
    std::vector<VertexFormat> vertices_l
    {
        VertexFormat(glm::vec3(-1, 0, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 1, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 0, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 1, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 0, -1), colorLeaf),
        VertexFormat(glm::vec3(-1, 1, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 0, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 1, -1), colorLeaf),


        VertexFormat(glm::vec3(-0.5, 1, 0.5), colorLeaf),
        VertexFormat(glm::vec3(-0.5, 2, 0.5), colorLeaf),
        VertexFormat(glm::vec3(0.5, 1, 0.5), colorLeaf),
        VertexFormat(glm::vec3(0.5 , 2, 0.5), colorLeaf),
        VertexFormat(glm::vec3(-0.5, 1, -0.5), colorLeaf),
        VertexFormat(glm::vec3(-0.5, 2, -0.5), colorLeaf),
        VertexFormat(glm::vec3(0.5, 1, -0.5), colorLeaf),
        VertexFormat(glm::vec3(0.5, 2, -0.5), colorLeaf)
    };

    std::vector<unsigned int> indices_l =
    {
        0, 1, 2,
        1, 3, 2,
        0, 4, 1,
        1, 4 ,5,
        2, 3, 7,
        2, 7, 6,
        7, 5, 4,
        6, 7, 4,
        0, 2, 4,
        2, 6, 4,
        1, 5, 7,
        1, 7, 3,

        8, 9, 10,
        9, 11, 10,
        8, 12, 9,
        9, 12, 13,
        10, 11, 15,
        10, 15, 14,
        15, 13, 12,
        14, 15, 12,
        8, 10, 12,
        10, 14, 12,
        9, 13, 15,
        9, 15, 11
    };

    Mesh* leaf = new Mesh("leafs");
    leaf->InitFromData(vertices_l, indices_l);

    /* created leafs mesh */
    std::vector<VertexFormat> vertices_t
    {
        VertexFormat(glm::vec3(-0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 1, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 1, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 1, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 1, -0.25), colorTrunk)
    };

    std::vector<unsigned int> indices_t =
    {
        0, 1, 2,
        1, 3, 2,
        0, 4, 1,
        1, 4 ,5,
        2, 3, 7,
        2, 7, 6,
        7, 5, 4,
        6, 7, 4,
        0, 2, 4,
        2, 6, 4,
        1, 5, 7,
        1, 7, 3
    };

    Mesh* trunk = new Mesh("trunk");
    trunk->InitFromData(vertices_t, indices_t);

    std::vector<Mesh*> mesh;
    mesh.push_back(leaf);
    mesh.push_back(trunk);

    return mesh;

}

std::vector<Mesh*> Tree::CreateHatTallTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk)
{
    /* created leafs mesh */
    std::vector<VertexFormat> vertices_l
    {
        VertexFormat(glm::vec3(-1, 1, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 2, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 1, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 2, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 1, -1), colorLeaf),
        VertexFormat(glm::vec3(-1, 2, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 1, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 2, -1), colorLeaf),


        VertexFormat(glm::vec3(-0.5, 2, 0.5), colorLeaf),
        VertexFormat(glm::vec3(-0.5, 3, 0.5), colorLeaf),
        VertexFormat(glm::vec3(0.5, 2, 0.5), colorLeaf),
        VertexFormat(glm::vec3(0.5 , 3, 0.5), colorLeaf),
        VertexFormat(glm::vec3(-0.5, 2, -0.5), colorLeaf),
        VertexFormat(glm::vec3(-0.5, 3, -0.5), colorLeaf),
        VertexFormat(glm::vec3(0.5, 2, -0.5), colorLeaf),
        VertexFormat(glm::vec3(0.5, 3, -0.5), colorLeaf)
    };

    std::vector<unsigned int> indices_l =
    {
        0, 1, 2,
        1, 3, 2,
        0, 4, 1,
        1, 4 ,5,
        2, 3, 7,
        2, 7, 6,
        7, 5, 4,
        6, 7, 4,
        0, 2, 4,
        2, 6, 4,
        1, 5, 7,
        1, 7, 3,

        8, 9, 10,
        9, 11, 10,
        8, 12, 9,
        9, 12, 13,
        10, 11, 15,
        10, 15, 14,
        15, 13, 12,
        14, 15, 12,
        8, 10, 12,
        10, 14, 12,
        9, 13, 15,
        9, 15, 11
    };

    Mesh* leaf = new Mesh("leafs");
    leaf->InitFromData(vertices_l, indices_l);

    /* created leafs mesh */
    std::vector<VertexFormat> vertices_t
    {
        VertexFormat(glm::vec3(-0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 2, -0.25), colorTrunk)
    };

    std::vector<unsigned int> indices_t =
    {
        0, 1, 2,
        1, 3, 2,
        0, 4, 1,
        1, 4 ,5,
        2, 3, 7,
        2, 7, 6,
        7, 5, 4,
        6, 7, 4,
        0, 2, 4,
        2, 6, 4,
        1, 5, 7,
        1, 7, 3
    };

    Mesh* trunk = new Mesh("trunk");
    trunk->InitFromData(vertices_t, indices_t);

    std::vector<Mesh*> mesh;
    mesh.push_back(leaf);
    mesh.push_back(trunk);

    return mesh;

}

std::vector<Mesh*> Tree::CreatePineTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk)
{
    /* created leafs mesh */
    std::vector<VertexFormat> vertices_l
    {
        VertexFormat(glm::vec3(0, 2, 0), colorLeaf),
        VertexFormat(glm::vec3(-1, 0, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 0, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 0, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 0, -1), colorLeaf),


        VertexFormat(glm::vec3(0, 3, 0), colorLeaf),
        VertexFormat(glm::vec3(-0.5, 1.6, 0.5), colorLeaf),
        VertexFormat(glm::vec3(0.5, 1.6, 0.5), colorLeaf),
        VertexFormat(glm::vec3(-0.5, 1.6, -0.5), colorLeaf),
        VertexFormat(glm::vec3(0.5, 1.6, -0.5), colorLeaf)
    };

    std::vector<unsigned int> indices_l =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        1, 2, 3,
        2, 3, 4,

        5, 6, 7,
        5, 7, 8,
        5, 8, 9,
        5, 9, 6,
        6, 7, 8,
        7, 8, 9
    };

    Mesh* leaf = new Mesh("leafs");
    leaf->InitFromData(vertices_l, indices_l);

    /* created leafs mesh */
    std::vector<VertexFormat> vertices_t
    {
        VertexFormat(glm::vec3(-0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 0, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 0, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 0, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 0, -0.25), colorTrunk)
    };

    std::vector<unsigned int> indices_t =
    {
        0, 1, 2,
        1, 3, 2,
        0, 4, 1,
        1, 4 ,5,
        2, 3, 7,
        2, 7, 6,
        7, 5, 4,
        6, 7, 4,
        0, 2, 4,
        2, 6, 4,
        1, 5, 7,
        1, 7, 3
    };

    Mesh* trunk = new Mesh("trunk");
    trunk->InitFromData(vertices_t, indices_t);

    std::vector<Mesh*> mesh;
    mesh.push_back(leaf);
    mesh.push_back(trunk);

    return mesh;

}

std::vector<Mesh*> Tree::CreateShapedTree(glm::vec3 colorLeaf, glm::vec3 colorTrunk)
{
    /* created leafs mesh */
    std::vector<VertexFormat> vertices_l
    {
        VertexFormat(glm::vec3(0, 0, 0), colorLeaf),
        VertexFormat(glm::vec3(-1, 2, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 2, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 2, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 2, -1), colorLeaf),

        VertexFormat(glm::vec3(0, 2, 0), colorLeaf),
        VertexFormat(glm::vec3(-1, 0, 1), colorLeaf),
        VertexFormat(glm::vec3(1, 0, 1), colorLeaf),
        VertexFormat(glm::vec3(-1, 0, -1), colorLeaf),
        VertexFormat(glm::vec3(1, 0, -1), colorLeaf)
    };

    std::vector<unsigned int> indices_l =
    {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        1, 2, 3,
        2, 3, 4,

        5, 6, 7,
        5, 7, 8,
        5, 8, 9,
        5, 9, 6,
        6, 7, 8,
        7, 8, 9
    };

    Mesh* leaf = new Mesh("leafs");
    leaf->InitFromData(vertices_l, indices_l);

    /* created leafs mesh */
    std::vector<VertexFormat> vertices_t
    {
        VertexFormat(glm::vec3(-0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 0, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, 0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 0, 0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(-0.25, 0, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, -2, -0.25), colorTrunk),
        VertexFormat(glm::vec3(0.25, 0, -0.25), colorTrunk)
    };

    std::vector<unsigned int> indices_t =
    {
        0, 1, 2,
        1, 3, 2,
        0, 4, 1,
        1, 4 ,5,
        2, 3, 7,
        2, 7, 6,
        7, 5, 4,
        6, 7, 4,
        0, 2, 4,
        2, 6, 4,
        1, 5, 7,
        1, 7, 3
    };

    Mesh* trunk = new Mesh("trunk");
    trunk->InitFromData(vertices_t, indices_t);

    std::vector<Mesh*> mesh;
    mesh.push_back(leaf);
    mesh.push_back(trunk);

    return mesh;

}