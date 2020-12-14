#include "Level1.h"

#define LEVEL1_HUMAN_COUNT 3
#define LEVEL1_ROCK_COUNT 6

#define PLAYER_SPEED 1.5f
#define HUMAN_SPEED 1.0f

#define LEVEL1_WIDTH 22
#define LEVEL1_HEIGHT 8
unsigned int level1_data[] =
{//     1    2    3    4    5    6    7    8    9   10   11   12   13   14   15   16   17   18   19   20   21   22   23
 173, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188,// 188, 188,
 173,   0,   0,   0,   0, 122,   0,   0,   0,   0,   0,   0,   0,   0, 122,   0,   0,   0,   0, 122, 188,   0,//   0,   0, //-1
 173,   0, 122,   0, 122,   0,   0,   0,   0,   0, 170, 122, 122,   0,   0,   0,   0,   0, 122,   0,   0, 122,//   0,   0, //-2
 173,   0, 122,   0, 122,   0, 122, 122, 122,   0, 186,   0, 122,   0, 122, 122,   0, 122,   0,   0, 172,   0,//   0,   0, //-3
 173,   0, 170,   0,   0,   0, 122,   0, 122,   0,   0,   0,   0,   0, 170,   0,   0, 122,   0,   0, 172,   0,//   0,   0, //-4
 173,   0, 186,   0, 170, 170, 170,   0, 122,   0, 122, 122,   0,   0, 186, 122, 122, 122,   0, 122, 172,   0,//   0,   0, //-5
 173,   0, 170,   0,   0,   0,   0,   0,   0,   0, 122,   0,   0,   0,   0,   0,   0,   0,   0,   0, 172,   0,//   0,   0, //-6
 173, 122, 186, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 122, 172, 122//, 122, 122
};

