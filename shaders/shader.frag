#version 460 core

out vec4 FragColor;

in vec3 gNormal;
in vec3 gPos;
in float gScalar;
in vec3 gBarycentric;

//uniform received from the user interface
uniform int colorMapType;
uniform vec2 scalarRange;//.x->lowerLimit and .y->upperLimit
uniform bool showWireFrame;

vec3 lightPosition = vec3(0.0f,2.0f,0.0f);//top of the model

//GOOCH COLOR RAMP
vec3 coolColor = vec3(0.0f,0.0f,0.55);//vertices facing away from the light will have cool blue color
vec3 warmColor = vec3(0.3,0.3,0.0);//vertices facing towards the light will have warm yellow color

//WireFrame color
vec3 wireframeColor = vec3(1.0f,1.0f,1.0f);//white color

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

//MAIN 
void main(){

  //Normal
  vec3 n = normalize(gNormal);

  //a vector pointing from the vertex position to the light position
  vec3 lightDir = normalize(lightPosition-gPos);

  //Dot Product between vertex normal(N) and light Direction(L)
  float NdotL = dot(n,lightDir);//in range[-1,1]

  //Scale the dot Product to range [0,1]
  float NdotL_scaled = NdotL *0.5f +0.5f;

  //Get the color based on the choosen colormap
  vec3 dataColor;
  if(gScalar < scalarRange.x || gScalar > scalarRange.y){
    //this is the out of bound range and colors it gray
    dataColor = vec3(0.25f,0.25f,0.25f);
  }else{
    dataColor = (colorMapType==0) ? viridis(gScalar) : jet(gScalar);
  }

  //GOOCH LIGHTING
  float dataColorStrength = 0.8f;
  vec3 cool = coolColor + dataColorStrength * dataColor;
  vec3 warm = warmColor + dataColorStrength * dataColor;

  //WIREFRAME
  float edgeFactor = 1.0;//intially do not show the wireframe
  if(showWireFrame){
  float thickness = 0.05f;

  //find the minimum distance to any edge
  float distanceFromEdge = min(min(gBarycentric.x,gBarycentric.y),gBarycentric.z);

  //step interpolation based on the distanceFromEdge
  //it will be 0.0 if distanceFromEdge < thickness else 1.0
  edgeFactor = step(thickness , distanceFromEdge);
  }


  vec3 finalColor = mix(cool,warm,NdotL_scaled);
  finalColor = mix(wireframeColor,finalColor,edgeFactor);

  FragColor = vec4(finalColor , 1.0f);
}
