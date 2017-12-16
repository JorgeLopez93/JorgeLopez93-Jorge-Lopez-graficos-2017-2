#version 120
in vec4 color;
in vec4 viewSpace;
in vec2 texCoord;

uniform int fogSelector;
uniform sampler2D texture_2D;
uniform int Texture_app_flag;
uniform float fogDensity;
uniform vec4 fogColor;

varying out vec4 fColor;

void main()
{
  vec4 finalColor = color;

  if (Texture_app_flag == 1)
    finalColor = finalColor * texture2D( texture_2D, texCoord );

  float dist;
  float fogFactor = 0;
  dist = length(viewSpace);

  if(fogSelector == 1)
  {
     fogFactor = (0 - dist)/(0 - 18);
     fogFactor = clamp( fogFactor, 0.0, 1.0 );
     finalColor = mix(fogColor, finalColor, 1 - fogFactor);
  }
  else if( fogSelector == 2)// exponential fog
  {
      fogFactor = 1.0 /exp(dist * fogDensity);
      fogFactor = clamp( fogFactor, 0.0, 1.0 );
      finalColor = mix(fogColor, finalColor, fogFactor);
  }
  else if( fogSelector == 3)
  {
     fogFactor = 1.0 /exp( (dist * fogDensity)* (dist * fogDensity));
     fogFactor = clamp( fogFactor, 0.0, 1.0 );
     finalColor = mix(fogColor, finalColor, fogFactor);
  }
  fColor = finalColor;
}
