#include "GraphicsManager.hpp"
#include "Log.hpp"
#include "SpriteBatch.hpp"
#include <png.h>

GraphicsManager::GraphicsManager(android_app* pApplication) :
    mApplication(pApplication),
    mRenderWidth(0),
    mRenderHeight(0),
    mScreenWidth(0),
    mScreenHeight(0),
    mDisplay(EGL_NO_DISPLAY),
    mSurface(EGL_NO_CONTEXT),
    mContext(EGL_NO_SURFACE),
    mProjectionMatrix(),
    mTextures(),
    mShaders(),
    mVertexBuffers(),
    mComponents(),
    mScreenFrameBuffer(0),
    mRenderFrameBuffer(0),
    mRenderVertexBuffer(0),
    mRenderTexture(0),
    mRenderShaderProgram(0),
    aPosition(0),
    aTexture(0),
    uProjection(0),
    uTexture(0)
{
    Log::info("Creating GraphicsManager.");
}

GraphicsManager::~GraphicsManager()
{
    Log::info("Destroying GraphicsManager.");
}

void GraphicsManager::registerComponent(GraphicsComponent* pComponent)
{
    mComponents.push_back(pComponent);
}

status GraphicsManager::start()
{
    Log::info("Starting GraphicsManager.");
    EGLint format, numConfigs, errorResult;
    GLenum status;
    EGLConfig config;
    // Defines display requirements. 16bits mode here.
    const EGLint DISPLAY_ATTRIBS[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_BLUE_SIZE, 8, EGL_GREEN_SIZE, 8, EGL_RED_SIZE, 8,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_NONE
    };
    // Request an OpenGL ES 2 context.
    const EGLint CONTEXT_ATTRIBS[] = {
            EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE
    };

    // Retrieves a display connection and initializes it.
    Log::info("Connecting to the display.");
    mDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (mDisplay == EGL_NO_DISPLAY) return startError();
    if (!eglInitialize(mDisplay, NULL, NULL)) return startError();

    // Selects the first OpenGL configuration found.
    Log::info("Selecting a display config.");
    if (!eglChooseConfig(mDisplay, DISPLAY_ATTRIBS, &config, 1, &numConfigs) || (numConfigs <= 0))
        return startError();

    // Reconfigures the Android window with the EGL format.
    Log::info("Configuring window format.");
    if (!eglGetConfigAttrib(mDisplay, config, EGL_NATIVE_VISUAL_ID, &format))
        return startError();

    ANativeWindow_setBuffersGeometry(mApplication->window, 0, 0, format);

    // Creates the display surface.
    Log::info("Initializing the display.");
    mSurface = eglCreateWindowSurface(mDisplay, config, mApplication->window, NULL);
    if (mSurface == EGL_NO_SURFACE) return startError();

    mContext = eglCreateContext(mDisplay, config, NULL, CONTEXT_ATTRIBS);
    if (mContext == EGL_NO_CONTEXT) return startError();

    // Activates the display surface.
    Log::info("Activating the display.");
    if (!eglMakeCurrent(mDisplay, mSurface, mSurface, mContext)
        || !eglQuerySurface(mDisplay, mSurface, EGL_WIDTH, &mScreenWidth)
        || !eglQuerySurface(mDisplay, mSurface, EGL_HEIGHT, &mScreenHeight)
        || (mScreenWidth <= 0) || (mScreenHeight <= 0))
        return startError();

    // Defines and initializes offscreen surface.
    if (initializeRenderBuffer() != STATUS_OK) return startError();

    // Z-Buffer is useless as we are ordering draw calls ourselves
    glViewport(0, 0, mRenderWidth, mRenderHeight);
    glDisable(GL_DEPTH_TEST);

    // Prepares the projection matrix.
    memset(mProjectionMatrix[0], 0, sizeof(mProjectionMatrix));
    mProjectionMatrix[0][0] = 2.0f / GLfloat(mRenderWidth);
    mProjectionMatrix[1][1] = 2.0f / GLfloat(mRenderHeight);
    mProjectionMatrix[2][2] = -1.0f;
    mProjectionMatrix[3][0] = -1.0f;
    mProjectionMatrix[3][1] = -1.0f;
    mProjectionMatrix[3][2] = 0.0f;
    mProjectionMatrix[3][3] = 1.0f;

    // Displays information about OpenGL.
    Log::info("Starting GraphicsManager");
    Log::info("Version   : %s", glGetString(GL_VERSION));
    Log::info("Vendor    : %s", glGetString(GL_VENDOR));
    Log::info("Renderer  : %s", glGetString(GL_RENDERER));
    Log::info("Viewport  : %d x %d", mScreenWidth, mScreenHeight);
    Log::info("Offscreen : %d x %d", mRenderWidth, mRenderHeight);

    // Loads graphics components.
    for (std::vector<GraphicsComponent *>::iterator componentIt = mComponents.begin();
         componentIt < mComponents.end(); ++componentIt)
    {
        if ((*componentIt)->load() != STATUS_OK) return STATUS_KO;
    }
    return STATUS_OK;
}

