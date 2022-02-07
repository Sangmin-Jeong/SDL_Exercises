/*******************************************************************************
* File Name   :
* Description :
* Author      :
* Created     :
* Modified    :
*******************************************************************************/

// SDL includes pasted for convenience. Move/copy to relevant files.
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Engine.h"
#include "Enemy.h"
#include "Bullet.h"
#include "PlayerBullet.h"

int main(int argc, char* argv[])
{
	PlayerBullet pb;
	Bullet bul;
	Enemy en;
	Engine game;
	return game.Run();
}