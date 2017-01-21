R"(
#version 330

in vec3 position;
in uvec3 terrainData;

flat out uvec3 fragTerrainData;
out vec2 texCoord;

uniform mat4 mvp;

void main() {
	// Copy terrain data to frag shader
	fragTerrainData = terrainData;

	// Calculate uv
	switch (gl_VertexID % 3) {
	case 0:
		texCoord = vec2(.5, 1);
		break;

	case 1:
		texCoord = vec2(1, 0);
		break;

	case 2:
		texCoord = vec2(0, 0);
		break;

	default:
		texCoord = vec2(0, 0);
	}

	gl_Position = mvp * vec4(position, 1);
}
)"
