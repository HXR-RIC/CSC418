/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
            CSC418, SPRING 2005

        Implementations of functions in raytracer.h, 
        and the main function which specifies the 
        scene to be rendered.   

***********************************************************/


#include "raytracer.h"
#include "bmp_io.h"
#include <cmath>
#include <iostream>
#include <cstdlib>
using namespace std;
Raytracer::Raytracer() : _lightSource(NULL) {
    _root = new SceneDagNode();
}

Raytracer::~Raytracer() {
    delete _root;
}

SceneDagNode* Raytracer::addObject( SceneDagNode* parent, 
        SceneObject* obj, Material* mat ) {
    SceneDagNode* node = new SceneDagNode( obj, mat );
    node->parent = parent;
    node->next = NULL;
    node->child = NULL;
    
    // Add the object to the parent's child list, this means
    // whatever transformation applied to the parent will also
    // be applied to the child.
    if (parent->child == NULL) {
        parent->child = node;
    }
    else {
        parent = parent->child;
        while (parent->next != NULL) {
            parent = parent->next;
        }
        parent->next = node;
    }
    
    return node;;
}

LightListNode* Raytracer::addLightSource( LightSource* light ) {
    LightListNode* tmp = _lightSource;
    _lightSource = new LightListNode( light, tmp );
    return _lightSource;
}

void Raytracer::rotate( SceneDagNode* node, char axis, double angle ) {
    Matrix4x4 rotation;
    double toRadian = 2*M_PI/360.0;
    int i;
    
    for (i = 0; i < 2; i++) {
        switch(axis) {
            case 'x':
                rotation[0][0] = 1;
                rotation[1][1] = cos(angle*toRadian);
                rotation[1][2] = -sin(angle*toRadian);
                rotation[2][1] = sin(angle*toRadian);
                rotation[2][2] = cos(angle*toRadian);
                rotation[3][3] = 1;
            break;
            case 'y':
                rotation[0][0] = cos(angle*toRadian);
                rotation[0][2] = sin(angle*toRadian);
                rotation[1][1] = 1;
                rotation[2][0] = -sin(angle*toRadian);
                rotation[2][2] = cos(angle*toRadian);
                rotation[3][3] = 1;
            break;
            case 'z':
                rotation[0][0] = cos(angle*toRadian);
                rotation[0][1] = -sin(angle*toRadian);
                rotation[1][0] = sin(angle*toRadian);
                rotation[1][1] = cos(angle*toRadian);
                rotation[2][2] = 1;
                rotation[3][3] = 1;
            break;
        }
        if (i == 0) {
            node->trans = node->trans*rotation;     
            angle = -angle;
        } 
        else {
            node->invtrans = rotation*node->invtrans; 
        }   
    }
}

void Raytracer::translate( SceneDagNode* node, Vector3D trans ) {
    Matrix4x4 translation;
    
    translation[0][3] = trans[0];
    translation[1][3] = trans[1];
    translation[2][3] = trans[2];
    node->trans = node->trans*translation;  
    translation[0][3] = -trans[0];
    translation[1][3] = -trans[1];
    translation[2][3] = -trans[2];
    node->invtrans = translation*node->invtrans; 
}

void Raytracer::scale( SceneDagNode* node, Point3D origin, double factor[3] ) {
    Matrix4x4 scale;
    
    scale[0][0] = factor[0];
    scale[0][3] = origin[0] - factor[0] * origin[0];
    scale[1][1] = factor[1];
    scale[1][3] = origin[1] - factor[1] * origin[1];
    scale[2][2] = factor[2];
    scale[2][3] = origin[2] - factor[2] * origin[2];
    node->trans = node->trans*scale;    
    scale[0][0] = 1/factor[0];
    scale[0][3] = origin[0] - 1/factor[0] * origin[0];
    scale[1][1] = 1/factor[1];
    scale[1][3] = origin[1] - 1/factor[1] * origin[1];
    scale[2][2] = 1/factor[2];
    scale[2][3] = origin[2] - 1/factor[2] * origin[2];
    node->invtrans = scale*node->invtrans; 
}

