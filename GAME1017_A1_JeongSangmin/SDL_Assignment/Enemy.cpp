#include "Enemy.h"

Enemy::Enemy(int x, int y, int life)
{
	m_enemySrc = { 0, 0, 98, 100 };
	m_enemyDst = { x, y, 98, 100 };
	m_enemyLife = 2;
}

void Enemy::Update()
{
	m_enemyDst.x -= SPEED;

}
