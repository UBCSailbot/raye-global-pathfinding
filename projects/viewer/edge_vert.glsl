R"(
#version 330

// Modelview-projection matrix
uniform mat4 mvp;

// Vertex position
in vec3 _position;
// Vertex colour (RGB)
in vec3 _colour;
// Processed vertex colour
out vec4 vs_colour;

void main() {
	gl_Position = mvp * vec4(_position, 1);
    // _colour -> RGB, alpha is always 1
    vs_colour = vec4(_colour, 1);
}
)"
