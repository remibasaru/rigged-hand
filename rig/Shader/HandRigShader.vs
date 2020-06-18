
#version 330                                                                
                                                                                  
layout (location = 0) in vec3 Position;                                          
layout (location = 1) in vec2 TexCoord;                                           
layout (location = 2) in vec3 Normal;   
                                  
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 Weights;

layout (location = 5) in ivec4 BoneIDs2;
layout (location = 6) in vec4 Weights2;

layout (location = 7) in ivec4 BoneIDs3;
layout (location = 8) in vec4 Weights3;

layout (location = 9) in vec3 Color;

const int MAX_BONES = 100;
uniform vec3 gOffset;                                                                                   
uniform mat4 gWVP;                                                                  
uniform mat4 gWorld;                                                                
uniform mat4 gBones[MAX_BONES];


                                                                                   
out vec2 TexCoord0;                                                                 
out vec3 Normal0;                                                                 
out vec3 WorldPos0;   
out vec3 Color0;                                                                                 
void main()                                                                        
{                                                                                  
    vec3 Position0=Position;//+ gOffset;
	
	mat4 BoneTransform = gBones[BoneIDs[0]] * Weights[0];
    BoneTransform     += gBones[BoneIDs[1]] * Weights[1];
    BoneTransform     += gBones[BoneIDs[2]] * Weights[2];
	BoneTransform     += gBones[BoneIDs[3]] * Weights[3];
	BoneTransform     += gBones[BoneIDs2[0]] * Weights2[0];
    BoneTransform     += gBones[BoneIDs2[1]] * Weights2[1];
    BoneTransform     += gBones[BoneIDs2[2]] * Weights2[2];
	BoneTransform     += gBones[BoneIDs2[3]] * Weights2[3];
	BoneTransform     += gBones[BoneIDs3[0]] * Weights3[0];
    BoneTransform     += gBones[BoneIDs3[1]] * Weights3[1];
    BoneTransform     += gBones[BoneIDs3[2]] * Weights3[2];
	BoneTransform     += gBones[BoneIDs3[3]] * Weights3[3];
    
	
	vec4 PosL    = BoneTransform * vec4(Position0, 1.0);
	vec4 NormalL = BoneTransform * vec4(Normal, 0.0);

	//gl_Position = gWVP * vec4(Position0, 1.0);
	 
	gl_Position =gWVP * PosL;                                  
    TexCoord0 = TexCoord;  
	Color0= Color;                                                        
    Normal0 = (gWorld * vec4(Normal, 0.0)).xyz;                                     
}