#version 460 core

layout (triangles) in;
layout (triangle_strip,max_vertices = 3) out;

//Input from the Vertex Shader
//will be an array of size 3 for the three vertices in a triangle
in vec3 vNormal[];
in vec3 vPos[];
in float vScalar[];

//Outputs to the fragment Shader
out vec3 fNormal;
out vec3 fPos;
out float fScalar;

void main(){
  //for the three vertices in the triangle
  for(int i=0;i<3;++i){
    gl_Position = gl_in[i].gl_Position;

    //pass the attributes
    fNormal = vNormal[i];
    fPos = vPos[i];
    fScalar = vScalar[i];

    //Emit the Vertex
    EmitVertex();
  }
  //Done with the triangle primitve
  EndPrimitive();
}
