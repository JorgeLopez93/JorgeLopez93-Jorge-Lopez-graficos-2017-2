#version 120
varying  vec4 color;



struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 ambient;
    //vec3 diffuse;
    vec3 specular;
};

varying vec3 FragPos;
varying vec3 Normal;
uniform vec3 globalAmbientLight;
uniform float shininess;
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform Material material;



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir , normal);
    float spec = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
    // combine results
    vec3 ambient = light.ambient * material.ambient;
    vec3 diffuse = light.diffuse * diff * vec3(color.r, color.g, color.b);
    vec3 specular = light.specular * spec * material.specular;
    return (ambient + diffuse + specular);
}
uniform vec4 fColor;

void main()
{
    //fColor = color;
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalcDirLight(dirLight, norm, viewDir);
    // phase 3: spot light
    //result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
    result = result + globalAmbientLight * material.ambient;
    gl_FragColor = vec4(result, 1.0);
}
