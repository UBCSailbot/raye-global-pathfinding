R"(
#version 330

in vec3 fragmentColor;
out vec4 fragColor;

void main() {
    // "xyz" -> RGB, alpha is always 1
    fragColor = vec4(fragmentColor.xyz, 1.0);
}
)"
