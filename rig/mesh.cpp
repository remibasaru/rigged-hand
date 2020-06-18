
#include <assert.h>
#include <iostream>
#include "mesh.h"
using namespace std;

#define POSITION_LOCATION    0
#define TEX_COORD_LOCATION   1
#define NORMAL_LOCATION      2
#define BONE_ID_LOCATION_1     3
#define BONE_WEIGHT_LOCATION_1 4
#define BONE_ID_LOCATION_2     5
#define BONE_WEIGHT_LOCATION_2 6
#define BONE_ID_LOCATION_3     7
#define BONE_WEIGHT_LOCATION_3 8
#define BONE_WEIGHT_LOCATION_4 9


#define NUM_OF_ID_AND_WEIGHT 12


float Mesh::VertexBoneData::sum(int j)
	{
	
	total = Weights[0] + Weights[1] + Weights[2] + Weights[3]
		+ Weights2[0] + Weights2[1] + Weights2[2] + Weights2[3]
		+ Weights3[0] + Weights3[1] + Weights3[2] + Weights3[3];

	return total; 
	}

void Mesh::VertexBoneData::AddBoneData(uint BoneID, float Weight, uint j)
	{
    
	if(Weight > 0.5)
		mainbone_index = BoneID;

	for (uint i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(IDs) ; i++) 
		{
        if (Weights[i] == 0.0) 
			{
            IDs[i] = BoneID;
            Weights[i] = Weight;
            return;
			}        
		}
    // should never get here - more bones than we have space for
    assert(0);
	}

