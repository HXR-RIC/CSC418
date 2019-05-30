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

void main() {
  // Your solution should go here.
  // Only the ambient colour calculations have been provided as an example.
  
  vec3 lightDirection = normalize(vertPos - lightPos);
   
  vec3 finalColor_diffuse;
  vec3 finalColor_ambient;
  vec3 finalColor_specular;
  vec3 lightReflect;
  
  //In camera coordinate, eye is the origin
  vec3 vertToEye = normalize(vertPos-vec3(0.0));
  
  float diffuse_factor = max(0.0,dot(-lightDirection, normalInterp));
  
  lightReflect = normalize(reflect(lightDirection,normalInterp));
  
  float specular_factor = pow(max(0.0, dot(lightReflect, -vertToEye)),shininessVal);
  
  finalColor_ambient = Ka * ambientColor;
  finalColor_diffuse = Kd *diffuseColor*diffuse_factor;
  finalColor_specular = Ks *specularColor*specular_factor;
  vec3 finalColor = finalColor_ambient + finalColor_diffuse + finalColor_specular;
  gl_FragColor = vec4(finalColor, 1.0);
}
