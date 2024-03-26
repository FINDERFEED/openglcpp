#version 330


in vec4 fragmentPos;
in vec4 vertexColor;
in vec2 texCoords;
in vec3 norm;
in vec4 lightCoord;

uniform sampler2D sampler0;
uniform sampler2D shadowSampler;

out vec4 fragColor;

void main(){

	vec3 pos = lightCoord.rgb / lightCoord.w;
	pos *= 0.5;
	pos += 0.5;
	vec4 sh = texture(shadowSampler,pos.xy);

	float mod = 1;
	float bias = 0.0005;

	if (sh.r < (pos.z + bias)){
		mod = 0.2;	
	}

	vec3 light = vec3(0,-1,0);

	float d = dot(light,normalize(norm));
	d = -d;
	d += 1;
	d /= 2;
	d = clamp(d,0.2,1);

	vec4 col = texture(sampler0,texCoords) * vertexColor;

	vec3 rgb = col.rgb * d;

	vec4 f = vec4(rgb * mod,col.w);
	

	fragColor = f;
}