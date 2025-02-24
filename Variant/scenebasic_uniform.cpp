#include "scenebasic_uniform.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iostream>
#include "helper/glutils.h"
#include "helper/texture.h"

using std::cerr;
using std::endl;

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() :
    tPrev(0), angle(90.0f), rotSpeed(glm::pi<float>()/8.0f), sky(100.0f), teapot(14,glm::mat4(1.0f))
{
    //Load Models:
    mesh = ObjMesh::load("media/soda can.obj", true);

}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST); // Enable depth testing
    projection = mat4(1.0f);

    // Load the HDR Cube Map for reflections
    cubeTex = Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");  


    //Applies the Skybox/cube tex
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

    //Applies the can tex
    glActiveTexture(GL_TEXTURE1);
    sodaCanTex = Texture::loadTexture("media/texture/nukacan.jpg");
    glBindTexture(GL_TEXTURE_CUBE_MAP, sodaCanTex);                //Can also use GL_TEXTURE_2D

}


void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
        skyProg.compileShader("shader/skybox.vert");
        skyProg.compileShader("shader/skybox.frag");
        skyProg.link();
        prog.link();
        prog.use();
    }
    catch (GLSLProgramException& e) {
        cerr << e.what() << endl;
        exit(EXIT_FAILURE);
    }
}

void SceneBasic_Uniform::update(float t)
{
    float deltaT = t - tPrev;

    if (tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;
    angle += 0.1f * deltaT;

    if (angle>glm::two_pi<float>())
    {
        angle -= glm::two_pi<float>();
    }
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color & depth buffers

    vec3 cameraPos = vec3(7.0f * cos(angle), 2.0f, 7.0f * sin(angle));
    view = glm::lookAt(cameraPos, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));


    skyProg.use();
    model = mat4(1.0f);
    setMatrices(skyProg);
    sky.render();

    prog.use();
    prog.setUniform("WorldCameraPosition", cameraPos);
    prog.setUniform("MaterialColor", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
    prog.setUniform("ReflectFactor", 0.85f);
    prog.setUniform("TextureMap", 1); // Tell the shader to use texture unit 1


    setMatrices(prog); // Apply model-view-projection transformations




    // Bind nukacan texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sodaCanTex);

    // Ensure the texture has a defined base level
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Generate mipmaps (fixes "undefined base level" issue)
    glGenerateMipmap(GL_TEXTURE_2D);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

    mesh->render();
}


void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices(GLSLProgram &p)
{
    mat4 mv = view * model;                // Compute the Model-View matrix
    p.setUniform("ModelMatrix", model);    // Send Model-View matrix to the shader
    p.setUniform("MVP", projection * mv); // Send Model-View-Projection matrix to the shader

}