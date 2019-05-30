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
//Cook-Torrance Model -> Ks = DFG/4(V.N)(N.L)
//V and L here is the vector to the eye and the vector to the Light source
void main() {
  vec3 lightDirection = normalize(vertPos - lightPos);
   
  vec3 finalColor_diffuse;
  vec3 finalColor_ambient;
  vec3 finalColor_specular;
  vec3 lightReflect;
  
  //In camera coordinate, eye is the origin
  vec3 EyeIncident = normalize(viewVec);
  vec3 normal = normalize(normalInterp);
  
  float diffuse_factor = max(0.0,dot(-lightDirection, normal));
  
  lightReflect = normalize(reflect(lightDirection,normal));
  
  float specular_factor = pow(max(0.0, dot(lightReflect, -EyeIncident)),shininessVal);
  
  finalColor_ambient = Ka * ambientColor;
  finalColor_diffuse = Kd *diffuseColor*diffuse_factor;
  
  //Calculating Beckmann distribution
  float Pi = 3.1415926535897932384626433832795;
  float m = 0.1;//The roughness of material slope of the surface
  vec3 LE = normalize(-lightDirection - EyeIncident);//incident vector L and the viewing vector E
  vec3 H = LE;// Half angle vector
  float alpha = acos(max(0.0,dot(normal,H)));
  float D_numerator = exp(-pow(tan(alpha),2.0)/pow(m,2.0));
  float D_denominator = Pi * pow(m,2.0)* pow(cos(alpha),4.0);
  float D = D_numerator/D_denominator;
  
  //Schlick's Approximation of the Fresnel Factor
  float F = mix(Ks,1.0,pow(1.0 - max(0.0,dot(H,-EyeIncident)),5.0));
  
  //G is the geometric attenuation term
  float mid_para1=(2.0*(dot(H,normal)*dot(-EyeIncident,normal)))/dot(-EyeIncident,H);
  float mid_para2=(2.0*(dot(H,normal)*dot(-lightDirection,normal)))/dot(-EyeIncident,H);
  float mid_min = min(mid_para1,mid_para2);
  float G = min(1.0,mid_min);
  
  //Calculating the specular coefficient in Cook-Torrance
  float Ks_cook_torrance = (D*F*G)/(4.0*dot(-EyeIncident,normal)*dot(normal,-lightDirection));
  
  finalColor_specular = Ks_cook_torrance *specularColor*specular_factor;
  vec3 finalColor = finalColor_ambient + finalColor_diffuse + finalColor_specular;
  gl_FragColor = vec4(finalColor, 1.0);
}
