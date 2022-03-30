#include "Engine.h"
#include <iostream>
#include <ctime>
#define WIDTH 1024
#define HEIGHT 768
#define FPS 60
using namespace std;

Engine::Engine():m_bRunning(false){	cout << "Engine class constructed!" << endl; }
Engine::~Engine(){}

bool Engine::Init(const char* title, int xpos, int ypos, int width, int height, int flags)
{
	cout << "Initializing game..." << endl;
	// Attempt to initialize SDL.
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		// Create the window.
		m_pWindow = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_pWindow != nullptr) // Window init success.
		{
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, 0);
			if (m_pRenderer != nullptr) // Renderer init success.
			{
				// Nothing needed right now.
			}
			else return false; // Renderer init fail.
		}
		else return false; // Window init fail.
	}
	else return false; // SDL init fail.
	m_fps = (Uint32)round((1 / (double)FPS) * 1000); // Sets FPS in milliseconds and rounds.
	m_iKeystates = SDL_GetKeyboardState(nullptr);
	srand((unsigned)time(NULL)); // Seed random number sequence.

	m_pTexture = IMG_LoadTexture(Engine::Instance().GetRenderer(), "Img/bg.png");

	// Create the vector now.
	m_vec.reserve(9);
	for (int i = 0; i < 9; i++)
	{
		m_vec.push_back(new Box({ 128 * i,550 }));
	}
	// Create the map of Boxes with sprites
	m_protos.emplace("net", new Box({ 1024, 550 }, true, 0, { 1024,550,128,128 }, { 0,0,849,341}));
	m_protos.emplace("anchor", new Box({ 1024, 550 }, true, 1, { 1056,0,64,384 }, { 0,0,219,620}));
	m_protos.emplace("barrel_lg", new Box({ 1024, 550 }, true, 2, { 1024,615,128,64 }, { 0,0,264,212}));
	m_protos.emplace("barrel_sm", new Box({ 1024, 550 }, true, 3, { 1056,645,64,32 }, { 0,0,264,212 }));


	// Set the gap properties
	m_gapCtr = 0;
	m_gapMax = 3;

	m_bRunning = true; // Everything is okay, start the engine.
	cout << "Init success!" << endl;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start;
	if (m_delta < m_fps) // Engine has to sleep.
		SDL_Delay(m_fps - m_delta);
}

void Engine::HandleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // User pressed window's 'x' button.
			m_bRunning = false;
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				m_bRunning = false;
			break;
		}
	}
}

// Keyboard utility function.
bool Engine::KeyDown(SDL_Scancode c)
{
	if (m_iKeystates != nullptr)
	{
		if (m_iKeystates[c] == 1)
			return true;
		else
			return false;
	}
	return false;
}

void Engine::Update()
{
	// Check if first column of main vector goes out of bounds.
	if (m_vec[0]->GetPos().x <= -128)
	{
		// Pop the first element off.
		delete m_vec[0];
		m_vec.erase(m_vec.begin()); // "pop_front"

		// Add a new Box to the end
		if (m_gapCtr++ % m_gapMax == 0) // Create Box with Sprite
		{
			/*SDL_Color col = { 100 + rand() % 156,100 + rand() % 156,100 + rand() % 156,255};*/
			//m_vec.push_back(new Box({ 1024, 384 }, true, { 1024,384,128,128 }, col));
			m_vec.push_back(m_protos[m_keys[rand()%4]]->Clone());
		}
		else
		{
			m_vec.push_back(new Box({ 1024, 550 })); // Create empty Box
		}
		
	}

	// Scroll(Update) the boxes, which scroll themselves.
	for (unsigned i = 0; i < m_vec.size(); i++)
	{
		m_vec[i]->Update();
	}
}

void Engine::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.
	SDL_RenderCopy(m_pRenderer, m_pTexture, NULL, NULL);
	// Render stuff.
	for (unsigned i = 0; i < m_vec.size(); i++)
	{
		m_vec[i]->Render();
	}
	
	// Draw anew.
	SDL_RenderPresent(m_pRenderer);
}

void Engine::Clean()
{
	cout << "Cleaning game." << endl;
	for (unsigned i = 0; i < m_vec.size(); i++)
	{
		delete m_vec[i];
		m_vec[i] = nullptr;
	}
	m_vec.clear();
	m_vec.shrink_to_fit(); // Optional

	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

int Engine::Run()
{
	if (m_bRunning) // What does this do and what can it prevent?
		return -1; 
	if (Init("GAME1017_LE5_JeongSangmin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0) == false)
		return 1;
	while (m_bRunning) // Main engine loop.
	{
		Wake();
		HandleEvents();
		Update();
		Render();
		if (m_bRunning)
			Sleep();
	}
	Clean();
	return 0;
}

Engine& Engine::Instance()
{
	static Engine instance; // C++11 will prevent this line from running more than once. Magic statics.
	return instance;
}

SDL_Renderer* Engine::GetRenderer() { return m_pRenderer; }
