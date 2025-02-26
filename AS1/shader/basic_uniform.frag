#version 460

in vec2 TexCoords;                                               // Receiving texture coordinates from the vertex shader
in vec3 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;
layout (binding=1) uniform sampler2D TextureMap;                  // Nukacan texture

uniform struct LightInfo {
   vec4 Position;
   vec3 La;
   vec3 L;

} Light; 


uniform struct MaterialInfo{
   vec3 Kd;
   vec3 Ka;
   vec3 Ks;
   float Shininess;

}Material;

uniform struct FogInfo
{
  float MaxDist;
  float MinDist;
  vec3 Color;

}Fog;

uniform float texScale;


vec3 blinnPhong ( vec3 position, vec3 n)
{
    vec3 diffuse=vec3(0), spec=vec3(0);
    vec3 ambient=Light.La*Material.Ka;
    vec3 s=normalize(Light.Position.xyz-position);
    float sDotN=max(dot(s,n),0.0);
    diffuse=Material.Kd*sDotN;
    if (sDotN>0.0)
     {
    vec3 v=normalize(-position.xyz);
    vec3 h=normalize(v+s);
    spec=Material.Ks*pow(max(dot(h,n),0.0),Material.Shininess);
      }
    return ambient+(diffuse+spec)*Light.L;
}

void main()
{
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    // Compute shading
    vec3 shadeColor = blinnPhong(Position, normalize(Normal));

    // Sample texture color
    vec4 texColor = texture(TextureMap, TexCoords * texScale);


    // Mix shading with texture color
    vec3 finalColor = mix(Fog.Color, shadeColor * texColor.rgb, fogFactor);

    FragColor = vec4(finalColor, texColor.a); // Preserve texture alpha
}
