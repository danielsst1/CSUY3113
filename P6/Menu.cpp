#include "Menu.h"

#define Menu_ENEMY_COUNT 1

#define Menu_WIDTH 10
#define Menu_HEIGHT 8
//check Map.cpp for making tiles visual only 
unsigned int Menu_data[] =
{
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,// 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,// 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,// 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,// 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,// 0, 0, 0, 0,
 0,11,12,12,12,12,12,12,12,13,// 0, 0, 0, 0,
 0,27,28,28,28,28,28,28,28,29,// 12,12,13, 0,
 0,43,44,44,44,44,44,44,44,45,// 2, 2, 2, 2
};

void Menu::Initialize() {
    //setLives()
    state.sceneNum = 0;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("pokemon-center.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME * 0.10f);

    found = Mix_LoadWAV("capture.wav");
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("tiles.png");
    state.map = new Map(Menu_WIDTH, Menu_HEIGHT, Menu_data, mapTextureID, 1.0f, 16, 16);

    fontTextureID = Util::LoadTexture("font1.png");

    // Initialize game objects
    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(5, -5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("characters.png");

    state.player->animRight = new int[3]{ 81, 82, 83 };
    state.player->animLeft = new int[3]{ 69, 70, 71 };
    state.player->animUp = new int[3]{ 93, 94, 95 };
    state.player->animDown = new int[3]{ 57, 58, 59 };

    state.player->animIndices = state.player->animDown;
    state.player->animFrames = 3;;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 12;
    state.player->animRows = 8;

    state.player->height = 0.8f;
    state.player->width = 0.8f;

    state.player->jumpPower = 5.0f;
    /// <summary>
    /// //////////////////////
    /// initialize human
    state.humans = new Entity[Menu_ENEMY_COUNT];
    GLuint HUMANTextureID = Util::LoadTexture("characters.png");

    state.humans[0].animRight = new int[3]{ 24, 25, 26 };
    state.humans[0].animLeft = new int[3]{ 12, 13, 14 };
    state.humans[0].animUp = new int[3]{ 36, 37, 38 };
    state.humans[0].animDown = new int[3]{ 1, 2, 0 };

    state.humans[0].animIndices = state.humans[0].animDown;
    state.humans[0].animFrames = 3; // 4;
    state.humans[0].animIndex = 0;
    state.humans[0].animTime = 0;
    state.humans[0].animCols = 12; //4;
    state.humans[0].animRows = 8; //4;

    state.humans[0].entityType = HUMAN;
    state.humans[0].textureID = HUMANTextureID;
    state.humans[0].height = 0.8f;
    state.humans[0].width = 0.8f;
    state.humans[0].position = glm::vec3(4.0f, -5, 0);
    state.humans[0].speed = 0;
    state.humans[0].acceleration = glm::vec3(0, -9.81f, 0);
    state.humans[0].aiType = STUCK;
    state.humans[0].aiState = IDLE;

    stopMotion();
}

void Menu::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.humans, Menu_ENEMY_COUNT, state.map);

    for (int i = 0; i < Menu_ENEMY_COUNT; i++) {
        state.humans[i].Update(deltaTime, state.player, state.humans, Menu_ENEMY_COUNT, state.map);
    }

    //triggers moving to next scene
    if (state.player->position.x >= 12) {
        state.nextScene = 1;
    }
}

void Menu::playJumpSound() {
    //Mix_PlayChannel(-1, jump, 0);
}

void Menu::Render(ShaderProgram* program) {
    state.map->Render(program);
    for (int i = 0; i < Menu_ENEMY_COUNT; i++) {
        state.humans[i].Render(program);
    }

    Util::DrawText(program, fontTextureID, "My Favorite Human", 0.75f, -0.25f, glm::vec3(1.0f, -1.0f, 0));
    
    Util::DrawText(program, fontTextureID, "Help the humans escape the cave", 0.5f, -0.25f, glm::vec3(1.4f, -2.0f, 0));
    Util::DrawText(program, fontTextureID, "before time runs out", 0.5f, -0.25f, glm::vec3(2.8f, -2.5f, 0));

    Util::DrawText(program, fontTextureID, "Press 'Enter' to start", 0.65f, -0.25f, glm::vec3(1.0, -3.75f, 0));
    //Util::DrawText(program, fontTextureID, "Lives: " + std::to_string(state.lives), 0.5f, -0.25f, glm::vec3(state.player->position.x + 3.0f, -0.5f, 0));

    state.player->Render(program);
}

int Menu::setLives(int num) {
    state.lives = num;
    return state.lives;
}

int Menu::getLives() {
    return state.lives;
}

int Menu::loseLife() {
    state.lives--;
    return state.lives;
    //if (lives == 0) gameOver();
}

int Menu::setHumansFound(int num) {
    state.humansFound = num;
    return state.humansFound;
}

int Menu::getHumansFound() {
    return state.humansFound;
}

int Menu::findHuman() {
    state.humansFound++;
    return state.humansFound;
}

void Menu::stopMotion() {
    state.player->speed = 0;
    state.player->jumpPower = 0;

    for (int i = 0; i < Menu_ENEMY_COUNT; i++) {
        state.humans->speed = 0;
    }
}

int Menu::getLevelWidth() {
    return 20;
}

int Menu::getRemainingTime() {
    float time = state.endTime - SDL_GetTicks();
    time = round(time / 1000);
    return(time);
}