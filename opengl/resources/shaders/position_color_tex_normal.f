#version 330



in vec4 vertexColor;
in vec2 texCoords;
in vec3 norm;


uniform sampler2D sampler0;

out vec4 fragColor;

void main(){

	vec3 light = vec3(0,-1,0);

	float d = dot(light,normalize(norm));
	d = -d;
	d += 1;
	d /= 2;
	d = clamp(d,0.2,1);

	vec4 col = texture(sampler0,texCoords) * vertexColor;

	vec3 rgb = col.rgb * d;

	fragColor = vec4(rgb,col.w);
}