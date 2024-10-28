#version 410 core

// uniform inputs
uniform mat4 mvpMatrix;                 // the precomputed Model-View-Projection Matrix
// TODO #D: add normal matrix
uniform mat3 nMatrix;

// TODO #A: add light uniforms
uniform vec3 direction;
uniform vec3 lightColor;

uniform vec3 cameraDirection;

uniform vec3 materialColor;             // the material color for our vertex (& whole object)

// attribute inputs
layout(location = 0) in vec3 vPos; // the position of this specific vertex in object space
layout(location = 1) in vec3 vNormal;


// varying outputs
layout(location = 0) out vec3 color;    // color to apply to this vertex

void main() {
    // transform & output the vertex in clip space
    gl_Position = mvpMatrix * vec4(vPos, 1.0);

    // TODO #B: compute Light vector
		vec3 lightVector = normalize(-direction);

    // TODO #E: transform normal vector
		vec3 transformedNormal = normalize(nMatrix * vNormal);

    // TODO #F: perform diffuse calculation
		float diffuseFactor = max(dot(transformedNormal, lightVector), 0.0);
		vec3 diffuse = lightColor * diffuseFactor;

    // Perform specular calculation
    vec3 eyeDirection = normalize(cameraDirection);
    vec3 reflection = reflect(-lightVector, transformedNormal);
    float specularFactor = pow(max(dot(reflection, eyeDirection), 0.0), 30);
    vec3 specular = lightColor * specularFactor;

    // Perform ambient calculation
    vec3 ambient = diffuseFactor * lightColor;

    // TODO #G: assign the color for this vertex
    color = (ambient + specular + diffuse) * materialColor;
}