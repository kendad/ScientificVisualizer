#version 460 core

out vec4 FragColor;

in vec3 fNormal;
in vec3 fPos;
in float fScalar;

vec3 lightPosition = vec3(0.0f,2.0f,0.0f);//top of the model
vec3 surfaceBaseColor = vec3(0.7f,0.7f,0.7f);

//GOOCH COLOR RAMP
vec3 coolColor = vec3(0.0f,0.0f,0.55);//vertices facing away from the light will have cool blue color
vec3 warmColor = vec3(0.3,0.3,0.0);//vertices facing towards the light will have warm yellow color


//Fucntion to generate a VIRIDIS colormap
vec3 viridis(float t){
  //Purple -> Blue -> Green -> Yellow
  vec3 purple = vec3(0.267f,0.004f,0.329f);
  vec3 blue = vec3(0.191f,0.407f,0.556f);
  vec3 green = vec3(0.208f,0.718f,0.473f);
  vec3 yellow = vec3(0.993f,0.906f,0.144f);

  //we will divide the color into the following segments based on values of "t"
  //t is the vScalar value in range [0,1]
  //range [0.0 - 0.33] will blend color purple ->blue
  //range[0.33 - 0.66] will blend blue -> green
  //range[0.66 - 1.00] will blend green -> yellow

  //mix() expects a range of [0,1] so we do
  // "t/0.33" so any values between [0.00 - 0.33] is in range [0,1]
  if(t<0.33f) return mix(purple,blue,t/0.33f);
  // "(t-0.33)/0.33" so the range [0.33 - 0.66 ] is in range [0,1]
  if(t<0.66f) return mix(blue,green,(t-0.33f)/0.33f);

  // "(t-0.66)/0.34" so the range [0.66 - 1.00] is in range [0,1]
  return mix(green,yellow,(t-0.66f)/0.34f);
}

//Function to generate a JET colormap
vec3 jet(float t){
  float r = clamp(min(4.0f * t - 1.5f, -4.0f * t + 4.5f),0.0f,1.0f);
  float g = clamp(min(4.0f * t - 0.5f, -4.0f * t + 3.5f),0.0f,1.0f);
  float b = clamp(min(4.0f * t + 0.5f, -4.0f * t + 2.5f),0.0f,1.0f);
  return vec3(r,g,b);
}

void main(){

  //Normal
  vec3 n = normalize(fNormal);

  //a vector pointing from the vertex position to the light position
  vec3 lightDir = normalize(lightPosition-fPos);

  //Dot Product between vertex normal(N) and light Direction(L)
  float NdotL = dot(n,lightDir);//in range[-1,1]

  //Scale the dot Product to range [0,1]
  float NdotL_scaled = NdotL *0.5f +0.5f;

  //Get the color based on the choosen colormap
  vec3 dataColor = viridis(fScalar);

  //GOOCH LIGHTING
  float dataColorStrength = 0.8f;
  vec3 cool = coolColor + dataColorStrength * dataColor;
  vec3 warm = warmColor + dataColorStrength * dataColor;

  vec3 finalColor = mix(cool,warm,NdotL_scaled);

  FragColor = vec4(finalColor , 1.0f);
}
