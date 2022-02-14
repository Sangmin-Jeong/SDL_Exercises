#pragma once
#ifndef _PLAYERBULLET_H_
#define _PLAYERBULLET_H_

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

class PlayerBullet
{
private:
	SDL_Rect m_playerBulletDst;
	SDL_Rect m_playerBulletSrc;
public:

	void getbulletDst();
	PlayerBullet(int = 0, int = 0);
	void Update();
};
#endif



