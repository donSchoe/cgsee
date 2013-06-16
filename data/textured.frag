#version 150 core

in vec2 fTexPos;
uniform sampler2D texture;

void main(void) {
    gl_FragColor = texture2D(texture, fTexPos);
}

