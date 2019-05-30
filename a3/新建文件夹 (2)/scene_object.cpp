/***********************************************************
     Starter code for Assignment 3

     This code was originally written by Jack Wang for
		    CSC418, SPRING 2005

		implements scene_object.h

***********************************************************/

#include <cmath>
#include <iostream>
#include "scene_object.h"

using namespace std;

bool UnitSquare::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSquare, which is
	// defined on the xy-plane, with vertices P1(0.5, 0.5, 0), 
	// P2(-0.5, 0.5, 0), P3(-0.5, -0.5, 0), P4(0.5, -0.5, 0), and normal
	// (0, 0, 1).
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
    
    Point3D  originModel    = worldToModel * ray.origin;
    Vector3D directionModel = worldToModel * ray.dir;
    
    //unitSquare can be looked as a composition of triangle, therefore to calculate the intersection of square is to calculate the intersetion of triangle
    //parameteric form of ray = q + r*t, t>0  where q is the pixel and r is the ray direction
	if (ray.intersection.none) {//Only the ray with none intersection can pass in the condition
		//Case 1: directionModel * normal if ray is parallel to 
		Vector3D normal(0, 0, 1); //due to the the unitSquare is in xy Plane
		double NdotDirection = normal.dot(directionModel);
		if (NdotDirection == 0) {
			return false;
		}
		Point3D p1(0.5, 0.5, 0);

		double t = (p1 - originModel).dot(normal) / NdotDirection;

		if (t < 0) {
			return false;
		}
		Point3D p2(-0.5, 0.5, 0);
		Point3D p3(-0.5, -0.5, 0);
		Point3D p4(0.5, -0.5, 0);
		Vector3D triangle_edge0 = p2 - p1;//p1->p2
		Vector3D triangle_edge1 = p4 - p1;//p1->p4
		Vector3D triangle_edge2 = p4 - p2;//p2->p4
		Vector3D triangle_edge3 = p4 - p3;// p3->p4
		Vector3D triangle_edge4 = p3 - p2; //p2->p3
		Point3D p = originModel + (t*directionModel);

		//if p is to the left of triangle_edge0, to the right of triange_edge1, to the right of triangle_2, then it should intersect with unitSquare  
		Vector3D P1toP = p - p1;
		//left >0, right<0
		//To determine the right side or left side of the edge, the dot product of (crossproduct of P1toP and edge) and normorl should >0
		//now for triangle P1P2P4
		if (normal.dot(triangle_edge0.cross(P1toP)) < 0) {//if on the right side of edge0, return false
			return false;
		}

		Vector3D triangle_edge5 = p1 - p4;
		Vector3D P4toP = p - p4;
		if (normal.dot(triangle_edge5.cross(P4toP)) < 0)
			return false;
		//if ( normal.dot(triangle_edge1.cross(P1toP))>0) {// if on the left side of edge1, return false 
		//    return false;
		//}
		//for triangle P1P3P4   
	//    Vector3D P4toP = p - p4;
		Vector3D P3toP = p - p3;
		if (normal.dot(triangle_edge3.cross(P4toP)) < 0) {//if on the left of edge3, return false
			return false;
		}
		//for triangle P1P2P3
		Vector3D P2toP = p - p2;
		if (normal.dot(triangle_edge4.cross(P2toP)) < 0) { // if on the right side of edge 4, return false
			return false;
		}
		ray.intersection.none = false;
		ray.intersection.point = modelToWorld * p;
		Vector3D normalWorld = transNorm(worldToModel, normal);
		normalWorld.normalize();
		ray.intersection.normal = normalWorld;
		ray.intersection.t_value = t;
		return true;
	}
	return false;
}

bool UnitSphere::intersect( Ray3D& ray, const Matrix4x4& worldToModel,
		const Matrix4x4& modelToWorld ) {
	// TODO: implement intersection code for UnitSphere, which is centred 
	// on the origin.  
	//
	// Your goal here is to fill ray.intersection with correct values
	// should an intersection occur.  This includes intersection.point, 
	// intersection.normal, intersection.none, intersection.t_value.   
	//
	// HINT: Remember to first transform the ray into object space  
	// to simplify the intersection test.
    //There is basic explanation that we consult the method to calculate the intersection from the following website
   // https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection
    //Ray Function P = q + r*t        Sphere Function R^2 = x^2+y^2+z^2
    //When P - S = 0 -> P^2 - R^2 = 0 -> q^2 +r^2t^2 +2qrt -R^2 =0 for unknown t 
    //then use the delta function delta = b^2 -4ac to calculate how many intersection in what condition
   // r = directionModel, 
   //a = r^2, b=2qr, c =q^2-R^2
    Point3D originSphere(0,0,0);
    Point3D  pixelModel= worldToModel * ray.origin;
    Vector3D directionModel = worldToModel * ray.dir;//r
    
    Vector3D q = pixelModel - originSphere;//q = origin of pixel - center of sphere
	//double t_ca = PixeltoOrigin.dot(directionModel);
	//if(t_ca <0.0) //PixeltoOrigin and directionModel at opposite direction
	//	return false;
	
    double square_q = q.dot(q);//q^2
    double c = square_q - 1;
    
    double a = directionModel.dot(directionModel);
    double b = 2*q.dot(directionModel);
    
    double delta = b*b - 4*a*c;
    double t0, t1;
    if(delta < 0)
        return false;
    
    else if (delta == 0)
        t0 = t1 = -b/(2*a);
    
    else {
        t0= (-b-sqrt(delta))/(2*a);
        t1= (-b+sqrt(delta))/(2*a);
    }

    if(t0>t1)
        swap(t0,t1);
    
    if(t0 < 0){       
     t0 = t1; 
     if(t0 <0)
         return false;
    }
    
   // double DirectionRay=PixeltoOrigin.dot(directionModel);
    //double powerDirectionRay = DirectionRay * DirectionRay;//
    Point3D point = pixelModel + t0*directionModel;
    Vector3D normal = point - originSphere;
	normal.normalize();
    Vector3D normalWorld = transNorm(worldToModel,normal);
    ray.intersection.normal = normalWorld;
    ray.intersection.point = modelToWorld*point;
    ray.intersection.t_value = t0;
    ray.intersection.none=false;
    return true;
}