status GraphicsManager::startError()
{
    Log::error("Error while starting GraphicsManager");
    stop();
    return STATUS_KO;
}

static const char* VERTEX_SHADER =
   "attribute vec2 aPosition;\n"
   "attribute vec2 aTexture;\n"
   "varying vec2 vTexture;\n"
   "void main() {\n"
   "    vTexture = aTexture;\n"
   "    gl_Position = vec4(aPosition, 1.0, 1.0 );\n"
   "}";

static const char* FRAGMENT_SHADER =
    "precision mediump float;"
    "uniform sampler2D uTexture;\n"
    "varying vec2 vTexture;\n"
    "void main() {\n"
    "  gl_FragColor = texture2D(uTexture, vTexture);\n"
    "}\n";

const int32_t DEFAULT_RENDER_WIDTH = 600;

status GraphicsManager::initializeRenderBuffer()
{
    Log::info("Loading offscreen buffer");
    const RenderVertex vertices[] = {
            {-1.0f, -1.0f, 0.0f, 0.0f},
            {-1.0f, 1.0f,  0.0f, 1.0f},
            {1.0f,  -1.0f, 1.0f, 0.0f},
            {1.0f,  1.0f,  1.0f, 1.0f}
    };

    float screenRatio = float(mScreenHeight) / float(mScreenWidth);
    mRenderWidth = DEFAULT_RENDER_WIDTH;
    mRenderHeight = float(mRenderWidth) * screenRatio;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &mScreenFrameBuffer);

    // Creates a texture for off-screen rendering.
    glGenTextures(1, &mRenderTexture);
    glBindTexture(GL_TEXTURE_2D, mRenderTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mRenderWidth, mRenderHeight,
                 0, GL_RGB, GL_UNSIGNED_SHORT_5_6_5, NULL);

    // Attaches the texture to the new framebuffer.
    glGenFramebuffers(1, &mRenderFrameBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mRenderFrameBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mRenderTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Creates the shader used to render texture to screen.
    mRenderVertexBuffer = loadVertexBuffer(vertices, sizeof(vertices));
    if (mRenderVertexBuffer == 0) return initializeRenderBufferError();

    // Creates and retrieves shader attributes and uniforms.
    mRenderShaderProgram = loadShader(VERTEX_SHADER, FRAGMENT_SHADER);
    if (mRenderShaderProgram == 0) return initializeRenderBufferError();
    aPosition = glGetAttribLocation(mRenderShaderProgram, "aPosition");
    aTexture = glGetAttribLocation(mRenderShaderProgram, "aTexture");
    uTexture = glGetUniformLocation(mRenderShaderProgram, "uTexture");

    return STATUS_OK;
}

status GraphicsManager::initializeRenderBufferError()
{
    Log::error("Error while loading offscreen buffer");
    return STATUS_KO;
}

void GraphicsManager::setSpriteEnabled(int id, bool enable)
{
    SpriteBatch* spriteBatch = static_cast<SpriteBatch*>(mComponents[0]);
    spriteBatch->setSpriteEnabled(id, enable);
}

void GraphicsManager::setSpriteAnimation(int id, int32_t pStartFrame, int32_t pFrameCount, float pSpeed, bool pLoop)
{
    SpriteBatch* spriteBatch = static_cast<SpriteBatch*>(mComponents[0]);
    spriteBatch->setSpriteAnimation(id, pStartFrame, pFrameCount, pSpeed, pLoop);
}

