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
//uniform sampler2DMS depthTex;
uniform sampler2D depthTex;


uniform sampler2D contourTex;

uniform bool useGammaCorrection;


uniform bool useFog;
uniform float fogIntensity;
uniform float fogStart;
uniform float fogEnd;
uniform vec3 fogColor;
uniform int fogType;


uniform float far;
uniform float near;
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



		/*ivec2 vpCoords = ivec2(texCoord.x, texCoord.y);
		vec4 totalSampling = vec4(0.0);
		for (int i = 0; i < 32; i++) {
			totalSampling += texelFetch(depthTex, ivec2(gl_FragCoord.xy), i);
		}
		float avrZ = (totalSampling / 32.0).x;
		float zz = (2.0 * near) / (far + near - avrZ * (far - near));*/

		//Poss processing
		//Fog TODO
		if (useFog) {

			//Linearize depth
			float z = (2.0 * near) / (far + near - texture2D(depthTex, texCoord).x * (far - near));
			//float z = (2.0 * near) / (far + near - avrZ * (far - near));

			float f;
			if (fogType == 0) {
				//f = (fogEnd / far - z) / (fogEnd / far - fogStart / near);
			}
			else {
				f = exp(-fogIntensity * z);
			}

			fragColor.rgb = f * fragColor.rgb + (1 - f) * fogColor;

		}

		//Bloom TODO

		//Gamma correction
		if (useGammaCorrection) {
			float gamma = 2.2;
			fragColor.rgb = pow(fragColor.rgb, vec3(1.0 / gamma));
		}
		//fragColor = vec4(vec3(zz), 1.0);

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
			else  fragColor = vec4(1.0f, 0.6f, 0.0f, 1.F);
		}
	}
}

