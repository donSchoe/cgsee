#version 150 core

in vec2 f_texc;
uniform sampler2D texture;

void main(void) {
    gl_FragColor = texture2D(texture, f_texc);
}

