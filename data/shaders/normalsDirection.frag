#version 150

out vec4 fragColor;

in float relToPeak;

void main(void) {
    fragColor = vec4(0.0 + relToPeak, 0.0 + relToPeak/2, 1.0 - relToPeak, 1.0);
}

