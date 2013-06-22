#include <glm/glm.hpp>
#include <IL/il.h>

#include <QMetaType>
#include "cgsee.h"

static void init() {
    qRegisterMetaType<glm::vec3>("glm::vec3");
    qRegisterMetaType<glm::vec2>("glm::vec2");

    // e.g. init libraries such as DevIL here
}

static void shutdown() {
    // shutdown libraries
}

int main(int argc, char* argv[])
{
    int result = -1;
    ilInit();
    init();

    CGSee * app = new CGSee(argc, argv);
    result = app->exec();

    delete app;

    shutdown();

    return result;
}