Matrix4x4 Raytracer::initInvViewMatrix( Point3D eye, Vector3D view, 
        Vector3D up ) {
    Matrix4x4 mat; 
    Vector3D w;
    view.normalize();
    up = up - up.dot(view)*view;
    up.normalize();
    w = view.cross(up);

    mat[0][0] = w[0];
    mat[1][0] = w[1];
    mat[2][0] = w[2];
    mat[0][1] = up[0];
    mat[1][1] = up[1];
    mat[2][1] = up[2];
    mat[0][2] = -view[0];
    mat[1][2] = -view[1];
    mat[2][2] = -view[2];
    mat[0][3] = eye[0];
    mat[1][3] = eye[1];
    mat[2][3] = eye[2];

    return mat; 
}


void Raytracer::computeTransforms( SceneDagNode* node )
{
    SceneDagNode *childPtr;
    if (node->parent != NULL )
    {
        node->modelToWorld = node->parent->modelToWorld*node->trans;
        node->worldToModel = node->invtrans*node->parent->worldToModel; 
    }
    else
    {
        node->modelToWorld = node->trans;
        node->worldToModel = node->invtrans; 
    }
    // Traverse the children.
    childPtr = node->child;
    while (childPtr != NULL) {
        computeTransforms(childPtr);
        childPtr = childPtr->next;
    }



}

void Raytracer::traverseScene( SceneDagNode* node, Ray3D& ray ) {
    SceneDagNode *childPtr;

    // Applies transformation of the current node to the global
    // transformation matrices.
    if (node->obj) {
        // Perform intersection.
        if (node->obj->intersect(ray, node->worldToModel, node->modelToWorld)) {
            ray.intersection.mat = node->mat;
        }
    }
    // Traverse the children.
    childPtr = node->child;
    while (childPtr != NULL) {
        traverseScene(childPtr, ray);
        childPtr = childPtr->next;
    }

}

void Raytracer::computeShading( Ray3D& ray ) {
    LightListNode* curLight = _lightSource;
    double bias = 0.001;
    bool isShadow = false;
    for (;;) {
        if (curLight == NULL) break;
        // Each lightSource provides its own shading function.

        // Implement shadows here if needed.
        Ray3D shadowRay;
        shadowRay.dir = curLight ->light->get_position() - ray.intersection.point;
        shadowRay.origin = ray.intersection.point + bias *shadowRay.dir;
        double hitToLight = shadowRay.dir.length();
        traverseScene(_root,shadowRay);
        if (!shadowRay.intersection.none && shadowRay.intersection.t_value <= hitToLight)
            isShadow = true;
        if (isShadow) {
            Colour diff = curLight->light->get_diffuse();
            //Colour temp(0,0,0);
            //LightListNode* nextLight = curLight->next;
            //if(nextLight != NULL)temp =0.5*0.2 * nextLight->light->get_diffuse() * ray.intersection.mat->diffuse;
            ray.col = 0.2 * ray.intersection.mat->diffuse * diff;
            //ray.col = ray.col + temp;
            //ray.col = 0.5 * ray.col;
        }
        else 
            curLight->light->shade(ray);

        curLight = curLight->next;
    }
}

void Raytracer::initPixelBuffer() {
    int numbytes = _scrWidth * _scrHeight * sizeof(unsigned char);
    _rbuffer = new unsigned char[numbytes];
    std::fill_n(_rbuffer, numbytes,0);
    _gbuffer = new unsigned char[numbytes];
    std::fill_n(_gbuffer, numbytes,0);
    _bbuffer = new unsigned char[numbytes];
    std::fill_n(_bbuffer, numbytes,0);
}

void Raytracer::flushPixelBuffer( std::string file_name ) {
    bmp_write( file_name.c_str(), _scrWidth, _scrHeight, _rbuffer, _gbuffer, _bbuffer );
    delete _rbuffer;
    delete _gbuffer;
    delete _bbuffer;
}

void Raytracer::fresnel(const Ray3D &ray, double &kr) {
    double NdotI = ray.dir.dot(ray.intersection.normal);
    //clamp function
    if (NdotI < -1)
        NdotI = -1;
    if (NdotI > 1)
        NdotI = 1;
    double n1 = 1.0; //air
    double n2 = ray.intersection.mat->refraction_coef;//material index

    if (NdotI > 0)//inside to air
        swap(n1, n2);
    //float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi)); <-computed by snell's law
    double sinTheta = (n1 / n2) * sqrt(fmax(0.0, 1 - NdotI*NdotI));
    if (sinTheta >= 1)// Total Internal Reflection
        kr = 1;
    else {
        double cosTheta = sqrt(fmax(0.0, 1 - sinTheta*sinTheta));//Triangle Geometry
        NdotI = abs(NdotI);
        double rs = ((n2*NdotI)-(n1*cosTheta))/((n2*NdotI) + (n1*cosTheta));
        double rp = ((n1*NdotI)-(n2*cosTheta))/((n1*NdotI) + (n2*cosTheta));
        kr = (rs*rs + rp*rp) / 2;
    }
}

