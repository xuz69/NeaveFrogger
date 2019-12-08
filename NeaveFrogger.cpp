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
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include "FrogPlayer.hpp"
#include "objParser.hpp"
#include "Car.hpp"
#include "Raft.hpp"
#include "PPM.h"
#include "Heightmap.hpp"


std::vector< std::vector<Point3D> > terrain;


/* some global variables */
int timer = 60; // time limit for each round

bool test = 0;

bool godmode = false;

double pi = 3.14159265;

std::vector<Car> car1;

std::vector<Car> car2;

std::vector<Car> car3;

std::vector<Raft> raft1;

std::vector<Raft> raft2;

std::vector<Raft> raft3;

std::vector<Raft> raft4;

float car1_speed = 1;

float car2_speed = 0.8;

float car3_speed = 0.5;

FrogPlayer player = FrogPlayer();

/* Textures */
/* Three texture */
GLubyte* river;
GLubyte* road;
GLubyte* grass;

/* arrays for image data */
GLuint textures[3];

int width1, height1, max1;
int width2, height2, max2;
int width3, height3, max3;


//frog
std::vector<Point3D> frog_verticess;
std::vector<Point3D> frog_vts;
std::vector<Vec3D> frog_normalss;
std::vector<std::vector<std::vector<int> > > frog_facess;


//car1
std::vector<Point3D> car_verticess;
std::vector<Point3D> car_vts;
std::vector<Vec3D> car_normalss;
std::vector<std::vector<std::vector<int> > > car_facess;


//car2
std::vector<Point3D> car2_verticess;
std::vector<Point3D> car2_vts;
std::vector<Vec3D> car2_normalss;
std::vector<std::vector<std::vector<int> > > car2_facess;


//car3
std::vector<Point3D> car3_verticess;
std::vector<Point3D> car3_vts;
std::vector<Vec3D> car3_normalss;
std::vector<std::vector<std::vector<int> > > car3_facess;

//rafts
std::vector<Point3D> raft_verticess;
std::vector<Point3D> raft_vts;
std::vector<Vec3D> raft_normalss;
std::vector<std::vector<std::vector<int> > > raft_facess;

//perspective setup
GLdouble eye[] = { player.position.mX, player.position.mY+7, player.position.mZ };
GLdouble lookAt[] = { player.position.mX, player.position.mY, player.position.mZ-30 };
GLdouble up[] = { 0, 1, 0 };

/*
    Two sets of lighting properties.
 */
GLfloat light_pos[2][4] = {
    {0,300,0,1}, //light0 position
    {150,150,70,1} //light1 position
};
GLfloat amb[2][4] = {
    { 1, 1, 1, 1 }, //light0
    { 1, 1, 1, 1 } //light1
};
GLfloat diff[2][4] = {
    { 1, 1, 1, 1 },//light0
    { 0, 0, 1, 1 } //light1
};
GLfloat spec[2][4] = {
     { 0.2, 0.2, 0.2, 1 },//light0
    { 1, 1, 1, 1 } //light1
};

/*
Materials setup
1. ruby
2. bronzer
3. turquoise
4. floor (white plastic)
5. green (black plastic)
6. water
7. Black
8. Green Emerald
 */
GLfloat materialAmbient[8][4] = {
    { 0.1745, 0.01175, 0.01175, 1.0 },
    { 0.2125, 0.1275, 0.054, 1.0 },
    { 0.1, 0.18725, 0.1745, 1.0 },
    { 0.0,0.0,0.0},
    {0.0,0.0,0.0},
    {0.0, 0.0, 0.3},
    {0.02, 0.02, 0.02, 1.0},
    { 0.0215, 0.1745, 0.0215, 0.55 }
};
GLfloat materialDiffuse[8][4] = {
    { 0.61424, 0.04136, 0.04136, 1.0 },
    { 0.714, 0.4284, 0.18144, 1.0 },
    { 0.396, 0.74151, 0.69102, 1.0 },
    {0.55,0.55,0.55},
    {0.1, 0.35, 0.1},
    {0.0, 0.0, 0.6},
    {0.01, 0.01, 0.01, 1.0},
    {0.07568, 0.61424, 0.07568, 0.55}
};
GLfloat materialSpecular[8][4] = {
    { 0.727811, 0.626959, 0.626959, 1.0 },
    { 0.393548, 0.271906, 0.166721, 1.0 },
    { 0.297254, 0.30829, 0.306678, 1.0 },
    {0.70,0.70,0.70},
    {0.45, 0.55, 0.45},
    {0.6, 0.6, 0.8},
    {0.4, 0.4, 0.4, 1.0},
    {0.633, 0.727811, 0.633, 0.55 }
};
GLfloat materialShiny[8] = {
    0.6,
    0.2,
    0.1,
    32.0,
    0.25,
    100,
    10.0,
    76.8
};

