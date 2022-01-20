#pragma once
#ifndef _ENGINE_H_
#define _ENGINE_H_

#include <iostream>
#include <map>
#include <string>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SoundManager.h"
#define FPS 60
#define WIDTH 1024
#define HEIGHT 768
using namespace std;

class Engine
{
private: // private properties.
	bool m_running = false;
	Uint32 m_start, m_end, m_delta, m_fps;
	const Uint8* m_keystates;
	SDL_Window* m_pWindow;
	SDL_Renderer* m_pRenderer;

	//// Sound effect objects
	//Mix_Chunk* m_begin;

	//// Music track objects.
	//Mix_Music* m_horror;
	//Mix_Music* m_boss;

	//// Map of Mix_Chunk*
	//map<string, Mix_Music*> m_mapMusic;
	//map<string, Mix_Chunk*> m_mapSFX;

private: // private method prototypes.
	Engine() // A private constructor prevents external instantation.
	{
		cout << "Creating instance of Engine..." << endl;
	}
	int Init(const char* title, int xPos, int yPos, int width, int height, int flags);
	void Clean();
	void Wake();
	void HandleEvents();
	// Moved KeyDown from here.
	void Update();
	void Render();
	void Sleep();

public: // public method prototypes.
	int Run();

	// Add static method for singleton here
	static Engine& Instance();
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
	bool KeyDown(SDL_Scancode c);

};

#endif

// Reminder: you can ONLY have declarations in headers, not logical code
