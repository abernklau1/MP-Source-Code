#version 410 core

// Uniform inputs
uniform mat4 mvpMatrix; // the precomputed Model-View-Projection Matrix
uniform mat4 modelMatrix;

// Add normal matrix
uniform mat3 nMatrix;

// Add Directional light uniforms
uniform vec3 direction;
uniform vec3 lightColor;

// Point light uniforms
uniform vec3 pointLightPosition;

// Spot light uniforms
uniform vec3 spotLightPosition;
uniform vec3 spotLightDirection;
// uniform vec3 spotLightColor;
uniform float spotLightCutoff;      // Cosine of the inner cutoff angle
uniform float spotLightOuterCutoff; // Cosine of the outer cutoff angle
//uniform float spotLightConstant;
//uniform float spotLightLinear;
//uniform float spotLightQuadratic;

uniform vec3 cameraDirection;
uniform vec3 cameraPosition;

uniform vec3 materialColor; // the material color for our vertex (& whole object)

// Attribute inputs
layout( location = 0 ) in vec3 vPos; // the position of this specific vertex in object space
layout( location = 1 ) in vec3 vNormal;

// Varying outputs
layout( location = 0 ) out vec3 color; // color to apply to this vertex

void main( )
{
  // Transform & output the vertex in clip space
  gl_Position = mvpMatrix * vec4( vPos, 1.0 );

  // Transform position to world space
  vec3 worldPos = vec3( modelMatrix * vec4( vPos, 1.0 ) );

  // Transform normal to world space and normalize
  vec3 transformedNormal = normalize( nMatrix * vNormal );

  // compute the vec from the light to the vertex
  vec3 lightToVertex = normalize(spotLightPosition-worldPos);

  // compute the cos of the angle between the light dir and light to vertex
  float myTheta = dot(lightToVertex, normalize(-spotLightDirection));

  // compute edge effects
  float epsilon = spotLightCutoff - spotLightOuterCutoff;
  float spotIntensity = clamp((myTheta-spotLightOuterCutoff)/epsilon, 0.0, 1.0);

  // Compute eye direction
  vec3 eyeDirection = normalize( cameraPosition - worldPos );

  // Directional light direction (should be normalized)
  vec3 lightVector = normalize( -direction );

  // Ambient component
  vec3 ambient = 0.05 * lightColor * materialColor;

  // Diffuse component for reg light
  float diffuseFactor = max( dot( transformedNormal, lightVector ), 0.0 );
  vec3 diffuse        = diffuseFactor * lightColor * materialColor;

  // Diffuse component for spotlight
  float diffuseFactor2 = max( dot( transformedNormal, lightToVertex ), 0.0 );
  vec3 diffuse2 = diffuseFactor2*vec3(1.0,1.0,1.0)*materialColor;



  // Specular component
  vec3 reflection      = reflect( -lightVector, transformedNormal );
  float shininess      = 16.0; // Adjust as needed
  float specularFactor = pow( max( dot( reflection, eyeDirection ), 0.0 ), shininess );
  vec3 specular        = specularFactor * lightColor; // Specular color doesn't depend on material color

  // Specular for spotlight
  vec3 reflection2      = reflect( -lightToVertex, transformedNormal );
  float shininess2      = 24.0; // Adjust as needed
  float specularFactor2 = pow( max( dot( reflection2, eyeDirection ), 0.0 ), shininess2 ); // should this be eye direction
  vec3 specular2 = specularFactor2 * vec3(1.0,1.0,1.0); // Specular color doesn't depend on material color

  //add spotlight intensity
  if(myTheta>=spotLightOuterCutoff){
    diffuse2 = diffuse2*spotIntensity*10;
    specular2 = specular2*spotIntensity*10;
  }else{
    diffuse2= vec3(0.0,0.0,0.0);
    specular2=vec3(0.0,0.0,0.0);
  }

  vec3 lightToVertex2 = normalize(pointLightPosition-worldPos);

  // Specular for spotlight
  vec3 reflection3      = reflect( -lightToVertex2, transformedNormal );
  float shininess3      = 24.0; // Adjust as needed
  float specularFactor3 = pow( max( dot( reflection3, eyeDirection ), 0.0 ), shininess3 ); // should this be eye direction
  vec3 specular3 = specularFactor3 * vec3(1.0,1.0,1.0); // Specular color doesn't depend on material color

  // Diffuse component for spotlight
  float diffuseFactor3 = max( dot( transformedNormal, lightToVertex2 ), 0.0 );
  vec3 diffuse3 = diffuseFactor2*vec3(1.0,1.0,1.0)*materialColor;




  // Combine components
  color = ambient + diffuse + specular + specular3 + diffuse3 + specular2 + diffuse2;
}