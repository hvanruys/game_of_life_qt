#version 420 core

//automata.fsh

in vec2 v_texCoord;
out vec4 color;

//varying vec2 v_texCoord;
uniform sampler2D tex; //the input texture
uniform float du; //the width of the cells
uniform float dv; //the height of the cells

/*
Any live cell with fewer than two live neighbours dies,
  as if caused by under-population.
Any live cell with two or three live neighbours
  lives on to the next generation.
Any live cell with more than three live neighbours dies,
  as if by overcrowding.
Any dead cell with exactly three live neighbours
  becomes a live cell, as if by reproduction.
*/

void main() {
  int count = 0;

  vec4 C = texture( tex, v_texCoord );
  vec4 E = texture( tex, vec2(v_texCoord.x + du, v_texCoord.y) );
  vec4 N = texture( tex, vec2(v_texCoord.x, v_texCoord.y + dv) );
  vec4 W = texture( tex, vec2(v_texCoord.x - du, v_texCoord.y) );
  vec4 S = texture( tex, vec2(v_texCoord.x, v_texCoord.y - dv) );
  vec4 NE = texture( tex, vec2(v_texCoord.x + du, v_texCoord.y + dv) );
  vec4 NW = texture( tex, vec2(v_texCoord.x - du, v_texCoord.y + dv) );
  vec4 SE = texture( tex, vec2(v_texCoord.x + du, v_texCoord.y - dv) );
  vec4 SW = texture( tex, vec2(v_texCoord.x - du, v_texCoord.y - dv) );

  if (E.r == 1.0) { count++; }
  if (N.r == 1.0) { count++; }
  if (W.r == 1.0) { count++; }
  if (S.r == 1.0) { count++; }
  if (NE.r == 1.0) { count++; }
  if (NW.r == 1.0) { count++; }
  if (SE.r == 1.0) { count++; }
  if (SW.r == 1.0) { count++; }

  if ( (C.r == 0.0 && count == 3) || (C.r == 1.0 && (count == 2 || count == 3)))
  {
    color = vec4(1.0, 1.0, 1.0, 1.0); //cell lives...
  } else {
    color = vec4(0.0, 0.0, 0.0, 1.0); //cell dies...
  }

}
