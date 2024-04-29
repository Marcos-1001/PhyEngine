#version 330 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec3 objectColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;


void main(){
    float ambient =  0.2; 
    vec3 ambientColor = ambient * lightColor;


    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diffuse = max(dot(norm, lightDir), 0.0);
    vec3 diffuseColor = diffuse * lightColor;
    vec3 viewdir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewdir, reflectDir), 0.0), 32);
    vec3 specularColor = 0.6 * spec * lightColor;


    float distance    = length(lightPos - FragPos);
    float attenuation = 1.0 / (1 + 0.09 * distance + 0.032  * (distance * distance));    
    ambientColor  *= attenuation;
    diffuseColor  *= attenuation;
    specularColor *= attenuation;

    vec3 result = (ambientColor + diffuseColor + specularColor) * objectColor;
    FragColor = vec4(result, 1.0);

}