struct Image {
    int mWidth;
    int mHeight;
    int max;
    GLubyte * mImage;

    void load(char * filename) {
        /**
         * YOUR CODE HERE
         */
        mImage = LoadPPM(filename, &mWidth, &mHeight, &max);

    }

    void draw(unsigned int x, unsigned int y) {
        glRasterPos2i(x + mWidth, y);
        /**
         * If you are on a retina display then you should replace the values
         * from -1, 1 to -2, 2 to ensure they appear at full size!
         *
         * The PPM parser packs the image mirrored horizontally. Thus we use
         * glPixelZoom to "flip" the image back the correct way by scaling the
         * x axis by -1. This is the same concept as mirroring an object with
         * glScalef(-1, 1, 1).
         *
         * Aside: Using the parsing code from last year. I don't think the code
         * is very good. There is definitely a way to parse PPM bitmaps without
         * needing to flip the image like this.
         */
        glPixelZoom(-1, 1);
        /**
         * YOUR CODE HERE
         */
        glDrawPixels(mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, mImage);
    }

    void texture() {
        /**
         * YOUR CODE HERE
         *
         * Add the glTexImage2D and glTexParameterf calls.
         * I strongly recommend reading the documentation to get a loose sense
         * of what these values mean.
         */
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, mImage);
        
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
};

Image frog_texture;
Image water;
Image carroad;
Image rock;
Image firetruck;
Image wood;
Image yellowcar;
Image pinkcar;



/* Create Cars Function */
void createCars(){
    car1.push_back(Car(Point3D(-130,0,80),Vec3D(1,0,0)));
    car1.push_back(Car(Point3D(-65,0,80),Vec3D(1,0,0)));
    car1.push_back(Car(Point3D(20,0,80),Vec3D(1,0,0)));
    car1.push_back(Car(Point3D(95,0,80),Vec3D(1,0,0)));

    car2.push_back(Car(Point3D(-120,5.5,50),Vec3D(1,0,0)));
    car2.push_back(Car(Point3D(-20,5.5,50),Vec3D(1,0,0)));
    car2.push_back(Car(Point3D(80,5.5,50),Vec3D(1,0,0)));

    car3.push_back(Car(Point3D(70,-2.5,20),Vec3D(1,0,0)));
    car3.push_back(Car(Point3D(-80,-2.5,20),Vec3D(1,0,0)));
}

