#include "objParser.hpp"
#include <vector>
#include <stdio.h>
#include <string>
#include <string.h>
#include <cstring>
#include <fstream>
#include <sstream> 
#include <iostream>

// Determine a string is whether or not a number
bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

/* 
 * obj file parser
 * 
 * This function will load the information into target vector from obj file 
 *
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
bool loadOBJ(const char* filename, std::vector<Point3D> & vertices, std::vector<Vec3D> & normals, std::vector<Point3D> & texture_cords, std::vector<std::vector<std::vector<int> > > & faces)
{
    //open a file
    std::ifstream file(filename);
    //determine the file whether or not exist
    if(!file){
        printf("Error! File does not exist.\n");
        return false;//if not exist, return false
    }
    // read file by lines
    std::string currentLine;
    while(std::getline(file,currentLine)){
        // List of geometric vertices, with (x, y, z [,w]) coordinates, w is optional and defaults to 1.0.
        // if the current line is start will v, which means a vertex
        if(currentLine.substr(0,2) == "v "){
            std::istringstream ss(currentLine);
            std::vector<std::string> vs;// stores the x,y,z coordinates
            std::string token;

            for(std::string currentLine; ss >> currentLine; ){// substring is separated by space
                if(currentLine != "v"){
                    vs.push_back(currentLine);
                }
            }
            
            // since it's a 3D object, we only consider x, y, and z coordinates
            vertices.push_back(Point3D(stof(vs[0]),stof(vs[1]),stof(vs[2])));

            
        }      
        // if the current line is start will vn, which means a vertex normal
        // List of vertex normals in (x,y,z) form; normals might not be unit vectors.
        else if(currentLine.substr(0,2) == "vn"){
            std::istringstream ss(currentLine);
            std::vector<std::string> vn;// stores the x,y,z values
            std::string token;

            for(std::string currentLine; ss >> currentLine; ){// substring is separated by space
                if(currentLine != "vn"){
                    vn.push_back(currentLine);
                }
            }

            normals.push_back(Vec3D(stof(vn[0]),stof(vn[1]),stof(vn[2])));

            
        }
        // if the current line is start will vt, which means a texture coordinates
        else if(currentLine.substr(0,2) == "vt"){
            std::istringstream ss(currentLine);
            std::vector<std::string> vt; // stores the values of texture coordinate
            std::string token;

            for(std::string currentLine; ss >> currentLine; ){
                if(currentLine != "vt"){
                    vt.push_back(currentLine);
                }
            }
            //in (u, [,v ,w]) coordinates, these will vary between 0 and 1. v, w are optional and default to 0.
            if(vt.size() == 3){ 
                texture_cords.push_back(Point3D(stof(vt[0]),stof(vt[1]),stof(vt[2])));      
            }
            else if(vt.size() == 2){
                texture_cords.push_back(Point3D(stof(vt[0]),stof(vt[1]),0.0));  
            } 
            else{
                texture_cords.push_back(Point3D(stof(vt[0]),0.0,0.0));  
            }
        }
        // if the current line is start will f, which means a face
        // Different formats for a face
        // f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 ...
        // f v1//vn1 v2//vn2 v3//vn3 ...
        else if(currentLine.substr(0,2) == "f "){
            if(currentLine.find("//") == std::string::npos){ // not the format of f v1//vn1 v2//vn2 v3//vn3 ...
                std::istringstream ss(currentLine);
                std::vector<std::string> faceElements;
                std::string token;

                while(std::getline(ss, token, ' ')) {//get the token separated by space except "f", each token have the format 'v1/vt1/vn1'
                    if(token != "f"){
                        faceElements.push_back(token);
                    }
                }

                std::vector<std::vector<int> > face;
                std::vector<int> v;
                std::vector<int> tc;
                std::vector<int> normal;
                // for each 'v1/vt1/vn1', store v, vt and vn into different vectors
                for(int i = 0; i < faceElements.size(); i++){
                    std::istringstream sub(faceElements[i]);
                    std::vector<std::string> elements;
                    std::string t;
                    while(std::getline(sub,t,'/')){
                        if(is_number(t)){
                            elements.push_back(t);
                        }
                    }
                    if(elements.size() != 0){
                        v.push_back(stod(elements[0])-1);
                        tc.push_back(stod(elements[1])-1);
                        normal.push_back(stod(elements[2])-1);
                    }

                }
                if(v.size()!= 0){
                    face.push_back(v);
                    face.push_back(normal);
                    face.push_back(tc);

                    faces.push_back(face);
                }
            

            }
            else{
                //similar to the previous format
                //handle format of f v1//vn1 v2//vn2 v3//vn3 ...
                std::istringstream ss(currentLine);
                std::vector<std::string> faceElements;
                std::string token;

                while(std::getline(ss, token, ' ')) {
                    if( token != "f"){
                        faceElements.push_back(token);// each token have the format 'v1//vn1'
                    }
                }

                std::vector<std::vector<int> > face;
                std::vector<int> v;
                std::vector<int> normal;
                // for each 'v1//vn1', store v and vn into different vectors
                for(int i = 0; i < faceElements.size(); i++){
                    std::istringstream sub(faceElements[i]);
                    std::vector<std::string> elements;
                    std::string t;
                    
                    while(std::getline(sub,t,'/')){
                        if(t != ""){
                            elements.push_back(t);
                        }
                    }

                    v.push_back(stod(elements[0])-1);
                    normal.push_back(stod(elements[1])-1);

                }
                if(v.size()!= 0){
                    face.push_back(v);
                    face.push_back(normal);

                    faces.push_back(face);
                }
            }

        }
    }



    return 1;
}
