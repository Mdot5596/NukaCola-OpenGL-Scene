#version 460

in vec3 ReflectDir;
in vec2 TexCoords;                                  // Receiving texture coordinates from the vertex shader

layout (binding=0) uniform samplerCube SkyBoxTex;
layout (binding=1) uniform sampler2D TextureMap;    // Nukacan texture

uniform float ReflectFactor;
uniform vec4 MaterialColor;

layout(location =0) out vec4 FragColor;

void main()
{
    vec3 texColor = texture(TextureMap, TexCoords).rgb;             // Sample from the nukacan texture

    vec3 reflectColor = texture(SkyBoxTex, ReflectDir).rgb;         // Sample from the reflection cubemap
    reflectColor = pow(reflectColor, vec3(1.0 / 2.2));              // Apply gamma correction

    vec3 finalColor = mix(texColor, reflectColor, ReflectFactor);  // Mix texture and reflection

    FragColor = vec4(finalColor, 1.0);

}