Mesh::Mesh()
	{
    m_VAO = 0;
    ZERO_MEM(m_Buffers);
	m_NumBones = 0;
    pScene = NULL;

	boneMap[0].name = "ribs.001"; boneMap[0].parentindex=-1;
	boneMap[1].name = "shoulder.L"; boneMap[1].parentindex=0;
	boneMap[2].name = "upper_arm.L"; boneMap[2].parentindex=1;
	boneMap[3].name = "forearm.L"; boneMap[3].parentindex=2;
	boneMap[4].name = "hand.L"; boneMap[4].parentindex=3;
	boneMap[5].name = "palm.02.L.001"; boneMap[5].parentindex=4;
	boneMap[6].name = "palm.02.L"; boneMap[6].parentindex=5;
	boneMap[7].name = "finger_middle.01.L"; boneMap[7].parentindex=6;
	boneMap[8].name = "finger_middle.02.L"; boneMap[8].parentindex=7;
	boneMap[9].name = "finger_middle.03.L"; boneMap[9].parentindex=8;
	boneMap[10].name = "palm.03.L.001"; boneMap[10].parentindex=4;
	boneMap[11].name = "palm.03.L"; boneMap[11].parentindex=10;
	boneMap[12].name = "finger_ring.01.L"; boneMap[12].parentindex=11;
	boneMap[13].name = "finger_ring.02.L"; boneMap[13].parentindex=12;
	boneMap[14].name = "finger_ring.03.L"; boneMap[14].parentindex=13;
	boneMap[15].name = "palm.04.L.001"; boneMap[15].parentindex=4;
	boneMap[16].name = "palm.04.L"; boneMap[16].parentindex=15;
	boneMap[17].name = "finger_pinky.01.L"; boneMap[17].parentindex=16;
	boneMap[18].name = "finger_pinky.02.L"; boneMap[18].parentindex=17;
	boneMap[19].name = "finger_pinky.03.L"; boneMap[19].parentindex=18;
	boneMap[20].name = "palm.01.L.001"; boneMap[20].parentindex=4;
	boneMap[21].name = "palm.01.L"; boneMap[21].parentindex=20;
	boneMap[22].name = "finger_index.01.L"; boneMap[22].parentindex=21;
	boneMap[23].name = "finger_index.02.L"; boneMap[23].parentindex=22;
	boneMap[24].name = "finger_index.03.L"; boneMap[24].parentindex=23;
	boneMap[25].name = "thumb.01.L.001"; boneMap[25].parentindex=21;
	boneMap[26].name = "thumb.01.L"; boneMap[26].parentindex=25;
	boneMap[27].name = "thumb.02.L"; boneMap[27].parentindex=26;
	boneMap[28].name = "thumb.03.L"; boneMap[28].parentindex=27;
	boneMap[29].name = "forearm.L.003"; boneMap[29].parentindex=3;
	boneMap[30].name = "shoulder.R"; boneMap[30].parentindex=0;
	boneMap[31].name = "upper_arm.R"; boneMap[31].parentindex=30;
	boneMap[32].name = "forearm.R"; boneMap[32].parentindex=31;
	boneMap[33].name = "hand.R"; boneMap[33].parentindex=32;
	boneMap[34].name = "palm.01.R.001"; boneMap[34].parentindex=33;
	boneMap[35].name = "palm.01.R"; boneMap[35].parentindex=34;
	boneMap[36].name = "finger_index.01.R"; boneMap[36].parentindex=35;
	boneMap[37].name = "finger_index.02.R"; boneMap[37].parentindex=36;
	boneMap[38].name = "finger_index.03.R"; boneMap[38].parentindex=37;
	boneMap[39].name = "thumb.01.R.001"; boneMap[39].parentindex=35;
	boneMap[40].name = "thumb.01.R"; boneMap[40].parentindex=39;
	boneMap[41].name =  "thumb.02.R"; boneMap[41].parentindex=40;
	boneMap[42].name = "thumb.03.R"; boneMap[42].parentindex=41;
	boneMap[43].name = "palm.02.R.001"; boneMap[43].parentindex=33;
	boneMap[44].name = "palm.02.R"; boneMap[44].parentindex=43;
	boneMap[45].name = "finger_middle.01.R"; boneMap[45].parentindex=44;
	boneMap[46].name = "finger_middle.02.R"; boneMap[46].parentindex=45;
	boneMap[47].name = "finger_middle.03.R"; boneMap[47].parentindex=46;
	boneMap[48].name = "palm.03.R.001"; boneMap[48].parentindex=33;
	boneMap[49].name = "palm.03.R"; boneMap[49].parentindex=48;
	boneMap[50].name = "finger_ring.01.R"; boneMap[50].parentindex=49;
	boneMap[51].name = "finger_ring.02.R"; boneMap[51].parentindex=50;
	boneMap[52].name = "finger_ring.03.R"; boneMap[52].parentindex=51;
	boneMap[53].name = "palm.04.R.001"; boneMap[53].parentindex=33;
	boneMap[54].name = "palm.04.R"; boneMap[54].parentindex=53;
	boneMap[55].name = "finger_pinky.01.R"; boneMap[55].parentindex=54;
	boneMap[56].name = "finger_pinky.02.R"; boneMap[56].parentindex=55;
	boneMap[57].name = "finger_pinky.03.R"; boneMap[57].parentindex=56;
	boneMap[58].name = "forearm.R.003"; boneMap[58].parentindex=32;

	std::vector<BoneSet> v(boneMap, boneMap + sizeof boneMap / sizeof boneMap[0]);
	vectorBoneMap = v;
	PopulateChildren();
	}

Mesh::~Mesh()
	{
    Clear();
	}

void Mesh::Clear()
	{
    for (unsigned int i = 0 ; i < m_Textures.size() ; i++) 
		{
        SAFE_DELETE(m_Textures[i]);
		}

    if (m_Buffers[0] != 0) 
		{
        glDeleteBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);
		}
       
    if (m_VAO != 0) 
		{
        glDeleteVertexArrays(1, &m_VAO);
        m_VAO = 0;
		}
	}

