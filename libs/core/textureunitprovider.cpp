#include "textureunitprovider.h"
#include <assert.h>

TextureUnitProvider *TextureUnitProvider::s_instance = nullptr;

TextureUnitProvider *TextureUnitProvider::instance()
{
    if(s_instance == nullptr) {
        s_instance = new TextureUnitProvider;
    }

    return s_instance;
}

void TextureUnitProvider::reset()
{
    m_cursor = 0;
}

void TextureUnitProvider::allocate(GLuint &unit, GLuint &id)
{
    // TODO: Ensure more tex-units are available
    assert(m_cursor < 16);

    unit = GL_TEXTURE0 + m_cursor;
    id = m_cursor;

    m_cursor++;
}

TextureUnitProvider::TextureUnitProvider()
{
    m_cursor = 0;
}
