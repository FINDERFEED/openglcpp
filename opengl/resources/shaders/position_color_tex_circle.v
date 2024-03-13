#version 330

in vec3 pos;
in vec4 color;
in vec2 uv;

out vec4 vertexColor;
out vec2 texCoords;

uniform mat4 projection;
uniform mat4 modelview;

void main(){

	texCoords = uv;

	vertexColor = color;
	vec4 p = projection * modelview * vec4(pos.x,pos.y,pos.z,1.);

	gl_Position = p;
	
}