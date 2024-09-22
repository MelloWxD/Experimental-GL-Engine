#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;   
    sampler2D texture_emission1;     
    


    bool hasEmission;
    float shininess;
}; 
uniform Material material;

struct SpotLight
{

    vec3 position;
    vec3 direction; // xyz for dir vector and w for power component

    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};
uniform SpotLight spotLight;

struct DirLight
{
    vec3 direction; // xyz for dir vector and w for power component

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};
uniform DirLight dirLight;
struct PointLight
{
    vec3 Position; // xyz for pos vector and w for power component
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;

   
};
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];


in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;

vec3 calcPointLighting(PointLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDirection = normalize(light.Position - FragPos);
    vec4 tex_color = texture(material.texture_diffuse1, TexCoords);
    
    if (tex_color.a < 0.1)
    {
        discard;
    }

    float diffuse_strength = max(dot(norm, lightDirection), 0.0);

    // specular
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);

    // attentuation
    float distance = length(light.Position - FragPos);
    float attentuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

  vec3 ambient = light.ambient * tex_color.rgb;
    vec3 diffuse = light.diffuse * diffuse_strength * tex_color.rgb;
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    vec3 emission = vec3(texture(material.texture_emission1, TexCoords));

    ambient *= attentuation;
    diffuse *= attentuation;
    specular *= attentuation;

   if (material.hasEmission)
    {
        return (ambient + diffuse + specular + emission) * light.color;

    }
        return (ambient + diffuse + specular) * light.color;

}

vec3 calDirectLighting(DirLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDirection = normalize(-light.direction);
    vec4 tex_color = texture(material.texture_diffuse1, TexCoords);
    if (tex_color.a < 0.1)
    {
        discard;
    }

    float diff_strength = max(dot(norm, lightDirection), 0.0);

    vec3 reflectDirection = reflect(-lightDirection, norm);

    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);

 
    vec3 ambient = light.ambient * tex_color.rgb;
    vec3 diffuse = light.diffuse * diff_strength * tex_color.rgb;

    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    vec3 emission = vec3(texture(material.texture_emission1, TexCoords));

  if (material.hasEmission)
    {
        return (ambient + diffuse + specular + emission) * light.color;

    }
        return (ambient + diffuse + specular) * light.color;

}

vec3 calcSpotLighting(SpotLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDirection = normalize(light.position - FragPos);

    vec4 tex_color = texture(material.texture_diffuse1, TexCoords);
    if (tex_color.a < 0.1)
    {
        discard;
    }
    float diff_strength = max(dot(norm, lightDirection), 0.0);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDir, reflectDirection), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spot intensity
    float t = dot(lightDirection, normalize(-light.direction));
    float e = light.cutOff - light.outerCutOff;
    float intensity = clamp((t - light.outerCutOff) / e, 0.0, 1.0);

    vec3 ambient = light.ambient * tex_color.rgb;
    vec3 diffuse = light.diffuse * diff_strength * tex_color.rgb;

    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, TexCoords));
    vec3 emission = vec3(texture(material.texture_emission1, TexCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
   if (material.hasEmission)
    {
        return (ambient + diffuse + specular + emission) * light.color;

    }
        return (ambient + diffuse + specular) * light.color;


}


void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDirection = normalize(viewPos - FragPos);

 
    vec3 res = calDirectLighting(dirLight, normal, viewDirection);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
    res += calcPointLighting(pointLights[i], normal, viewDirection);    
    }

    res += calcSpotLighting(spotLight, normal, viewDirection);
    FragColor = vec4(res, 1.0);

    
} 