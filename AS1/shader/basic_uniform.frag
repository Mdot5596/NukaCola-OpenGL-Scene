#version 460

in vec3 Position;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;




uniform struct LightInfo {
   vec4 Position;
   vec3 La;
   vec3 L;
} Lights[3]; 


uniform struct MaterialInfo{
   vec3 Kd;
   vec3 Ka;
   vec3 Ks;
   float Shininess;

}Material;

vec3 phongModel ( int light, vec3 position, vec3 n)
{
    vec3 ambient=Lights[light].La*Material.Ka;

    vec3 s=normalize(vec3(Lights[light].Position.xyz)-position);
    float sDotN=max(dot(s,n),0.0);
    vec3 diffuse=Material.Kd*sDotN;

    vec3 spec=vec3(0.0);
    if (sDotN>0.0)
    {
    vec3 v=normalize(-position.xyz);
    vec3 r=reflect(-s,n);
    spec=Material.Ks*pow(max(dot(r,v),0.0),Material.Shininess);
    }
    return ambient+(diffuse+spec)*Lights[light].L;
}

void main()
{

   vec3 Colour=vec3(0.0);
   for (int i=0;i<3;i++)
    Colour+=phongModel(i, Position,Normal);

    FragColor = vec4(Colour, 1.0);
}