bool Mesh::LoadMesh(const std::string& Filename)
	{
    // Release the previously loaded mesh (if it exists)
    Clear();
    // Create the VAO
	glGenVertexArrays(1, &m_VAO);   
    glBindVertexArray(m_VAO);
	
	// Create the buffers for the vertices attributes
	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	//Buffer	text = open("resources//models//RiggedModel//Hand_v.4.md5mesh", "rt").read()
    bool Ret = false;
    //Assimp::Importer Importer;
	

	pScene = Importer.ReadFile(Filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs|aiProcess_LimitBoneWeights);
	
    if (pScene) 
		{
		m_GlobalInverseTransform = pScene->mRootNode->mTransformation;
        m_GlobalInverseTransform.Inverse();
        Ret = InitFromScene(pScene, Filename);
		}
    else 
		{
        printf("Error parsing '%s': '%s'\n", Filename.c_str(), Importer.GetErrorString());
		}
	glBindVertexArray(0);	
    return Ret;
	}

bool Mesh::InitFromScene(const aiScene* pScene, const std::string& Filename)
	{  
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);
	
    vector<Vector3f> Positions;
    vector<Vector3f> Normals;
    vector<Vector2f> TexCoords;
	vector<VertexBoneData> Bones;
    vector<unsigned int> Indices;

    unsigned int NumVertices = 0;
    unsigned int NumIndices = 0;
    
    // Count the number of vertices and indices
	m_Entries.pop_back();
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) 
		{
        m_Entries[i].MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;        
        m_Entries[i].NumIndices = pScene->mMeshes[i]->mNumFaces * 3;
        m_Entries[i].BaseVertex = NumVertices;
        m_Entries[i].BaseIndex = NumIndices;
        
        NumVertices += pScene->mMeshes[i]->mNumVertices;
        NumIndices  += m_Entries[i].NumIndices;
		}
	
    // Reserve space in the vectors for the vertex attributes and indices
    Positions.reserve(NumVertices);
    Normals.reserve(NumVertices);
    TexCoords.reserve(NumVertices);
	Bones.resize(NumVertices);
    Indices.reserve(NumIndices);

    // Initialize the meshes in the scene one by one
	uint num_of_bones;
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) 
		{
        const aiMesh* paiMesh = pScene->mMeshes[i];
		num_of_bones=(uint)paiMesh->mNumBones;
        InitMesh(i,paiMesh, Positions, Normals, TexCoords,  Bones,Indices);
		}