/* Create Rafts Function */
void createRafts(){
    raft1.push_back(Raft(Point3D(-120,0,-30),Vec3D(1,0,0),6,0.8,1));
    raft1.push_back(Raft(Point3D(-40,0,-30),Vec3D(1,0,0),6,0.8,2));
    raft1.push_back(Raft(Point3D(40,0,-30),Vec3D(1,0,0),6,0.8,3));
    raft1.push_back(Raft(Point3D(120,0,-30),Vec3D(1,0,0),6,0.8,4));

    raft2.push_back(Raft(Point3D(-70,0,-50),Vec3D(1,0,0),12,0.5,1));
    raft2.push_back(Raft(Point3D(70,0,-50),Vec3D(1,0,0),12,0.5,2));

    raft3.push_back(Raft(Point3D(-85,0,-70),Vec3D(1,0,0),8,0.9,1));
    raft3.push_back(Raft(Point3D(0,0,-70),Vec3D(1,0,0),8,0.9,2));
    raft3.push_back(Raft(Point3D(85,0,-70),Vec3D(1,0,0),8,0.9,3));

    raft4.push_back(Raft(Point3D(-120,0,-90),Vec3D(1,0,0),6,0.4,1));
    raft4.push_back(Raft(Point3D(-40,0,-90),Vec3D(1,0,0),6,0.4,2));
    raft4.push_back(Raft(Point3D(40,0,-90),Vec3D(1,0,0),6,0.4,3));
    raft4.push_back(Raft(Point3D(120,0,-90),Vec3D(1,0,0),6,0.4,4));

}
void draw_ground(){
    glPushMatrix();
    //end tile
    glPushMatrix();
    rock.texture();
    for(int i = 0; i < 300-1; i ++){
        for(int j = 0; j < 20; j++){
            glColor3f(0,0,1);
            glBegin(GL_QUADS);
                glNormal3f(0,1,0);
                glTexCoord2f(1, 0);
                glVertex3f(terrain[i][j].mX,terrain[i][j].mY,terrain[i][j].mZ);
                glTexCoord2f(0, 0);
                glVertex3f(terrain[i][j+1].mX,terrain[i][j+1].mY,terrain[i][j+1].mZ);
                glTexCoord2f(0, 1);
                glVertex3f(terrain[i+1][j+1].mX,terrain[i+1][j+1].mY,terrain[i+1][j+1].mZ);
                glTexCoord2f(1, 1);
                glVertex3f(terrain[i+1][j].mX,terrain[i+1][j].mY,terrain[i+1][j].mZ);
            glEnd();
        }
    }
    glPopMatrix();

    //water tiles
    glPushMatrix();
    water.texture();
    for(int i = 0; i < 300-1; i ++){
        for(int j = 20; j < 110; j++){
            glColor3f(0,0,1);
            glBegin(GL_QUADS);
                glNormal3f(0,1,0);
                glTexCoord2f(1, 0);
                glVertex3f(terrain[i][j].mX,terrain[i][j].mY,terrain[i][j].mZ);
                glTexCoord2f(0, 0);
                glVertex3f(terrain[i][j+1].mX,terrain[i][j+1].mY,terrain[i][j+1].mZ);
                glTexCoord2f(0, 1);
                glVertex3f(terrain[i+1][j+1].mX,terrain[i+1][j+1].mY,terrain[i+1][j+1].mZ);
                glTexCoord2f(1, 1);
                glVertex3f(terrain[i+1][j].mX,terrain[i+1][j].mY,terrain[i+1][j].mZ);
            glEnd();
        }
    }
    glPopMatrix();

    //middle tiles
    glPushMatrix();
    rock.texture();
    for(int i = 0; i < 300-1; i ++){
        for(int j = 110; j < 130; j++){
            glColor3f(0,0,1);
            glBegin(GL_QUADS);
                glNormal3f(0,1,0);
                glTexCoord2f(1, 0);
                glVertex3f(terrain[i][j].mX,terrain[i][j].mY,terrain[i][j].mZ);
                glTexCoord2f(0, 0);
                glVertex3f(terrain[i][j+1].mX,terrain[i][j+1].mY,terrain[i][j+1].mZ);
                glTexCoord2f(0, 1);
                glVertex3f(terrain[i+1][j+1].mX,terrain[i+1][j+1].mY,terrain[i+1][j+1].mZ);
                glTexCoord2f(1, 1);
                glVertex3f(terrain[i+1][j].mX,terrain[i+1][j].mY,terrain[i+1][j].mZ);
            glEnd();
        }
    }
    glPopMatrix();

    //car tiles
    glPushMatrix();
    carroad.texture();
    for(int i = 0; i < 300-1; i ++){
        for(int j = 130; j < 220; j++){
            glColor3f(0,0,1);
                glBegin(GL_QUADS);
                glNormal3f(0,1,0);
                glTexCoord2f(1, 0);
                glVertex3f(terrain[i][j].mX,terrain[i][j].mY,terrain[i][j].mZ);
                glTexCoord2f(0, 0);
                glVertex3f(terrain[i][j+1].mX,terrain[i][j+1].mY,terrain[i][j+1].mZ);
                glTexCoord2f(0, 1);
                glVertex3f(terrain[i+1][j+1].mX,terrain[i+1][j+1].mY,terrain[i+1][j+1].mZ);
                glTexCoord2f(1, 1);
                glVertex3f(terrain[i+1][j].mX,terrain[i+1][j].mY,terrain[i+1][j].mZ);
            glEnd();
        }
    }
    glPopMatrix();

    //start tile
    glPushMatrix();
    rock.texture();
    for(int i = 0; i < 300-1; i ++){
        for(int j = 220; j < 240-1; j++){
            glColor3f(0,0,1);
            glBegin(GL_QUADS);
                glNormal3f(0,1,0);
                glTexCoord2f(1, 0);
                glVertex3f(terrain[i][j].mX,terrain[i][j].mY,terrain[i][j].mZ);
                glTexCoord2f(0, 0);
                glVertex3f(terrain[i][j+1].mX,terrain[i][j+1].mY,terrain[i][j+1].mZ);
                glTexCoord2f(0, 1);
                glVertex3f(terrain[i+1][j+1].mX,terrain[i+1][j+1].mY,terrain[i+1][j+1].mZ);
                glTexCoord2f(1, 1);
                glVertex3f(terrain[i+1][j].mX,terrain[i+1][j].mY,terrain[i+1][j].mZ);
            glEnd();
        }
    }
    glPopMatrix();


    glPopMatrix();
}

