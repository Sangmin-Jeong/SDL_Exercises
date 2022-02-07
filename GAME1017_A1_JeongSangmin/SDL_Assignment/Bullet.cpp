#include "Bullet.h"

Bullet::Bullet(int x, int y)
{
	m_bulletSrc = { 0, 0, 19, 6 };
	m_bulletDst = { x, y, 65, 27 };
}

void Bullet::Update()
{
	m_bulletDst.x -= 3;
}