#ifndef TOTAL_NUM_OF_BONES 
#define TOTAL_NUM_OF_BONES num_of_bones
#endif

	
	int ite = 0;
	int seven = 0;
	float colo[21][3];
    for (uint j = 0 ; j < 21 ; j++)
		{
		colo[j][0] = 0.0f;
		colo[j][1] = 0.0f;
		colo[j][2] = 0.0f;
		int level;
		if(ite > -1 && ite < 7)
			level=0;
		else if (ite > 6 && ite < 14)
			level=1;
		else
			level=2;
		float temp=1/((float)seven+1);
		colo[j][level]=temp;

		ite++;
		seven++;
		if(seven>6)
			seven=0;
		}
		
	uint incomplete = 0;
    for (uint j = 0 ; j < NumVertices ; j++)
			{
			Bones[j].sum(j);
			// assert that the sum of weighing should be equal to or at least close to 1
			if(Bones[j].total <= 1.01 && Bones[j].total >= 0.99)
				{
				Bones[j].incom=false;
				incomplete++;
				}
			else
				Bones[j].incom=true;
		
			if(Bones[j].mainbone_index>-1)
				{
				Bones[j].color[0]= colo[Bones[j].mainbone_index][0];
				Bones[j].color[1]= colo[Bones[j].mainbone_index][1];
				Bones[j].color[2]= colo[Bones[j].mainbone_index][2];
				
				}
			}
		cout << endl << incomplete;
		

    if (!InitMaterials(pScene, Filename))
		{
        return false;
		}
	//aiNode *x= pScene->mRootNode;
	
    // Generate and populate the buffers with vertex attributes and the indices

	//buffer for position of vertexes in mesh
  	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Positions[0]) * Positions.size(), &Positions[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(POSITION_LOCATION);
    glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);    

	//buffer for text coordinate of vertexes
    glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(TexCoords[0]) * TexCoords.size(), &TexCoords[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEX_COORD_LOCATION);
    glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//buffer for normals of vertexes
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals[0]) * Normals.size(), &Normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_LOCATION);
    glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);


	//buffer for 3 sets of bone id(4 int ) and weight(4 float) and also a color(4 float) 
   	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Bones[0]) * Bones.size(), &Bones[0], GL_STATIC_DRAW);
	
	
	//first set of bone id and weight
    glEnableVertexAttribArray(BONE_ID_LOCATION_1);
    glVertexAttribIPointer(BONE_ID_LOCATION_1, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);



	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION_1);    
    glVertexAttribPointer(BONE_WEIGHT_LOCATION_1, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)(sizeof(uint [4])));
	//second set of bone id and weight
	glEnableVertexAttribArray(BONE_ID_LOCATION_2);
    glVertexAttribIPointer(BONE_ID_LOCATION_2, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)( sizeof(uint [4]) +sizeof(float [4]) ));
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION_2);    
    glVertexAttribPointer(BONE_WEIGHT_LOCATION_2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)(sizeof(uint [8]) +sizeof(float [4])));
	//third set of bone id and weight  i.e. a total of 12.
	glEnableVertexAttribArray(BONE_ID_LOCATION_3);
    glVertexAttribIPointer(BONE_ID_LOCATION_3, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)( sizeof(uint [8]) +sizeof(float [8]) ));

	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION_3);    
    glVertexAttribPointer(BONE_WEIGHT_LOCATION_3, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)(sizeof(uint [12]) +sizeof(float [8])));
	//color array
	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION_4);    
    glVertexAttribPointer(BONE_WEIGHT_LOCATION_4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)(sizeof(uint [12]) +sizeof(float [12])  )   );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices[0]) * Indices.size(), &Indices[0], GL_STATIC_DRAW);
    
    return GLCheckError();
	
	}

void Mesh::InitMesh(uint MeshIndex,
					const aiMesh* paiMesh,
                    vector<Vector3f>& Positions,
                    vector<Vector3f>& Normals,
                    vector<Vector2f>& TexCoords,
					vector<VertexBoneData>& Bones,
                    vector<unsigned int>& Indices)
	{    
    const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);
	float totalx = 0;
	float totaly = 0;
	float totalz = 0;
	float Avx = 0;
	float Avy = 0;
	float Avz = 0;
    // Populate the vertex attribute vectors
    for (uint i = 0 ; i < paiMesh->mNumVertices ; i++) 
		{
        const aiVector3D* pPos      = &(paiMesh->mVertices[i]);
        const aiVector3D* pNormal   = &(paiMesh->mNormals[i]);
        const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		totalx=totalx+pPos->x;
		totaly=totaly+pPos->y;
		totalz=totalz+pPos->z;
        Positions.push_back(Vector3f(pPos->x, pPos->y, pPos->z));
        Normals.push_back(Vector3f(pNormal->x, pNormal->y, pNormal->z));
        TexCoords.push_back(Vector2f(pTexCoord->x, pTexCoord->y));
		}
	Avx = totalx / paiMesh->mNumVertices;
	Avy = totaly / paiMesh->mNumVertices;
	Avz = totalz / paiMesh->mNumVertices;
	Vector3f offsetvectemp((Avx=totalx/paiMesh->mNumVertices)*-1,(Avy=totaly/paiMesh->mNumVertices)*-1,(Avz=totalz/paiMesh->mNumVertices)*-1);
	offsetvec = offsetvectemp;
	LoadBones(MeshIndex, paiMesh, Bones);

    // Populate the index buffer
    for (unsigned int i = 0 ; i < paiMesh->mNumFaces ; i++) 
		{
        const aiFace& Face = paiMesh->mFaces[i];
        assert(Face.mNumIndices == 3);
        Indices.push_back(Face.mIndices[0]);
        Indices.push_back(Face.mIndices[1]);
        Indices.push_back(Face.mIndices[2]);
		}
	}