/*
 since we load the obj file of frog by using our parser class,
 now we can draw the graphics of frog in opengl
 */
void drawFrog(){
    glPushMatrix();
    glTranslatef(player.position.mX,player.position.mY,player.position.mZ);
    glRotatef(-90,0,1,0);
    glScalef(0.1,0.1,0.1);
    //setMaterials(4);
    // in each face of the frag
    frog_texture.texture();
    for(int i = 0; i < frog_facess.size(); i++){
        glBegin(GL_POLYGON);
            glColor3f(0,1,0);
            // vertices in the face
            for(int y = 0; y < frog_facess[i][0].size(); y++){
                // set the vertex i be point3D p
                Point3D p = frog_verticess[frog_facess[i][0][y]];
                // the vertex normal of the point p
                Vec3D n = frog_normalss[frog_facess[i][1][y]];
                glNormal3f(n.mX,n.mY,n.mZ);
                glVertex3f(p.mX,p.mY,p.mZ);
            }
        glEnd();
    }
    glPopMatrix();
}

/*
 we are doing the same thing with drawFrog function, now we can draw the raft
 */
void drawRaft(Point3D p, int length){
    
    glPushMatrix();
    glTranslatef(p.mX, p.mY, p.mZ);
    glRotatef(90,0,1,0);
    glScalef(6,6,length);
    for(int i = 0; i < raft_facess.size(); i++){
        glBegin(GL_POLYGON);
            glColor3f(0.5f, 0.35f, 0.05f);
            for(int y = 0; y < raft_facess[i][0].size(); y++){
                Point3D p = raft_verticess[raft_facess[i][0][y]];
                Vec3D n = raft_normalss[raft_facess[i][1][y]];
                glNormal3f(n.mX,n.mY,n.mZ);
                glVertex3f(p.mX,p.mY,p.mZ);
            }
        glEnd();
    }
    glPopMatrix();
};

/*
 Similar as the drawFrog, and drawRaft, we can draw car 1
 */
void drawCar1(Point3D p){
    glPushMatrix();
    glTranslatef(p.mX, p.mY, p.mZ);
    glScalef(6,6,6);
    glRotatef(180,0,1,0);
    for(int i = 0; i < car_facess.size(); i++){
        glBegin(GL_POLYGON);
            glColor3f(0,0,1);
            for(int y = 0; y < car_facess[i][0].size(); y++){
                Point3D p = car_verticess[car_facess[i][0][y]];
                Vec3D n = car_normalss[car_facess[i][1][y]];
                glNormal3f(n.mX,n.mY,n.mZ);
                glVertex3f(p.mX,p.mY,p.mZ);
            }
        glEnd();
    }
    glPopMatrix();
}

/*
 Similar as the drawFrog, and drawRaft, we can draw car 2
 */
void drawCar2(Point3D p){
    glPushMatrix();
    glTranslatef(p.mX, p.mY, p.mZ);
    glScalef(6,6,6);
    for(int i = 0; i < car2_facess.size(); i++){
        glBegin(GL_POLYGON);
            glColor3f(1,1,0);
            for(int y = 0; y < car2_facess[i][0].size(); y++){
                Point3D p = car2_verticess[car2_facess[i][0][y]];
                Vec3D n = car2_normalss[car2_facess[i][1][y]];
                glNormal3f(n.mX,n.mY,n.mZ);
                glVertex3f(p.mX,p.mY,p.mZ);
            }
        glEnd();
    }
    glPopMatrix();
}

