#include "SpriteBatch.hpp"
#include "Log.hpp"

SpriteBatch::SpriteBatch(TimeManager& pTimeManager, GraphicsManager& pGraphicsManager) :
    mTimeManager(pTimeManager),
    mGraphicsManager(pGraphicsManager),
    mSprites(),
    mVertices(),
    mIndexes(),
    mShaderProgram(0),
    aPosition(-1),
    aTexture(-1),
    uProjection(-1),
    uTexture(-1)
{
    mGraphicsManager.registerComponent(this);
}

SpriteBatch::~SpriteBatch()
{
    std::vector<Sprite*>::iterator spriteIt;
    for (spriteIt = mSprites.begin(); spriteIt < mSprites.end(); ++spriteIt)
    {
        delete (*spriteIt);
    }
}

Sprite* SpriteBatch::registerSprite(Resource& pTextureResource, int32_t pWidth, int32_t pHeight)
{
    int32_t spriteCount = mSprites.size();
    int32_t index = spriteCount * 4; // Points to 1st vertex.

    // Precomputes the index buffer.
    mIndexes.push_back(index+0); mIndexes.push_back(index+1);
    mIndexes.push_back(index+2); mIndexes.push_back(index+2);
    mIndexes.push_back(index+1); mIndexes.push_back(index+3);
    for (int i = 0; i < 4; ++i)
    {
        mVertices.push_back(Sprite::Vertex());
    }

    // Appends a new sprite to the sprite array.
    mSprites.push_back(new Sprite(mGraphicsManager, pTextureResource, pHeight, pWidth));
    return mSprites.back();
}

void SpriteBatch::setSpriteEnabled(int id, bool enable)
{
    std::vector<Sprite*>::iterator spriteIt;
    for (spriteIt = mSprites.begin(); spriteIt < mSprites.end(); ++spriteIt)
    {
        if ((*spriteIt)->getId() == id)
        {
            (*spriteIt)->setEnabled(enable);
        }
    }
}

void SpriteBatch::setSpriteAnimation(int id, int32_t pStartFrame, int32_t pFrameCount, float pSpeed, bool pLoop)
{
    std::vector<Sprite*>::iterator spriteIt;
    for (spriteIt = mSprites.begin(); spriteIt < mSprites.end(); ++spriteIt)
    {
        if ((*spriteIt)->getId() == id)
        {
            (*spriteIt)->setAnimation(pStartFrame, pFrameCount, pSpeed, pLoop);
        }
    }
}

bool SpriteBatch::getSpriteEnabled(int id)
{
    std::vector<Sprite*>::iterator spriteIt;
    for (spriteIt = mSprites.begin(); spriteIt < mSprites.end(); ++spriteIt)
    {
        if ((*spriteIt)->getId() == id)
        {
            return (*spriteIt)->getEnabled();
        }
    }

    return false;
}

static const char* VERTEX_SHADER =
   "attribute vec4 aPosition;\n"
   "attribute vec2 aTexture;\n"
   "varying vec2 vTexture;\n"
   "uniform mat4 uProjection;\n"
   "void main() {\n"
   "    vTexture = aTexture;\n"
   "    gl_Position =  uProjection * aPosition;\n"
   "}";

static const char* FRAGMENT_SHADER =
    "precision mediump float;\n"
    "varying vec2 vTexture;\n"
    "uniform sampler2D u_texture;\n"
    "void main() {\n"
    "  gl_FragColor = texture2D(u_texture, vTexture);\n"
    "}";

status SpriteBatch::load()
{
    GLint result;
    int32_t spriteCount;

    // Creates and retrieves shader attributes and uniforms.
    mShaderProgram = mGraphicsManager.loadShader(VERTEX_SHADER, FRAGMENT_SHADER);
    if (mShaderProgram == 0) return STATUS_KO;
    aPosition = glGetAttribLocation(mShaderProgram, "aPosition");
    aTexture = glGetAttribLocation(mShaderProgram, "aTexture");
    uProjection = glGetUniformLocation(mShaderProgram, "uProjection");
    uTexture = glGetUniformLocation(mShaderProgram, "u_texture");

    // Loads sprites.
    std::vector<Sprite *>::iterator spriteIt;
    for (spriteIt = mSprites.begin(); spriteIt < mSprites.end(); ++spriteIt)
    {
        if ((*spriteIt)->load(mGraphicsManager) != STATUS_OK) return loadError();
    }
    return STATUS_OK;
}

status SpriteBatch::loadError()
{
    Log::error("Error loading sprite batch");
    return STATUS_KO;
}

void SpriteBatch::draw()
{
    // Selects sprite shader and passes its parameters.
    glUseProgram(mShaderProgram);
    glUniformMatrix4fv(uProjection, 1, GL_FALSE, mGraphicsManager.getProjectionMatrix());
    glUniform1i(uTexture, 0);

    // Indicates to OpenGL how position and uv coordinates are stored.
    glEnableVertexAttribArray(aPosition);
    glVertexAttribPointer(aPosition, // Attribute Index
                          2, // Size in bytes (x and y)
                          GL_FLOAT, // Data type
                          GL_FALSE, // Normalized
                          sizeof(Sprite::Vertex),// Stride
                          &(mVertices[0].x)); // Location
    glEnableVertexAttribArray(aTexture);
    glVertexAttribPointer(aTexture, // Attribute Index
                          2, // Size in bytes (u and v)
                          GL_FLOAT, // Data type
                          GL_FALSE, // Normalized
                          sizeof(Sprite::Vertex), // Stride
                          &(mVertices[0].u)); // Location
    // Activates transparency.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Renders all sprites in batch.
    const int32_t vertexPerSprite = 4;
    const int32_t indexPerSprite = 6;
    float timeStep = mTimeManager.elapsed();
    int32_t spriteCount = mSprites.size();
    int32_t currentSprite = 0, firstSprite = 0;
    while (currentSprite < spriteCount)
    {
        // Switches texture.
        Sprite* sprite = mSprites[currentSprite];

        if (!sprite->getEnabled())
        {
            ++currentSprite;
            firstSprite = currentSprite;
            continue;
        }

        GLuint currentTexture = sprite->mTexture;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sprite->mTexture);

        // Generate sprite vertices for current textures.
        do {
            sprite = mSprites[currentSprite];
            if (sprite->mTexture == currentTexture)
            {
                Sprite::Vertex* vertices = (&mVertices[currentSprite * 4]);
                sprite->draw(vertices, timeStep);
            }
            else
            {
                break;
            }
        } while (++currentSprite < spriteCount);

        // Renders sprites each time texture changes.
        glDrawElements(GL_TRIANGLES,
                // Number of indexes
                (currentSprite - firstSprite) * indexPerSprite,
                GL_UNSIGNED_SHORT, // Indexes data type
                // First index
                &mIndexes[firstSprite * indexPerSprite]);

        firstSprite = currentSprite;
    }

    // Cleans up OpenGL state.
    glUseProgram(0);
    glDisableVertexAttribArray(aPosition);
    glDisableVertexAttribArray(aTexture);
    glDisable(GL_BLEND);
}
