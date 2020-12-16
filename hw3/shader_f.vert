#version 330 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;

uniform mat4 modelview;
uniform mat4 projection;

flat out vec4 color;

void main()
{
	gl_Position = projection * modelview * vec4(pos, 1.0f);
    
	vec3 frag_pos = vec3(modelview * vec4(pos, 1.0f));
	vec3 frag_normal = normalize(mat3(transpose(inverse(modelview))) * normal);

    vec3 object_color = vec3(0.6f, 0.4f, 0.8f);
    vec3 light_color = vec3(1.0f, 1.0f, 1.0f);

    vec3 light_pos = vec3(-1.0f, 1.0f, 0.0f);
    vec3 view_pos = vec3(0.0f, 0.0f, 0.0f);

    float ambient_rate = 0.3f;
    vec3 ambient = ambient_rate * light_color;
  	
    float diffuse_rate = 1.0f;
    vec3 light_dir = normalize(light_pos - frag_pos);
    float diff = max(dot(frag_normal, light_dir), 0.0f);
    vec3 diffuse = diffuse_rate * diff * light_color;

    float specular_rate = 0.5f;
    vec3 view_dir = normalize(view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, frag_normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 64);
    vec3 specular = specular_rate * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * object_color;
    color = vec4(result, 1.0f);
}