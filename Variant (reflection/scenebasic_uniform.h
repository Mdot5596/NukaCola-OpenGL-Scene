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
    Teapot teapot;
    std::unique_ptr<ObjMesh> mesh;
    SkyBox sky;
    GLuint sodaCanTex;
    GLuint cubeTex;  

    float rotSpeed;
    GLuint brick , moss;
    float tPrev;
    float angle;
    GLSLProgram prog, skyProg;
    void setMatrices(GLSLProgram &p);
    void compile();


public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
};

#endif // SCENEBASIC_UNIFORM_H
