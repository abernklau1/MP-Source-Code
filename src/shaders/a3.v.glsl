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
uniform vec3 pointLightColor;
uniform float pointLightConstant;
uniform float pointLightLinear;
uniform float pointLightQuadratic;

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




  // Combine components
  color = ambient + diffuse + specular + specular2 + diffuse2;
}

// void main( )
// {
//   // Transform & output the vertex in clip space
//   gl_Position = mvpMatrix * vec4( vPos, 1.0 );

//   // Compute Light vector
//   vec3 lightVector = normalize( -direction );

//   // Transform normal vector
//   vec3 transformedNormal = normalize( nMatrix * vNormal );

//   // Compute eye direction
//   vec3 eyeDirection = normalize( cameraDirection );

//   // Perform diffuse calculation
//   float diffuseFactor = max( dot( transformedNormal, lightVector ), 0.0 );
//   vec3 diffuse        = lightColor * diffuseFactor;

//   // Perform specular calculation
//   vec3 reflection      = reflect( -lightVector, transformedNormal );
//   float specularFactor = pow( max( dot( reflection, eyeDirection ), 0.0 ), 30 );
//   vec3 specular        = lightColor * specularFactor;

//   // Perform ambient calculation
//   vec3 ambient = 0.3 * lightColor;

//   // Assign the color for this vertex
//   color = ( ambient + specular + diffuse ) * materialColor;
// }

// void main( )
// {
//   // Transform & output the vertex in clip space
//   gl_Position = mvpMatrix * vec4( vPos, 1.0 );

//   // Transform normal vector
//   vec3 transformedNormal = normalize( nMatrix * vNormal );

//   // Compute eye direction
//   vec3 eyeDirection = normalize( cameraDirection );

//   // --- Directional Light Calculations ---
//   vec3 lightVector = normalize( -direction );

//   // Diffuse component
//   float diffuseFactor = max( dot( transformedNormal, lightVector ), 0.0 );
//   vec3 diffuse        = lightColor * diffuseFactor;

//   // Specular component
//   vec3 reflection      = reflect( -lightVector, transformedNormal );
//   float specularFactor = pow( max( dot( reflection, eyeDirection ), 0.0 ), 30 );
//   vec3 specular        = lightColor * specularFactor;

//   // Ambient component
//   vec3 ambient = 0.3 * lightColor;

//   // Combine directional light components
//   vec3 directionalLight = ( ambient + diffuse + specular ) * materialColor;

//   // --- Point Light Calculations ---
//   vec3 pointLightVector = pointLightPosition - vPos;
//   float pointDistance   = length( pointLightVector );
//   vec3 pointLightDir    = normalize( pointLightVector );

//   // Diffuse component
//   float pointDiffuseFactor = max( dot( transformedNormal, pointLightDir ), 0.0 );
//   vec3 pointDiffuse        = pointLightColor * pointDiffuseFactor;

//   // Specular component
//   vec3 pointReflection      = reflect( -pointLightDir, transformedNormal );
//   float pointSpecularFactor = pow( max( dot( pointReflection, eyeDirection ), 0.0 ), 30 );
//   vec3 pointSpecular        = pointLightColor * pointSpecularFactor;

//   // Attenuation
//   float pointAttenuation = 1.0 / ( pointLightConstant + pointLightLinear * pointDistance + pointLightQuadratic * ( pointDistance * pointDistance ) );

//   // Combine point light components
//   vec3 pointLightResult = ( pointDiffuse + pointSpecular ) * pointAttenuation * materialColor;

//   // --- Spot Light Calculations ---
//   vec3 spotLightVector = spotLightPosition - vPos;
//   float spotDistance   = length( spotLightVector );
//   vec3 spotLightDir    = normalize( spotLightVector );

//   // Angle between spot light direction and the vector to the fragment
//   float theta = dot( spotLightDir, normalize( -spotLightDirection ) );

//   // Intensity based on the spot light's cutoff
//   float epsilon   = spotLightCutoff - spotLightOuterCutoff;
//   float intensity = clamp( ( theta - spotLightOuterCutoff ) / epsilon, 0.0, 1.0 );

//   // Diffuse component
//   float spotDiffuseFactor = max( dot( transformedNormal, spotLightDir ), 0.0 );
//   vec3 spotDiffuse        = spotLightColor * spotDiffuseFactor;

//   // Specular component
//   vec3 spotReflection      = reflect( -spotLightDir, transformedNormal );
//   float spotSpecularFactor = pow( max( dot( spotReflection, eyeDirection ), 0.0 ), 30 );
//   vec3 spotSpecular        = spotLightColor * spotSpecularFactor;

//   // Attenuation
//   float spotAttenuation = 1.0 / ( spotLightConstant + spotLightLinear * spotDistance + spotLightQuadratic * ( spotDistance * spotDistance ) );

//   // Combine spot light components
//   vec3 spotLightResult = ( spotDiffuse + spotSpecular ) * spotAttenuation * intensity * materialColor;

//   // --- Final Color Calculation ---
//   color = directionalLight + pointLightResult + spotLightResult;
// }