Colour Raytracer::shadeRay( Ray3D& ray, int bounces) {
    Colour col(0.0, 0.0, 0.0);
    Colour reflectedCol(0.0,0.0,0.0);
    Colour refractedCol(0.0,0.0,0.0);
    double bias = 0.001;
    traverseScene(_root, ray); 

    // Don't bother shading if the ray didn't hit 
    // anything.
    if (ray.intersection.none) {
        //computeShading(ray); don't compute if there are no hits
        return col;
    }
    computeShading(ray);
    if(bounces != 0){
        Vector3D incident = ray.intersection.point - ray.origin;
        incident.normalize();
        Vector3D normal = ray.intersection.normal;


        //reflection
        if(ray.intersection.mat->specular_exp > 0 && ray.intersection.mat->reflection_coef >0){
                Ray3D reflectedRay;
                reflectedRay.dir = incident - 2 * ((normal.dot(incident)) * normal);
                reflectedRay.dir.normalize();
                double r1, r2;
                double pdf = 1 / (2 * M_PI);
                r1 = rand() / RAND_MAX;
                r2 = rand() / RAND_MAX;
                double sintheta = sqrt(1 - r1*r1);
                double phi = 2 * M_PI *r2;
                double x = sintheta * cos(phi);
                double y = sintheta * sin(phi);
                double z = sqrt(1 - x*x - y*y);
                //build local coordinate system
                Vector3D u = ray.intersection.normal.cross(reflectedRay.dir);
                u.normalize();
                Vector3D v = reflectedRay.dir.cross(u);
                v.normalize();
                //Transform from local to world coordinates
                reflectedRay.dir = (x*u + y*v + z*reflectedRay.dir);//now the new ray direction is in the world coordinate
                reflectedRay.dir.normalize();

                    //the origin of the reflected light has equation R.o = P + bias + R.dir
                reflectedRay.origin = ray.intersection.point + bias * reflectedRay.dir;
                    //call recursively until it reaches base case
                reflectedCol = (1 / pdf)*r1*shadeRay(reflectedRay,bounces-1);
                //colour = colour + reflection Coefficient * specular colour * reflected colour
                ray.col = ray.col + ray.intersection.mat->reflection_coef * ray.intersection.mat->specular * reflectedCol;

        }
        //sinθ1/sinθ2=η2/η1.
        if (ray.intersection.mat->refraction_coef > 0) {
            double kr;
            fresnel(ray, kr);
            Vector3D rayDirection = ray.dir;
            Vector3D rayNormal = ray.intersection.normal;
            double NdotRayDir = rayNormal.dot(rayDirection);
            double air_index = 1.0;
            double material_refraction_index = ray.intersection.mat->refraction_coef;
            double airToglass = air_index / material_refraction_index;
            double glassToair = material_refraction_index / air_index;
            bool isAirToGlass = false;
            Ray3D refracRay;
            if (kr < 1) {
                double k;
                if (NdotRayDir < 0) {//from air to glass
                    isAirToGlass = true;
                    NdotRayDir = -NdotRayDir;
                    k = 1 - airToglass * airToglass * (1 - NdotRayDir*NdotRayDir);
                }
                else {//from glass to air
                    rayNormal = -rayNormal;
                    k = 1 - glassToair*glassToair*(1 - NdotRayDir*NdotRayDir);
                }
                if (k >= 0.0) {
                    if (isAirToGlass) {
                        refracRay.dir = airToglass * rayDirection + (airToglass*NdotRayDir - sqrt(k))*rayNormal;
                        refracRay.dir.normalize();
                        refracRay.origin = ray.origin + bias*ray.dir;
                        refractedCol = shadeRay(refracRay, bounces - 1);
                    }
                    else {
                        refracRay.dir = glassToair * rayDirection + (glassToair * NdotRayDir - sqrt(k))*rayNormal;
                        refracRay.dir.normalize();
                        refracRay.origin = ray.origin + bias*ray.dir;
                        refractedCol = shadeRay(refracRay, bounces - 1);
                    }
                    ray.col = ray.col + (1-kr)*refractedCol;
                }
            }
            
        }
    }
            col = ray.col;
            col.clamp();
            return col; 
}   

