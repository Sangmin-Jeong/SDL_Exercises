#pragma once
#ifndef _BULLET_H_
#define _BULLET_H_

#include <SDL.h>

class Bullet
{
private:
	SDL_FRect m_dst;
	double m_dx, m_dy;
	SDL_Rect m_rect;
	SDL_Point m_point;

public:
	Bullet(SDL_FRect dst, double dx, double dy);
	void Update();
	void Render();
	SDL_FPoint GetPos() const { return { m_dst.x + m_dst.w / 2, m_dst.y + m_dst.h / 2 }; }
	SDL_Rect GetRect();
	SDL_Point getCurrentPoint();
	void setCurrentPoint();
};

#endif