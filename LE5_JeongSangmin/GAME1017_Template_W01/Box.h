#pragma once
#ifndef _BOX_
#define _BOX_
#include <SDL.h>

class Sprite  // could be renamed Obstacle for A2
{
private:
	// SDL_Rect m_src; // HINT! for A2 or use GameObject.h
	SDL_Rect m_dst;
	SDL_Color m_color; // HINT! Don't need color for A2

public:
	Sprite(); // If you have a dynamic array of Sprite*
	Sprite(const SDL_Rect r, const SDL_Color c);
	//void Update(); // HINT! may need for A2, e.g. spinning blade
	void Render();
	friend class Box; // Alternately I can make a setter for m_dst

};

class Box // Proxy for the Obstacle/sprite
{
private:
	SDL_Point m_pos; // Position on screen. Y is optional
	Sprite* m_pSprite; // Pointer to a dynamic Sprite object(S) // HINT! Dynamic allocate?
										// m_pSprite = new Sprite[m_numSprites];
	int m_numSprites; // HINT! For bonus of A2

public:
	Box(const SDL_Point p, bool makeSprite = false, 
		const SDL_Rect r = {0,0,0,0}, const SDL_Color c = { 255,255,255,255 });
	~Box();
	Box* Clone(); // We are making Box also a prototype
	void Update();
	void Render();
	const SDL_Point GetPos() { return m_pos; }
	// Map stuff later


};

#endif

