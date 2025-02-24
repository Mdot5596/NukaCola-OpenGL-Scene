#version 460

layout(location = 0) in vec3 VertexPosition; // Position of the vertex
layout(location = 1) in vec3 VertexNormal; // Position of the vertex
layout(location = 2) in vec2 VertexTexCoord; // Position of the vertex

out vec3 ReflectDir;
out vec2 TexCoords; // Pass texture coordinates to fragment shader



uniform vec3 WorldCameraPosition;
uniform mat4 ModelViewMatrix;
uniform mat4 ModelMatrix;
uniform mat4 NormalMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 MVP;

void main()
{
  vec3 worldPos=vec3(ModelMatrix*vec4(VertexPosition, 1.0));
  vec3 worldNorm=vec3(ModelMatrix*vec4(VertexNormal, 0.0));
  vec3 worldView=normalize(WorldCameraPosition-worldPos);

 ReflectDir=reflect(-worldView,worldNorm);
     TexCoords = VertexTexCoord; // Pass texture coordinates to fragment shader


  gl_Position = MVP*vec4(VertexPosition,1.0);

}