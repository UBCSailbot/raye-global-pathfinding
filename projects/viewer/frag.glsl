R"(
#version 330

//flat in uvec3 frag_terrain_data;
//in vec2 texCoord;

out vec4 fragColor;

//vec3 colorForTerrain(uint t) {
//    switch (t)
//    {
//    case 0u: // water
//        return vec3(0., 0., 1.);
//    case 1u: // desert (brown)
//        return vec3(0.625, 0.625, 0.);
//    case 2u: // grass
//        return vec3(0., 1., 0.);
//    case 3u: // forest
//        return vec3(0., 0.5, 0.);
//    case 4u: // mountain
//        return vec3(0.25, 0.25, 0.25);
//    }
//
//    return vec3(0., 0., 0.);
//}

void main() {
    //set every drawn pixel to white
    fragColor = vec4(1.0, 1.0, 1.0, 1.0);

//    fragColor.a = 1;
//
//	float d0 = distance(vec2(0.5, 1.0), texCoord);
//	float d1 = distance(vec2(1.0, 0.0), texCoord);
//	float d2 = distance(vec2(0.0, 0.0), texCoord);
//
//	int terrainIndex =
//	    (d0 < d1) ?
//	        (
//	            (d0 < d2) ? 0 : 2
//	        ) :
//	        (
//	            (d1 < d2) ? 1 : 2
//	        );
//
//    fragColor.rgb = colorForTerrain(frag_terrain_data[terrainIndex]);
}
)"
