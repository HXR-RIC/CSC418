attribute vec3 position; // Given vertex position in object space
attribute vec3 normal; // Given vertex normal in object space
attribute vec3 worldPosition; // Given vertex position in world space

uniform mat4 projection, modelview, normalMat; // Given scene transformation matrices

// These will be given to the fragment shader and interpolated automatically
varying vec3 normalInterp; // Normal
varying vec3 vertPos; // 
varying vec3 viewVec; // Vector from the eye to the vertex

void main() {
  vec4 vertPos4 = modelview * vec4(position, 1.0);
  gl_Position = projection * vertPos4;
  
  vertPos = vec3(vertPos4);
  
  vec4 normalPos4 = normalMat * vec4(normal,1.0);
  normalInterp = vec3(normalPos4);
  
  viewVec =vertPos-vec3(projection * modelview * vec4(worldPosition,1.0));
  }
