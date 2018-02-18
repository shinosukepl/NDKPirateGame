#include "Sprite.hpp"

Sprite::Sprite(GraphicsManager& pGraphicsManager, Resource& pTextureResource,
               int32_t pHeight, int32_t pWidth) :
    location(),
    mTextureResource(pTextureResource),
    mTexture(0),
    mSheetWidth(0),
    mSheetHeight(0),
    mSpriteHeight(pHeight),
    mSpriteWidth(pWidth),
    mFrameCount(0),
    mFrameXCount(0),
    mFrameYCount(0),
    mAnimStartFrame(0),
    mAnimFrameCount(1),
    mAnimSpeed(0),
    mAnimFrame(0),
    mAnimLoop(false),
    mId(0),
    mEnabled(true)
{

}

status Sprite::load(GraphicsManager& pGraphicsManager)
{
    TextureProperties* textureProperties = pGraphicsManager.loadTexture(mTextureResource);
    if (textureProperties == NULL) return STATUS_KO;
    mTexture = textureProperties->texture;
    mSheetWidth = textureProperties->width;
    mSheetHeight = textureProperties->height;

    mFrameXCount = mSheetWidth / mSpriteWidth;
    mFrameYCount = mSheetHeight / mSpriteHeight;
    mFrameCount = (mSheetHeight / mSpriteHeight) * (mSheetWidth / mSpriteWidth);
    return STATUS_OK;
}

void Sprite::setAnimation(int32_t pStartFrame, int32_t pFrameCount, float pSpeed, bool pLoop)
{
    mAnimStartFrame = pStartFrame;
    mAnimFrame = 0.0f, mAnimSpeed = pSpeed, mAnimLoop = pLoop;
    mAnimFrameCount = pFrameCount;
}

void Sprite::draw(Vertex pVertices[4], float pTimeStep)
{
    if (!mEnabled) return;

    int32_t currentFrame, currentFrameX, currentFrameY;
    // Updates animation in loop mode.
    mAnimFrame += pTimeStep * mAnimSpeed;
    if (mAnimLoop)
    {
        currentFrame = (mAnimStartFrame + int32_t(mAnimFrame) % mAnimFrameCount);
    }
    else
    {
        // Updates animation in one-shot mode.
        if (animationEnded())
        {
            currentFrame = mAnimStartFrame + (mAnimFrameCount-1);
        }
        else
        {
            currentFrame = mAnimStartFrame + int32_t(mAnimFrame);
        }
    }
    // Computes frame X and Y indexes from its id.
    currentFrameX = currentFrame % mFrameXCount;
    // currentFrameY is converted from OpenGL coordinates
    // to top-left coordinates.
    currentFrameY = mFrameYCount - 1 - (currentFrame / mFrameXCount);

    // Draws selected frame.
    GLfloat posX1 = location.x - float(mSpriteWidth / 2);
    GLfloat posY1 = location.y - float(mSpriteHeight / 2);
    GLfloat posX2 = posX1 + mSpriteWidth;
    GLfloat posY2 = posY1 + mSpriteHeight;
    GLfloat u1 = GLfloat(currentFrameX * mSpriteWidth) / GLfloat(mSheetWidth);
    GLfloat u2 = GLfloat((currentFrameX + 1) * mSpriteWidth) / GLfloat(mSheetWidth);
    GLfloat v1 = GLfloat(currentFrameY * mSpriteHeight) / GLfloat(mSheetHeight);
    GLfloat v2 = GLfloat((currentFrameY + 1) * mSpriteHeight) / GLfloat(mSheetHeight);

    pVertices[0].x = posX1; pVertices[0].y = posY1;
    pVertices[0].u = u1;    pVertices[0].v = v1;
    pVertices[1].x = posX1; pVertices[1].y = posY2;
    pVertices[1].u = u1;    pVertices[1].v = v2;
    pVertices[2].x = posX2; pVertices[2].y = posY1;
    pVertices[2].u = u2;    pVertices[2].v = v1;
    pVertices[3].x = posX2; pVertices[3].y = posY2;
    pVertices[3].u = u2;    pVertices[3].v = v2;
}
