#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <ctime>
#include <vector>

#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define SPEED 2

using namespace std;

class Bullet
{
private:

public:

	SDL_Rect m_bulletDst;
	SDL_Rect m_bulletSrc;
	Bullet(int = 0, int = 0);
	void Update();
};
#endif



