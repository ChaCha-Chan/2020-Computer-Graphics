#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 modelview;
uniform mat4 projection; 

out vec4 color;

void main()
{
	gl_Position = projection * modelview * vec4(pos, 1.0f);
	vec3 n_pos = normalize(pos);
	color = vec4(n_pos, 1.0f);
}