#version 460 core

layout (triangles) in;
layout (triangle_strip,max_vertices = 3) out;

//Input from the Vertex Shader
//will be an array of size 3 for the three vertices in a triangle
in vec3 vNormal[];
in vec3 vPos[];
in float vScalar[];

//Outputs to the fragment Shader
out vec3 gNormal;
out vec3 gPos;
out float gScalar;
out vec3 gBarycentric;

void main(){

  //the 3 vertex of the triangle will have values
  //[1,0,0] , [0,1,0] , [0,0,1]
  vec3 barycentric[3]=vec3[](
      vec3(1,0,0),
      vec3(0,1,0),
      vec3(0,0,1)
      );

  //for the three vertices in the triangle
  for(int i=0;i<3;++i){
    gl_Position = gl_in[i].gl_Position;

    //pass the attributes
    gNormal = vNormal[i];
    gPos = vPos[i];
    gScalar = vScalar[i];
    gBarycentric = barycentric[i];

    //Emit the Vertex
    EmitVertex();
  }
  //Done with the triangle primitve
  EndPrimitive();
}
