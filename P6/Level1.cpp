#include "Level1.h"

#define LEVEL1_HUMAN_COUNT 2

#define LEVEL1_WIDTH 24
#define LEVEL1_HEIGHT 8
unsigned int level1_data[] =
{
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
 3, 0, 0, 0, 0, 3, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
 3, 0, 3, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0,
 3, 0, 3, 0, 3, 0, 0, 0, 3, 0, 1, 0, 0, 0, 1, 1, 0, 3, 0, 0, 0, 0, 0, 0,
 3, 0, 3, 0, 0, 0, 0, 1, 3, 0, 2, 0, 0, 0, 2, 0, 0, 3, 0, 0, 0, 0, 0, 0,
 3, 0, 3, 0, 0, 0, 0, 0, 3, 0, 2, 1, 0, 0, 2, 0, 2, 3, 0, 0, 0, 0, 0, 0,
 3, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3
};

Level1::gameMode mode;
void Level1::Initialize() {
    state.sceneNum = 1;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("dooblydoo.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME * 0.10f);

    jump = Mix_LoadWAV("bounce.wav");
    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("tileset.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);

    fontTextureID = Util::LoadTexture("font1.png");

    // Initialize game objects
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -6, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -0, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("characters.png");//("george_0.png");

    /*state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };*/

    state.player->animRight = new int[4]{ 81, 82, 83 };
    state.player->animLeft = new int[4]{ 69, 70, 71 };
    state.player->animUp = new int[4]{ 93, 94, 95 };
    state.player->animDown = new int[4]{ 57, 58, 59 }; 

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 3; // 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 12; //4;
    state.player->animRows = 8; //4;

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.player->jumpPower = 6.5f;

    state.enemies = new Entity[LEVEL1_HUMAN_COUNT];
    GLuint HUMANTextureID = Util::LoadTexture("ctg.png");

    state.enemies[0].entityType = HUMAN;
    state.enemies[0].textureID = HUMANTextureID;
    state.enemies[0].height = 0.8f;
    state.enemies[0].width = 0.8f;
    state.enemies[0].position = glm::vec3(7, -1, 0);
    state.enemies[0].speed = 1;
    state.enemies[0].acceleration = glm::vec3(0, 0.0f, 0);
    state.enemies[0].aiType = WAITANDGOHORIZONTAL;
    state.enemies[0].aiState = IDLE;

    state.enemies[1].entityType = HUMAN;
    state.enemies[1].textureID = HUMANTextureID;
    state.enemies[1].height = 0.8f;
    state.enemies[1].width = 0.8f;
    state.enemies[1].position = glm::vec3(1, -3, 0);
    state.enemies[1].speed = 1;
    state.enemies[1].acceleration = glm::vec3(0, 0.0f, 0);
    state.enemies[1].aiType = WAITANDGOVERTICAL;
    state.enemies[1].aiState = IDLE;

    mode = PLAY;
}

void Level1::Update(float deltaTime) {
    int collisionObj;

	collisionObj = state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_HUMAN_COUNT, state.map);

    for (int i = 0; i < LEVEL1_HUMAN_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_HUMAN_COUNT, state.map);
    }
    
    //fall off map bottom = -8.1
    //if (state.player->position.y < -8.1f || collisionObj != -1) loseLife();
    if (collisionObj != -1) findHuman();

    //triggers moving to next scene
    if (state.player->position.x >= 20) { //12) {
        state.nextScene = 2;
    }
}

void Level1::playJumpSound() {
    Mix_PlayChannel(-1, jump, 0);
}

void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
    for (int i = 0; i < LEVEL1_HUMAN_COUNT; i++) {
        state.enemies[i].Render(program);
    }

    state.player->Render(program);
    //Util::DrawText(program, fontTextureID, std::to_string(state.player->position.x) + std::to_string(state.player->position.y), 0.5f, -0.25f, glm::vec3(state.player->position.x - 2.0f, -2.5f, 0));

    if (state.player->position.x > 5) {
        Util::DrawText(program, fontTextureID, "Level 1", 0.5f, -0.25f, glm::vec3(state.player->position.x - 4.5f, -0.5f, 0));
        Util::DrawText(program, fontTextureID, "Found: " + std::to_string(getHumansFound()) + "/" + std::to_string(LEVEL1_HUMAN_COUNT) + " Humans", 0.5f, -0.25f, glm::vec3(state.player->position.x - 2.0f, -0.5f, 0));
        Util::DrawText(program, fontTextureID, "Lives: " + std::to_string(getLives()), 0.5f, -0.25f, glm::vec3(state.player->position.x + 3.0f, -0.5f, 0));

        if (mode == LOSE) Util::DrawText(program, fontTextureID, "You Lose!", 0.75f, -0.25f, glm::vec3(state.player->position.x - 1.5f, -2.5f, 0));
    }
    else {
        Util::DrawText(program, fontTextureID, "Level 1", 0.5f, -0.25f, glm::vec3(0.5f, -0.5f, 0));
        Util::DrawText(program, fontTextureID, "Found: " + std::to_string(getHumansFound()) + "/" + std::to_string(LEVEL1_HUMAN_COUNT) + " Humans", 0.5f, -0.25f, glm::vec3(3.0f, -0.5f, 0));
        Util::DrawText(program, fontTextureID, "Lives: " + std::to_string(getLives()), 0.5f, -0.25f, glm::vec3(8.0f, -0.5f, 0));

        if (mode == LOSE) Util::DrawText(program, fontTextureID, "You Lose!", 0.75f, -0.25f, glm::vec3(3.5f, -2.5f, 0));
    }	
}

int Level1::setLives(int num) {
    state.lives = num;
    return state.lives;
}

int Level1::getLives() {
    return state.lives;
}

int Level1::loseLife() {
    if (getLives() > 0) state.lives--;
    
    if (getLives() == 0) loseGame();
    else {
        state.player->position = glm::vec3(1, 0, 0);
        for (int i = 0; i < LEVEL1_HUMAN_COUNT; i++) {
            state.enemies[i].isActive = true;
            state.enemies[i].aiState = IDLE;
        }
        state.enemies[0].position = glm::vec3(7, -5, 0);
    }
    return state.lives;
}

int Level1::setHumansFound(int num) {
    state.humansFound = num;
    return state.humansFound;
}

int Level1::getHumansFound() {
    return state.humansFound;
}

int Level1::findHuman() {
    state.humansFound++;
    return state.humansFound;
}

void Level1::stopMotion() {
    state.player->StopMovement();

    for (int i = 0; i < LEVEL1_HUMAN_COUNT; i++) {
        state.enemies->StopMovement();
    }
}

void Level1::loseGame() {
    stopMotion();
    mode = LOSE;
}