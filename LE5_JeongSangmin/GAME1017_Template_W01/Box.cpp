#include "Box.h"
#include <SDL_render.h>
#include "Engine.h"
#define SCROLLSPEED 2

Sprite::Sprite(): m_dst({0,0,0,0}), m_src({ 0,0,0,0 })
{
}

Sprite::Sprite(int select, const SDL_Rect d, const SDL_Rect s): m_dst(d), m_src(s), m_pSelection(select)
{
	// Textures
	m_pSaw = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/saw.png");
	m_pSpike_wall = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/spike_wall.png");
	m_pSpike_lg = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/net.png");
	m_pSpike_sm = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/net.png");
}
// HINT! add default values to the parameters and not need a default constructor

void Sprite::Render()
{
	switch (m_pSelection)
	{
	case 0:
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pSaw, &m_src, &m_dst);
		break;
	case 1:
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pSpike_wall, &m_src, &m_dst);
		break;
	case 2:
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pSpike_lg, &m_src, &m_dst);
		break;
	case 3:
		SDL_RenderCopy(Engine::Instance().GetRenderer(), m_pSpike_sm, &m_src, &m_dst);
		break;
	}
	//SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), m_color.r, m_color.g, m_color.b, m_color.a);
	//SDL_RenderFillRect(Engine::Instance().GetRenderer(), &m_dst);
}

Box::Box(const SDL_Point p, bool makeSprite, int select, const SDL_Rect d, const SDL_Rect s)
	: m_pos(p), m_pSprite(nullptr) // r and c are for Sprite
{
	if (makeSprite)
	{
		// m_pSprite = new Sprite[m_numSprites]; // For dynamic array
		m_pSprite = new Sprite(select, d, s);
	}
}

Box::~Box()
{
	if (m_pSprite != nullptr)
	{
		delete m_pSprite;
	}
}

Box* Box::Clone()
{
	Box* clone = new Box(this->m_pos, false); // Deep copy for brand new Box obj
	clone->m_pSprite = new Sprite(this->m_pSprite->m_pSelection, this->m_pSprite->m_dst, this->m_pSprite->m_src);
	return clone;
}

void Box::Update()
{
	m_pos.x -= SCROLLSPEED;
	if (m_pSprite != nullptr)
	{
		m_pSprite->m_dst.x -= SCROLLSPEED;
	}
}

void Box::Render()
{
	if (m_pSprite != nullptr)
	{
		m_pSprite->Render();
	}
	// If we want to render a border around each box
	SDL_Rect dst = { m_pos.x, m_pos.y, 128, 128 };
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 156, 230, 200, 255);
	SDL_RenderDrawRect(Engine::Instance().GetRenderer(), &dst);

}

