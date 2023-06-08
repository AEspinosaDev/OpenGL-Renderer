#shader vertex
#version 460 core

layout(location = 0) in vec3 a_Pos;
layout(location = 1) in vec2 a_texCoord;

uniform mat4 u_proj;

out vec2 texCoord;

void main() {

    texCoord = a_texCoord;

    gl_Position = u_proj * vec4(a_Pos, 1.0);

}

#shader fragment
#version 460 core
out vec4 FragColor;
in vec2 texCoord;

uniform sampler2D vignetteTex;
uniform float radius;
uniform float gridX; // 1/screenWidth
uniform float gridY; // 1/screenHeight

void main() {

    //Dilation algorythm -----------------------------------------------------
    for (float i = -radius; i < radius; ++i) {
        for (float j = -radius; j < radius; ++j) {
            if (i * i + j * j < (1e-6 + radius * radius)) { // inside a circle
                vec2 uv = texCoord.st + vec2(i * gridX, j * gridY);
                vec3 colour = texture(vignetteTex, uv).rgb;
                if (colour.r + colour.g + colour.b > 0) {
                    FragColor = vec4(colour, 1.F);
                    return;
                }
            }
        }
    }
    // not near the selected object, discard
    discard;
}