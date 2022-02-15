#pragma once
#ifndef _ASTEROIDSPRITES_H_
#define _ASTEROIDSPRITES_H_

#include "GameObject.h"
#include "SDL.h"
#include <vector>

class Asteroid : public SpriteObject
{
public:
	Asteroid(SDL_Rect s, SDL_FRect d);
	void Update();
	void Render();
	const SDL_FPoint& GetCenter() { return m_center; }
	const double& GetRadius() { return m_radius; }
	

	int Getlife() { return m_life; }
	void Setlife(int c) { m_life = c; }

	void SetAngle(double a) { this->m_angle = a; }
	double GetAngle() { return this->m_angle; }

	void SetDelXY(double dx, double dy)
	{
		m_dx = m_dx + dx/3.9;
		m_dy = m_dy + dy/3.9;
	}
	
private:
	double m_angle,
		m_radius,
		m_rotSpeed;
	double m_dx, m_dy;

	int m_life;
	
};

class AsteroidField : public GameObject
{ // Manager class for the individual asteroids.
public:
	AsteroidField(unsigned int sz = 6);
	~AsteroidField();
	void Update();
	void Render();
	vector<Asteroid*>& GetAsteroids() { return m_asteroids; }
	const unsigned int GetSize() { return m_size; }

private:
	vector<Asteroid*> m_asteroids;
	int m_size;
};

class Bullet : public SpriteObject
{
public:
	Bullet(SDL_Rect s, SDL_FRect d, const char* key, const double angle);
	void Update();
	void Render();
	const SDL_FPoint& GetCenter() { return m_center; }
	const double& GetRadius() { return m_radius; }

	double GetDelX() { return m_dx; }
	double GetDelY() { return m_dy; }

	void SetAngle(double a) { this->m_angle = a; }
	double GetAngle() { return this->m_angle; }

private:
	SDL_FPoint m_center;
	int m_ctr, m_ctrMax;
	double m_angle,
		m_dx, m_dy,
		m_radius;
	std::string m_key;
};

class ShipAsteroids : public AnimatedSpriteObject
{
public: // Methods.
	ShipAsteroids(SDL_Rect s, SDL_FRect d);
	~ShipAsteroids();
	void Update();
	void Render();
	const SDL_FPoint& GetCenter() { return m_center; }
	const double& GetRadius() { return m_radius; }
	vector<Bullet*>& GetBullets() { return m_bullets; }
private: // Properties.
	SDL_FPoint m_center;
	double m_radius;
	// Properties for movement:
	double m_angle,
		m_dx, m_dy,  // Vector deltas
		m_velX, m_velY, // Velocities, but no acceleration yet
		m_velMax,
		m_rotSpeed,
		m_thrust; // Speed factor/force
	
	enum PlayerState { STATE_IDLING, STATE_MOVING } m_state;
	vector<Bullet*> m_bullets;
};

#endif