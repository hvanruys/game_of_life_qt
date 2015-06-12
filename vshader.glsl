#version 330 core

out vec2 tex_coord;
void main()
{
//    const vec2 position[] = vec2[](vec2(-0.90,  0.90),
//                                   vec2( 0.90,  0.90),
//                                   vec2(-0.90, -0.90),
//                                   vec2( 0.90,  0.90),
//                                   vec2(-0.90, -0.90),
//                                   vec2( 0.90, -0.90));

    const vec2 position[] = vec2[](vec2(-1.0,  1.0),
                                   vec2( 1.0,  1.0),
                                   vec2(-1.0, -1.0),
                                   vec2( 1.0,  1.0),
                                   vec2(-1.0, -1.0),
                                   vec2( 1.0, -1.0));

    const vec2 texcoord[] = vec2[](vec2( 0.0, 1.0),
                                   vec2( 1.0, 1.0),
                                   vec2( 0.0, 0.0),
                                   vec2( 1.0, 1.0),
                                   vec2( 0.0, 0.0),
                                   vec2( 1.0, 0.0));
    gl_Position = vec4(position[gl_VertexID], 0.0f, 1.0f);
    tex_coord = texcoord[gl_VertexID];
}
