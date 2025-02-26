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
    tPrev(0), plane(50.0f, 50.0f, 1, 1)// teapot(14, glm::mat4(1.0f))
{
    //Load Models:
    mesh = ObjMesh::load("media/soda can.obj", true);

}


void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // Initialise model matrix
    model = mat4(1.0f);
    view = glm::lookAt(vec3(0.0f, 4.0f, 6.0f), vec3(0.0f, 0.2f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
    projection = mat4(1.0f);
    angle = 0.0f;

    //Spotlight Setup
    vec3 spotPos = vec3(2.0f, 5.0f, 3.0f);             // Position of spotlight in world space
    vec3 spotDir = normalize(vec3(-0.5f, -1.0f, 0.0f)); // Direction the spotlight points
    float spotCutoff = glm::cos(glm::radians(1.0f));   // Convert cutoff angle (25°) to cosine
    float spotExponent = 15.0f;                        // Controls edge softness

    // Set spotlight uniforms
    prog.setUniform("Spot.Position", vec3(view * vec4(spotPos, 1.0f))); // Convert to camera space
    prog.setUniform("Spot.Direction", mat3(view) * spotDir);            // Transform to camera space
    prog.setUniform("Spot.L", vec3(1.0f, 1.0f, 1.0f));  // Light intensity
    prog.setUniform("Spot.La", vec3(0.2f, 0.2f, 0.2f)); // Ambient light
    prog.setUniform("Spot.Exponent", spotExponent);
    prog.setUniform("Spot.Cutoff", spotCutoff);

    //Material Properties
    prog.setUniform("Material.Ka", vec3(0.2f, 0.2f, 0.2f));
    prog.setUniform("Material.Kd", vec3(0.8f, 0.8f, 0.8f));
    prog.setUniform("Material.Ks", vec3(1.0f, 1.0f, 1.0f));
    prog.setUniform("Material.Shininess", 50.0f);

    //Fog Properties
    prog.setUniform("Fog.MinDist", 5.0f);
    prog.setUniform("Fog.MaxDist", 25.0f);
    prog.setUniform("Fog.Color", vec3(0.5f, 0.5f, 0.5f));

    //Texture Scaling
    prog.setUniform("texScale", 1.0f);

    //Load Textures
    glActiveTexture(GL_TEXTURE1);
    sodaCanTex = Texture::loadTexture("media/texture/nukacan.jpg");
    glBindTexture(GL_TEXTURE_2D, sodaCanTex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);

    //Load Plane Texture
    glActiveTexture(GL_TEXTURE2);
    planeTex = Texture::loadTexture("media/texture/FALLOUTFLOOR.jpg");
    glBindTexture(GL_TEXTURE_2D, planeTex);
}



void SceneBasic_Uniform::compile()
{
    try {
        prog.compileShader("shader/basic_uniform.vert");
        prog.compileShader("shader/basic_uniform.frag");
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
    angle += 0.3f * deltaT;
    if (angle > glm::two_pi<float>())angle -= glm::two_pi<float>();
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color & depth buffers

    vec4 spotPos = vec4(10.0f * cos(angle), 10.0f, 10.0f * sin(angle), 1.0f);
    prog.setUniform("Spot.Position", vec3(view * spotPos));

    /*
    prog.setUniform("Material.Kd", vec3(0.2f, 0.55f, 0.9f));
    prog.setUniform("Material.Ks", vec3(0.95f, 0.95f, 0.95f));
    prog.setUniform("Material.Ka", vec3(0.2f * 0.3f, 0.55f * 0.3f, 0.9f * 0.3f));
    prog.setUniform("Material.Shininess", 100.0f);                 // Higher = sharper highlight

    float dist = 0.0f;
    for (int i = 0; i < 5; i++)
    {
        model = mat4(1.0f);
        model = glm::translate(model, vec3(dist * 0.6f - 1.0f, 0.0f, -dist));
        model = glm::rotate(model, glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
        setMatrices(); // Apply model-view-projection transformations
       // teapot.render(); // Render the torus object
        dist += 7.0f;
    }
    */

    //NOT SURE IF I EVEN AM USING THIS
    prog.setUniform("Material.Kd", vec3(0.7f, 0.7f, 0.7f));
    prog.setUniform("Material.Ks", vec3(0.0f, 0.0f, 0.0f));
    prog.setUniform("Material.Ka", vec3(0.2f, 0.2f, 0.2f));
    prog.setUniform("Material.Shininess", 180.0f);
    //

    //Rendering plane
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, planeTex);
    model = mat4(1.0f);
    model = translate(model, vec3(0.0f, -1.0f, 0.0f));
    setMatrices();
    prog.setUniform("texScale", 5.0f); //Scale texture for plane
    plane.render();
    //

    // Bind nukacan texture
    // Render Soda Can (with spotlight effect)
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sodaCanTex);
    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, 0.6f, 0.0f));
    setMatrices();
    prog.setUniform("texScale", 1.0f);
    mesh->render();
    //
}


void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view * model; // Compute the Model-View matrix

    // Send Model-View matrix to the shader
    prog.setUniform("ModelViewMatrix", mv);

    // Compute and send Normal Matrix (used for transforming normals correctly)
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

    // Send Model-View-Projection matrix to the shader
    prog.setUniform("MVP", projection * mv);
}