#include "Bullet.h"
#include "Engine.h"

Bullet::Bullet(SDL_Rect d)
{
	m_src = { 0, 0, 19, 6 };
	m_dst = { d.x, (d.y+40), 65, 27 };
}

void Bullet::Update()
{
	m_dst.x -= 3;
}

void Bullet::Render()
{
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pBulletsTexture1, &m_src, &m_dst, NULL, NULL, SDL_FLIP_HORIZONTAL);
}