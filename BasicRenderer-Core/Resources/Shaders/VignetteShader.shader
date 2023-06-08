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

in vec2 texCoord;

uniform sampler2D vignetteTex;
uniform sampler2D contourTex;
uniform bool useGammaCorrection;

uniform bool dilationPass;
uniform bool contourPass;
uniform float radius;
uniform float gridX; // 1/screenWidth
uniform float gridY; // 1/screenHeight
//uniform int screenWidth;
//uniform int screenHeight;


out vec4 fragColor;

void main() {

    if (!dilationPass) {
        fragColor = vec4(texture(vignetteTex, texCoord).rgb, 1.0);


        //Poss processing


        //Gamma correction
        if (useGammaCorrection) {
            float gamma = 2.2;
            fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / gamma));
        }
    }
    else {
        //Dilation pass
        // 
        if (!contourPass) {
            //Dilation algorythm -----------------------------------------------------
            for (float i = -radius; i < radius; ++i) {
                for (float j = -radius; j < radius; ++j) {
                    if (i * i + j * j < (1e-6 + radius * radius)) { // inside a circle
                        vec2 uv = texCoord.st + vec2(i * gridX, j * gridY);
                        vec3 colour = texture(vignetteTex, uv).rgb;
                        if (colour.r + colour.g + colour.b > 0) {
                            //if (uv.x <0.0)discard;
                            fragColor = vec4(colour, 1.F);
                            return;
                        }
                    }
                }
            }
            // not near the selected object, discard
            discard;
        }
        else {
            if (texture(vignetteTex, texCoord).b == texture(contourTex, texCoord).b) discard;
            else  fragColor = vec4(1.0f,0.6f,0.0f, 1.F);
        }
    }
}

