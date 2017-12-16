#version 120
in  vec4 color;
in vec2 texCoord;
in vec3 FragPos;
in vec3 Normal;

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    vec3 specular;
};

uniform vec3 globalAmbientLight;
uniform float shininess;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;
uniform int fogSelector;
uniform sampler2D texture_2D;
uniform int Texture_app_flag;
uniform float fogDensity;
uniform vec4 fogColor;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir , normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * vec3(color.r, color.g, color.b);
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}
varying out vec4 fColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    vec4 finalColor  = vec4(result + globalAmbientLight * material.ambient, 1.0);

    if (Texture_app_flag == 1)
      finalColor = finalColor * texture2D( texture_2D, texCoord );

    float dist;
    float fogFactor = 0;
    dist = length(-FragPos);

    if(fogSelector == 1)
    {
       fogFactor = (0 - dist)/(0 - 18);
       fogFactor = clamp( fogFactor, 0.0, 1.0 );
       finalColor = mix(fogColor, finalColor, 1 - fogFactor);
    }
    else if( fogSelector == 2)
    {
        fogFactor = 1.0 /exp(dist * fogDensity);
        fogFactor = clamp( fogFactor, 0.0, 1.0 );
        finalColor = mix(fogColor, finalColor, fogFactor);
    }
    else if( fogSelector == 3)
    {
       fogFactor = 1.0 /exp( (dist * fogDensity)* (dist * fogDensity));
       fogFactor = clamp( fogFactor, 0.0, 1.0 );
       finalColor = mix(fogColor, finalColor, fogFactor);
    }
    fColor = finalColor;
}
