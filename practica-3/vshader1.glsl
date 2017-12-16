
#version 120

in  vec3 vPosition;
in  vec4 vColor;
in  vec2 vTexCoord;

varying out vec4 color;
varying out vec4 viewSpace;
varying out vec2 texCoord;

uniform mat4 model_view;
uniform mat4 projection;

void main()
{
  vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
  vec4 vColor4 = vec4(vColor.r, vColor.g, vColor.b,  vColor.a);
  viewSpace = model_view * vPosition4;
  gl_Position = projection * viewSpace;
  color = vColor4;
  texCoord.x = vTexCoord.s * 10 / 8;
  texCoord.y = vTexCoord.t * 12 / 8;
}
