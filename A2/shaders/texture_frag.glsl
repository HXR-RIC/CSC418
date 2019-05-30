precision mediump float;  // It is required to set a floating point precision in all fragment shaders.

// Interpolated values from vertex shader
varying highp vec2 texCoordInterp;

// uniform values are the same across the scene
uniform sampler2D uSampler;	// A GLSL sampler represents a single texture. A sampler2D can be used to sample a 2D texture.

void main() {
  // Your solution should go here.
  // The model is currently rendered in black
  
  
  //texCoordIt(0.5 + atan(vertPos[2],vertPos[0])/(2*pi),0.5-asin(vertPos[1])/pi);
  //The texture stored in openGL is upside down, in order to flipp it back, we need to 1-texCoordInterp
  gl_FragColor = texture2D(uSampler,1.0-texCoordInterp);
}