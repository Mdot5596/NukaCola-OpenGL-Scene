#version 460

in vec2 TexCoords;   // Receiving texture coordinates from the vertex shader
in vec3 Position;    // Fragment position in camera space
in vec3 Normal;      // Normal vector
in vec3 Vec;

layout(location = 0) out vec4 FragColor;
layout(binding = 0) uniform samplerCube SkyBoxTex;
layout(binding = 1) uniform sampler2D TextureMap;        // Texture
layout(binding = 2) uniform sampler2D SecondTextureMap;  // Second texture
uniform float texScale;
uniform float mixFactor;                                 // Blend factor (0.0 = first texture, 1.0 = second texture)
uniform bool UseSecondTexture;                           // Weather the obj should be second texed or not


// Light struct
uniform struct SpotLightInfo {
    vec3 Position;    // Position in camera space
    vec3 L;           // Diffuse/specular intensity
    vec3 La;          // Ambient intensity
    vec3 Direction;   // Direction of the spotlight in camera space
    float Exponent;   // Angular attenuation exponent
    float Cutoff;     // Cutoff angle (cosine value)
} Spot;

// Material struct
uniform struct MaterialInfo {
    vec3 Kd;        // Diffuse reflectivity
    vec3 Ka;        // Ambient reflectivity
    vec3 Ks;        // Specular reflectivity
    float Shininess;
} Material;

// Fog struct
uniform struct FogInfo {
    float MaxDist;
    float MinDist;
    vec3 Color;
} Fog;


vec3 blinnPhongSpot(vec3 position, vec3 n) {
    // Ambient component
    vec3 ambient = Spot.La * Material.Ka;

    // Light direction vector
    vec3 s = normalize(Spot.Position - position);

    // Check if fragment is inside the spotlight cone
    float cosAng = dot(-s, normalize(Spot.Direction)); // Negative since we compare opposite vectors
    float angle = acos(cosAng);
    float spotScale = 0.0;

    if (angle < Spot.Cutoff) {  // Inside the spotlight cone
        spotScale = pow(cosAng, Spot.Exponent);

        // Diffuse component
        float sDotN = max(dot(s, n), 0.0);
        vec3 diffuse = Material.Kd * sDotN;

        // Specular component
        vec3 spec = vec3(0.0);
        if (sDotN > 0.0) {
            vec3 v = normalize(-position); // View direction
            vec3 h = normalize(v + s);    // Halfway vector
            spec = Material.Ks * pow(max(dot(h, n), 0.0), Material.Shininess);
        }

        return ambient + spotScale * Spot.L * (diffuse + spec);
    }

    // If outside spotlight, return only ambient
    return ambient;
}

void main() 
{
   //Skybox
   vec3 texColor=texture(SkyBoxTex, normalize(Vec)).rgb;
   
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    // Sample first texture
    vec4 texColor1 = texture(TextureMap, TexCoords * texScale);
    
    // Sample and mix second texture **only if UseSecondTexture is true**
    vec4 finalTex = texColor1;  // Default to first texture
    if (UseSecondTexture) {
        vec4 texColor2 = texture(SecondTextureMap, TexCoords * texScale);
        finalTex = mix(texColor1, texColor2, 0.5); // 50-50 mix for the can
    }

    // Compute shading
    vec3 shadeColor = blinnPhongSpot(Position, normalize(Normal));

    // Final color with shading and fog
    vec3 finalColor = mix(Fog.Color, shadeColor * finalTex.rgb, fogFactor);

    FragColor = vec4(finalColor, finalTex.a); // Preserve texture alpha
}