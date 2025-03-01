#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H
#include "helper/scene.h"
#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "helper/torus.h"
#include "helper/teapot.h"
#include "glm/glm.hpp"
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/cube.h"
#include "helper/skybox.h"

class SceneBasic_Uniform : public Scene
{
private:
    //  Torus torus;
    Plane plane;
    // Teapot teapot;
    std::unique_ptr<ObjMesh> mesh;
    GLuint sodaCanTex;
    GLuint cubeTex;
    SkyBox sky;
    GLuint planeTex;
    GLuint mixTex;
    float tPrev;
    float angle;
    GLSLProgram prog;
    void setMatrices();
    void compile();


public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H