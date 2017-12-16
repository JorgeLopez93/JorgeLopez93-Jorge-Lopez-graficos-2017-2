
#version 120

in  vec3 vPosition;
in  vec3 vNormal;
in  vec4 vColor;
in  vec2 vTexCoord;

varying out vec4 color;
varying out vec3 Normal;
varying out vec3 FragPos;
varying out vec2 texCoord;

uniform mat4 model_view;
uniform mat4 imodel;
uniform mat4 projection;

void main()
{
  vec4 vPosition4 = vec4(vPosition.x, vPosition.y, vPosition.z, 1.0);
  vec4 vColor4 = vec4(vColor.r, vColor.g, vColor.b, vColor.a);
  FragPos = -vec3(model_view * vPosition4);
  Normal = mat3(transpose(imodel)) * vNormal;
  gl_Position = projection * model_view * vPosition4;
  color = vColor4;
  texCoord.s = vTexCoord.s * 10 / 8;
  texCoord.t = vTexCoord.t * 12 / 8;
}
