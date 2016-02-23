#version 330

smooth in vec2 vTexCoord;
out vec4 outputColor;

uniform sampler2D gSampler;
uniform vec4 vColor;

void main()
{
   vec4 vTexColor = texture2D(gSampler, vTexCoord);
   vec4 vMixedColor = vTexColor*vColor;
   
   outputColor = vMixedColor;                                     
}

