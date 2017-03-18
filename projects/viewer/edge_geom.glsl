R"(
#version 330

// Viewport size, in pixels
uniform vec2 _viewport;
// Line width, in pixels
uniform float _line_width = 2.0;
// Processed vertex colour (from VS, in clip space)
in vec4 vs_colour[2];
// Processed primitive vertex colour
out vec4 geo_colour;

layout (lines) in;
layout (triangle_strip, max_vertices = 4) out;

void main() {
    vec3 ndc0 = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
    vec3 ndc1 = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;

    vec2 lineScreenForward = normalize(ndc1.xy - ndc0.xy);
    vec2 lineScreenRight = vec2(-lineScreenForward.y, lineScreenForward.x);
    vec2 lineScreenOffset = (vec2(_line_width) / _viewport) * lineScreenRight;

    vec4 cpos = gl_in[0].gl_Position;
    gl_Position = vec4(cpos.xy + lineScreenOffset*cpos.w, cpos.z, cpos.w);
    geo_colour = vs_colour[0];
    EmitVertex();

    gl_Position = vec4(cpos.xy - lineScreenOffset*cpos.w, cpos.z, cpos.w);
    geo_colour = vs_colour[0];
    EmitVertex();

    cpos = gl_in[1].gl_Position;
    gl_Position = vec4(cpos.xy + lineScreenOffset*cpos.w, cpos.z, cpos.w);
    geo_colour = vs_colour[1];
    EmitVertex();

    gl_Position = vec4(cpos.xy - lineScreenOffset*cpos.w, cpos.z, cpos.w);
    geo_colour = vs_colour[1];
    EmitVertex();

    EndPrimitive();
}
)"
