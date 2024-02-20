#version 330

layout (location=0) in vec3 pos;
layout (location=1) in vec4 color;

out vec4 vertexColor;

uniform mat4 projection;
uniform mat4 modelview;

void main(){

	vertexColor = color;
	vec4 p = projection * modelview * vec4(pos.x,pos.y,pos.z,1.);

	gl_Position = p;
	
}