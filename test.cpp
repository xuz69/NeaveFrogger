#include "loadMtl.hpp"
#include "objParser.hpp"
#include <iostream>
vector<Material> mtl;
vector<Point3D> vertice;
vector<Vec3D> normal;
vector<Point3D> texture_cord;
vector<vector<vector<int> > > face;
vector<string> mtls;

int main(int argc, char** argv){
    loadmtl("offroadcar.mtl",mtl);
    loadOBJ("offroadcar.obj",vertice,normal,texture_cord,face,mtls);
    for(int i = 0; i < mtl.size(); i++){
        std::cout << mtl[i].materialName << "\n";
        std::cout << mtl[i].materialShiny << "\n";
        std::cout << "Ka value: \n";
        for(int j = 0; j < 4; j++){
            std::cout << mtl[i].materialAmbient[j] << "\n";
            
        }
        std::cout << "Kd value: \n";
        for(int j = 0; j < 4; j++){
            std::cout << mtl[i].materialDiffuse[j] << "\n";
            
        }
        std::cout << "Ks value: \n";
        for(int j = 0; j < 4; j++){
            std::cout << mtl[i].materialSpecular[j] << "\n";
            
        }
        std::cout << "\n";
    }
    //test for loadingOBJ
    /* 
    for(int i = 0; i < face.size(); i++){
        std::cout << i << "th face \n";
        std::cout << face[i][0][0] << "th point's x value \n";
        std::cout << mtls[i] << " is the face's material \n";
        std::cout << "\n";
    }*/
    return 0;
}