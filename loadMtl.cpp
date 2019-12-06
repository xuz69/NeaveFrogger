#include "loadMtl.hpp"
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <fstream>
#include <sstream> 
#include <iostream>

bool loadmtl(const char* filename, vector<Material> &materials){
    ifstream file(filename);

    if(!file){
        printf("Error! File does not exist!\n");
        return false;
    }

    string currentLine;
    while(getline(file,currentLine)){
        if(currentLine.substr(0,6) == "newmtl"){
            Material tmp;
            istringstream ss(currentLine);

            for(string currentLine; ss >> currentLine; ){
                if(currentLine != "newmtl"){
                    tmp.materialName = currentLine;
                }
            }
            while(getline(file,currentLine)){
                if(currentLine.substr(0,5) == "illum"){
                    break;
                }
                else if(currentLine.substr(0,2) == "Ns"){
                    istringstream s(currentLine);

                    for(string currentLine; s >> currentLine; ){
                        if(currentLine != "Ns"){
                            tmp.materialShiny = stof(currentLine);
                        }
                    }
                }
                else if(currentLine.substr(0,2) == "Ka"){
                    istringstream s(currentLine);
                    int i = 0;

                    for(string currentLine; s >> currentLine; ){
                        if(currentLine != "Ka"){
                            tmp.materialAmbient[i] = stof(currentLine);
                            i++;
                        }
                    }
                }
                else if(currentLine.substr(0,2) == "Kd"){
                    istringstream s(currentLine);
                    int i = 0;

                    for(string currentLine; s >> currentLine; ){
                        if(currentLine != "Kd"){
                            tmp.materialDiffuse[i] = stof(currentLine);
                            i++;
                        }
                    }
                }
                else if(currentLine.substr(0,2) == "Ks"){
                    istringstream s(currentLine);
                    int i = 0;

                    for(string currentLine; s >> currentLine; ){
                        if(currentLine != "Ks"){
                            tmp.materialSpecular[i] = stof(currentLine);
                            i++;
                        }
                    }
                }
            }
            materials.push_back(tmp);
        }
    }
}