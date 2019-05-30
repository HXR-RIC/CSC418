precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position

uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 lightPos; // Light position

//Cel Shading/Toon Shading is non-photorealistic rendering to make the image more like cartoon.
//To achieve the cel shading, we need to discretize the diffuse factor and specular factor based on phong //shading
void main() {
  // Your solution should go here
  // Only the ambient colour calculations have been provided as an example.
  vec3 lightDirection = normalize(vertPos - lightPos);
   
  vec3 finalColor_diffuse;
  vec3 finalColor_ambient;
  vec3 finalColor_specular;
  vec3 lightReflect;
  float colorSteps = 5.0;
  //In camera coordinate, eye is the origin
  vec3 vertToEye = normalize(vertPos-vec3(0.0));
  float cel_effect = 0.5;//The coefficient which showwing the effect how deep 3D perform in cartoon-like
  
  float diffuse_factor = max(0.0,dot(-lightDirection, normalInterp));
  diffuse_factor = smoothstep(0.0,1.0,diffuse_factor); //smoothstep to perform a smooth interpolation from 0-1
  float cel_diffuse = floor(diffuse_factor*colorSteps)/colorSteps;//Divide color into separte color steps
  
  //Do the linear interpolation to associate cel and diffuse together
  diffuse_factor = mix(diffuse_factor, cel_diffuse, cel_effect); 
  
  lightReflect = normalize(reflect(lightDirection,normalInterp));
  
  //Doing the same process as the diffuse_factor
  float specular_factor = pow(max(0.0, dot(lightReflect, -vertToEye)),shininessVal);
  specular_factor = smoothstep(0.0,1.0,specular_factor);
  float cel_specular = floor(specular_factor*colorSteps)/colorSteps;
  specular_factor = mix(specular_factor, cel_specular,cel_effect);
  
  finalColor_ambient = Ka * ambientColor;
  finalColor_diffuse = Kd *diffuseColor*diffuse_factor;
  finalColor_specular = Ks *specularColor*specular_factor;
  vec3 finalColor = finalColor_ambient + finalColor_diffuse + finalColor_specular;
  gl_FragColor = vec4(finalColor, 1.0);
}