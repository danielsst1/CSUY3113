#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES 1

#include <SDL_mixer.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"
#include "Map.h"

struct GameState {
	Map* map;
	Entity* player;
	Entity* humans;
	Entity* rocks;
	int nextScene;
	int sceneNum;
	int lives;
	int humansFound;
	float endTime;
	float currTime;
};

class Scene {
public:
	Mix_Music* music;
	Mix_Chunk* found;

	GLuint fontTextureID;

	GameState state;
	virtual void Initialize() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void playJumpSound() = 0;
	virtual void Render(ShaderProgram* program) = 0;

	virtual int setLives(int num) = 0;
	virtual int getLives() = 0;
	virtual int loseLife() = 0;

	virtual int setHumansFound(int num) = 0;
	virtual int getHumansFound() = 0;
	virtual int findHuman() = 0;

	virtual void stopMotion() = 0;

	virtual int getRemainingTime() = 0;
};