/*
 Similar as the drawFrog, and drawRaft, we can draw car 3 (Fire truck)
 */
void drawCar3(Point3D p){
    //setMaterials(1);
    
    glPushMatrix();
    glTranslatef(p.mX, p.mY, p.mZ);
    glScalef(18,18,18);
    for(int i = 0; i < car3_facess.size(); i++){
        glBegin(GL_POLYGON);
            glColor3f(1,0,0);
            for(int y = 0; y < car3_facess[i][0].size(); y++){
                Point3D p = car3_verticess[car3_facess[i][0][y]];
                Vec3D n = car3_normalss[car3_facess[i][1][y]];
                glNormal3f(n.mX,n.mY,n.mZ);
                glVertex3f(p.mX,p.mY,p.mZ);
            }
        glEnd();
    }
    glPopMatrix();
}

/*
* Display Function
* Set up the lights
* Set up the camera
* Call drawFrog, drawCar1, drawCar2, drawCar3, drawRaft, and drawFloor function
* Using double buffer
*/
void display(){
    /* WRITE CODE HERE
     LIGHTING, SET CHANNELS */
    for(int i = 0; i < 2; i++){
        glLightfv(GL_LIGHT0+i,GL_POSITION,light_pos[i]);
        glLightfv(GL_LIGHT0+i,GL_DIFFUSE,diff[i]);
        glLightfv(GL_LIGHT0+i,GL_AMBIENT,amb[i]);
        glLightfv(GL_LIGHT0+i,GL_SPECULAR,spec[i]);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2],up[0],up[1],up[2]);

    //call draw_ground function
    draw_ground();
    
    //draw frog
    drawFrog();
    
    /* three types of car */
    yellowcar.texture();
    for(int i = 0; i < car1.size(); i++){
        drawCar1(car1[i].position);
    }
    pinkcar.texture();
    for(int i = 0; i < car2.size(); i++){
        drawCar2(car2[i].position);
    }
    firetruck.texture();
    for(int i = 0; i < car3.size(); i++){
        drawCar3(car3[i].position);
    }
    
    /* rafts */
    wood.texture();
    for(int i = 0; i < raft1.size(); i++){
        drawRaft(raft1[i].position, raft1[i].length);
    }
    for(int i = 0; i < raft2.size(); i++){
        drawRaft(raft2[i].position, raft2[i].length);
    }
    for(int i = 0; i < raft3.size(); i++){
        drawRaft(raft3[i].position, raft3[i].length);
    }
    for(int i = 0; i < raft4.size(); i++){
        drawRaft(raft4[i].position, raft4[i].length);
    }

    glutSwapBuffers();

}

void moveCar1(void){
    for (int i = 0; i < car1.size(); i++){
        if (-164 <= car1[i].position.mX && car1[i].position.mX <= 164){
            car1[i].position.mX = car1[i].position.mX - car1_speed;
        }else
        {
            car1[i].position.mX = 163;
        }
        
    }
}


void moveCar2(void){
    for (int i = 0; i < car2.size(); i++){
        if (-164 <= car2[i].position.mX && car2[i].position.mX <= 164){
            car2[i].position.mX = car2[i].position.mX - car2_speed;
        }else
        {
            car2[i].position.mX = 163;
        }
        
    }
}

void moveCar3(void){
    for (int i = 0; i < car3.size(); i++){
        if (-190 <= car3[i].position.mX && car3[i].position.mX <= 180){
            car3[i].position.mX = car3[i].position.mX - car3_speed;
        }else
        {
            car3[i].position.mX = 180;
        }
        
    }
}

void moveRaft1(){
    for (int i = 0; i < raft1.size(); i++){
        if (-164 <= raft1[i].position.mX && raft1[i].position.mX <= 175){
            raft1[i].position.mX = raft1[i].position.mX - raft1[i].speed;
        }else
        {
            raft1[i].position.mX = 175;
        }
        
    }
}

void moveRaft2(){
    for (int i = 0; i < raft1.size(); i++){
        if (-165 <= raft2[i].position.mX && raft2[i].position.mX <= 197){
            raft2[i].position.mX = raft2[i].position.mX - raft2[i].speed;
        }else
        {
            raft2[i].position.mX = 197;
        }
        
    }
}

