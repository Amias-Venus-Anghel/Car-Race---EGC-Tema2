#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec4 car_position;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
    // Compute world space vectors
    vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
	vec3 world_normal = normalize( mat3(Model) * normalize(v_normal) );

    // Send color light output to fragment shader
    color = object_color;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    gl_Position.y = gl_Position.y - length(car_position - gl_Position) * length(car_position - gl_Position) * 0.005f;
}