bool GraphicsManager::getSpriteEnabled(int id)
{
    SpriteBatch* spriteBatch = static_cast<SpriteBatch*>(mComponents[0]);
    return spriteBatch->getSpriteEnabled(id);
}

void GraphicsManager::stop()
{
    Log::info("Stopping GraphicsManager.");
    // Releases textures.
    std::map<Resource*, TextureProperties>::iterator textureIt;
    for (textureIt = mTextures.begin(); textureIt != mTextures.end(); ++textureIt)
    {
        glDeleteTextures(1, &textureIt->second.texture);
    }
    mTextures.clear();

    // Releases shaders.
    std::vector<GLuint>::iterator shaderIt;
    for (shaderIt = mShaders.begin(); shaderIt < mShaders.end(); ++shaderIt)
    {
        glDeleteProgram(*shaderIt);
    }
    mShaders.clear();

    // Releases vertex buffers.
    std::vector<GLuint>::iterator vertexBufferIt;
    for (vertexBufferIt = mVertexBuffers.begin();
         vertexBufferIt < mVertexBuffers.end(); ++vertexBufferIt)
    {
        glDeleteBuffers(1, &(*vertexBufferIt));
    }
    mVertexBuffers.clear();

    // Releases offscreen rendering resources.
    // Vertex buffer and shader are released by the loops above.
    if (mRenderFrameBuffer != 0)
    {
        glDeleteFramebuffers(1, &mRenderFrameBuffer);
        mRenderFrameBuffer = 0;
    }
    if (mRenderTexture != 0)
    {
        glDeleteTextures(1, &mRenderTexture);
        mRenderTexture = 0;
    }

    // Destroys OpenGL context.
    if (mDisplay != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(mDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (mContext != EGL_NO_CONTEXT)
        {
            eglDestroyContext(mDisplay, mContext);
            mContext = EGL_NO_CONTEXT;
        }
        if (mSurface != EGL_NO_SURFACE)
        {
            eglDestroySurface(mDisplay, mSurface);
            mSurface = EGL_NO_SURFACE;
        }
        eglTerminate(mDisplay);
        mDisplay = EGL_NO_DISPLAY;
    }
}

status GraphicsManager::update()
{
    // Uses the offscreen FBO for scene rendering.
    glBindFramebuffer(GL_FRAMEBUFFER, mRenderFrameBuffer);
    glViewport(0, 0, mRenderWidth, mRenderHeight);
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.125f, 0.565f, 1.0f, 0.0f);

    // Render graphic components.
    std::vector<GraphicsComponent*>::iterator componentIt;
    for (componentIt = mComponents.begin(); componentIt < mComponents.end(); ++componentIt)
    {
        (*componentIt)->draw();
    }

    // The FBO is rendered and scaled into the screen.
    glBindFramebuffer(GL_FRAMEBUFFER, mScreenFrameBuffer);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, mScreenWidth, mScreenHeight);

    // Select the offscreen texture as source.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mRenderTexture);
    glUseProgram(mRenderShaderProgram);
    glUniform1i(uTexture, 0);

    // Indicates to OpenGL how position and uv coordinates are stored.
    glBindBuffer(GL_ARRAY_BUFFER, mRenderVertexBuffer);
    glEnableVertexAttribArray(aPosition);
    glVertexAttribPointer(aPosition, // Attribute Index
                          2, // Number of components (x and y)
                          GL_FLOAT, // Data type
                          GL_FALSE, // Normalized
                          sizeof(RenderVertex), // Stride
                          (GLvoid*) 0); // Offset
    glEnableVertexAttribArray(aTexture);
    glVertexAttribPointer(aTexture, // Attribute Index
                          2, // Number of components (u and v)
                          GL_FLOAT, // Data type
                          GL_FALSE, // Normalized
                          sizeof(RenderVertex), // Stride
                          (GLvoid*) (sizeof(GLfloat) * 2)); // Offset
    // Renders the offscreen buffer into screen.
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Restores device state.
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Shows the result to the user.
    if (eglSwapBuffers(mDisplay, mSurface) != EGL_TRUE)
    {
        Log::error("Error %d swapping buffers.", eglGetError());
        return STATUS_KO;
    }
    else
    {
        return STATUS_OK;
    }
}

