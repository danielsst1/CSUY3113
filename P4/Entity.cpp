#include "Entity.h"
#include <cmath>


Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;

    modelMatrix = glm::mat4(1.0f);
}

void Entity::Stop() {
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;
}

bool Entity::CheckCollision(Entity* other) {
    if (other == this) return false;
    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0) {
        return true;
    }

    return false;
}

// returns the index of the object collided with - if no collision returns -1
int Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                return i;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                return i;
            }
        }
    }
    return -1;
}

// returns the index of the object collided with - if no collision returns -1
int Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                return i;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                return i;
            }
        }
    }
    return -1;
}

void Entity::AIWalker(Entity* player) {
    if (player->position.x < position.x) {
        movement = glm::vec3(-1, 0, 0);
    }
    else {
        movement = glm::vec3(1, 0, 0);
    }
}

void Entity::AIWaitAndGo(Entity* player) {
    switch (aiState)
    {
    case IDLE:
        movement = glm::vec3(0, 0, 0);
         if (glm::distance(position, player->position) < 3.0f) {
            aiState = WALKING;
         }
        break;

    case WALKING:
        if (player->position.x < position.x) {
            movement = glm::vec3(-1, 0, 0);
        }
        else {
            movement = glm::vec3(1, 0, 0);
        }
        break;

    case ATTACKING:
        break;

    case DEAD:
        movement = glm::vec3(0, 0, 0);
        //default:
        //    break;
    }
}

void Entity::AIStuck(Entity* player) {
    switch (aiState)
    {
    case IDLE:
        movement = glm::vec3(0, 0, 0);
        if (abs(player->position.y - position.y) < 0.5f) {
            aiState = ATTACKING;
        }
        break;
    case WALKING:
        break;
    case ATTACKING:
        if (abs(player->position.y - position.y) >= 0.5f) {
            aiState = IDLE;
        }
        if (player->position.x < position.x) {
            movement = glm::vec3(-1, 0, 0);
        }
        else {
            movement = glm::vec3(1, 0, 0);
        }
        break;
    case DEAD:
        break;
    default:
        break;
    }
    
}

void Entity::AI(Entity* player) {
    switch (aiType) {
    case WALKER:
        AIWalker(player);
        break;

    case WAITANDGO:
        AIWaitAndGo(player);
        break;    

    case STUCK:
        AIStuck(player);
    }
}

void Entity::killAI() {
    aiState = DEAD;
    isActive = false;
}

// returns the index of the object collided with - if no collision returns -1
int Entity::Update(float deltaTime, Entity* player, Entity* enemies, int enemyCount, Entity* platforms, int platformCount)
{
    if (isActive == false) return -1;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
   
    if (entityType == ENEMY) {
        AI(player);
    }

    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        }
        else {
            animIndex = 0;
        }
    }

    if (jump) {
        jump = false;
        velocity.y += jumpPower;
    }

    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;

    position.y += velocity.y * deltaTime; // Move on Y
    //platform collision
    int yCollision = CheckCollisionsY(platforms, platformCount);// Fix if needed
    
    position.x += velocity.x * deltaTime; // Move on X
    //platform collision
    int xCollision = CheckCollisionsX(platforms, platformCount);// Fix if needed
    

    

    if (entityType == PLAYER) {
        //enemy collision
        yCollision = CheckCollisionsY(enemies, enemyCount);// Fix if needed
        
        //enemy collision
        xCollision = CheckCollisionsX(enemies, enemyCount);// Fix if needed
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);

    if (entityType == PLAYER) {
        if (xCollision != -1 || (yCollision != -1 && collidedTop)) return xCollision;
        else if (yCollision != -1) enemies[yCollision].killAI();
    }
    
    return -1;
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {

    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);

    //verticey coordinates
    if (entityType == EntityType::PLAYER) {
        float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    }
    else if (entityType == EntityType::PLATFORM) {
        float vertices[] = { -(width / 2), -(height / 2),
                        (width / 2), -(height / 2),
                        (width / 2), (height / 2),
                       -(width / 2), -(height / 2),
                        (width / 2), (height / 2),
                       -(width / 2), (height / 2) };
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    }
    else { // default vertices
        float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
        glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    }
    glEnableVertexAttribArray(program->positionAttribute);

    //texture coordinates
    if (entityType == EntityType::PLAYER) {
        float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    }
    else if (entityType == EntityType::PLATFORM) {
        float texCoords[] = { 0.0, height,
                            width, height,
                            width, 0.0,
                            0.0, height,
                            width, 0.0,
                            0.0, 0.0 };
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    }
    else { // default vertices
        float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
        glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    }

    //glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    //
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}
