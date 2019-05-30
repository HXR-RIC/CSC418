/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
            CSC418, SPRING 2005

        implements light_source.h

***********************************************************/

#include <cmath>
#include "light_source.h"

void PointLight::shade( Ray3D& ray ) {
    // TODO: implement this function to fill in values for ray.col 
    // using phong shading.  Make sure your vectors are normalized, and
    // clamp colour values to 1.0.
    //
    // It is assumed at this point that the intersection information in ray 
    // is available.  So be sure that traverseScene() is called on the ray 
    // before this function.  
    Vector3D N = ray.intersection.normal;
    N.normalize(); //normalize

    Vector3D vecLight = _pos-ray.intersection.point; 
    vecLight.normalize(); //normalize

    //since R+L = (2(NdotL))N
    //we can find R be (2(NdotL))N - L
    Vector3D R = (2.0*N.dot(vecLight))*N - vecLight;
    vecLight.normalize(); //normalize

    Vector3D viewPoint = -ray.dir;
    viewPoint.normalize(); //normalize

    double specExp = (*ray.intersection.mat).specular_exp;

    Colour ambient_Ill = ray.intersection.mat->ambient * _col_ambient;
    Colour diffuse_Ill =  fmax(0,N.dot(vecLight))*(ray.intersection.mat->diffuse) * _col_diffuse;
    Colour specular_Ill = pow(fmax(0,R.dot(viewPoint)),specExp) * ray.intersection.mat->specular * _col_specular;
    ray.col = ambient_Ill + diffuse_Ill + specular_Ill;
    ray.col.clamp();
}

