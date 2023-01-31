#include "simpleobject3D.h"

#include "core/engine.h"
#include "utils/gl_utils.h"

Mesh* simpleobject3D::CreateSquare(const std::string& name, float length, glm::vec3 color, bool fill)
{
    glm::vec3 corner = glm::vec3(0, 0, 0);;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color) ,
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, 0, length), color),
        VertexFormat(corner + glm::vec3(0, 0, length), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    }
    else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* simpleobject3D::CreateCar(const std::string& name, glm::vec3 color)
{
    std::vector<VertexFormat> vertices
    {
        /* up base up */
        VertexFormat(glm::vec3(-0.5, 0.5,  0.6), color),
        VertexFormat(glm::vec3(0.5, 0.5,  0.6), color),
        VertexFormat(glm::vec3(-0.5, 0.5,  -0.85), color),
        VertexFormat(glm::vec3(0.5, 0.5,  -0.85), color),

        /* down base up */
        VertexFormat(glm::vec3(-0.5, 0.15,  0.65), color),
        VertexFormat(glm::vec3(0.5, 0.15,  0.65), color),
        VertexFormat(glm::vec3(-0.5, 0.15,  -0.75), color),
        VertexFormat(glm::vec3(0.5, 0.15,  -0.75), color),

        /* up base down */
        VertexFormat(glm::vec3(-0.5, 0.15,  1), color),
        VertexFormat(glm::vec3(0.5, 0.15,  1), color),
        VertexFormat(glm::vec3(-0.5, 0.15,  -1), color),
        VertexFormat(glm::vec3(0.5, 0.15,  -1), color),

        /* down base down */
        VertexFormat(glm::vec3(-0.5, -0.5,  1), color),
        VertexFormat(glm::vec3(0.5, -0.5,  1), color),
        VertexFormat(glm::vec3(-0.5, -0.5,  -1), color),
        VertexFormat(glm::vec3(0.5, -0.5,  -1), color)
    };

    std::vector<unsigned int> indices =
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

    Mesh* mesh = new Mesh(name);

    mesh->InitFromData(vertices, indices);

    return mesh;

}

std::vector<Mesh*> simpleobject3D::CreateStartGate(const std::string& name, glm::vec3 mainColor, glm::vec3 secundarColor) {
    std::vector<Mesh*> meshes;
    std::vector<VertexFormat> vertices 
    {
        VertexFormat(glm::vec3(-1, 0,  1), mainColor),
        VertexFormat(glm::vec3(-1, 7,  1), mainColor),
        VertexFormat(glm::vec3(1, 0,  1), mainColor),
        VertexFormat(glm::vec3(1, 7,  1), mainColor),
        VertexFormat(glm::vec3(-1, 0,  -1), mainColor),
        VertexFormat(glm::vec3(-1, 7,  -1), mainColor),
        VertexFormat(glm::vec3(1, 0,  -1), mainColor),
        VertexFormat(glm::vec3(1, 7,  -1), mainColor),

        VertexFormat(glm::vec3(-1, 6,  1), mainColor),
        VertexFormat(glm::vec3(-1, 7,  1), mainColor),
        VertexFormat(glm::vec3(15, 6,  1), mainColor),
        VertexFormat(glm::vec3(15, 7,  1), mainColor),
        VertexFormat(glm::vec3(-1, 6,  -1), mainColor),
        VertexFormat(glm::vec3(-1, 7,  -1), mainColor),
        VertexFormat(glm::vec3(15, 6,  -1), mainColor),
        VertexFormat(glm::vec3(15, 7,  -1), mainColor),


        VertexFormat(glm::vec3(13, 0,  1), mainColor),
        VertexFormat(glm::vec3(13, 7,  1), mainColor),
        VertexFormat(glm::vec3(15, 0,  1), mainColor),
        VertexFormat(glm::vec3(15, 7,  1), mainColor),
        VertexFormat(glm::vec3(13, 0,  -1), mainColor),
        VertexFormat(glm::vec3(13, 7,  -1), mainColor),
        VertexFormat(glm::vec3(15, 0,  -1), mainColor),
        VertexFormat(glm::vec3(15, 7,  -1), mainColor)
    };

    std::vector<unsigned int> indices =
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
        9, 12 ,13,
        10, 11, 15,
        10, 15, 14,
        15, 13, 12,
        14, 15, 12,
        8, 10, 12,
        10, 14, 12,
        9, 13, 15,
        9, 15, 11,

        16, 17, 18,
        17, 19, 18,
        16, 20, 17,
        17, 20 ,21,
        18, 19, 23,
        18, 23, 22,
        23, 21, 20,
        22, 23, 20,
        16, 18, 20,
        18, 22, 20,
        17, 21, 23,
        17, 23, 19
    };

    Mesh* mesh = new Mesh(name);
    mesh->InitFromData(vertices, indices);
    meshes.push_back(mesh);
    
    std::vector<VertexFormat> vertices2
    {
        VertexFormat(glm::vec3(0, 3,  1.1), secundarColor),
        VertexFormat(glm::vec3(0, 2,  1.1), secundarColor),
        VertexFormat(glm::vec3(-0.7, 1,  1.1), secundarColor),
        VertexFormat(glm::vec3(0.7, 1,  1.1), secundarColor),
       
        VertexFormat(glm::vec3(0, 6,  1.1), secundarColor),
        VertexFormat(glm::vec3(0, 5,  1.1), secundarColor),
        VertexFormat(glm::vec3(-0.7, 4,  1.1), secundarColor),
        VertexFormat(glm::vec3(0.7, 4,  1.1), secundarColor),

        VertexFormat(glm::vec3(14, 3,  1.1), secundarColor),
        VertexFormat(glm::vec3(14, 2,  1.1), secundarColor),
        VertexFormat(glm::vec3(13.3, 1,  1.1), secundarColor),
        VertexFormat(glm::vec3(14.7, 1,  1.1), secundarColor),

        VertexFormat(glm::vec3(14, 6,  1.1), secundarColor),
        VertexFormat(glm::vec3(14, 5,  1.1), secundarColor),
        VertexFormat(glm::vec3(13.3, 4,  1.1), secundarColor),
        VertexFormat(glm::vec3(14.7, 4,  1.1), secundarColor)
    };

    std::vector<unsigned int> indices2 =
    {
        0, 1, 2,
        0, 3, 1,

        4, 5, 6,
        4, 7, 5,

        8, 9, 10,
        8, 11, 9,

        12, 13, 14,
        12, 15, 13,
    };

    Mesh* mesh2 = new Mesh("sec");
    mesh2->InitFromData(vertices2, indices2);
    meshes.push_back(mesh2);

    return meshes;
}