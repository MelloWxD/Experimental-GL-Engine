#version 330 core
out vec4 FragColor;
precision highp float;
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

    float ambientStrength;
    float diffuseStrength;

    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;

    sampler2D shadowMap;

    vec3 color;
};
uniform SpotLight spotLight;

struct DirLight
{
    vec3 direction; // xyz for dir vector and w for power component
    float diffuseStrength;
    float ambientStrength;
    vec3 color;
    sampler2D shadowMap;
};
uniform DirLight dirLight;
struct PointLight
{
    vec3 Position; // xyz for pos vector and w for power component
    float ambientStrength;

    vec3 color;
    float constant;
    float linear;
    float quadratic;
    samplerCube shadowCubeMap;
    mat4 shadowMatrices[6];
   
};
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
    vec4 FragPosLight;
    mat3 TBN;
} fs_in;

in mat3 TBN;
in vec3 Normal;
uniform mat4 lightSpaceMatrix;



uniform float farPlane;


vec3 calcPointLighting(PointLight light, vec3 norm, vec3 viewDir, vec3 color, float kEnergyConservation);
vec3 calDirectLighting(DirLight light, vec3 norm, vec3 viewDir, vec3 color, float kEnergyConservation);
vec3 calcSpotLighting(SpotLight light, vec3 norm, vec3 viewDir, vec3 color, float kEnergyConservation);
float ShadowCalculationPointLight(PointLight light, vec3 norm, vec3 lightDir);
uniform vec3 viewPos;
const float kPi = 3.14159265;
const float gamma = 2.2;
uniform float biasLow;
uniform float biasHigh;

void main()
{
    
     vec4 color = texture(material.texture_diffuse1, fs_in.TexCoords); // obtain colour from diffuse/abledo texture
    if (color.a < 0.1)
    {
        discard;
    }
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(material.texture_normal1, fs_in.TexCoords).rgb ;//normalize(Normal)
    normal = normalize(normal * 2.0 - 1.0);   
    normal = normalize(fs_in.TBN * normal); 
    // transform normal vector to range [-1,1]
    //normal = normalize(normal * 2.0 - 1.0);      
    vec3 viewDirection = normalize(viewPos - fs_in.FragPos);

    float kEnergyConservation = ( 8.0 + material.shininess ) / ( 8.0 * kPi ); 

    vec3 res;
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        
    }
    res += calcPointLighting(pointLights[0], normal, viewDirection, color.rgb, kEnergyConservation);  
    //res += calcSpotLighting(spotLight, normal, viewDirection, color.rgb, kEnergyConservation);  
    //res += calDirectLighting(dirLight, normal, viewDirection, color.rgb, kEnergyConservation);
     
    res.rgb = pow(res.rgb, vec3(1.0/gamma));

    FragColor = vec4(res, 1.0); 
    

} 
float ShadowCalculationPointLight(PointLight light, vec3 norm, vec3 lightDir)
{
    // Shadows
    vec3 fragToLight = fs_in.FragPos - light.Position;
	float currentDepth = length(fragToLight);
    float shadow = 0.0;
	float bias = max(0.01 * (1.0 - dot(norm, lightDir)), 0.00005); 

	// Not really a radius, more like half the width of a square
	int sampleRadius = 2;
	float offset = 0.025;
	for(int z = -sampleRadius; z <= sampleRadius; z++)
	{
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(light.shadowCubeMap, normalize(fragToLight) + vec3(x, y, z) * offset).r;
				// Remember that we divided by the farPlane?
				// Also notice how the currentDepth is not in the range [0, 1]
				closestDepth *= farPlane;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0;     
		    }    
		}
	}
	// Average shadow
    if (shadow > 0 )
    {
	    shadow = (shadow / pow((sampleRadius * 2 + 1), 3));
        return shadow;
    }
    else
    {
        return 0.0f;
    }


}


vec3 calcPointLighting(PointLight light, vec3 norm, vec3 viewDir, vec3 color, float kEnergyConservation)
{
    
    vec3 lightDirection = normalize(light.Position - fs_in.FragPos);
    float diffuse_strength = max(dot(norm, lightDirection), 0.0);


    //Blinn-Phong spec
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    float spec = kEnergyConservation * pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    // attentuation
    float distance = length(light.Position - fs_in.FragPos);
    float attentuation = 1.0 /  (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Lighting calculations
    vec3 ambient = light.ambientStrength * color;
    vec3 diffuse = diffuse_strength * color;
    vec3 specular =  spec * vec3(texture(material.texture_specular1, fs_in.TexCoords).r);

    ambient *= attentuation;
    diffuse *= attentuation;
    specular *= attentuation;

    
    float shadow = ShadowCalculationPointLight(light, norm, lightDirection);

   
    if (material.hasEmission)
    {   
        vec3 emission = vec3(texture(material.texture_emission1, fs_in.TexCoords));
        return (ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow)) + emission) * light.color;
    }

    return (ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow))) * light.color;
}

vec3 calDirectLighting(DirLight light, vec3 norm, vec3 viewDir, vec3 color, float kEnergyConservation)
{
    vec3 lightDirection = normalize(-light.direction);
    vec3 halfwayDir = normalize(lightDirection + viewDir);




    float diff_strength = max(dot(norm, lightDirection), 0.0);

    float spec = kEnergyConservation * pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);

    float shadow = 0.0;
    vec3 lightCoords = fs_in.FragPosLight.xyz / fs_in.FragPosLight.w;
    if (lightCoords.z <= 1.0)
    {
       	// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0) / 2.0;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
        float bias = max(0.05 * (1.0 - dot(norm, lightDirection)), 0.005);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(light.shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(light.shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);
    }
    shadow = 0;
    vec3 ambient = light.ambientStrength * color.rgb;
    vec3 diffuse = diff_strength * light.diffuseStrength * color.rgb;
    vec3 specular = spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));

  if (material.hasEmission)
    {    
        vec3 emission = vec3(texture(material.texture_emission1, fs_in.TexCoords));
        return (ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow)) + emission) * light.color;

    }
        return (ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow)) ) * light.color;

}

vec3 calcSpotLighting(SpotLight light, vec3 norm, vec3 viewDir, vec3 color, float kEnergyConservation) 
{
    highp vec3 lightDirection = normalize(light.position - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDirection + viewDir);


  
    float diff_strength = max(dot(norm, lightDirection), 0.0);
    float spec = kEnergyConservation * pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    // Calculate the spotlight effect (cosine-based)
    float theta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    
    //float shadow = ShadowCalculation(fs_in.FragPosLight);       
    float shadow = 0.0;
    vec3 lightCoords = fs_in.FragPosLight.xyz / fs_in.FragPosLight.w;
       if (lightCoords.z <= 1.0)
    {
       	// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0) / 2.0;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
        float bias = max(0.0025 * (1.0 - dot(norm, lightDirection)), 0.00025);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(light.shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(light.shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);
    }
    
    // Calculate ambient, diffuse, and specular components
    vec3 ambient = light.ambientStrength * color;
    vec3 diffuse = light.diffuseStrength * diff_strength * color;
    vec3 specular = spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));

    // Apply attenuation and intensity
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;

   
  if (material.hasEmission)
    {    
        vec3 emission = vec3(texture(material.texture_emission1, fs_in.TexCoords));

        return (ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow)) + emission) * light.color;

    }
        return (ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow)) ) * light.color;


}