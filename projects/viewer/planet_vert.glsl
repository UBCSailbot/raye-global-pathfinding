R"(
#version 330

in vec3 position;
in vec3 vertexColor;
out vec3 fragmentColor;

uniform mat4 mvp;

void main() {
    fragmentColor = vertexColor;
	gl_Position = mvp * vec4(position, 1);
}
)"
