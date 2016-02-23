#version 330

smooth in vec2 vTexCoord;
smooth in vec3 vNormal;
smooth in vec4 vEyeSpacePos;
smooth in vec3 vWorldPos;
out vec4 outputColor;
smooth in float biasCalc;
uniform sampler2D gSampler;
uniform vec4 vColor;

#include "DirectionalLight.txt"

uniform DirectionalLight sunLight;
uniform vec3 vEyePosition;

uniform Material matActive;

#include "shadows.frag"

smooth in vec4 ShadowCoord;
uniform sampler2D shadowMap;

uniform int hasTex;
vec4 vTexColor;
vec4 vMixedColor;

void main()
{
	if(hasTex == 1)
	{
		vTexColor = texture2D(gSampler, vTexCoord);
		vMixedColor = vTexColor*vColor;
	}
	else
	{
		vMixedColor = vColor;
	}
  vec4 AmbientColor = vec4(0.2,0.2,0.2,0.2);
  vec3 vNormalized = normalize(vNormal);

  float biasc = 0.0029;
  vec3 vVeye = normalize(vEyePosition-vWorldPos);
  float cosTheta = max(0.0,dot(vNormalized,vVeye));

  biasc = tan(acos(cosTheta));
  biasc = clamp(biasc,0.005,0.01);

  float visibility = GetVisibility(shadowMap, ShadowCoord, biasc);

  vec4 vDiffuseColor = GetDirectionalLightColor(sunLight, vNormalized, visibility);
  vec4 vSpecularColor = GetSpecularColor(vWorldPos, vEyePosition, matActive, sunLight, vNormalized, visibility);
   
  //if(length(vSpecularColor) > 0.0)
	//outputColor = vMixedColor * (vDiffuseColor + vec4(vSpecularColor.xyz,1.0));
  //
  //else
	outputColor = vMixedColor * vDiffuseColor;
}