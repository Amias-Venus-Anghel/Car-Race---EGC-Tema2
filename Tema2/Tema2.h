#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "Tema2_camera.h"
#include "Road.h"
#include "simpleobject3D.h"
#include "Tree.h"
#include "EnemyCar.h"
#include "RoadPoints.h"

#include <vector>
#include <string>
#include <iostream>


namespace m1
{
    class Tema2  : public gfxc::SimpleScene
    {
    public:
        struct ViewportArea
        {
            ViewportArea() : x(0), y(0), width(1), height(1) {}
            ViewportArea(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };

        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderMeshMain(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::vec3 color);
        void RenderMeshMiniMap(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void RenderScene(float deltaTimeSeconds);
        void RenderSceneMiniMap(float deltaTimeSeconds);
        bool CheckCarInRoad(glm::vec3 position);
        bool CheckCollisionEnemy(glm::vec3 position);

        void DrawHUD();

    protected:
        /* player car params */
        float rotation; // car rotation
        float speed; // car speed
        float minSpeed; // car minimum speed
        float maxSpeed; // car maximum speed
        float rotationSensibility; // car rotation sensitivity

        glm::mat4 car_position;

        /* colors */
        glm::vec3 car_color;
        glm::vec3 grass_color;
        glm::vec3 gate_main_color;
        glm::vec3 gate_sec_color;

        ViewportArea miniViewportArea;

        /* camera */
        implementedTema::Camera* cameraCar; /* camera for car */
        implementedTema::Camera* cameraMiniMap; /* camera for minimap */
        glm::mat4 projectionMatrixC; /* projection matrix for car */
        glm::mat4 projectionMatrixM; /* projection matrix for mini map */

        /* projection matrix params */
        GLfloat zNear, zFar, ortholeft, orthoright, orthobottom, orthotop; 

        /* game object referances */
        RoadPoints::Points* points;
        Road* road;
        Mesh* car;
        Tree* trees;
        std::vector<EnemyCar*> enemies;
        std::vector<Mesh*> gate;

        /* other */
        int nrEnemies;
        float roadWidth;
        bool collision;

        /* time */
        float timeout;

        bool counting;
        float bestTime;
        float lastTime;
        float startTime;

        /* text */
        gfxc::TextRenderer* textRenderer;
        const glm::vec3 kTextColor = NormalizedRGB(166, 172, 205);
        bool showControls;
 
    };
}   // namespace m1

