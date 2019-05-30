precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // View vector (eye to fragment)

uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
// HINT: Use the environment map as the ambient color
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 lightPos; // Light position

uniform samplerCube envTexSampler; // A GLSL sampler represents a single texture. A samplerCube can be used to sample a cubemap texture.

void main() {
  vec3 lightDirection = normalize(vertPos - lightPos);
  vec3 finalColor_ambient;
  vec3 finalColor_diffuse;
  vec3 finalColor_specular;
  vec3 lightReflect;
  
  //In camera coordinate, eye is the origin
  vec3 EyeIncident = normalize(viewVec);
  vec3 normal = normalize(normalInterp);
  float diffuse_factor = max(0.0,dot(-lightDirection, normal));
  vec3 EyeReflectR = reflect(EyeIncident,normal);
  vec4 environment_color = textureCube(envTexSampler,EyeReflectR);
  vec3 environment_color3 = vec3(environment_color);
  lightReflect = reflect(lightDirection,normal);
  
  float specular_factor = pow(max(0.0, dot(lightReflect, -EyeIncident)),shininessVal);
  
  finalColor_ambient = Ka*environment_color3;
  finalColor_diffuse = Kd *diffuseColor*diffuse_factor;
  finalColor_specular = Ks *specularColor*specular_factor;
  vec3 finalColor_DS =  finalColor_ambient+finalColor_diffuse + finalColor_specular;
  vec4 finalColor_DS4 = vec4(finalColor_DS,1.0);
  //vec4 finalColor = Ka*environment_color + finalColor_DS4;
  gl_FragColor = finalColor_DS4;
}
