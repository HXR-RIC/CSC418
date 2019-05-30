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
  
  vertPos = vec3(vertPos4);
  
  vec4 normalPos4 = normalMat *vec4(normal, 1.0);
  normalInterp = vec3(normalPos4);
  
  vec4 eyePos4 = modelview * vec4(eyePos,1.0);
  vec3 eyePos3 = vec3(eyePos4);
  viewVec =vertPos-eyePos3;
}
