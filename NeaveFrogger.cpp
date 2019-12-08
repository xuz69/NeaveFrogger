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
#include <string>
#include <math.h>
#include "FrogPlayer.hpp"
#include "objParser.hpp"
#include "Car.hpp"
#include "Raft.hpp"
#include "PPM.h"
#include "Heightmap.hpp"
#include <chrono>
#include <unistd.h>
#include <fstream>

using namespace std::chrono;

//create a player
FrogPlayer player = FrogPlayer();

/* Some global variables */

//2d vector to store the 3D point of the terrain (the ground)
std::vector< std::vector<Point3D> > terrain;

//store the start time of each round
steady_clock::time_point clock_begin = steady_clock::now();

//timer of the game round
int timer = 60;

//window size (width and height)
unsigned int viewportWidth  = 600;
unsigned int viewportHeight = 600;

//if true, god' perspective; else, frog's perspective
bool godmode = false;

//only record the score into file once for each round
bool recorded = false;

//pi value
double pi = 3.14159265;

/* Car vectors to store different type of cars */
std::vector<Car> car1;

std::vector<Car> car2;

std::vector<Car> car3;

/* Raft vectors to store different length of rafts */
std::vector<Raft> raft1;

std::vector<Raft> raft2;

std::vector<Raft> raft3;

std::vector<Raft> raft4;

/* cars' speeds */
float car1_speed = 1;

float car2_speed = 0.8;

float car3_speed = 0.5;


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
    {0,300,130,1}, //light0 position
    {0,300,-130,1} //light1 position
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
     { 0.5, 0.5, 0.5, 1 },//light0
    { 1, 1, 1, 1 } //light1
};


/* Image struct: reference from our tut */
struct Image {
    int mWidth;
    int mHeight;
    int max;
    GLubyte * mImage;

    void load(char * filename) {

        mImage = LoadPPM(filename, &mWidth, &mHeight, &max);

    }

    void draw(unsigned int x, unsigned int y) {
        glRasterPos2i(x, y);
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

        glDrawPixels(mWidth, mHeight, GL_RGB, GL_UNSIGNED_BYTE, mImage);
    }

