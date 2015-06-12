#version 420 core

//automata.vsh

//attribute vec4 a_position;
//attribute vec2 a_texCoord;
//varying vec2 v_texCoord;
//void main() {
//  gl_Position = a_position;
//  v_texCoord = a_texCoord;
//}

out vec2 v_texCoord;

void main()
{
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
    v_texCoord = texcoord[gl_VertexID];
}
