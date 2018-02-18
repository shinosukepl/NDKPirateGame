#pragma once

#include "GraphicsManager.hpp"
#include "Sprite.hpp"
#include "TimeManager.hpp"
#include "Types.hpp"
#include <GLES2/gl2.h>
#include <vector>

class SpriteBatch : public GraphicsComponent
{
public:
    SpriteBatch(TimeManager& pTimeManager, GraphicsManager& pGraphicsManager);
    ~SpriteBatch();

    Sprite* registerSprite(Resource& pTextureResource, int32_t pWidth, int32_t pHeight);
    status load();
    status loadError();
    void draw();
    void setSpriteEnabled(int id, bool enable);
    bool getSpriteEnabled(int id);
    void setSpriteAnimation(int id, int32_t pStartFrame, int32_t pFrameCount,
                            float pSpeed, bool pLoop);

private:
    SpriteBatch(const SpriteBatch&);
    void operator=(const SpriteBatch&);

    TimeManager& mTimeManager;
    GraphicsManager& mGraphicsManager;

    std::vector<Sprite*> mSprites;
    std::vector<Sprite::Vertex> mVertices;
    std::vector<GLushort> mIndexes;
    GLuint mShaderProgram;
    GLuint aPosition;
    GLuint aTexture;
    GLuint uProjection;
    GLuint uTexture;
};
