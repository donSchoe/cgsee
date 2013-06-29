
#include <glm/glm.hpp>
#include <QMetaType>

// Insert registration code of types that you use in vertex attributes
// Don't forget also to call
//
// qRegisterMetaType<T>("T").
//
// from your main function.

Q_DECLARE_METATYPE(glm::vec4);
Q_DECLARE_METATYPE(glm::vec3);
Q_DECLARE_METATYPE(glm::vec2);
