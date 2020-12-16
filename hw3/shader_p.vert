#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 modelview;
uniform mat4 projection;

out vec3 frag_pos;  
out vec3 frag_normal;

void main()
{
	gl_Position = projection * modelview * vec4(pos, 1.0f);

	frag_pos = vec3(modelview * vec4(pos, 1.0f));
	frag_normal = normalize(mat3(transpose(inverse(modelview))) * pos);
}