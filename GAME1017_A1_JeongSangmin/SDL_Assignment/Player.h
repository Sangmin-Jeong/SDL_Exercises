#pragma once
#include <SDL_rect.h>
#include <SDL_render.h>

class Player
{
private:
	SDL_Rect m_dst;
	SDL_Rect m_src;

	SDL_Texture* m_pPlayerTexture;

	int m_life;
	int m_tint;
public:
	Player();
	void Update();
	void Render();

	SDL_Rect& GetDst() { return m_dst; }
	SDL_Rect& GetSrc() { return m_src; }
	int GetLife() { return m_life; }
	int GetTint() { return m_tint; }
	SDL_Texture* GetTexture() const { return m_pPlayerTexture; }

	void SetLife(int l) { m_life = l; }
	void SetTint(int t) { m_tint = t; }
	void SetTexture(SDL_Texture* t) { m_pPlayerTexture = t; }
	void SetDst(SDL_Rect d) { m_dst = d; }

	static Player& instance();
};