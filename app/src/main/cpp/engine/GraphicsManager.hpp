#pragma once

#include "Resource.hpp"
#include "Types.hpp"
#include <android_native_app_glue.h>
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <map>
#include <vector>
#include <png/jni/png.h>

class GraphicsComponent
{
public:
    virtual status load() = 0;
    virtual void draw() = 0;
};

struct TextureProperties
{
    GLuint texture;
    int32_t width;
    int32_t height;
};

class GraphicsManager
{
public:
    GraphicsManager(android_app* pApplication);
    ~GraphicsManager();

    inline int32_t getRenderWidth() const { return mRenderWidth; }
    inline int32_t getRenderHeight() const { return mRenderHeight; }
    inline int32_t getScreenWidth() const { return mScreenWidth; }
    inline int32_t getScreenHeight() const { return mScreenHeight; }
    inline GLfloat* getProjectionMatrix() { return mProjectionMatrix[0]; }

    void registerComponent(GraphicsComponent* pComponent);
    void setSpriteEnabled(int id, bool enable);
    bool getSpriteEnabled(int id);
    void setSpriteAnimation(int id, int32_t pStartFrame, int32_t pFrameCount, float pSpeed, bool pLoop);
    status start();
    status startError();
    void stop();
    status update();

    TextureProperties* loadTexture(Resource& pResource);
    TextureProperties* loadTextureError(Resource& pResource, png_structp& pngPtr,
                                        png_infop& infoPtr, png_bytep* rowPtrs, png_byte* image);
    GLuint loadShader(const char* pVertexShader, const char* pFragmentShader);
    GLuint loadShaderError(GLuint& vertexShader, GLuint& fragmentShader);
    GLuint loadVertexBuffer(const void* pVertexBuffer, int32_t pVertexBufferSize);
    GLuint loadVertexBufferError(GLuint& vertexBuffer);

private:
    GraphicsManager(const GraphicsManager&);
    void operator=(const GraphicsManager&);
    status initializeRenderBuffer();
    status initializeRenderBufferError();

    struct RenderVertex
    {
        GLfloat x, y, u, v;
    };

    android_app* mApplication;

    int32_t mRenderWidth;
    int32_t mRenderHeight;
    int32_t mScreenWidth;
    int32_t mScreenHeight;
    EGLDisplay mDisplay;
    EGLSurface mSurface;
    EGLContext mContext;
    GLfloat mProjectionMatrix[4][4];

    // Graphics resources.
    std::map<Resource*, TextureProperties> mTextures;
    std::vector<GLuint> mShaders;
    std::vector<GLuint> mVertexBuffers;
    std::vector<GraphicsComponent*> mComponents;

    // Rendering resources.
    GLint mScreenFrameBuffer;
    GLuint mRenderFrameBuffer;
    GLuint mRenderVertexBuffer;
    GLuint mRenderTexture;
    GLuint mRenderShaderProgram;
    GLuint aPosition;
    GLuint aTexture;
    GLuint uProjection;
    GLuint uTexture;
};
