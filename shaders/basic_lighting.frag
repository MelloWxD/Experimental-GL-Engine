#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;   
    sampler2D texture_normal1;   
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
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
   
};
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

in mat3 TBN;
in vec3 Normal;

vec3 calcPointLighting(PointLight light, vec3 norm, vec3 viewDir);
vec3 calDirectLighting(DirLight light, vec3 norm, vec3 viewDir);
vec3 calcSpotLighting(SpotLight light, vec3 norm, vec3 viewDir);

uniform vec3 viewPos;
const float kPi = 3.14159265;
const float gamma = 2.2;

void main()
{
    // obtain normal from normal map in range [0,1]
    vec3 normal = Normal;//texture(material.texture_normal1, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    //normal = normalize(normal * 2.0 - 1.0);      
    vec3 viewDirection = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);

 
    vec3 res = calDirectLighting(dirLight, normal, viewDirection);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        res += calcPointLighting(pointLights[i], normal, viewDirection);    
    }

    res += calcSpotLighting(spotLight, normal, viewDirection);
        
    res.rgb = pow(res.rgb, vec3(1.0/gamma));

    FragColor = vec4(res, 1.0);
    
} 



vec3 calcPointLighting(PointLight light, vec3 norm, vec3 viewDir)
{

    vec3 lightDirection = normalize((TBN * light.Position) - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDirection + viewDir);
   vec4 tex_color = texture(material.texture_diffuse1, fs_in.TexCoords);
    vec3 diff_color = pow(texture(material.texture_diffuse1, fs_in.TexCoords).rgb, vec3(gamma));
    float kEnergyConservation = ( 8.0 + material.shininess ) / ( 8.0 * kPi ); 

    if (tex_color.a < 0.1)
    {
        discard;
    }

    float diffuse_strength = max(dot(norm, lightDirection), 0.0);

    // specular
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = kEnergyConservation * pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    // attentuation
    float distance = length(light.Position - fs_in.FragPos);
    float attentuation = 1.0 /  distance;//(light.constant + light.linear * distance + light.quadratic * (distance * distance));

  vec3 ambient = light.ambient * diff_color.rgb;
    vec3 diffuse = light.diffuse * diffuse_strength * diff_color.rgb;
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
    vec3 emission = vec3(texture(material.texture_emission1, fs_in.TexCoords));

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
    vec3 lightDirection = normalize(light.direction);
    vec3 halfwayDir = normalize(lightDirection + viewDir);

    vec4 tex_color = texture(material.texture_diffuse1, fs_in.TexCoords);
    vec3 diff_color = pow(texture(material.texture_diffuse1, fs_in.TexCoords).rgb, vec3(gamma));
    float kEnergyConservation = ( 8.0 + material.shininess ) / ( 8.0 * kPi ); 

    if (tex_color.a < 0.1)
    {
        discard;
    }

    float diff_strength = max(dot(norm, lightDirection), 0.0);

   // vec3 reflectDirection = reflect(-lightDirection, norm);

    float spec = kEnergyConservation * pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

 
    vec3 ambient = light.ambient * diff_color.rgb;
    vec3 diffuse = light.diffuse * diff_strength * diff_color.rgb;

    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
    vec3 emission = vec3(texture(material.texture_emission1, fs_in.TexCoords));

  if (material.hasEmission)
    {
        return (ambient + diffuse + specular + emission) * light.color;

    }
        return (ambient + diffuse + specular) * light.color;

}

vec3 calcSpotLighting(SpotLight light, vec3 norm, vec3 viewDir)
{
    vec3 lightDirection = normalize(light.position - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    vec4 tex_color = texture(material.texture_diffuse1, fs_in.TexCoords);
    vec3 diff_color = pow(texture(material.texture_diffuse1, fs_in.TexCoords).rgb, vec3(gamma));

    float kEnergyConservation = ( 8.0 + material.shininess ) / ( 8.0 * kPi ); 

    if (tex_color.a < 0.1)
    {
        discard;
    }
    float diff_strength = max(dot(norm, lightDirection), 0.0);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = kEnergyConservation * pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / distance;//(light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // spot intensity
    float t = dot(lightDirection, normalize(-light.direction));
    float e = light.cutOff - light.outerCutOff;
    float intensity = clamp((t - light.outerCutOff) / e, 0.0, 1.0);

    vec3 ambient = light.ambient * diff_color.rgb;
    vec3 diffuse = light.diffuse * diff_strength * diff_color.rgb;

    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
    vec3 emission = vec3(texture(material.texture_emission1, fs_in.TexCoords));
   // ambient *= attenuation * intensity;
    diffuse *=   intensity;
    specular *=  intensity;
   if (material.hasEmission)
    {
        return (ambient + diffuse + specular + emission) * light.color;

    }
        return (ambient + diffuse + specular) * light.color;


}


