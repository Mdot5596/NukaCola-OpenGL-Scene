#version 460

in vec2 TexCoords;   // Receiving texture coordinates from the vertex shader
in vec3 Position;    // Fragment position in camera space
in vec3 Normal;      // Normal vector

layout(location = 0) out vec4 FragColor;
layout(binding = 1) uniform sampler2D TextureMap;        // Texture
layout(binding = 2) uniform sampler2D SecondTextureMap;  // Second texture
uniform float mixFactor;                                 // Blend factor (0.0 = first texture, 1.0 = second texture)

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

uniform float texScale;

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

void main() {
    float dist = abs(Position.z);
    float fogFactor = (Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist);
    fogFactor = clamp(fogFactor, 0.0, 1.0);

    // Sample both textures
    vec4 texColor1 = texture(TextureMap, TexCoords * texScale);
    vec4 texColor2 = texture(SecondTextureMap, TexCoords * texScale);

    // Mix textures based on mixFactor (0.0 = only texColor1, 1.0 = only texColor2)
    vec4 blendedTex = mix(texColor1, texColor2, mixFactor);

    // Compute shading
    vec3 shadeColor = blinnPhongSpot(Position, normalize(Normal));

    // Final color with shading and fog
    vec3 finalColor = mix(Fog.Color, shadeColor * blendedTex.rgb, fogFactor);

    FragColor = vec4(finalColor, blendedTex.a); // Preserve texture alpha
}
