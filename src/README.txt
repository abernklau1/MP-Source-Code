Magic Land / Tav, Gumpop, Glass of Soda, Doofenshmirtz
MP - Over Hill and Under Hill

This program features 4 characters that you can switch between to
explore our magical world filled with bunnies, trees, and even a cabin.

Controls:
W & S - forward and backward
A & D - left and right
Y & H - zoom in and out (w/ cams)
Drag mouse to navigate world
1 - arcball cam
2 - free cam
3 - first person viewport

Switching character controls:
Z = tav
X = being
C = horse
V = car

To build and run the project
Command line:
cd to mp (soon to change)
cmake -S . -B build
cmake --build build
./build/mp

Through CLion:
set target to Tav_A3
build and run
(Note: for me, setting the working directory to src and running also worked)

No bugs currently

Teammate contributions:
All: character drawing/movement
Doofenshmirtz - Point light calculations, debugging, attenuation
Tav - Arcball, freecam, first person cam, skybox texture, lighting
Glass of Soda - Arcball, freecam, first person cam, spotlight, point light
Gumpop - arcball, freecam, lighting errors, designing the skybox, skybox texture, designing the world and loading obj files

How long did this assignment take you?: 4 hrs
How much did the lab help you for this assignment?: 8
How fun was this assignment?: 9