#version 460 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 vNormal;
out vec3 vPos;
out float vScalar;//for each vertex we will genearte a scalar value

uniform float displacementScale;

void main(){
  //Generate Synthetic scalar value
  //range [-1,1]
  float s = sin(aPos.x * 50.0f) * cos(aPos.z *50.0f);

  //vScalar in range [0,1]
  vScalar = s * 0.5 + 0.5;

  //Displace the vertex along its normal scaled by this Synthetic scalar value
  vec3 displacedPos = aPos + (aNormal * vScalar * displacementScale);

  gl_Position = projection * view * model * vec4(displacedPos,1.0f);

  //All light calculations will be in the model space
  vPos = vec3(model*vec4(displacedPos,1.0f));
  vNormal = aNormal;
}
