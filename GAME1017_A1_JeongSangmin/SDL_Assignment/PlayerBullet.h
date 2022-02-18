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
	SDL_Rect m_dst;
	SDL_Rect m_src;

	SDL_Texture* m_pPlayerBulletTexture;

public:
	PlayerBullet(SDL_Rect d);
	void Update();
	void Render();
	SDL_Rect& GetDst() { return m_dst; }
	SDL_Rect& GetSrc() { return m_src; }
	SDL_Texture* GetTexture() const { return m_pPlayerBulletTexture; }

	void SetTexture(SDL_Texture* t) { m_pPlayerBulletTexture = t; }

};
#endif



