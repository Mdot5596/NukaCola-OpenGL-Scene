# NukaCola-OpenGL-Scene
An OpenGL project utilising a custom shading model implemented in GLSL, showcasing a textured based technique.



## Youtube Walkthrough - [https://youtu.be](https://youtu.be/Fvy_i1Z4Yjc)

## How does it work?

This project was created on PC running Windows 11 and using Visual Studio 2022, there was a project template that i was required to adhere to.

This project is a 3D OpenGL scene featuring dynamic lighting and atmospheric effects. It uses a moving spotlight, realistic fog blending, and the Blinn-Phong shading model for smooth reflective surfaces—all controlled by keyboard and mouse for an immersive experience. 

My Objective was to implement a custom shading model using GLSL that makes use of the current 
techniques and approaches used in the industry. The shading model should work with at least one textured 3D model and one light.    


### In order to run the exe:
Unpack the OpenGLzip and place under C:\Users\Public\OpenGL, then the project should run

## Features
### Aesthetics
I am a huge fan of the Fallout series. For this project, I wanted to create something from that franchise. I had a vision to create an apocalyptic scene with the Nuka-Cola bottle in the center of the screen and a vast wasteland as the background/skybox.

When it came to creating the scene, I found it very hard to find a free Fallout skybox that was already a cubemap. Not only that, but all the semi-decent Fallout 3D models were expensive. I was left with having to use only free models and textures for this project.

I managed to keep the Fallout aesthetic strong throughout the project, despite having to use questionable models such as a Nuka-Cola can (normally, they use glass bottles). But this was the only choice I had, as it was the only free model available. I textured my floor/plane to look like an abandoned kitchen, and I set the skybox as a dark green apocalyptic night. I also used a second texture to mix onto the Nuka-Cola can, a green moss texture to give the effect that the can had been sitting there for some time.

### Textures and Mixing

Textures in this project are loaded using helper functions and then bound to specific texture units for later use in the shaders. For example, a high dynamic range(HDR) cube map is loaded for the skybox with Texture::loadHdrCubeMap, and standard 2D textures for the soda can, plane, and mix are loaded with Texture::loadTexture. Once loaded, each texture is activated on a particular texture unit (using glActiveTexture) and configured with parameters like filtering and wrapping before being passed to the shader via uniforms.

Here's a code snippet showing the texture loading and binding process:
```cpp
// Load and apply the HDR cube map for the skybox on texture unit 0
glActiveTexture(GL_TEXTURE0);
cubeTex = Texture::loadHdrCubeMap("media/texture/cube/skybox-hdr/skybox");
prog.setUniform("SkyBoxTex", 0);  
glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);

// Load and apply the soda can texture on texture unit 1
glActiveTexture(GL_TEXTURE1);
sodaCanTex = Texture::loadTexture("media/texture/nukacan.jpg");
glBindTexture(GL_TEXTURE_2D, sodaCanTex);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glGenerateMipmap(GL_TEXTURE_2D);

// Load and apply the plane texture and mix texture on texture unit 2
glActiveTexture(GL_TEXTURE2);
planeTex = Texture::loadTexture("media/texture/FALLOUTFLOOR.jpg");
glBindTexture(GL_TEXTURE_2D, planeTex);

glActiveTexture(GL_TEXTURE2);
mixTex = Texture::loadTexture("media/texture/moss.jpg");
glBindTexture(GL_TEXTURE_2D, mixTex);

```
During rendering, these textures are re-bound to the same texture units so that the fragment shader can sample them, allowing for effects like texture mixing and proper skybox rendering.


I did texture sampling in the shader, texture sampling is done by first scaling the texture coordinates (using a uniform called "texScale") and then sampling the texture with the GLSL texture() function. If enabled, a second texture is also sampled and the two are blended using the mix() function with a fixed factor (0.5). 
```cpp
// Sample the first texture with scaled coordinates
vec4 texColor1 = texture(TextureMap, TexCoords * texScale);

vec4 finalTex = texColor1;
if (UseSecondTexture) {
    // If a second texture is enabled, sample it similarly
    vec4 texColor2 = texture(SecondTextureMap, TexCoords * texScale);
    // Blend the two textures using a 50/50 mix
    finalTex = mix(texColor1, texColor2, 0.5);
}
```

### Skybox
Before i could render anything i had to convert my skybox textures into HDR files so the loader would be able to read them. I used a website called https://convertio.co/download/033be95f1ae5b9780b751f7d183d28eac6e158/ for this. 

In the C++ code, the skybox is rendered by first binding the cube map texture to GL_TEXTURE0 and setting the "IsSkybox" uniform to true so that the shader samples from the cube map. After drawing the skybox geometry, the flag is reset to false, ensuring that the rest of the scene is rendered with the standard lighting and shading.

In the fragment shader, the skybox is rendered separately by checking the IsSkybox flag. When true, the shader samples the cube map using the normalised direction vector, ensuring the background appears infinitely far away.


### Animation controls

The movement controls allow the user to navigate the 3D scene using the keyboard and mouse. Keyboard inputs (W, A, S, D, Space, and Left Control) update the camera's position in the scene, moving it forward, backward, sideways, and vertically. Meanwhile, the mouse input adjusts the camera’s direction, allowing you to look around smoothly. I have had previous experience using this movement method before, so this was not particularly challenging for me. For mouse movement, the cursor position is tracked and used to update the camera's yaw and pitch, which in turn updates the front vector that determines the viewing direction.


## Resources Used
### OpenGL handbook
- https://learnopengl.com/Advanced-OpenGL/Cubemaps

### Labs
- **My Repository:** [Link](https://github.com/Mdot5596/3105-Labs)
  
I created a repository where I completed and stored every week's lab session up to the assignment's date. I saved each section of the lab (e.g., Part 1, 2, 3). These labs helped me complete certain aspects of this assignment.

### Models
- **Website links:**
  - [NukaCola Can](https://www.turbosquid.com/3d-models/nuka-cola-can-1338119)
  - [Skybox Gen](https://tools.wwwtyro.net/space-3d/index.html#animationSpeed=1&fov=80&nebulae=true&pointStars=true&resolution=256&seed=2stncnkuzc20&stars=true&sun=true)

