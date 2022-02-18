#include "Enemy.h"

#include "Engine.h"

Enemy::Enemy(SDL_Rect d)
{
	m_src = { 0, 0, 98, 100 };
	m_dst = { (d.x + 1000), (100 + (rand() & 400)), 98, 100};
	m_life = 2;
	m_tint = 255;
}

void Enemy::Update()
{
	m_dst.x -= SPEED;
	SetDst(GetDst());
}

void Enemy::Render()
{
	SDL_SetTextureColorMod(m_pEnemyTexture, 255, m_tint, m_tint);
	if (GetLife() > 0)
	{
		SDL_RenderCopyEx(Engine::Instance().GetRenderer(), m_pEnemyTexture, &m_src, &m_dst, NULL, NULL, SDL_FLIP_HORIZONTAL);

	}
}
