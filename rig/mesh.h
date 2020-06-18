#ifndef MESH_H
#define	MESH_H

#include <map>
#include <vector>
#include <assert.h>
#include <GL/glew.h>
#include <assimp/Importer.hpp>// C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags


#include "texture.h"
#include "util.h"
#include "math_3d.h"

#define TOTAL_NUM_OF_BONES 21
#define BONES_COUNT 59
#define M_PI       3.14159265358979323846

struct Vertex2
	{
    Vector3f m_pos;
    Vector2f m_tex;
    Vector3f m_normal;

    Vertex2() {}

    Vertex2(const Vector3f& pos, const Vector2f& tex, const Vector3f& normal)
		{
        m_pos    = pos;
        m_tex    = tex;
        m_normal = normal;
		}
	};


class Mesh
	{
	public:
		Mesh();
		~Mesh();
	
		Matrix4f Rotate(float Angle, Vector3f& Axis);
		bool LoadMesh(const std::string& Filename);

		void Render();
		unsigned int NumBones() const
			{
			return m_NumBones;
			}

		void BoneTransform( std::vector<Matrix4f>& Transforms);
		void Transform(uint boneindex,float angle_flex, float angle_abduc);
		Vector3f getoff()
			{
			return offsetvec;
			}
	private:
		#define NUM_BONES_PER_VEREX 4  //9 would suffice

		struct BoneInfo
			{
			Matrix4f BoneOffset;
			Matrix4f FinalTransformation;        
			Matrix4f Abduction_Transformation;
			Matrix4f Flexion_Transformation;
			BoneInfo()
				{
				BoneOffset.SetZero();
				FinalTransformation.SetZero(); 
				Abduction_Transformation.InitIdentity();
				Flexion_Transformation.InitIdentity();
				}
			};

		struct VertexBoneData
			{      
			uint IDs[NUM_BONES_PER_VEREX];
			float Weights[NUM_BONES_PER_VEREX];
			uint IDs2[NUM_BONES_PER_VEREX];
			float Weights2[NUM_BONES_PER_VEREX];
			uint IDs3[NUM_BONES_PER_VEREX];
			float Weights3[NUM_BONES_PER_VEREX];

			float color[3];

			uint fill;
			float total;
			int mainbone_index;
			//float total2;
			VertexBoneData()
				{
				Reset();
				};
        
			void Reset()
				{
				ZERO_MEM(IDs);
				ZERO_MEM(Weights);    
				ZERO_MEM(IDs2);
				ZERO_MEM(Weights2);   
				ZERO_MEM(IDs3);
				ZERO_MEM(Weights3);   
				/*ZERO_MEM(IDs4);
				ZERO_MEM(Weights4);  */
				mainbone_index=-1;
				fill=0;
				color[0]=1;
				color[1]=1;
				color[2]=1;
				}
			float sum(int j);
			bool incom;
			void AddBoneData(uint BoneID, float Weight,uint j);
		
			};
		struct BoneSet
			{
				std::string name;
				int parentindex;
				Matrix4f BoneOffset;
				vector<int> children;
				void setZero();
				Matrix4f Tranformation;
				Matrix4f GlobalTransformation;  //parent multiplied by transformation
				Matrix4f Final_Transformation;  //considered with offset matrix
				Matrix4f Parent_Transformation;  //transfortmation in the parent node
				BoneSet()
				{
					Tranformation.InitIdentity();
					Parent_Transformation.InitIdentity();
					BoneOffset.SetZero();
					Final_Transformation.SetZero(); 
				}
			};
		void PopulateChildren();
		void ReadNodeHeirarchy(uint boneindex,  Matrix4f& parent_traformation);
		void ReadNodeHeirarchy( Matrix4f& parent_traformation,const aiNode* pNode );
		bool InitFromScene(const aiScene* pScene, const std::string& Filename);
		void InitMesh(unsigned int Index, const aiMesh* paiMesh);
		void InitMesh(uint MeshIndex,
					  const aiMesh* paiMesh,
					  std::vector<Vector3f>& Positions,
					  std::vector<Vector3f>& Normals,
					  std::vector<Vector2f>& TexCoords,
					  std::vector<VertexBoneData>& Bones,
					  std::vector<unsigned int>& Indices);
		void LoadBones(uint MeshIndex, const aiMesh* paiMesh, vector<VertexBoneData>& Bones);
		bool InitMaterials(const aiScene* pScene, const std::string& Filename);
		void Clear();

	#define INVALID_MATERIAL 0xFFFFFFFF

		enum VB_TYPES 
			{
			INDEX_BUFFER,
			POS_VB,
			NORMAL_VB,
			TEXCOORD_VB,
			BONE_VB,
			NUM_VBs            
			};
 

		GLuint m_VAO;
		GLuint m_Buffers[4];

		struct MeshEntry 
			{
			MeshEntry()
				{
				NumIndices = 0;
				BaseVertex = 0;
				BaseIndex = 0;
				MaterialIndex = INVALID_MATERIAL;
				}

			unsigned int NumIndices;
			unsigned int BaseVertex;
			unsigned int BaseIndex;
			unsigned int MaterialIndex;
			};

		std::vector<MeshEntry> m_Entries;
		std::vector<CTexture*> m_Textures;
		BoneSet boneMap [BONES_COUNT];
		vector<BoneSet> vectorBoneMap;

		map<string,uint> m_BoneMapping; // maps a bone name to its index
		uint m_NumBones;
		vector<BoneInfo> m_BoneInfo;
		Matrix4f m_GlobalInverseTransform;
    
		const aiScene* pScene;
		Vector3f offsetvec;
		Assimp::Importer Importer;
	};


#endif	/* MESH_H */
