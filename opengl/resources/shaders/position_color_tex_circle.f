#version 330

in vec4 vertexColor;
in vec2 texCoords;

uniform sampler2D sampler0;

out vec4 fragColor;

void main(){

	float l = (length(0.5 - texCoords));
	if (l > 0.5){
		discard;
	}

	vec4 col = texture(sampler0,texCoords) * vertexColor;

	fragColor = col;
}