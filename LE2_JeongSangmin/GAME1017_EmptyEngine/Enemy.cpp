#include "Enemy.h"
#include "Engine.h"
#include "TextureManager.h"
#include "EventManager.h"

Enemy::Enemy(SDL_Rect dst) :m_dst(dst), m_src({ 80,0,40,57 }), m_speed(1), m_tint(255)
{
	float x = m_dst.x;
	float y = m_dst.y;
	float w = m_dst.w;
	float h = m_dst.h;
	m_Fdst = { x,y,w,h };
	setHits(4);
}

void Enemy::Update()
{
	m_dst.y += m_speed;
	setCurrentDst();
}

void Enemy::Render()
{
	SDL_SetTextureColorMod(TEMA::GetTexture("enemy"), 255, m_tint, m_tint);
	SDL_RenderCopyEx(Engine::Instance().GetRenderer(), TEMA::GetTexture("enemy"), &m_src, &m_dst, 180.0, NULL, SDL_FLIP_NONE);
}

void Enemy::setHits(int c)
{
	m_hits = c;
}

int Enemy::getHits()
{
	return m_hits;
}

SDL_FRect Enemy::GetFDst() const
{
	return m_Fdst;
}

void Enemy::setCurrentDst()
{
	m_dst = this->GetDst();
}

SDL_Rect Enemy::getCurrentDst()
{
	return m_dst;
}

