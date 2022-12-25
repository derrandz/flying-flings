#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <cstdio>
#include <sstream>
#include <string>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 600;
const int circleHeight = 80;
const int circleWidth = 80;

const int CURSOR_WIDTH = 80;

extern SDL_Window* 	 gWindow; //The window we'll be rendering to
extern SDL_Renderer* gRenderer; //The window renderer

extern SDL_Rect *gRectExPtr; //to eliminate leak momery in some cases;

extern TTF_Font *gFont;

extern std::string diePATH;

#endif // HEADER_H_INCLUDED
