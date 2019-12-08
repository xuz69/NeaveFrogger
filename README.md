CS3GC3 Final Project

Group Members:
1. Amy Xu [400079409]

2. Baikai Wang [400084727]


How To Run:
1. Open terminal
2. Go to the directory that contains all my files
3. run the command: make
4. The "neave_frogger" program should be running 

Keyboard Commands:
1. Press ‘g’ or ‘G’ to turn on or off the god mode
    - If god mode is on, then it is god perspective
    - Otherwise, it is frog perspective(default perspective).
2. Press RIGHT arrow button to move right.
3. Press LEFT arrow button to move left.
4. Press UP arrow button to move forward.
5. Press DOWN arrow button to move backward.
6. Press ‘q’, ‘Q’ or ‘esc’ to termination the program.
7. Press 'a' to rotate the camera position (God Mode)  about y-axis in CCW
8. Press 'd' to rotate the camera position (God Mode) about y-axis in CW
9. Press 'w' to rotate the camera position (God Mode) about x-axis in CW
10. Press 's' to rotate the camera position  (God Mode) about x-axis in CCW
11. Press ENTER key to restart game if and only if the round of game is end

Features;
1. Timer: the time limit is 60s for each round
2. Score file: record the score and time used for each round
3. Lighting: two lights
4. Textures: textures for drive way, safe place, river, cars and rafts
5. Terrain: The ground is using terrain generation
6. smooth shading
7. back face culling
8. basic camera: god's perspective and frog's perspective
9. animated cars and rafts
10. alpha blending


Reference:
1. some references about how to write an obj parser
    - http://web.cse.ohio-state.edu/~shen.94/581/Site/Lab3_files/Labhelp_Obj_parser.htm
    - https://en.wikipedia.org/wiki/Wavefront_.obj_file
    - https://en.wikibooks.org/wiki/OpenGL_Programming/Modern_OpenGL_Tutorial_Load_OBJ#Flat-shading_-_duplicating_vertices_and_normals
    - http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/#reading-the-file

2. is_number() function citation
    - https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c

3. Obj files (downloaded from the website)
    - car1: https://free3d.com/3d-model/low-poly-cop-car-89986.html
    - car2: https://www.turbosquid.com/FullPreview/Index.cfm/ID/1328883
    - fire truck: https://www.cgtrader.com/free-3d-models/vehicle/truck/low-poly-civil-protection-truck
    - frog: https://www.cgtrader.com/free-3d-models/animals/other/3ds-max-frog
    - wood: https://free3d.com/3d-model/simple-wood-billet-54853.html

4. Timer implementation reference:
    - http://www.cplusplus.com/reference/chrono/steady_clock/now/
    - https://en.cppreference.com/w/cpp/chrono/steady_clock/now
    - https://www.techiedelight.com/measure-elapsed-time-program-chrono-library/

5. Text display reference: (alpha blending)
    - http://rohithsreeragam.blogspot.com/2012/12/how-to-display-scorevariable-value-in.html

6. PPM reference to TUT6