void moveRaft3(){
    for (int i = 0; i < raft1.size(); i++){
        if (-164 <= raft3[i].position.mX && raft3[i].position.mX <= 180){
            raft3[i].position.mX = raft3[i].position.mX - raft3[i].speed;
        }else
        {
            raft3[i].position.mX = 180;
        }
        
    }
}

void moveRaft4(){
    for (int i = 0; i < raft1.size(); i++){
        if (-164 <= raft4[i].position.mX && raft4[i].position.mX <= 168){
            raft4[i].position.mX = raft4[i].position.mX - raft4[i].speed;
        }else
        {
            raft4[i].position.mX = 168;
        }
        
    } 
}

/* 
 * Check life of the frog 
 *
 * Two conditions that the frog will lose life(s): crush the car and fall into water
 * 
 * */
void checkLife(){
    double frog_x = player.position.mX;
    double frog_z = player.position.mZ;
    //crush the car
    //first type of car
    
    for(int i = 0; i < car1.size(); i++){
        double x = car1[i].position.mX;
        double z = car1[i].position.mZ;
        if(frog_x < x + 17 && frog_x > x - 20 && frog_z> z - 18 && frog_z < z + 18){
            player.lifes -= 1;
            std::cout << "Crush!\n";
            player.position = Point3D(0,0,105);
        }
    }
    //second type of car
     
    for(int i = 0; i < car2.size(); i++){
        double x = car2[i].position.mX;
        double z = car2[i].position.mZ;
        if(frog_x < x + 15 && frog_x > x - 20 && frog_z  > z - 18 && frog_z < z + 18){
            player.lifes -= 1;
            std::cout << "Crush!\n";
            player.position = Point3D(0,0,105);
        }
    }
    //third type of car
    for(int i = 0; i < car3.size(); i++){
        double x = car3[i].position.mX;
        double z = car3[i].position.mZ;
        if(frog_x < x + 40 && frog_x > x - 25 && frog_z > z - 15 && frog_z < z + 15){
            player.lifes -= 1;
            std::cout << "Crush!\n";
            player.position = Point3D(0,0,105);
        }
    }

    //fall into water
    bool on_raft = false;
    if(player.num_tile_raft == 1){
        for(int i = 0; i < raft1.size(); i++){
            double x = raft1[i].position.mX;
            double z = raft1[i].position.mZ;
            if(frog_x < x + 20 && frog_x > x - 25){
                on_raft = true;
                player.on_raft_num = i+1;
                break;
            }
        }
        if(!on_raft){
            player.lifes -= 1;
            player.num_tile_raft = 0;
            player.on_raft_num = 0;
            std::cout << "Fall in water!\n";
            player.position = Point3D(0,0,105);
        }
    }
    else if(player.num_tile_raft == 2){
        for(int i = 0; i < raft2.size(); i++){
            double x = raft2[i].position.mX;
            double z = raft2[i].position.mZ;
            if(frog_x < x + 25 && frog_x > x - 50){
                on_raft = true;
                player.on_raft_num = i+1;
                break;
            }
        }
        if(!on_raft){
            player.lifes -= 1;
            player.num_tile_raft = 0;
            player.on_raft_num = 0;
            std::cout << "Fall in water!\n";
            player.position = Point3D(0,0,105);
        }
    }
    else if(player.num_tile_raft == 3){
        for(int i = 0; i < raft3.size(); i++){
            double x = raft3[i].position.mX;
            double z = raft3[i].position.mZ;
            if(frog_x < x + 15 && frog_x > x - 35){
                on_raft = true;
                player.on_raft_num = i+1;
                break;
            }
        }
        if(!on_raft){
            player.lifes -= 1;
            player.num_tile_raft = 0;
            player.on_raft_num = 0;
            std::cout << "Fall in water!\n";
            player.position = Point3D(0,0,105);
        }
    }
    else if(player.num_tile_raft == 4){
        for(int i = 0; i < raft4.size(); i++){
            double x = raft4[i].position.mX;
            double z = raft4[i].position.mZ;
            if(frog_x < x + 20 && frog_x > x - 25){
                on_raft = true;
                player.on_raft_num = i+1;
                break;
            }
        }
        if(!on_raft){
            player.lifes -= 1;
            player.num_tile_raft = 0;
            player.on_raft_num = 0;
            std::cout << "Fall in water!\n";
            player.position = Point3D(0,0,105);
        }
    }

    if(player.lifes == 0){
        std::cout << "Game over!\n";
    }
}

