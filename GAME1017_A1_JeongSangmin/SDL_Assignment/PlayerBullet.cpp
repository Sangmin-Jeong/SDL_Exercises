#include "PlayerBullet.h"
#include "Engine.h"
#include "MathManager.h"
#include "States.h"
#include "Player.h"

PlayerBullet::PlayerBullet(SDL_Rect d)
{
	m_src = { 0, 0, 19, 6 };
	m_dst = { d.x, (d.y+40), 65, 27};
}

void PlayerBullet::Update()
{
	m_dst.x += 3;
}

void PlayerBullet::Render()
{
	SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pPlayerBulletTexture, &m_src, &m_dst);
}