void callback_readPng(png_structp pStruct, png_bytep pData, png_size_t pSize)
{
    Resource* resource = ((Resource*) png_get_io_ptr(pStruct));
    if (resource->read(pData, pSize) != STATUS_OK)
    {
        resource->close();
    }
}

TextureProperties* GraphicsManager::loadTexture(Resource& pResource)
{
    // Looks for the texture in cache first.
    std::map<Resource *, TextureProperties>::iterator textureIt = mTextures.find(&pResource);
    if (textureIt != mTextures.end())
    {
        return &textureIt->second;
    }

    Log::info("Loading texture %s", pResource.getPath());
    TextureProperties *textureProperties;
    GLuint texture;
    GLint format;
    png_byte header[8];
    png_structp pngPtr = NULL;
    png_infop infoPtr = NULL;
    png_byte *image = NULL;
    png_bytep *rowPtrs = NULL;
    png_int_32 rowSize;
    bool transparency;

    // Opens and checks image signature (first 8 bytes).
    if (pResource.open() != STATUS_OK)
        return loadTextureError(pResource, pngPtr, infoPtr, rowPtrs, image);
    Log::info("Checking signature.");
    if (pResource.read(header, sizeof(header)) != STATUS_OK)
        return loadTextureError(pResource, pngPtr, infoPtr, rowPtrs, image);
    if (png_sig_cmp(header, 0, 8) != 0)
        return loadTextureError(pResource, pngPtr, infoPtr, rowPtrs, image);

    // Creates required structures.
    Log::info("Creating required structures.");
    pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!pngPtr) return loadTextureError(pResource, pngPtr, infoPtr, rowPtrs, image);
    infoPtr = png_create_info_struct(pngPtr);
    if (!infoPtr) return loadTextureError(pResource, pngPtr, infoPtr, rowPtrs, image);

    // Prepares reading operation by setting-up a read callback.
    png_set_read_fn(pngPtr, &pResource, callback_readPng);
    // Set-up error management. If an error occurs while reading,
    // code will come back here and jump
    if (setjmp(png_jmpbuf(pngPtr)))
        return loadTextureError(pResource, pngPtr, infoPtr, rowPtrs, image);

    // Ignores first 8 bytes already read and processes header.
    png_set_sig_bytes(pngPtr, 8);
    // Retrieves PNG info and updates PNG struct accordingly.
    png_int_32 depth, colorType;
    png_uint_32 width, height;
    png_read_info(pngPtr, infoPtr);
    png_get_IHDR(pngPtr, infoPtr, &width, &height, &depth, &colorType, NULL, NULL, NULL);

    // Creates a full alpha channel if transparency is encoded as
    // an array of palette entries or a single transparent color.
    transparency = false;
    if (png_get_valid(pngPtr, infoPtr, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(pngPtr);
        transparency = true;
    }
    // Expands PNG with less than 8bits per channel to 8bits.
    if (depth < 8)
    {
        png_set_packing(pngPtr);
        // Shrinks PNG with 16bits per color channel down to 8bits.
    }
    else if (depth == 16)
    {
        png_set_strip_16(pngPtr);
    }
    // Indicates that image needs conversion to RGBA if needed.
    switch (colorType)
    {
        case PNG_COLOR_TYPE_PALETTE:
            png_set_palette_to_rgb(pngPtr);
            format = transparency ? GL_RGBA : GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGB:
            format = transparency ? GL_RGBA : GL_RGB;
            break;
        case PNG_COLOR_TYPE_RGBA:
            format = GL_RGBA;
            break;
        case PNG_COLOR_TYPE_GRAY:
            png_set_expand_gray_1_2_4_to_8(pngPtr);
            format = transparency ? GL_LUMINANCE_ALPHA : GL_LUMINANCE;
            break;
        case PNG_COLOR_TYPE_GA:
            png_set_expand_gray_1_2_4_to_8(pngPtr);
            format = GL_LUMINANCE_ALPHA;
            break;
    }
    // Validates all tranformations.
    png_read_update_info(pngPtr, infoPtr);

    // Get row size in bytes.
    rowSize = png_get_rowbytes(pngPtr, infoPtr);
    if (rowSize <= 0) return loadTextureError(pResource, pngPtr, infoPtr, rowPtrs, image);
    // Ceates the image buffer that will be sent to OpenGL.
    image = new png_byte[rowSize * height];
    if (!image) return loadTextureError(pResource, pngPtr, infoPtr, rowPtrs, image);
    // Pointers to each row of the image buffer. Row order is
    // inverted because different coordinate systems are used by
    // OpenGL (1st pixel is at bottom left) and PNGs (top-left).
    rowPtrs = new png_bytep[height];
    if (!rowPtrs) return loadTextureError(pResource, pngPtr, infoPtr, rowPtrs, image);
    for (int32_t i = 0; i < height; ++i)
    {
        rowPtrs[height - (i + 1)] = image + i * rowSize;
    }
    // Reads image content.
    png_read_image(pngPtr, rowPtrs);
    // Frees memory and resources.
    pResource.close();
    png_destroy_read_struct(&pngPtr, &infoPtr, NULL);
    delete[] rowPtrs;

    // Creates a new OpenGL texture.
    GLenum errorResult;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // Set-up texture properties.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Loads image data into OpenGL.
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
    // Finished working with the texture.
    glBindTexture(GL_TEXTURE_2D, 0);
    delete[] image;
    if (glGetError() != GL_NO_ERROR)
        return loadTextureError(pResource, pngPtr, infoPtr, rowPtrs, image);
    Log::info("Texture size: %d x %d", width, height);

    // Caches the loaded texture.
    textureProperties = &mTextures[&pResource];
    textureProperties->texture = texture;
    textureProperties->width = width;
    textureProperties->height = height;
    return textureProperties;
}