    void texture() {
        /**
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

/* Creating texture images */
// textures for objects
Image frog_texture;
Image water;
Image carroad;
Image rock;
Image firetruck;
Image wood;
Image yellowcar;
Image pinkcar;

// image textures
Image heart;
Image gameover;
Image gamewin;



/*
 * Create Cars Function
 *
 * Creating cars and giving their positions and direction
 * 
 */
void createCars(){
    // cars in the first line
    car1.push_back(Car(Point3D(-130,0,80),Vec3D(1,0,0)));
    car1.push_back(Car(Point3D(-65,0,80),Vec3D(1,0,0)));
    car1.push_back(Car(Point3D(20,0,80),Vec3D(1,0,0)));
    car1.push_back(Car(Point3D(95,0,80),Vec3D(1,0,0)));

    // cars in the second line
    car2.push_back(Car(Point3D(-120,5.5,50),Vec3D(1,0,0)));
    car2.push_back(Car(Point3D(-20,5.5,50),Vec3D(1,0,0)));
    car2.push_back(Car(Point3D(80,5.5,50),Vec3D(1,0,0)));

    // cars in the third line
    car3.push_back(Car(Point3D(70,-2.5,20),Vec3D(1,0,0)));
    car3.push_back(Car(Point3D(-80,-2.5,20),Vec3D(1,0,0)));
}

/*
* Create Rafts Function
*
* Creating rafts and giving their positions and direction from bottom to top
*/
void createRafts(){
    // rafts in the first line
    raft1.push_back(Raft(Point3D(-120,0,-30),Vec3D(1,0,0),6,0.8,1));
    raft1.push_back(Raft(Point3D(-40,0,-30),Vec3D(1,0,0),6,0.8,2));
    raft1.push_back(Raft(Point3D(40,0,-30),Vec3D(1,0,0),6,0.8,3));
    raft1.push_back(Raft(Point3D(120,0,-30),Vec3D(1,0,0),6,0.8,4));

    // rafts in the second line
    raft2.push_back(Raft(Point3D(-70,0,-50),Vec3D(1,0,0),12,0.5,1));
    raft2.push_back(Raft(Point3D(70,0,-50),Vec3D(1,0,0),12,0.5,2));

    // rafts in the third line
    raft3.push_back(Raft(Point3D(-85,0,-70),Vec3D(1,0,0),8,0.9,1));
    raft3.push_back(Raft(Point3D(0,0,-70),Vec3D(1,0,0),8,0.9,2));
    raft3.push_back(Raft(Point3D(85,0,-70),Vec3D(1,0,0),8,0.9,3));

    // rafts in the fourth line
    raft4.push_back(Raft(Point3D(-120,0,-90),Vec3D(1,0,0),6,0.4,1));
    raft4.push_back(Raft(Point3D(-40,0,-90),Vec3D(1,0,0),6,0.4,2));
    raft4.push_back(Raft(Point3D(40,0,-90),Vec3D(1,0,0),6,0.4,3));
    raft4.push_back(Raft(Point3D(120,0,-90),Vec3D(1,0,0),6,0.4,4));

}

/*
 * Drawing Ground Function
 *
 * drawing flat of the game, and add textures into the ground,
 * for the ground add rock texture; for the water surface add water
 * ripple texture; for road add road texture
 *
 */
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

    //Middle tiles that is between road and water
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
 * Frog Function
 *
 * since we load the obj file of frog by using our parser class,
 * now we can draw the graphics of frog in opengl and add texture for it
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
 * Rafts Function
 *
 * we are doing the same thing with drawFrog function, now we can draw the raft
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
 * Car1 Function
 *
 * Similar as the drawFrog, and drawRaft, we can draw car 1
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
 * Car2 Function
 *
 * Similar as the Car1, we can draw car 2
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
 * Car3 Function
 *
 * Similar as the Car1, and Car2; we can draw car 3
 */
void drawCar3(Point3D p){
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
 * Check Score
 *
 * Player will get the game score after game. Winning with less time speed
 * will get higher score, and lose the game will have lower score.
 */

void checkScore(){
    if(player.onStart){
        player.score = 0;
    }
    else if(player.onRoad){
        if(player.position.mZ > 65){
            player.score = 10;
        }
        else if(player.position.mZ > 35){
            player.score = 20;
        }
        else if(player.position.mZ > 5){
            player.score = 30;
        }
    }
    else if(player.onMiddle){
        player.score = 40;
    }
    else if(player.onRiver){
        player.score = (player.num_tile_raft+4)*10;
    }
    else{
        player.score = 90*timer;
    }
}

/*
 *
 * Write Score Record into a Score File
 * 
 */
void writeRecord(){
    std::ifstream file("scores.txt");
    int count;
    std::string line;
    
    while(getline(file,line)){
        count++;
    }

    std::ofstream myfile;
    myfile.open ("scores.txt",std::ios_base::app);
    myfile << count << ", " << player.score << ", " << 60-timer << "\n";
    myfile.close();

}

/*
 * Time Display Function
 *
 * Display remain time of the game on the screen
 *
 */
void timedisplay (int posx, int posy, int space_char, int remain_time)
{
        int j=0,p,k;
        GLvoid *font_style1 = GLUT_BITMAP_TIMES_ROMAN_24;
        p = remain_time;
        j = 0;
        k = 0;
        glRasterPos2f ((posx-(j*space_char))+(viewportWidth-30), posy);   
        glutBitmapCharacter(font_style1,'s');
        j++;
        while(p > 9)
        {
            k = p % 10;
            glRasterPos2f ((posx-(j*space_char))+(viewportWidth-30),posy);
            glutBitmapCharacter(font_style1,48+k);
            j++;
            p /= 10;
        }
        glRasterPos2f ((posx-(j*space_char))+(viewportWidth-30), posy);   
        glutBitmapCharacter(font_style1,48+p);
        
      
}

/*
* Score Display Function
*
* Display how many scores you have at end of the game
*
*/
void scoredisplay (int posx, int posy, int space_char, int scorevar)
{
        int j=0,p,k;
        GLvoid *font_style1 = GLUT_BITMAP_TIMES_ROMAN_24;
        p = scorevar;
        j = 0;
        k = 0;
        while(p > 9)
        {
            k = p % 10;
            glRasterPos2f ((posx-(j*space_char)),posy);
            glutBitmapCharacter(font_style1,48+k);
            j++;
            p /= 10;
        }
        glRasterPos2f ((posx-(j*space_char)), posy);   
        glutBitmapCharacter(font_style1,48+p);
        std::string text = " si erocs ruoy";
        for(int i = 0; i < text.size(); i++){
            j++;
            glRasterPos2f ((posx-(j*space_char)), posy);   
            glutBitmapCharacter(font_style1,text[i]);
        }


        
      
}


// References from the tut
/**
 * Set the orthographic projection
 */

void setOrthographicProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, viewportWidth, 0, viewportHeight);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/**
 * 2D renderings (like bitmap images) in here!
 */
void displayOrthographic() {
    setOrthographicProjection();
    /**
     *
     * draw the player's life
     */
    if(player.lifes == 3){
        heart.draw(40,0);
        heart.draw(80,0);
        heart.draw(120,0);
    }
    else if(player.lifes == 2){
        heart.draw(40,0);
        heart.draw(80,0);
    }
    else if(player.lifes == 1){
        heart.draw(40,0);
    }
    else if(player.lifes == 0){
        gameover.draw(viewportWidth*(3.0/4),200);
        scoredisplay((370.0/600)*viewportWidth,10,10,player.score);

    }
    // when player wins the game display the score
    if(player.win){
        checkScore();
        gamewin.draw(viewportWidth*(2.0/3),200);
        scoredisplay((370.0/600)*viewportWidth,10,10,player.score);
        if(!recorded){
            writeRecord();
            recorded = true;
        }

    }

    // game's start time
    steady_clock::time_point clock_end = steady_clock::now();
    
    // duration
    steady_clock::duration time_span = clock_end - clock_begin;

    double nseconds = double(time_span.count()) * steady_clock::period::num / steady_clock::period::den;

    if(player.live){
        timer = 60 - nseconds;
    }
    timedisplay (0, 10 , 15, timer);
    if(timer == 0){
        checkScore();
        player.live = false;
        if(!recorded){
            writeRecord();
            recorded = true;
        }
        gameover.draw(viewportWidth*(2.0/3),200);
        scoredisplay((370.0/600)*viewportWidth,10,10,player.score);
    }
    
}

/**
 * Set Perspective Projection Function
 */
void setPerspectiveProjection() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

/*
* Display Function
* Set up the lights
* Set up the camera
* Call drawFrog, drawCar1, drawCar2, drawCar3, drawRaft, and drawGround function
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

    setPerspectiveProjection();
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

    displayOrthographic();

    glutSwapBuffers();

}

/**
 * Move Funtions
 *
 * Cars and rafts  movement  funtions are written below, since each car and each raft
 * have different sizes, we need count their movement position independently(different range)
 * for cars and rafts move out the screen or move into the screen logically.
 *
 */
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
            if(player.lifes == 0){
                checkScore();
            }
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
            if(player.lifes == 0){
                checkScore();
            }
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
            if(player.lifes == 0){
                checkScore();
            }
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
            if(player.lifes == 0){
                checkScore();
            }
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
            if(player.lifes == 0){
                checkScore();
            }
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
            if(player.lifes == 0){
                checkScore();
            }
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
            if(player.lifes == 0){
                checkScore();
            }
            player.position = Point3D(0,0,105);
        }
    }

    if(player.lifes == 0){
        player.live = false;
        if(!recorded){
            writeRecord();
            recorded = true;
        }
    }
}

