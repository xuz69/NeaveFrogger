#ifndef loadMtl_hpp
#define loadMtl_hpp

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include<vector>
#include<string>
#include "mathLib3D.hpp"

using namespace std;

struct Material{
    GLfloat materialAmbient[4] = {0,0,0,1.0};
    GLfloat materialDiffuse[4] = {0,0,0,1.0};
    GLfloat materialSpecular[4] = {0,0,0,1.0};
    GLfloat materialShiny;

    string materialName;
};

bool loadmtl(const char* filename, vector<Material> &materials);


#endif