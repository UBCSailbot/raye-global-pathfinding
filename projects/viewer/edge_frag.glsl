R"(
#version 330

in vec4 geo_colour;
out vec4 frag_colour;

void main() {
    frag_colour = geo_colour;
}
)"