void Mesh::PopulateChildren()
	{
	
	for(uint i = 0; i<vectorBoneMap.size();i++)
		{
			if(vectorBoneMap[i].parentindex>-1)
			vectorBoneMap[vectorBoneMap[i].parentindex].children.push_back(i);
			
		}

	}

void Mesh::LoadBones(uint MeshIndex, const aiMesh* pMesh, vector<VertexBoneData>& Bones)
	{
	for (uint i = 0 ; i < pMesh->mNumBones ; i++) 
		{ 
        uint BoneIndex = 0;        
        string BoneName(pMesh->mBones[i]->mName.data);
        for(uint j = 0; j<vectorBoneMap.size();j++)
			{
			if(vectorBoneMap[j].name==BoneName)
				{
					vectorBoneMap[j].BoneOffset=pMesh->mBones[i]->mOffsetMatrix;
				}
			}

        if (m_BoneMapping.find(BoneName) == m_BoneMapping.end()) 
			{
            // Allocate an index for a new bone
            BoneIndex = m_NumBones;
            m_NumBones++;            
	        BoneInfo bi;			
			m_BoneInfo.push_back(bi);
            m_BoneInfo[BoneIndex].BoneOffset = pMesh->mBones[i]->mOffsetMatrix;            
            m_BoneMapping[BoneName] = BoneIndex;
			}
        else 
			{
            BoneIndex = m_BoneMapping[BoneName];
			}                      
        
	
        for (uint j = 0 ; j < pMesh->mBones[i]->mNumWeights ; j++)
			{
			
			
            uint VertexID = m_Entries[MeshIndex].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
            float Weight  = pMesh->mBones[i]->mWeights[j].mWeight;   
			if(VertexID>3108)
				cout<<324;
            Bones[VertexID].AddBoneData(BoneIndex, Weight,VertexID);
			int c=34;
			}
		} 
}

bool Mesh::InitMaterials(const aiScene* pScene, const std::string& Filename)
	{
    // Extract the directory part from the file name
    std::string::size_type SlashIndex = Filename.find_last_of("/");
    std::string Dir;
	
    if (SlashIndex == std::string::npos) {
        Dir = ".";
    }
    else if (SlashIndex == 0) {
        Dir = "/";
    }
    else {
        Dir = Filename.substr(0, SlashIndex);
    }

    bool Ret = true;

    // Initialize the materials
    for (unsigned int i = 0 ; i < pScene->mNumMaterials ; i++)
	{
        const aiMaterial* pMaterial = pScene->mMaterials[i];

        m_Textures[i] = NULL;

        if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0) 
			{
            aiString Path;

            if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &Path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS) 
				{
                std::string FullPath = Dir + "/" + Path.data;
                m_Textures[i] = new CTexture();
				m_Textures[i]->SetTarget(GL_TEXTURE_2D);

                if (!m_Textures[i]->Load(FullPath.c_str())) 
					{
                    printf("Error loading texture '%s'\n", FullPath.c_str());
                    delete m_Textures[i];
                    m_Textures[i] = NULL;
                    Ret = false;
					}
                else 
					{
                    printf("Loaded texture '%s'\n", FullPath.c_str());
					}
				}
			}

        // Load a white texture in case the model does not include its own texture
        if (!m_Textures[i]) 
			{
            m_Textures[i] = new CTexture();
			m_Textures[i]->SetTarget(GL_TEXTURE_2D);

			Ret = m_Textures[i]->Load("resources//models//RiggedModel//HAND_C_Copy.jpg");
			}

    }

	return Ret;
	}

