#!/bin/bash
echo \n\nCompilo!\n\n
/usr/bin/g++ Launcher.C -O1 -fPIC -std=c++1z -pthread -I /usr/local/include -I /usr/include/GL NutPlanet.h NutPlanet.C -L /usr/local/lib64 -lGG  -lXt -lX11 -lXext -lGL -lGLU -o mio `pkg-config --cflags --libs gtk+-3.0` -L/home/enrico/Programmi/glfw-3.3/glfw-build/src/ -lglfw3 -lGL -lglut -lm -lGLU -Wall -pedantic