void frogOnRaftMovement(){
    if(player.num_tile_raft == 1){
        if(player.on_raft_num != 0){
            player.position.mX = player.position.mX - raft1[player.on_raft_num-1].speed;
        }
    }
    else if(player.num_tile_raft == 2){
        if(player.on_raft_num != 0){
            player.position.mX = player.position.mX - raft2[player.on_raft_num-1].speed;
        }
    }
    else if(player.num_tile_raft == 3){
        if(player.on_raft_num != 0){
            player.position.mX = player.position.mX - raft3[player.on_raft_num-1].speed;
        }
    }
    else if(player.num_tile_raft == 4){
        if(player.on_raft_num != 0){
            player.position.mX = player.position.mX - raft4[player.on_raft_num-1].speed;
        }
    }
}

/*
 * Some keyboard functions
 * 1. Press 'esc', 'q' or 'Q' to terminate the program
 * 2. God's Prespective of the game
 */

void handleKeyboard(unsigned char key, int x, int y){
    double eye_x = eye[0];
    double eye_y = eye[1];
    double eye_z = eye[2];
    double rotate_angle = 5.0 * pi /180 ;
    switch (key){
    case 27:
    case 'q':
    case 'Q':
        std::cout << "Particle fountain system closed!\n";
        exit(0);
        break;
    // god's perspective
    case 'g':
        godmode = !godmode;
        if(godmode){
            // the the camera position from frog perspective to god's mode
            eye[0] = 0;
            eye[1] = 360;
            eye[2] = 1;
            lookAt[0] = 0;
            lookAt[1] = 0;
            lookAt[2] = 0;
        }
        break;
    case 'a':
        if(godmode){
            eye[0] = eye_x*cos(-rotate_angle)+eye_z*sin(-rotate_angle);
            eye[2] = eye_z*cos(-rotate_angle)-eye_x*sin(-rotate_angle); 
        }
        break;
    case 'd':
        if(godmode){
            eye[0] = eye_x*cos(rotate_angle)+eye_z*sin(rotate_angle);
            eye[2] = eye_z*cos(rotate_angle)-eye_x*sin(rotate_angle);        
        }
        break;
    case 's':
        if(godmode){
            eye[1] = eye_y*cos(rotate_angle)-eye_z*sin(rotate_angle);
            eye[2] = eye_y*sin(rotate_angle)+eye_z*cos(rotate_angle);
        }
        break;
    case 'w':
        if(godmode){
            eye[1] = eye_y*cos(-rotate_angle)-eye_z*sin(-rotate_angle);
            eye[2] = eye_y*sin(-rotate_angle)+eye_z*cos(-rotate_angle);
        }
        break;
    }
}

/*
 * Special key functions
 *
 * 1. Right and Left arrow: move frog right or left
 *
 * 1. Up and down arrow: move the frog up or down
 */

void special(int key, int x, int y){
    switch(key){
        case GLUT_KEY_RIGHT: 
            player.position.mX += 10;
            break;
        case GLUT_KEY_LEFT:
            player.position.mX -= 10;
            break;
        case GLUT_KEY_UP:
            if(player.onMiddle){
                player.position.mZ -= 25;
                player.position.mY += 3;
                player.num_tile_raft += 1;
                std::cout << player.num_tile_raft << std::endl;
            }
            else if(player.onRiver){
                player.position.mZ -= 20;
                player.num_tile_raft += 1;
                std::cout << player.num_tile_raft << std::endl;
            }
            else if(player.onStart){
                player.position.mZ -= 25;
            }
            else if(player.onRoad && player.position.mZ > 20){
                player.position.mZ -= 30;
            }
            else if(player.onRiver && player.position.mZ < -80){
                player.position.mZ -= 20;
                player.position.mY -=3;//dump off the raft
            }
            else{
                player.position.mZ -= 25;
            }
            break;
        case GLUT_KEY_DOWN: 
            if(player.onRiver && player.num_tile_raft != 1){
                player.position.mZ += 20;
                player.num_tile_raft -= 1;
                std::cout << player.num_tile_raft << std::endl;
            }
            else if(player.onRiver && player.num_tile_raft == 1){
                player.position.mZ += 25;
                player.position.mY -= 3;
                player.num_tile_raft -= 1;
                std::cout << player.num_tile_raft << std::endl;
            }
            else if(player.onMiddle){
                player.position.mZ += 25;
            }
            else if(player.onRoad && player.position.mZ < 80){
                player.position.mZ += 30;
            }
            else{
                player.position.mZ += 25;
            }
            break;
    }
}