TextureProperties* GraphicsManager::loadTextureError(Resource& pResource, png_structp& pngPtr,
                                                     png_infop& infoPtr, png_bytep* rowPtrs,
                                                     png_byte* image)
{
    Log::error("Error loading texture into OpenGL.");
    pResource.close();
    delete[] rowPtrs;
    delete[] image;
    if (pngPtr != NULL)
    {
        png_infop* infoPtrP = infoPtr != NULL ? &infoPtr: NULL;
        png_destroy_read_struct(&pngPtr, infoPtrP, NULL);
    }
    return NULL;
}

GLuint GraphicsManager::loadShader(const char* pVertexShader, const char* pFragmentShader)
{
    GLint result;
    char log[256];
    GLuint vertexShader, fragmentShader, shaderProgram;

    // Builds the vertex shader.
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &pVertexShader, NULL);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        glGetShaderInfoLog(vertexShader, sizeof(log), 0, log);
        Log::error("Vertex shader error: %s", log);
        return loadShaderError(vertexShader, fragmentShader);
    }

    // Builds the fragment shader.
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &pFragmentShader, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        glGetShaderInfoLog(fragmentShader, sizeof(log), 0, log);
        Log::error("Fragment shader error: %s", log);
        return loadShaderError(vertexShader, fragmentShader);
    }

    // Builds the shader program.
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
    // Once linked, shaders are useless.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (result == GL_FALSE)
    {
        glGetProgramInfoLog(shaderProgram, sizeof(log), 0, log);
        Log::error("Shader program error: %s", log);
        return loadShaderError(vertexShader, fragmentShader);
    }

    mShaders.push_back(shaderProgram);
    return shaderProgram;
}

GLuint GraphicsManager::loadShaderError(GLuint& vertexShader, GLuint& fragmentShader)
{
    Log::error("Error loading shader.");
    if (vertexShader > 0) glDeleteShader(vertexShader);
    if (fragmentShader > 0) glDeleteShader(fragmentShader);
    return 0;
}

GLuint GraphicsManager::loadVertexBuffer(const void* pVertexBuffer, int32_t pVertexBufferSize)
{
    GLuint vertexBuffer;
    // Upload specified memory buffer into OpenGL.
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, pVertexBufferSize, pVertexBuffer,
                 GL_STATIC_DRAW);
    // Unbinds the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    if (glGetError() != GL_NO_ERROR) return loadVertexBufferError(vertexBuffer);

    mVertexBuffers.push_back(vertexBuffer);
    return vertexBuffer;
}

GLuint GraphicsManager::loadVertexBufferError(GLuint& vertexBuffer)
{
    Log::error("Error loading vertex buffer.");
    if (vertexBuffer > 0) glDeleteBuffers(1, &vertexBuffer);
    return 0;
}
