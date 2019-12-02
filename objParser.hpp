#ifndef objParser_hpp
#define objParser_hpp

#include<vector>
#include "mathLib3D.hpp"

/* This function will load the information into target vector from obj file */
/*
 * vertices is a vector of 3D point, stores all vertice of the mesh object.
 * 
 * normals is a vector of Vec3D, stores all the vertex normal of the mesh object
 * 
 * texture_cords is a vector of Point3D, stores all the texture coordinates of the mesh object
 * 
 * faces stores the information of each face, has the form of {{{v1,v2,v3,...},{vn1,vn2,...}[,{vt1,vt2,vt3}]},{},{},...,{}}
 * 
 * For each face, there is a vector to store the index of the vertice of the face and a vector to store the index of the vertex normal of the face. If possible,
 * there is another vector to store the index of the texture coordinates of the face.
 * 
 */
bool loadOBJ(const char* filename, std::vector<Point3D> & vertices, std::vector<Vec3D> & normals, std::vector<Point3D> & texture_cords, std::vector<std::vector<std::vector<int> > > & faces);

#endif
