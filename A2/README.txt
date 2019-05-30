In the renderer.js function
Add Three Lines after Line 96:
gl.enable(gl.BLEND);//Enable the blending function
gl.blendFunc(gl.SRC_ALPHA, gl.DST_ALPHA);//Alpha Test is used for the translucent object
gl.disable(gl.DEPTH_TEST);//Disable the Z-buffer

Since I have tried two bonus questions about Cook-Torrance and X-ray Shader.
Here is just for the stuff of X-ray Shader
The code for X-ray Shader:
In Bonus_vert.glsl
// Vertex shader template for the bonus question

attribute vec3 position; // Given vertex position in object space
attribute vec3 normal; // Given vertex normal in object space
attribute vec3 worldPosition; // Given vertex position in world space

uniform mat4 projection, modelview, normalMat; // Given scene transformation matrices
uniform vec3 eyePos;	// Given position of the camera/eye/viewer

// These will be given to the fragment shader and interpolated automatically
// NOTE: You may need to edit this section to add additional variables
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Vector from the eye to the vertex


void main(){
 vec4 vertPos4 = modelview * vec4(position, 1.0);
  gl_Position = projection * vertPos4;
  
  vertPos = normalize(vec3(vertPos4));
  
  vec4 normalPos4 = normalMat * vec4(normal,1.0);
  normalInterp = normalize(vec3(normalPos4));
  viewVec =normalize(vertPos-vec3(0,0,0));
}




In Bonus_frag.glsl
precision mediump float; // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
// NOTE: You may need to edit this section to add additional variables
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // Vertex position
varying vec3 viewVec; // Interpolated view vector

// uniform values remain the same across the scene
// NOTE: You may need to edit this section to add additional variables
uniform float Ka;   // Ambient reflection coefficient
uniform float Kd;   // Diffuse reflection coefficient
uniform float Ks;   // Specular reflection coefficient
uniform float shininessVal; // Shininess

// Material color
uniform vec3 ambientColor;
uniform vec3 diffuseColor;
uniform vec3 specularColor;

uniform vec3 lightPos; // Light position

uniform sampler2D uSampler;	// 2D sampler for the earth texture
uniform samplerCube envTexSampler;	// cube sampler for the environment map
void main() {
  vec3 lightDirection = normalize(vertPos - lightPos);
   
  vec3 finalColor_diffuse;
  vec3 finalColor_ambient;
  vec3 finalColor_specular;
  vec3 lightReflect;
  
  //In camera coordinate, eye is the origin
   vec3 EyeIncident = viewVec;
  
  float diffuse_factor = max(0.0,dot(-lightDirection, normalInterp));
  
  lightReflect = normalize(reflect(lightDirection,normalInterp));
  
  float specular_factor = pow(max(0.0, dot(lightReflect, -EyeIncident)),shininessVal);
  
  finalColor_ambient = Ka * ambientColor;
  finalColor_diffuse = Kd *diffuseColor*diffuse_factor;
  finalColor_specular = Ks*specularColor*specular_factor;
  vec3 finalColor = finalColor_ambient + finalColor_diffuse + finalColor_specular;
  vec4 finalColor4 = vec4(finalColor,1.0);
  finalColor4.a = 0.5;//Set Coefficient of Transparency to 0.5
  gl_FragColor = vec4(finalColor, 1.0);
}
