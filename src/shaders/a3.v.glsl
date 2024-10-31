#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;
uniform mat3 nMatrix;
uniform vec3 direction; //for directional light
uniform vec3 lightColor;
uniform vec3 spotDirectionCoords;
uniform float spotCutoff;
uniform vec3 cameraDirection;
uniform vec3 materialColor;

// attribute inputs
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

// varying outputs
layout(location = 0) out vec3 color;

void main() {
    gl_Position = mvpMatrix * vec4(vPos, 1.0);

    vec3 lightVector = normalize(-direction);
    vec3 transformedNormal = normalize(nMatrix * vNormal);

    //diffuse lighting
    float diffuseFactor = max(dot(transformedNormal, lightVector), 0.0);
    vec3 diffuse = lightColor * diffuseFactor;

    //specular lighting
    vec3 eyeDirection = normalize(cameraDirection);
    vec3 reflection = reflect(-lightVector, transformedNormal);
    float specularFactor = pow(max(dot(reflection, eyeDirection), 0.0), 30);
    vec3 specular = lightColor * specularFactor;

    //ambient lighting
    vec3 ambient = 0.3 * lightColor;

    //spotlight stuff
    vec3 lightPositionEyeSpace = (mvpMatrix * vec4(vPos, 1.0)).xyz;
    vec3 spotDirection = normalize(nMatrix * spotDirectionCoords);
    vec3 lightDirection = normalize(lightPositionEyeSpace - vec3(0.0, 0.0, 0.0)); // Assuming the spotlight is at the origin in eye space
    float angle = dot(spotDirection, lightDirection);

    if (angle > cos(radians(spotCutoff))) {
        color = vec3(1.0, 1.0, 1.0); //white if in spotlight
    }

    //add
    color = (ambient + diffuse + specular) * materialColor * color;
}