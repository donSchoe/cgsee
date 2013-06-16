#include <glm/glm.hpp>
#include <IL/il.h>

#include <QMetaType>
#include "cgsee.h"


int main(int argc, char* argv[])
{
    int result = -1;
    ilInit();
    qRegisterMetaType<glm::vec3>("glm::vec3");
    qRegisterMetaType<glm::vec2>("glm::vec2");
    CGSee * app = new CGSee(argc, argv);
    result = app->exec();

    delete app;

    return result;
}
