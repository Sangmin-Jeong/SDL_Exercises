#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include "../../include/GLM/glm/glm.hpp"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <ctime>
#include <vector>
#include "Enemy.h"
#include "Bullet.h"
#include "PlayerBullet.h"

#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
#define SPEED 2

using namespace std;

class Engine
{
private:

	bool m_running = false;
	bool m_fire = false;
	Uint32 m_start, m_end, m_delta, m_fps;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;


private:
	Engine() {  }
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	void Render();
	void Sleep();
	bool KeyDown(SDL_Scancode c);
	void Update();

public:
	static Engine& Instance();
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	bool& Running() { return m_running; }
	int Run();
};
#endif
