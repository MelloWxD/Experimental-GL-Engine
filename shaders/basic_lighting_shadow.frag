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
    float ambientStrength;

    vec3 color;
    float constant;
    float linear;
    float quadratic;

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
} fs_in;

in mat3 TBN;
in vec3 Normal;
uniform mat4 lightSpaceMatrix;
uniform sampler2D shadowMap;


uniform samplerCube shadowCubeMap;
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
    /*
    vec4 color = texture(material.texture_diffuse1, fs_in.TexCoords); // obtain colour from diffuse/abledo texture
    if (color.a < 0.1)
    {
        discard;
    }
    // obtain normal from normal map in range [0,1]
    vec3 normal = normalize(Normal);//texture(material.texture_normal1, fs_in.TexCoords).rgb;
    // transform normal vector to range [-1,1]
    //normal = normalize(normal * 2.0 - 1.0);      
    vec3 viewDirection = normalize(viewPos - fs_in.FragPos);

    float kEnergyConservation = ( 8.0 + material.shininess ) / ( 8.0 * kPi ); 

    vec3 res;// = calDirectLighting(dirLight, normal, viewDirection, color.rgb, kEnergyConservation);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        
    }
     res += calcPointLighting(pointLights[0], normal, viewDirection, color.rgb, kEnergyConservation);  
    //res += calcSpotLighting(spotLight, normal, viewDirection);
        
    res.rgb = pow(res.rgb, vec3(1.0/gamma));

    FragColor = vec4(res, 1.0);
    */
    
    // Shadows
    vec3 fragToLight = fs_in.FragPos - pointLights[0].Position;
    fragToLight.y *= -1;
	float currentDepth = length(fragToLight);
    float shadow = 0.0;
    float closestDepth = texture(shadowCubeMap, fragToLight).r * farPlane;
    if (currentDepth > closestDepth) {
        shadow = 1.0;
    }
    FragColor = vec4(vec3(shadow), 1.0); 
    
} 
float ShadowCalculationPointLight(PointLight light, vec3 norm, vec3 lightDir)
{
    // Shadows
    vec3 fragToLight = fs_in.FragPos - light.Position;
	float currentDepth = length(fragToLight);
    float shadow = 0.0f;
	
	float bias = 0.0f;//max(0.025f * (1.0f - dot(norm, lightDir)), 0.00025f); 

	// Not really a radius, more like half the width of a square
	int sampleRadius = 2;
	float offset = 0.005f;
	for(int z = -sampleRadius; z <= sampleRadius; z++)
	{
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowCubeMap, fragToLight + vec3(x, y, z) * offset).r;
				// Remember that we divided by the farPlane?
				// Also notice how the currentDepth is not in the range [0, 1]
				closestDepth *= farPlane;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
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
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;
    return shadow;
}  
vec3 gridSamplingDisk[20] = vec3[]
(
   vec3(1, 1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1, 1,  1), 
   vec3(1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
   vec3(1, 1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1, 1,  0),
   vec3(1, 0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1, 0, -1),
   vec3(0, 1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0, 1, -1)
);

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

    
    //float shadow = ShadowCalculationPointLight(light, norm, lightDirection);

    float shadow = 0.0;
    vec3 fragToLight = fs_in.FragPos - light.Position;
	float currentDepth = length(fragToLight);
    float closestDepth = texture(shadowCubeMap, fragToLight).r * farPlane;
    if (currentDepth > closestDepth) {
        shadow = 1.0;
    }
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




    float diff_strength = max(dot(norm, -lightDirection), 0.0);

    vec3 reflectDirection = reflect(-lightDirection, norm);

    float spec = kEnergyConservation * pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);

    float shadow = 0.0;
    vec3 lightCoords = fs_in.FragPosLight.xyz / fs_in.FragPosLight.w;
    if (lightCoords.z <= 1.0)
    {
       	// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0) / 2.0;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
		float bias = max(0.025 * (1.0 - dot(norm, lightDirection)), 0.0005);

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);
    }
    

    vec3 ambient = light.ambient * color.rgb;
    vec3 diffuse = diff_strength * color.rgb;

    vec3 specular = spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
    vec3 emission = vec3(texture(material.texture_emission1, fs_in.TexCoords));

  if (material.hasEmission)
    {
        return (ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow)) + emission) * light.color;

    }
        return (ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow)) ) * light.color;

}

vec3 calcSpotLighting(SpotLight light, vec3 norm, vec3 viewDir, vec3 color, float kEnergyConservation) 
{
    highp vec3 lightDirection = normalize(light.position - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDirection + viewDir);


  
    float diff_strength = max(dot(norm, lightDirection), 0.0);
    vec3 reflectDirection = reflect(-lightDirection, norm);
    float spec = kEnergyConservation * pow(max(dot(norm, halfwayDir), 0.0), material.shininess);

    // Calculate the spotlight effect (cosine-based)
    float theta = dot(normalize(-lightDirection), normalize(light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    // Attenuation based on distance
    float distance = length(light.position - fs_in.FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    //float shadow = ShadowCalculation(fs_in.FragPosLight);       
     float shadow = 0.0;
    vec3 lightCoords = fs_in.FragPosLight.xyz / fs_in.FragPosLight.w;
    if (lightCoords.z <= 1.0)
    {
       	// Get from [-1, 1] range to [0, 1] range just like the shadow map
		lightCoords = (lightCoords + 1.0) / 2.0;
		float currentDepth = lightCoords.z;
		// Prevents shadow acne
        float bias = 0.0000045;//max(biasHigh, biasLow * (1.0 - dot(norm, lightDirection))); // low = 0.0000045

		// Smoothens out the shadows
		int sampleRadius = 2;
		vec2 pixelSize = 1.0 / textureSize(shadowMap, 0);
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowMap, lightCoords.xy + vec2(x, y) * pixelSize).r;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
		// Get average shadow
		shadow /= pow((sampleRadius * 2 + 1), 2);
    }
    
    
    // Calculate ambient, diffuse, and specular components
    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff_strength * color;
    vec3 specular = light.specular * spec * vec3(texture(material.texture_specular1, fs_in.TexCoords));
    vec3 emission = vec3(texture(material.texture_emission1, fs_in.TexCoords));

    // Apply attenuation and intensity
    diffuse *= intensity * attenuation;
    specular *= intensity * attenuation;

   
  if (material.hasEmission)
    {
        return (ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow)) + emission) * light.color;

    }
        return (ambient + (diffuse * (1.0 - shadow)) + (specular * (1.0 - shadow)) ) * light.color;


}