Level1::gameMode mode;
void Level1::Initialize() {
    state.sceneNum = 1;

    float levelTime = 120000; //120000ms = 120sec

    state.endTime = SDL_GetTicks() + levelTime;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("pokemon-center.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME * 0.10f);

    found = Mix_LoadWAV("capture.wav");
    Mix_VolumeChunk(found, MIX_MAX_VOLUME * 0.10f);
    state.nextScene = -1;

	//GLuint mapTextureID = Util::LoadTexture("tileset.png");
	//state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 4, 1);
    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 16, 16);

    fontTextureID = Util::LoadTexture("font1.png");

    ////////////////////////////////////////////////////////////////////////////////////////////
    // Initialize game objects
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -6, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -0, 0);
    state.player->speed = PLAYER_SPEED;
    state.player->textureID = Util::LoadTexture("characters.png");

    state.player->animRight = new int[3]{ 81, 82, 83 };
    state.player->animLeft = new int[3]{ 69, 70, 71 };
    state.player->animUp = new int[3]{ 93, 94, 95 };
    state.player->animDown = new int[3]{ 57, 58, 59 }; 

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 3;;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 12;
    state.player->animRows = 8;

    state.player->height = 0.7f;
    state.player->width = 0.7f;

    state.player->jumpPower = 6.5f;

    //////////////////////////////////////////////////////////////////////////////////////
    // initialize humans
    state.humans = new Entity[LEVEL1_HUMAN_COUNT];
    GLuint HUMANTextureID = Util::LoadTexture("characters.png");

    state.humans[0].animRight = new int[3]{ 24, 25, 26 };
    state.humans[0].animLeft = new int[3]{ 12, 13, 14 };
    state.humans[0].animUp = new int[3]{ 36, 37, 38 };
    state.humans[0].animDown = new int[3]{ 0, 1, 2 };

    state.humans[0].animIndices = state.humans[0].animRight;
    state.humans[0].animFrames = 3; // 4;
    state.humans[0].animIndex = 0;
    state.humans[0].animTime = 0;
    state.humans[0].animCols = 12; //4;
    state.humans[0].animRows = 8; //4;

    state.humans[0].entityType = HUMAN;
    state.humans[0].textureID = HUMANTextureID;
    state.humans[0].height = 0.8f;
    state.humans[0].width = 0.8f;
    state.humans[0].position = glm::vec3(15, -4, 0);
    state.humans[0].speed = HUMAN_SPEED;
    state.humans[0].acceleration = glm::vec3(0, 0.0f, 0);
    state.humans[0].aiType = WAITANDGOHORIZONTAL;
    state.humans[0].aiState = IDLE;

    /// //////////////////////////////////////////////////////////////
    /// human2
    state.humans[1].animRight = new int[3]{ 24, 25, 26 };
    state.humans[1].animLeft = new int[3]{ 12, 13, 14 };
    state.humans[1].animUp = new int[3]{ 36, 37, 38 };
    state.humans[1].animDown = new int[3]{ 0, 1, 2 };

    state.humans[1].animIndices = state.humans[1].animDown;
    state.humans[1].animFrames = 3; // 4;
    state.humans[1].animIndex = 0;
    state.humans[1].animTime = 0;
    state.humans[1].animCols = 12; //4;
    state.humans[1].animRows = 8; //4;

    state.humans[1].entityType = HUMAN;
    state.humans[1].textureID = HUMANTextureID;
    state.humans[1].height = 0.8f;
    state.humans[1].width = 0.8f;
    state.humans[1].position = glm::vec3(1, -3, 0);
    state.humans[1].speed = HUMAN_SPEED;
    state.humans[1].acceleration = glm::vec3(0, 0.0f, 0);
    state.humans[1].aiType = WAITANDGOVERTICAL;
    state.humans[1].aiState = IDLE;

    // human3
    state.humans[2].animRight = new int[3]{ 24, 25, 26 };
    state.humans[2].animLeft = new int[3]{ 12, 13, 14 };
    state.humans[2].animUp = new int[3]{ 36, 37, 38 };
    state.humans[2].animDown = new int[3]{ 0, 1, 2 };

    state.humans[2].animIndices = state.humans[2].animLeft;
    state.humans[2].animFrames = 3; // 4;
    state.humans[2].animIndex = 0;
    state.humans[2].animTime = 0;
    state.humans[2].animCols = 12; //4;
    state.humans[2].animRows = 8; //4;

    state.humans[2].entityType = HUMAN;
    state.humans[2].textureID = HUMANTextureID;
    state.humans[2].height = 0.8f;
    state.humans[2].width = 0.8f;
    state.humans[2].position = glm::vec3(7, -4, 0);
    state.humans[2].speed = HUMAN_SPEED;
    state.humans[2].acceleration = glm::vec3(0, 0.0f, 0);
    state.humans[2].aiType = WAITANDGOHORIZONTAL;
    state.humans[2].aiState = IDLE;

    //////////////////////////////////////////////////////////////////////////////////////
    // initialize rocks
    state.rocks = new Entity[LEVEL1_ROCK_COUNT];
    GLuint ROCKTextureID = Util::LoadTexture("tiles.png");

    state.rocks[0].animRight = new int[1]{ 33 };
    state.rocks[0].animIndices = state.rocks[0].animRight;
    state.rocks[0].animFrames = 1; // 4;
    state.rocks[0].animIndex = 0;
    state.rocks[0].animTime = 0;
    state.rocks[0].animCols = 16;
    state.rocks[0].animRows = 16;

    state.rocks[0].entityType = ROCK;
    state.rocks[0].textureID = ROCKTextureID;
    state.rocks[0].height = 0.8f;
    state.rocks[0].width = 0.8f;
    state.rocks[0].position = glm::vec3(2, -1, 0);

    //rock2
    state.rocks[1].animRight = new int[1]{ 33 };
    state.rocks[1].animIndices = state.rocks[1].animRight;
    state.rocks[1].animFrames = 1; // 4;
    state.rocks[1].animIndex = 0;
    state.rocks[1].animTime = 0;
    state.rocks[1].animCols = 16;
    state.rocks[1].animRows = 16;

    state.rocks[1].entityType = ROCK;
    state.rocks[1].textureID = ROCKTextureID;
    state.rocks[1].height = 0.8f;
    state.rocks[1].width = 0.8f;
    state.rocks[1].position = glm::vec3(9, -1, 0);

    //rock3
    state.rocks[2].animRight = new int[1]{ 33 };
    state.rocks[2].animIndices = state.rocks[2].animRight;
    state.rocks[2].animFrames = 1; // 4;
    state.rocks[2].animIndex = 0;
    state.rocks[2].animTime = 0;
    state.rocks[2].animCols = 16;
    state.rocks[2].animRows = 16;

    state.rocks[2].entityType = ROCK;
    state.rocks[2].textureID = ROCKTextureID;
    state.rocks[2].height = 0.8f;
    state.rocks[2].width = 0.8f;
    state.rocks[2].position = glm::vec3(9, -2, 0);

    //rock4
    state.rocks[3].animRight = new int[1]{ 33 };
    state.rocks[3].animIndices = state.rocks[3].animRight;
    state.rocks[3].animFrames = 1; // 4;
    state.rocks[3].animIndex = 0;
    state.rocks[3].animTime = 0;
    state.rocks[3].animCols = 16;
    state.rocks[3].animRows = 16;

    state.rocks[3].entityType = ROCK;
    state.rocks[3].textureID = ROCKTextureID;
    state.rocks[3].height = 0.8f;
    state.rocks[3].width = 0.8f;
    state.rocks[3].position = glm::vec3(7, -6, 0);

    //rock5
    state.rocks[4].animRight = new int[1]{ 33 };
    state.rocks[4].animIndices = state.rocks[4].animRight;
    state.rocks[4].animFrames = 1; // 4;
    state.rocks[4].animIndex = 0;
    state.rocks[4].animTime = 0;
    state.rocks[4].animCols = 16;
    state.rocks[4].animRows = 16;

    state.rocks[4].entityType = ROCK;
    state.rocks[4].textureID = ROCKTextureID;
    state.rocks[4].height = 0.8f;
    state.rocks[4].width = 0.8f;
    state.rocks[4].position = glm::vec3(11, -4, 0);

    //rock5
    state.rocks[5].animRight = new int[1]{ 33 };
    state.rocks[5].animIndices = state.rocks[5].animRight;
    state.rocks[5].animFrames = 1; // 4;
    state.rocks[5].animIndex = 0;
    state.rocks[5].animTime = 0;
    state.rocks[5].animCols = 16;
    state.rocks[5].animRows = 16;

    state.rocks[5].entityType = ROCK;
    state.rocks[5].textureID = ROCKTextureID;
    state.rocks[5].height = 0.8f;
    state.rocks[5].width = 0.8f;
    state.rocks[5].position = glm::vec3(18, -5, 0);

    mode = PLAY;
}

