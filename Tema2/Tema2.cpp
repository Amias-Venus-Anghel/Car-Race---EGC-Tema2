#include "Tema2.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{ 
   
    /* Sets the resolution of the small viewport */
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    /* text */
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 18);

    /* time */
    bestTime = -1; 
    lastTime = -1;
    startTime = 0;

    showControls = false;
 
    /* set cameras positions */
    cameraCar = new implementedTema::Camera();
    cameraCar->Set(glm::vec3(1, 3, 1), glm::vec3(0, 2.3, 0), glm::vec3(0, 1, 0));

    cameraMiniMap = new implementedTema::Camera();
    cameraMiniMap->Set(glm::vec3(0, 4, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
   
    /* set projection matrixes for cameras */
    /* use window ratio to scale corespondently the ortho prokection */
    float ratio = (float)resolution.y / resolution.x;
    float dist = 60;

    zNear = 0.01f;
    zFar = 200;
    ortholeft = -dist;
    orthoright = dist;
    orthobottom = -ratio * dist;  
    orthotop = ratio * dist;

    projectionMatrixC = glm::perspective(RADIANS(60), window->props.aspectRatio, zNear, zFar);
    projectionMatrixM = glm::ortho(ortholeft, orthoright, orthobottom, orthotop, zNear, zFar);
    

    /* set width of road */
    roadWidth = 5.f;

    /* create road */
    points = new RoadPoints::Points();
    vector<glm::vec3> points_ = points->GetPoints();
    road = new Road(points_, roadWidth);

    /* create trees */
    trees = new Tree(road->pointsCenter, roadWidth + 3);

    /* create enemy 1 to 10 enemys cars */
    nrEnemies = std::rand() % 41;

    for (int it = 0; it < nrEnemies; it++) {
        /* get a random dist from center of road, make distance negative if itterator is even */
        float enemyDist = 0.5 + std::rand() % (int)roadWidth;
        if (it % 2 == 0) {
            enemyDist *= -1;
        }
        /* get random color */
        glm::vec3 color = glm::vec3(std::rand()%100 * 0.01, std::rand() % 100 * 0.01, std::rand() % 100 * 0.01);
        enemies.push_back(new EnemyCar(points_, enemyDist, color));
    }

    /* create additional meshes */
    {
        car_color = glm::vec3(1, 1, 0.3);
        Mesh* car = simpleobject3D::CreateCar("car", car_color);
        AddMeshToList(car);
    }
    {
        grass_color = glm::vec3(0, 0.7, 0.3);
        Mesh* plane = new Mesh("plane");
        plane->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        AddMeshToList(plane);

        Mesh* grassMiniMap = simpleobject3D::CreateSquare("grass", 4000, grass_color, true);
        AddMeshToList(grassMiniMap);
    }
    {
        gate_main_color = glm::vec3(1, 1, 1);
        gate_sec_color = glm::vec3(1, 0, 0);
        gate = simpleobject3D::CreateStartGate("gate", gate_main_color, gate_sec_color);
    }

    /* create shader */
    {
        Shader* shader = new Shader("ShaderT2");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShaderTema2.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShaderTema2.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    /* initializations */
    minSpeed = 20.f;
    maxSpeed = 50.f;
    speed = minSpeed;
    rotation = 0;
    rotationSensibility = 2;
    collision = false;

}

void Tema2::DrawHUD()
{
    const float kTopY = 10.f;
    const float kRowHeight = 25.f;

    int rowIndex = 0;
    string bestT = "";
    string lastT = "";
    string currT = "";
    if (bestTime > 0) {
        bestT = std::to_string(bestTime);
    }
    if (lastTime > 0) {
        lastT = std::to_string(lastTime);
    }
    if (startTime != 0) {
        currT = std::to_string(glfwGetTime() - startTime);
    }

    if (!showControls) {
        textRenderer->RenderText("Time: " + currT, 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("BEST TIME: " + bestT, 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("LAST TIME: " + lastT, 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("current tour stated at: " + std::to_string(startTime), 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);

        textRenderer->RenderText("  There are currently: " + std::to_string(nrEnemies) + " enemy cars on the road", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("  Car speed: " + std::to_string(speed), 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("  Press P to see car controls", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    }
    else {
        textRenderer->RenderText("  W = move forward", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("  S = move backward", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("  A & D = rotate", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("  I = accelerate", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("  K = dezaccelerate", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
        textRenderer->RenderText("  P = toggle controls", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, kTextColor);
    }
    
    if (collision) {
        textRenderer->RenderText("!!YOU HAVE COLLIDED WITH AN ENEMY CAR!!", 5.0f, kTopY + kRowHeight * rowIndex++, 1.0f, glm::vec3(1, 0, 0));
    }

}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0.4, 0.8, 1); /* set sky color */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::RenderScene(float deltaTimeSeconds) {
    /* renders scene in the main window (big view) */
    { 
        /* render player car */
        glm::mat4 modelMatrix = glm::mat4(1);
        /* rededer player car at main camera target so player is always in front of camera */
        modelMatrix = glm::translate(modelMatrix, cameraCar->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, rotation + RADIANS(45), glm::vec3(0, 1, 0));
        car_position = modelMatrix;
        RenderMeshMain(meshes["car"], shaders["ShaderT2"], modelMatrix, car_color);
    }
    {
        /* render enemy cars */
        for (int it = 0; it < nrEnemies; it++) {
            RenderMeshMain(enemies[it]->GetMesh(), shaders["ShaderT2"], enemies[it]->GetModelMatrix(), enemies[it]->color);
        }
    }
    {
        /* render road */
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMeshMain(road->mesh, shaders["ShaderT2"], modelMatrix, road->color);
    }
    {
        /* render gate */
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-9.5, -0.5, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45), glm::vec3(0, 1, 0));
        RenderMeshMain(gate[0], shaders["ShaderT2"], modelMatrix, gate_main_color);
        RenderMeshMain(gate[1], shaders["ShaderT2"], modelMatrix, gate_sec_color);
    }
    {
        /* render grass */
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = car_position;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.5, 0));
        RenderMeshMain(meshes["plane"], shaders["ShaderT2"], modelMatrix, grass_color);
    }
    {
        /* render trees */
        vector<vector<Mesh*>> tree = trees->GetMesh();
        vector<glm::mat4> modelMat = trees->GetModelMatrix();
        vector<glm::vec3> colors_leafs = trees->color_leaf;
        vector<glm::vec3> colors_trunks = trees->color_trunk;

        for (int it = 0; it < tree.size(); it++) {
            RenderMeshMain(tree[it][0], shaders["ShaderT2"], modelMat[it], colors_leafs[it]);
            RenderMeshMain(tree[it][1], shaders["ShaderT2"], modelMat[it], colors_trunks[it]);
        }
    }
}

void Tema2::RenderSceneMiniMap(float deltaTimeSeconds) {
    /* render scene for minimap window */
    {
        /* render player car */
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, cameraCar->GetTargetPosition());
        modelMatrix = glm::rotate(modelMatrix, rotation + RADIANS(45), glm::vec3(0, 1, 0));
        RenderMeshMiniMap(meshes["car"], shaders["VertexColor"], modelMatrix);
    }
    {
        /* render enemy cars */
        for (int it = 0; it < nrEnemies; it++) {
            RenderMeshMiniMap(enemies[it]->GetMesh(), shaders["VertexColor"], enemies[it]->GetModelMatrix());
        }
    }
    {
        /* render road */
        glm::mat4 modelMatrix = glm::mat4(1);
        RenderMeshMiniMap(road->mesh, shaders["VertexColor"], modelMatrix);
    }
    {
        /* render grass */
        glm::mat4 modelMatrix = car_position;
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-2000, -0.5, -2000));
        RenderMeshMiniMap(meshes["grass"], shaders["VertexColor"], modelMatrix);
    }
    {
        /* render gate */
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(-9.5, -0.5, 0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(45), glm::vec3(0, 1, 0));
        RenderMeshMiniMap(gate[0], shaders["VertexColor"], modelMatrix);
    }
    {
        {
            /* render trees */
            vector<vector<Mesh*>> tree = trees->GetMesh();
            vector<glm::mat4> modelMat = trees->GetModelMatrix();

            for (int it = 0; it < tree.size(); it++) {
                RenderMeshMiniMap(tree[it][0], shaders["VertexColor"], modelMat[it]);
            }
        }
    }
 
}


void Tema2::Update(float deltaTimeSeconds)
{
    /* first render scene in main window */
    RenderScene(deltaTimeSeconds);
    DrawHUD();
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);

    /* position minimap camera at main camera's coordonates */
    cameraMiniMap->position.x = cameraCar->position.x;
    cameraMiniMap->position.z = cameraCar->position.z;
    /* render scene in minimap window using minimap camera */
    RenderSceneMiniMap(deltaTimeSeconds);

    /* check for car collision with enemy cars; if collision occurs, reset speed */
    if (CheckCollisionEnemy(cameraCar->GetTargetPosition())) {
        speed = minSpeed;
        collision = true;
        /* to make collision more obvios, give player a penality time of waiting */
        timeout = glfwGetTime() + 0.3;
    }
    if (glfwGetTime() >= timeout) {
        collision = false;
    }

    if (points->CheckPosition(cameraCar->GetTargetPosition())) {
        
        if (lastTime != -1) {
            lastTime = glfwGetTime() - startTime;
            if (bestTime == -1) {
                /* first tour, only time is best time */
                bestTime = lastTime;
            }
            else if (lastTime < bestTime) {
                bestTime = lastTime;
            }
        } 
        else {
            /* this is first tour, dont register this time */
            lastTime = -2;
        }
        
        startTime = glfwGetTime();
    }

}


void Tema2::FrameEnd()
{
    
}

bool Tema2::CheckCarInRoad(glm::vec3 position) {
    /* check if position is inside road */
    bool inRange = false;
    /* using the center points of the road, check if there is any center point such that distance to position
    is less than the distance from center to the distance to a side of the road */
    float dist = roadWidth - 0.3; /* make accepted distance a bit smaller for safety */
   
    for (int it = 0; it < road->pointsCenter.size(); it++) {

        /* if a point is found then car is on the road, return true and stop searching */
        if (distance(position, road->pointsCenter[it]) < dist) {
            inRange = true;
            break;
        }
    }
    return inRange;
}

bool Tema2::CheckCollisionEnemy(glm::vec3 position) {
    bool collision = false;
    float dist = 1;

    for (int it = 0; it < nrEnemies; it++) {
        if (distance(enemies[it]->position, position) < dist) {
            collision = true;
            break;
        }
    }
    return collision;

}

void Tema2::RenderMeshMain(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color)
{
    /* render mesh using implemented main camera */
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set shader uniforms
    GLint car_pos = glGetUniformLocation(shader->program, "car_position");
    glUniform4fv(car_pos, 1, glm::value_ptr(car_position[0]));

    GLint color_shader = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(color_shader, 1, glm::value_ptr(color));
   
    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(cameraCar->GetViewMatrix()));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrixC));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::RenderMeshMiniMap(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    /* render mesh using implemented minimap camera */
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(cameraMiniMap->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrixM));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    /* on imput update modify main camera position and rotation and player rotation angle
    if a collision is ongoing then car can not be moved */
    if (!collision) {
        if (window->KeyHold(GLFW_KEY_I)) { // accelerate
            if (speed < maxSpeed) {
                speed += 3 * deltaTime;
                if (speed > maxSpeed) { /* cap speed if needed */
                    speed = maxSpeed;
                }
                rotationSensibility = (int)speed / 10;
            }

        }
        if (window->KeyHold(GLFW_KEY_K)) { // deccelerate
            if (speed > minSpeed) {
                speed -= 3 * deltaTime;
                if (speed < minSpeed) { /* cap speed if needed */
                    speed = minSpeed;
                }
                rotationSensibility = (int)speed / 10;
            }
        }

        if (window->KeyHold(GLFW_KEY_W)) { // forward
            cameraCar->MoveForward(deltaTime * speed);
            /* if car gets out of road, revert movement so visually, movement is ignored */
            if (!CheckCarInRoad(cameraCar->GetTargetPosition())) {
                speed = minSpeed; // reset car speed as well
                cameraCar->MoveForward(-deltaTime * speed);
            }
        }

        if (window->KeyHold(GLFW_KEY_S)) { //backward
            cameraCar->MoveForward(-deltaTime * speed);
            /* if car gets out of road, revert movement so visually, movement is ignored */
            if (!CheckCarInRoad(cameraCar->GetTargetPosition())) {
                speed = minSpeed; // reset car speed as well
                cameraCar->MoveForward(deltaTime * speed);
            }
        }

        if (window->KeyHold(GLFW_KEY_A)) { //rotate left
            rotation += deltaTime * rotationSensibility;
            cameraCar->RotateThirdPerson_OY(deltaTime * rotationSensibility);
            /* if car gets out of road, revert movement so visually, movement is ignored */
            if (!CheckCarInRoad(cameraCar->GetTargetPosition())) {
                speed = minSpeed; // reset car speed as well
                rotation += -deltaTime * rotationSensibility;
                cameraCar->RotateThirdPerson_OY(-deltaTime * rotationSensibility);
            }
        }

        if (window->KeyHold(GLFW_KEY_D)) { //rotate right
            rotation += -deltaTime * rotationSensibility;
            cameraCar->RotateThirdPerson_OY(-deltaTime * rotationSensibility);
            /* if car gets out of road, revert movement so visually, movement is ignored */
            if (!CheckCarInRoad(cameraCar->GetTargetPosition())) {
                speed = minSpeed; // reset car speed as well
                rotation += deltaTime * rotationSensibility;
                cameraCar->RotateThirdPerson_OY(deltaTime * rotationSensibility);
            }
        }
    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (window->KeyHold(GLFW_KEY_P)) {
        showControls = !showControls;
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}