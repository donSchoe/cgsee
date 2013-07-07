#version 150 core

//Phong Lighting model with two light sources

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D emissionTex;
uniform sampler2D ambientTex;

struct light_t
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
    vec3 direction;
    float distance;
    vec3 attenuation;
};

light_t lightFromMatrix(mat4 mat, vec3 vec)
{
    light_t light;

    light.ambient = mat[0];
    light.diffuse = mat[1];
    light.specular = mat[2];
    light.shininess = mat[3].w;
    light.attenuation= mat[3].xyz;
    light.direction = normalize(vec);
    light.distance = length(vec);

    return light;
}

vec4 phongLighting(vec3 n, vec3 v_pos, vec3 cameraposition, vec3 lightdir1, vec3 lightdir2, mat4 light1, mat4 light2, vec4 lightambientglobal, vec2 texc)
{
    const int numLights = 2;
    light_t lights[numLights];

    //declaration of attenuation and dotProduct of normal and lightdirection
    //diffuse and specular calculation
    float attenuation[2];
    float nxDir[2];
    vec4 diffuse[2];
    vec4 specular[2];

    // To be replaced by lightning system with variable light-count
    mat4 lightMats[numLights];
    lightMats[0] = light1;
    lightMats[1] = light2;

    vec3 lightDirs[2];
    lightDirs[0] = lightdir1;
    lightDirs[0] = lightdir2;
    // <<

    // calculate color from lighting coeff and material coeff
    vec4 diffuseColor  = texture2D(diffuseTex, texc);
    vec4 emissionColor = texture2D(emissionTex, texc);
    vec4 ambientColor  = texture2D(ambientTex, texc);
    vec4 specularColor = texture2D(specularTex, texc);

    vec4 color = lightambientglobal * ambientColor + emissionColor;

    for(int l = 0; l < numLights; l++) {
        lights[l] = lightFromMatrix(lightMats[l], lightDirs[l]);

        specular[l] = vec4(0.0);

        //calculate attenuation and dotProduct of normal -> calculate diffuse
        attenuation[l]= min(1 / (lights[l].attenuation.x*lights[l].distance + lights[l].attenuation.y*lights[l].distance + lights[l].attenuation.z*lights[l].distance), 1);
        nxDir[l] = max(0.0 , dot(n , lights[l].direction));
        diffuse[l] = lights[l].diffuse * nxDir[l] * attenuation[l];

        //calculate specular
        if(nxDir[l] != 0.0)
        {
            vec3 cameraVector = normalize(cameraposition - v_pos);
            vec3 halfVector = normalize(lights[l].direction + cameraVector);
            float nxHalf = max(0.0 , dot(n , halfVector));
            float specularPower = max(pow(nxHalf , lights[l].shininess),0.0);
            specular[l] = lights[l].specular * specularPower * attenuation[l];
        }

        color += lights[l].ambient * ambientColor;
        color += attenuation[l] * (diffuse[l] * diffuseColor + specular[l] * specularColor);
    }

    return color;
}
