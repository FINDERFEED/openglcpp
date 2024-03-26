#version 330

in vec3 pos;
in vec4 color;
in vec2 uv;
in vec3 normal;

out vec4 vertexColor;
out vec2 texCoords;
out vec3 norm;
out vec4 lightCoord;
out vec4 fragmentPos;

uniform mat4 projection;
uniform mat4 modelview;

uniform mat4 lightProjection;
uniform mat4 lightModelview;



void main(){

	texCoords = uv;

	vertexColor = color;

	norm = normal;

	vec4 p = projection * modelview * vec4(pos.x,pos.y,pos.z,1.);

	lightCoord = lightProjection * lightModelview * vec4(pos.x,pos.y,pos.z,1.);

	fragmentPos = p;

	gl_Position = p;
	
}