/**
 *
 * Frog's movement on rafts
 *
 */
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
 * Check Win Function
 * 
 */
void checkWin(){
    if(player.position.mZ < -105){
        player.live = false;
        player.win = true;
    }
}

/*
 *
 * Restart Function
 *  
 */

void restart(){
    player = FrogPlayer();
    clock_begin = steady_clock::now();
    /* frog's perspective */
    eye[0] = player.position.mX;
    eye[1] = player.position.mY+7;
    eye[2] = player.position.mZ;
    lookAt[0] = player.position.mX;
    lookAt[1] = player.position.mY;
    lookAt[2] = player.position.mZ-30;
    //set recorded to false
    recorded = false;

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
    case 13:
        if(!player.live){
            restart();
        }
        break;
    case 27:
    case 'q':
    case 'Q':
        std::cout << "NeaveFrogger Game Closed!\n";
        exit(0);
        break;
    // god's perspective
    case 'g':
    case 'G':
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
            if(player.live && player.position.mX + 15 <= 150){
                player.position.mX += 10;
            }
            break;
        case GLUT_KEY_LEFT:
            if(player.live && player.position.mX - 15 >= -150){
                player.position.mX -= 10;
            }
            break;
        case GLUT_KEY_UP:
            if(player.live){
                if(player.onMiddle){
                    player.position.mZ -= 25;
                    player.position.mY += 3;
                    player.num_tile_raft += 1;
                }
                else if(player.onRiver){
                    player.position.mZ -= 20;
                    player.num_tile_raft += 1;
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
            }
            break;
        case GLUT_KEY_DOWN: 
            if(player.live){
               if(player.onRiver && player.num_tile_raft != 1){
                    player.position.mZ += 20;
                    player.num_tile_raft -= 1;
                }
                else if(player.onRiver && player.num_tile_raft == 1){
                    player.position.mZ += 25;
                    player.position.mY -= 3;
                    player.num_tile_raft -= 1;
                }
                else if(player.onMiddle){
                    player.position.mZ += 25;
                }
                else if(player.onRoad && player.position.mZ < 80){
                    player.position.mZ += 30;
                }
                else if(player.onStart){
                    break;
                }
                else{
                    player.position.mZ += 25;
                }
    
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
    checkWin();
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
    viewportHeight = h;
    viewportWidth = w;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

/*
 * CallBackInit function
 */
void callbackInit(){
    //programInstr();
    createCars();
    createRafts();
    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyboard);
    glutSpecialFunc(special);
    glutReshapeFunc(handleReshape);
    glutTimerFunc(0, FPS, 0);
    clock_begin = steady_clock::now();

}

/*
 * Initialization Function
 */
void init(void){

    // operation guide of game in terminal.
    std::cout << "Welcome to The Frogger Game!\n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "KEYBOARD commands:\n";
    std::cout << "1. Pressing 'q', 'Q' or 'ESC' to exit the system.\n";
    std::cout << "2. Press 'a' to rotate the camera position (God Mode)  about y-axis in CCW \n";
    std::cout << "3. Press 'd' to rotate the camera position (God Mode) about y-axis in CW\n";
    std::cout << "4. Press 'w' to rotate the camera position (God Mode) about x-axis in CW\n";
    std::cout << "5. Press 's' to rotate the camera position  (God Mode) about x-axis in CCW\n";
    std::cout << "6. Press right arrow button to change the frog position along x-axis (positive)\n";
    std::cout << "7. Press left arrow button to change the frog position along x-axis (negative)\n";
    std::cout << "8. Press down arrow button to change the frog position along z-axis (negative)\n";
    std::cout << "9. Press up arrow button to change the frog position along z-axis (positive)\n";
    std::cout << "10. Press 'g' or 'G' to change the perspective mode between god and frog \n";
    std::cout << "11. Press ENTER key to restart game if and only if the round of game is end \n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "Have Fun!\n";
    std::cout << std::endl;
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
    heart.load("ppm/life.ppm");
    gameover.load("ppm/gameover.ppm");
    gamewin.load("ppm/win.ppm");
    

    glutInit(&argc, argv);
    glutInitWindowSize(viewportWidth,viewportHeight);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Neave Frogger");

    init();
    callbackInit();

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    /* enable textures */
    glEnable(GL_TEXTURE_2D);

    glEnable(GL_DEPTH_TEST);


    glClearColor(0.3, 0.3, 0.3, 1);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glShadeModel(GL_SMOOTH);

    glutMainLoop();
    
    return 0;
}

