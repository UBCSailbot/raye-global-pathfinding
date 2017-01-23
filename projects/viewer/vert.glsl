R"(
#version 330

in vec3 position;
//in uvec3 terrain_data;

//flat out uvec3 frag_terrain_data;

uniform mat4 mvp;

void main() {
	// Copy terrain data to frag shader
//	frag_terrain_data = terrain_data;

	gl_Position = mvp * vec4(position, 1);
}
)"
