cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib
add_library(native_app_glue STATIC
     ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)

# Export ANativeActivity_onCreate(),
# Refer to: https://github.com/android-ndk/ndk/issues/381.
set(CMAKE_SHARED_LINKER_FLAGS
    "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

# Set common compiler options
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")
add_definitions("-DGLM_FORCE_SIZE_T_LENGTH -DGLM_FORCE_RADIANS")

# Import the CMakeLists.txt for the glm library
add_subdirectory(glm)
# Import png
add_subdirectory(png)

# now build app's shared lib
add_library(game SHARED
     android_main.cpp
     engine/EventLoop.cpp
     engine/GraphicsManager.cpp
     engine/InputManager.cpp
     engine/Log.cpp
     engine/MoveableBody.cpp
     engine/PhysicsManager.cpp
     engine/Resource.cpp
     engine/Sound.cpp
     engine/SoundManager.cpp
     engine/SoundQueue.cpp
     engine/Sprite.cpp
     engine/SpriteBatch.cpp
     engine/TimeManager.cpp
     pirate/Background.cpp
     pirate/Box.cpp
     pirate/Cave.cpp
     pirate/Coin.cpp
     pirate/Coins.cpp
     pirate/Lives.cpp
     pirate/Pirate.cpp
     pirate/PirateShip.cpp
     pirate/Player.cpp
     pirate/PredefinedText.cpp
     pirate/Slots.cpp
     pirate/SlotMachine.cpp
     pirate/TutorialScreen.cpp)

target_include_directories(game PRIVATE
     ${CMAKE_CURRENT_SOURCE_DIR}
     ${CMAKE_CURRENT_SOURCE_DIR}/data
     ${ANDROID_NDK}/sources/android/native_app_glue)

# add lib dependencies
target_link_libraries(game
     android
     native_app_glue
     atomic
     EGL
     GLESv2
     glm
     png
     log
     OpenSLES)
