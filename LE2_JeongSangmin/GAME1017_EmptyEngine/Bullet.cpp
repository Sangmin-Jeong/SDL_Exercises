#include "Bullet.h"
#include "Engine.h"
#include "MathManager.h"
#include "States.h"

Bullet::Bullet(SDL_FRect dst, double dx, double dy):m_dst(dst), m_dx(dx), m_dy(dy)
{
	m_rect.x = dst.x;
	m_rect.y = dst.y;
	m_rect.w = dst.w;
	m_rect.y = dst.y;
}

void Bullet::Update()
{
	m_dst.x += m_dx;
	m_dst.y += m_dy;
	setCurrentPoint();
}

void Bullet::Render()
{
	SDL_SetRenderDrawColor(Engine::Instance().GetRenderer(), 240, 255, 0, 255);
	SDL_RenderFillRectF(Engine::Instance().GetRenderer(), &m_dst);
}

SDL_Rect Bullet::GetRect()
{
	return m_rect;
}

SDL_Point Bullet::getCurrentPoint()
{
	return m_point;
}

void Bullet::setCurrentPoint()
{
	m_point = MAMA::ConvertFPoint2Point(this->GetPos());
}
