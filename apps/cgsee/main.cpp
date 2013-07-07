#include <glm/glm.hpp>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <core/textureunitprovider.h>

#include <QMetaType>
#include "cgsee.h"

static void init() {
    qRegisterMetaType<glm::vec3>("glm::vec4");
    qRegisterMetaType<glm::vec3>("glm::vec3");
    qRegisterMetaType<glm::vec2>("glm::vec2");

    // Init libs
    ilInit();
}

static void shutdown() {
    // shutdown libraries
    delete TextureUnitProvider::instance();
}

int main(int argc, char* argv[])
{
    int result = -1;
    init();

    CGSee * app = new CGSee(argc, argv);
    result = app->exec();

    delete app;

    shutdown();

    return result;
}
