
#version 120

attribute  vec3 vPosition;
attribute  vec3 vNormal;
attribute  vec4 vColor;
varying vec4 color;
varying vec3 Normal;
varying vec3 FragPos;

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
}