/*
 Frog perspective
 */
void frogCam(){
    eye[0] = player.position.mX;
    eye[1] = player.position.mY+7;
    eye[2] = player.position.mZ;
    lookAt[0] = player.position.mX;
    lookAt[1] = player.position.mY;
    lookAt[2] = player.position.mZ-30;
}


/*
 * FPS function
 * Move the frog continously.
 */

void FPS(int val){
    if(!godmode){
        frogCam();
    }
    if(player.position.mZ>-15 && player.position.mZ < 5){
        player.onMiddle = true;
        player.onRiver = false;
        player.onRoad = false;
        player.onStart = false;
    }
    else if(player.position.mZ<-15 && player.position.mZ>-105){
        player.onRiver = true;
        player.onMiddle = false;
        player.onRoad = false;
        player.onStart = false;
    }
    else if(player.position.mZ<115 && player.position.mZ>95){
        player.onRiver = false;
        player.onMiddle = false;
        player.onRoad = false;
        player.onStart = true;
    }
    else if(player.position.mZ<95 && player.position.mZ>05){
        player.onRiver = false;
        player.onMiddle = false;
        player.onRoad = true;
        player.onStart = false;
    }
    else{
        player.onRiver = false;
        player.onMiddle = false;
        player.onRoad = false;
        player.onStart = false;
    }
    checkLife();
    frogOnRaftMovement();
    /* moving of the cars */
    moveCar1();
    moveCar2();
    moveCar3();

    /* moving of the rafts */
    moveRaft1();
    moveRaft2();
    moveRaft3();
    moveRaft4();


    glutPostRedisplay();
    glutTimerFunc(17, FPS, 0);
}

/* Reshape function */
void handleReshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void handleMouse(int button, int state, int x, int y) {
    /**
     * YOUR CODE HERE
     *
     * Check for GLUT_LEFT_BUTTON and GLUT_DOWN and then call
     * mouseHandler.leftClickDown, remember viewportHeight - y.
     */
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            std::cout << "Left click at " << x << ", " << 600 - y << std::endl;
        }
    }
}

/*
 * CallBackInit function
 */
void callbackInit(){
    //programInstr();
    createCars();
    createRafts();
    glutMouseFunc(handleMouse);
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(special);
    glutReshapeFunc(handleReshape);
    glutTimerFunc(0, FPS, 0);
}

int main(int argc, char** argv){
    Heightmap(terrain, 300, 240);
    // obj files load to graphics
    loadOBJ("obj/Frog.obj", frog_verticess, frog_normalss, frog_vts, frog_facess);// frog
    loadOBJ("obj/cap.obj", car_verticess, car_normalss, car_vts, car_facess);// car1
    loadOBJ("obj/woodTxt.obj", raft_verticess, raft_normalss, raft_vts, raft_facess);// logs
    loadOBJ("obj/offroadcar.obj", car2_verticess, car2_normalss, car2_vts, car2_facess); // car 2
    loadOBJ("obj/fireTruck.obj", car3_verticess, car3_normalss, car3_vts, car3_facess); // car 3

    /* Load road, river and grass texture */

    frog_texture.load("ppm/grass.ppm");
    water.load("ppm/river.ppm");
    rock.load("ppm/walkway.ppm");
    carroad.load("ppm/car_road.ppm");
    firetruck.load("ppm/firetruck.ppm");
    yellowcar.load("ppm/yellowcar.ppm");
    pinkcar.load("ppm/pinkcar.ppm");
    wood.load("ppm/wood.ppm");
    

    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Neave Frogger");

    callbackInit();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);

    /* enable textures */
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);
    glutMainLoop();
    
    return 0;
}

