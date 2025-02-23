#include "scenebasic_uniform.h"

#include <cstdio>
#include <cstdlib>

#include <string>
using std::string;
#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"

using glm::vec3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(10.0f,10.0f,100,100) 
{
    mesh = ObjMesh::load("media/soda can.obj", true);
}

void SceneBasic_Uniform::initScene()
{
    
    compile(); 
    glEnable(GL_DEPTH_TEST); 



    // Initialise model matrix
    model = mat4(1.0f);
    view = glm::lookAt(vec3(1.5f, 1.0f, 1.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

    projection = mat4(1.0f); 

    float x, z;
    for (int i = 0; i < 3; i++)
    {
        std::stringstream name;
        name << "Lights[" << i << "].Position";
        x = 2.0f * cosf((glm::two_pi<float>() / 3) * i);
        z = 2.0f * sinf((glm::two_pi<float>() / 3) * i);
        prog.setUniform(name.str().c_str(), view * glm::vec4(x, 1.2f, z + 1.0f, 1.0f));
    }

    // Set shader uniforms varuiaubles
   // prog.setUniform("LightPosition", view * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));
    prog.setUniform("SodaCanTex", 0); // Texture unit 0


    prog.setUniform("Lights[0].L", vec3(0.0f, 0.0f, 0.8f));  
    prog.setUniform("Lights[1].L", vec3(0.0f, 0.8f, 0.0f));
    prog.setUniform("Lights[2].L", vec3(0.8f, 0.0f, 0.0f));

    prog.setUniform("Lights[0].La", vec3(0.0f, 0.0f, 0.2f));
    prog.setUniform("Lights[1].La", vec3(0.0f, 0.2f, 0.0f));
    prog.setUniform("Lights[2].La", vec3(0.2f, 0.0f, 0.0f));
   
}


void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
		prog.use();
	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color & depth buffers

    prog.setUniform("Material.Kd", vec3(0.4f, 0.4f, 0.4f));
    prog.setUniform("Material.Ka", vec3(0.5f, 0.5f, 0.5f));
    prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
    prog.setUniform("Material.Shininess", 180.0f);                 // Higher = sharper highlight

    model = mat4(1.0f);
    model = glm::scale(model, vec3(0.5f));  // Scale down if too big
    model = glm::rotate(model, glm::radians(180.0f), vec3(0.0f, 1.0f, 0.0f)); // Adjust rotation if needed
    model = glm::translate(model, vec3(0.0f, -0.3f, 0.0f));  // Adjust height


    setMatrices(); // Apply model-view-projection transformations
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sodaCanTex);

    mesh->render();

    prog.setUniform("Material.Kd", vec3(0.1f, 0.1f, 0.1f));
    prog.setUniform("Material.Ka", vec3(0.4f, 0.4f, 0.4f)); //use to adgust light
    prog.setUniform("Material.Ks", vec3(0.9f, 0.9f, 0.9f));
    prog.setUniform("Material.Shininess", 180.0f);                 

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices(); 
    plane.render(); 
}


void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(50.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices()
{
    mat4 mv = view*model; // Compute the Model-View matrix

    // Send Model-View matrix to the shader
    prog.setUniform("ModelViewMatrix", mv);

    // Compute and send Normal Matrix (used for transforming normals correctly)
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));

    // Send Model-View-Projection matrix to the shader
    prog.setUniform("MVP", projection * mv);
}