#version 330 core

out vec4 frag_color;

in vec3 frag_pos;
in vec3 frag_normal;

void main()
{
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
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0f), 32);
    vec3 specular = specular_rate * spec * light_color;

    vec3 result = (ambient + diffuse + specular) * object_color;
    frag_color = vec4(result, 1.0f);
}