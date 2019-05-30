attribute vec3 position; // Given vertex position in object space
attribute vec3 normal; // Given vertex normal in object space

uniform mat4 projection, modelview, normalMat; // Given scene transformation matrices

// These will be given to the fragment shader and interpolated automatically
varying vec3 normalInterp;
varying vec3 vertPos;
varying vec4 color;//Calculate the intensity in vertex shader and pass to the frag shader

uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;
uniform vec3 lightPos; // Light position

//C_ambient=Ka*la
//C_diffuse=Kd*li*max(0,normalInterp · lightDirection) where n is normal, and l is the coming light
//C_specular=Ks*limax(0,V·R)^n_shinny where V is viewing direction, and R is reflect vector  
//C=I=C_ambient+C_diffuse+C_specular
  
void main(){
  vec4 vertPos4 = modelview * vec4(position, 1.0);
  gl_Position = projection * vertPos4;
 
  vec4 normalPos4 = normalMat * vec4(normal,1.0);
  normalInterp = vec3(normalPos4);

  vertPos = vec3(vertPos4);
  
  //The incident light direction
  //vec3 lightDirection = normalize(lightPos - vertPos);
  vec3 lightDirection = normalize(vertPos-lightPos);//Ending point - Starting point = light -> Vertex
  vec3 finalColor_diffuse;
  vec3 finalColor_ambient;
  vec3 finalColor_specular;
  vec3 lightReflect;
  
  //In camera coordinate, eye is the origin
  vec3 vertToEye = normalize(vertPos-vec3(0.0));// Eye -> vertex
  
  float diffuse_factor = max(0.0,dot(-lightDirection,normalInterp));// not sure whether to use normalInterp
  
  //Using built-in function to find the reflect vector of incident light
  lightReflect = reflect(lightDirection, normalInterp);
  float specular_factor = pow(max(0.0,dot(lightReflect,-vertToEye)),shininessVal);

  finalColor_ambient = Ka*ambientColor;
  finalColor_diffuse=Kd*diffuseColor*diffuse_factor;
  finalColor_specular = Ks*specularColor*specular_factor;
  vec3 finalColor = finalColor_ambient+finalColor_diffuse+finalColor_specular;
  color = vec4(finalColor,1.0);
}