void Raytracer::render( int width, int height, Point3D eye, Vector3D view, 
        Vector3D up, double fov, std::string fileName ) {
    computeTransforms(_root);
    Matrix4x4 viewToWorld;
    _scrWidth = width;
    _scrHeight = height;
    double focalPlaneDistance=6.0;
    float r = 1.0f;
    //jittering
    //Vector3D uLens = Vector3D(1,0,0);
    //Vector3D vLens = Vector3D(0,1,0);
    double factor = (double(height)/2)/tan(fov*M_PI/360.0);

    initPixelBuffer();
    viewToWorld = initInvViewMatrix(eye, view, up);

    // Construct a ray for each pixel.
    for (int i = 0; i < _scrHeight; i++) {
        for (int j = 0; j < _scrWidth; j++) {
            // Sets up ray origin and direction in view space, 
            // image plane is at z = -1.
            Point3D origin(0, 0, 0);
            Point3D imagePlane;
            Point3D focalPoint;
            imagePlane[2] = -1;
            //focalPoint[2] = origin[2] + imagePlane[2] * focalPlaneDistance;

            Colour col;
            Colour col_fov;
            int N = 4; // Number of samples
            //implement anti-aliasing with 16 rays.
            for(int u = -N/2; u<N/2;u++){
                for(int v = -N/2; v<N/2; v++){
                    //calculate image plane
                    imagePlane[0] = (-double(width)/2 + 0.5 +u*0.25 + j)/factor;
                    imagePlane[1] = (-double(height)/2 + 0.5 +v*0.25 + i)/factor;
            /*
                    //focal plane = C + t*D
                    focalPoint[0] = origin[0] + imagePlane[0] * focalPlaneDistance;
                    focalPoint[1] = origin[1] + imagePlane[1] * focalPlaneDistance;
                    //implementing depth of field with 3 random rays
                    //int R = 2;
                    //for(int p=0;p<R;p++){
                        float du = rand()/float(RAND_MAX)/33; //generating random number <1
                        float dv = rand()/float(RAND_MAX)/33;
                        //ray start using jittering
                        //update: don't use jittering, just randomize O+r 
                        //where r=random number between 0 and radius of aperture
                        Point3D start = origin; // + double(du) * Vector3D(1,0,0) + double(dv) * Vector3D(0,1,0);
                        Ray3D ray;
                        ray.origin = viewToWorld * start;
                        ray.dir = (viewToWorld * (focalPoint - start));
                        ray.dir.normalize();

                        Colour tempCol = shadeRay(ray,0);
                        col_fov = col_fov + tempCol;
                    //}
                    //col_fov = 1/R * col_fov;
                    col = col + col_fov;
                }
            }
            */

                    Ray3D ray;
               
                    //move the view origin to world space
                    ray.origin = viewToWorld * imagePlane;
                    //ray direction = imagePlane - origin also have to normalize
                    Point3D sanityCheck = viewToWorld * origin;
                    ray.dir = ray.origin-sanityCheck;
                    ray.dir.normalize();

                    Ray3D new_ray;
                    double random_num = double(rand())/double(RAND_MAX);
                    Point3D C = ray.origin + focalPlaneDistance * ray.dir;
                    new_ray.origin = ray.origin + random_num*Vector3D(1,0,0)+random_num*Vector3D(0,1,0);
                    new_ray.dir = C - new_ray.origin;
                    new_ray.dir.normalize();

                    col = col + shadeRay(ray,2); 
                    }
            }
            col = (1.0/pow(N,2)) * col; //average out
                 
            _rbuffer[i*width+j] = int(col[0]*255);
            _gbuffer[i*width+j] = int(col[1]*255);
            _bbuffer[i*width+j] = int(col[2]*255);
        }
        if (i%20 == 0){
            std::cout << "Row "<< i << " shaded.\n";
        }
    }

    flushPixelBuffer(fileName);
}

