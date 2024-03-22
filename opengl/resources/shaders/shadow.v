#version 330

in vec3 pos;

uniform mat4 projection;
uniform mat4 modelview;

void main(){
	gl_Position = projection * modelview * vec4(pos,1.0);
}