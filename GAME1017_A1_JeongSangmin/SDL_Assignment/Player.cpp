#include "Player.h"
#include "Engine.h"
#include "TextureManager.h"

Player::Player()
{
	m_dst = { WIDTH / 8, HEIGHT / 2, 94, 80 };
	m_src = { 0, 0, 94, 80 };
	m_life = 4; // Player Life, 0 = game over
	m_tint = 255;
}

void Player::Update()
{
	SetDst(GetDst());
}

void Player::Render()
{
	SDL_SetTextureColorMod(m_pPlayerTexture, 255, m_tint, m_tint);
	if (GetLife() > 0)
	{
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pPlayerTexture, &m_src, &m_dst);
	}
}

Player& Player::instance()
{
	static Player instance; // Creating an object of Engine. Magic statics.
	return instance;
}
