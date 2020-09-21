#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, profMatrix, projectionMatrix;

float robo_x = 0;
float prof_x = 0;
float prof_rotate = 0;

GLuint robotTextureID;
GLuint profTextureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("P1: Race", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    profMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f); 

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);

    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    robotTextureID = LoadTexture("robot.png");
    profTextureID = LoadTexture("ctg.png");
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

float lastTicks = 0.0f;
float robo_velocity = 1.0f;
float prof_velocity = 1.1f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    robo_x += robo_velocity * deltaTime;
    prof_x += prof_velocity * deltaTime;

    prof_rotate += 90.0f * deltaTime;

    modelMatrix = glm::mat4(1.0f);
    //modelMatrix = glm::translate(modelMatrix, glm::vec3(-0.0f, -3.25f, 0.0f));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(robo_x, -3.25f, 0.0f));
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(prof_rotate), glm::vec3(0.0f, 0.0f, 1.0f));

    profMatrix = glm::mat4(1.0f);
    profMatrix = glm::translate(profMatrix, glm::vec3(prof_x, -1.25f, 0.0f));
    profMatrix = glm::rotate(profMatrix, glm::radians(prof_rotate), glm::vec3(0.0f, 0.0f, 1.0f));

    //bounce
    if ((robo_x >= 5.0f) || (robo_x <= -5.0f)) {
        robo_velocity = robo_velocity * -1;
    }

    if ((prof_x >= 5.0f) || (prof_x <= -5.0f)) {
        prof_velocity = prof_velocity * -1;
    }
}

void DrawRobot() {
    program.SetModelMatrix(modelMatrix);
    glBindTexture(GL_TEXTURE_2D, robotTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void DrawProf() {
    program.SetModelMatrix(profMatrix);
    glBindTexture(GL_TEXTURE_2D, profTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render() {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glClear(GL_COLOR_BUFFER_BIT);
    //program.SetModelMatrix(modelMatrix);
    
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);

    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    //glBindTexture(GL_TEXTURE_2D, robotTextureID);
    //glDrawArrays(GL_TRIANGLES, 0, 6);

    DrawRobot();
    DrawProf();

    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}
