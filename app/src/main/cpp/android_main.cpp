#include <android_native_app_glue.h>
#include "pirate/Pirate.h"

extern "C"
{
    void android_main(struct android_app* state);
};

void android_main(struct android_app* state)
{
    Pirate(state).run();
}
