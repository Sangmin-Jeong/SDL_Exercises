#include "PlayerBullet.h"
#include "Engine.h"
#include "MathManager.h"
#include "States.h"

PlayerBullet::PlayerBullet(int x, int y)
{
	m_playerBulletSrc = { 0, 0, 19, 6 };
	m_playerBulletDst = { x, y, 65, 27 };
}

void PlayerBullet::Update()
{
	m_playerBulletDst.x += 3;
}

void PlayerBullet::Render()
{
	
}