void Level1::Update(float deltaTime) {
    int collisionObj;

	collisionObj = state.player->Update(deltaTime, state.player, state.humans, LEVEL1_HUMAN_COUNT, state.map);

    for (int i = 0; i < LEVEL1_HUMAN_COUNT; i++) {
        state.humans[i].Update(deltaTime, state.player, state.humans, LEVEL1_HUMAN_COUNT, state.map);
    }

    //fall off map bottom = -8.1
    //if (state.player->position.y < -8.1f || collisionObj != -1) loseLife();
    if (collisionObj != -1) findHuman();

    state.player->checkRockContact(deltaTime, state.player, state.rocks, LEVEL1_ROCK_COUNT, state.map);
    for (int i = 0; i < LEVEL1_ROCK_COUNT; i++) {
        state.rocks[i].Update(deltaTime, state.player, state.rocks, LEVEL1_ROCK_COUNT, state.map);
    }

    //triggers moving to next scene
    if (state.player->position.x >= getLevelWidth() && getHumansFound() == LEVEL1_HUMAN_COUNT) { //12) {
        Mix_FreeChunk(found);
        Mix_FreeMusic(music);
        state.nextScene = 2;
    }
}

void Level1::playJumpSound() {
    //Mix_PlayChannel(-1, jump, 0);
}

void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
    for (int i = 0; i < LEVEL1_HUMAN_COUNT; i++) {
        state.humans[i].Render(program);
    }

    for (int i = 0; i < LEVEL1_ROCK_COUNT; i++) {
        state.rocks[i].Render(program);
    }

    state.player->Render(program);
    if (state.player->position.x > 5 && state.player->position.x < (getLevelWidth() - 5)) {
        showText(program, state.player->position.x);
    }
    else if (state.player->position.x <= 5) {
        //text at front
        showText(program, 5.0f);
    }
    else {
        //text at end
        showText(program, getLevelWidth() - 5.0f);
    }	
}

void Level1::showText(ShaderProgram* program, float xVal) {
    Util::DrawText(program, fontTextureID, "Level 1", 0.5f, -0.25f, glm::vec3(xVal - 4.5f, -0.5f, 0));
    Util::DrawText(program, fontTextureID, "Found: " + std::to_string(getHumansFound()) + "/" + std::to_string(LEVEL1_HUMAN_COUNT) + " Humans", 0.5f, -0.25f, glm::vec3(xVal - 2.25f, -0.5f, 0));
    //Util::DrawText(program, fontTextureID, "Lives: " + std::to_string(getLives()), 0.5f, -0.25f, glm::vec3(xVal + 3.0f, -0.5f, 0));

    if (mode == PLAY) Util::DrawText(program, fontTextureID, "Time: " + std::to_string(getRemainingTime()), 0.5f, -0.25f, glm::vec3(xVal + 2.75f, -0.5f, 0));//xVal - 4.5f, -1.5f, 0));

    if (mode == LOSE) Util::DrawText(program, fontTextureID, "You Lose!", 0.75f, -0.25f, glm::vec3(xVal - 1.5f, -2.5f, 0));
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
            state.humans[i].isActive = true;
            state.humans[i].aiState = IDLE;
        }
        state.humans[0].position = glm::vec3(7, -5, 0);
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

    int sfxChannel = 1;
   
    Mix_PlayChannel(sfxChannel, found, 0);
   
    //stop movement while sound is played
    while (Mix_Playing(sfxChannel) != 0) {
        Mix_PauseMusic();
        state.player->StopMovement();
        for (int i = 0; i < LEVEL1_HUMAN_COUNT; i++) {
            state.humans->StopMovement();
        }
    }
    //resume movement

    state.player->speed = PLAYER_SPEED;
    for (int i = 0; i < LEVEL1_HUMAN_COUNT; i++) {
        state.humans->speed = HUMAN_SPEED;
    }
    Mix_ResumeMusic();

    //add time to make up for pause
    int approxSfxTime = 4000; //sound effect time is about 4000ms
    state.endTime += approxSfxTime;

    return state.humansFound;
}

void Level1::stopMotion() {
    state.player->StopMovement();

    for (int i = 0; i < LEVEL1_HUMAN_COUNT; i++) {
        state.humans->StopMovement();
    }
}

void Level1::loseGame() {
    stopMotion();
    Mix_HaltMusic();
    mode = LOSE;
}

//return index of furthest right wall
int Level1::getLevelWidth() {
    return LEVEL1_WIDTH - 2;
}

int Level1::getRemainingTime() {
    float time = state.endTime - SDL_GetTicks();
    time = round(time / 1000);
    //if (time == 0) loseGame();
    return(time);
}