void Mesh::Render()
	{
    glBindVertexArray(m_VAO);
    
    for (unsigned int i = 0 ; i < m_Entries.size() ; i++) {
        const unsigned int MaterialIndex = m_Entries[i].MaterialIndex;

        assert(MaterialIndex < m_Textures.size());
        
        if (m_Textures[MaterialIndex]) {
            m_Textures[MaterialIndex]->Bind(GL_TEXTURE0);
        }

		glDrawElementsBaseVertex(GL_TRIANGLES, 
                                 m_Entries[i].NumIndices, 
                                 GL_UNSIGNED_INT, 
                                 (void*)(sizeof(unsigned int) * m_Entries[i].BaseIndex), 
                                 m_Entries[i].BaseVertex);
    }

    // Make sure the VAO is not changed from the outside    
    glBindVertexArray(0);
	}



void Mesh::BoneTransform( vector<Matrix4f>& Transforms)
	{
    Matrix4f Identity;
    Identity.InitIdentity();
    
    ReadNodeHeirarchy( Identity,pScene->mRootNode);
    Transforms.resize(m_NumBones);
    for (uint i = 0 ; i < m_NumBones ; i++) 
		{
        Transforms[i] = m_BoneInfo[i].FinalTransformation;
		}

	}


void Mesh::ReadNodeHeirarchy(Matrix4f& parent_traformation, const aiNode* pNode)
	{    
	string NodeName(pNode->mName.data);
    Matrix4f NodeTransformation(pNode->mTransformation);

	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) 
		{
        uint BoneIndex = m_BoneMapping[NodeName];
		Matrix4f a = m_BoneInfo[BoneIndex].Abduction_Transformation;
		Matrix4f f = m_BoneInfo[BoneIndex].Flexion_Transformation;
		Matrix4f x = NodeTransformation;
		NodeTransformation = a * f * x;
        
		}

	Matrix4f GlobalTransformation = parent_traformation * NodeTransformation;
	
    if (m_BoneMapping.find(NodeName) != m_BoneMapping.end()) 
		{
        uint BoneIndex = m_BoneMapping[NodeName];
        m_BoneInfo[BoneIndex].FinalTransformation = m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
		}
    
    for (uint i = 0 ; i < pNode->mNumChildren ; i++) 
		{
        ReadNodeHeirarchy(GlobalTransformation, pNode->mChildren[i] );
		}
	}

Matrix4f Mesh::Rotate(float Angle, Vector3f& Axe)
	{
    const float SinHalfAngle = sinf(ToRadian(Angle/2));
    const float CosHalfAngle = cosf(ToRadian(Angle/2));

    const float Rx = Axe.x * SinHalfAngle;
    const float Ry = Axe.y * SinHalfAngle;
    const float Rz = Axe.z * SinHalfAngle;
    const float Rw = CosHalfAngle;
    Quaternion RotationQ(Rx, Ry, Rz, Rw);

    Quaternion ConjugateQ = RotationQ.Conjugate();
    ConjugateQ.Normalize();

	aiQuaternion RotationQQ= aiQuaternion(Rx, Ry, Rz, Rw);
	RotationQQ.Normalize();
	Matrix4f RotationM=RotationQQ.GetMatrix();
	return RotationM;
	}

void Mesh::Transform(uint boneindex,float angle_flex, float angle_abduc)
	{
	Vector3f Axil_flex(1.0f,0.0f,0.0f);
	Matrix4f Flexion_Matrix;
	Flexion_Matrix.InitIdentity();

	Vector3f Axil_abduc(0.0f,0.0f,1.0f);
	Matrix4f Abduction_Matrix;
	Abduction_Matrix.InitIdentity();
	Matrix4f negiden;
	negiden.InitNegativeIdentity();
	Flexion_Matrix=Rotate(angle_flex, Axil_flex)*negiden;
	Abduction_Matrix=Rotate(angle_abduc, Axil_abduc)*negiden;
			
	m_BoneInfo[boneindex].Abduction_Transformation=Abduction_Matrix;
	m_BoneInfo[boneindex].Flexion_Transformation=Flexion_Matrix;
	}
