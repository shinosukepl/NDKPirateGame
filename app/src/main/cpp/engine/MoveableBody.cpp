#include "MoveableBody.hpp"
#include <math.h>

MoveableBody::MoveableBody(android_app* pApplication,
      InputManager& pInputManager, PhysicsManager& pPhysicsManager) :
  mInputManager(pInputManager),
  mPhysicsManager(pPhysicsManager),
  mBody(NULL)
{

}

PhysicsBody* MoveableBody::registerMoveableBody(Location& pLocation, int32_t pSizeX, int32_t pSizeY)
{
    mBody = mPhysicsManager.loadBody(pLocation, pSizeX, pSizeY);
    mInputManager.setRefPoint(&pLocation);
    return mBody;
}

void MoveableBody::initialize()
{
    mBody->velocityX = 0.0f;
    mBody->velocityY = 0.0f;
}

void MoveableBody::update()
{
    static const float MOVE_SPEED = 140.0f;
    mBody->velocityX = mInputManager.getDirectionX() * MOVE_SPEED;
    mBody->velocityY = mInputManager.getDirectionY() * MOVE_SPEED;
}

bool MoveableBody::checkCollisionWithBody(PhysicsBody* body)
{
    float deltaCenterX = body->location->x - mBody->location->x;
    float deltaCenterY = body->location->y - mBody->location->y;
    float delta = sqrt(deltaCenterX*deltaCenterX + deltaCenterY*deltaCenterY);

    int32_t deltaRadius = body->radius + mBody->radius;

    return delta < deltaRadius;
}
