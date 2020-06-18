
#include <assert.h>

#include <math.h>
#include <GL\glew.h>
#include "GL\freeglut.h"
#include <iostream>

#ifndef WIN32
#include <sys/time.h>
#include <unistd.h>
#endif
#include <sys/types.h>

#include "engine_common.h"
#include "util.h"
#include "pipeline.h"
#include "camera.h"

#include "lighting_technique.h"
#include "glut_backend.h"
#include "mesh.h"
#ifndef WIN32
#include "freetypeGL.h"
#endif

using namespace std;

#define WINDOW_WIDTH  1280  
#define WINDOW_HEIGHT 1024

#ifndef WIN32
Markup sMarkup = { (char*)"Arial", 64, 1, 0, 0.0, 0.0,
                   {.1,1.0,1.0,.5}, {1,1,1,0},
                   0, {1,0,0,1}, 0, {1,0,0,1},
                   0, {0,0,0,1}, 0, {0,0,0,1} };
#endif


int con = 14;
int imPos = 0, increM = 0;
static bool i = false;

struct Vertex
	{
    Vector3f m_pos;
    Vector2f m_tex;
	Vector3f m_normal;
    Vertex() {}

    Vertex(Vector3f pos, Vector2f tex)
		{
        m_pos = pos;
        m_tex = tex;
		m_normal = Vector3f(0.0f, 0.0f, 0.0f);
		}
	};

struct Deformation
	{
    
    float angle_Flexion; //angle is perpendicular to palm plane
	float angle_Abduction; //angle is parallel to palm plane

	Deformation()
		{
		angle_Flexion = 0.0f;
		angle_Abduction = 0.0f;
		}
	};

float incre = 0.01f;

class Rigger : public ICallbacks
{
public:
	Rigger()
    {
        pGameCamera = NULL;
        pTexture = NULL;
        m_pEffect = NULL;
        m_directionalLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
        m_directionalLight.AmbientIntensity = 0.3f;
        m_directionalLight.DiffuseIntensity = 0.95f;
        m_directionalLight.Direction = Vector3f(1.0f, 0.50, 0.0);
		
		m_persProjInfo.FOV = 20.0f;
        m_persProjInfo.Height = WINDOW_HEIGHT;
        m_persProjInfo.Width = WINDOW_WIDTH;
        m_persProjInfo.zNear = 1.0f;
        m_persProjInfo.zFar = 100.0f;  
		
		m_frameCount = 0;
        m_fps = 0.0f;
        
        m_position = Vector3f(0.0f, 0.0f, 6.0f);   

		i = 0;
    }

	~Rigger()
    {
        SAFE_DELETE(m_pEffect);
        SAFE_DELETE(pGameCamera);
        SAFE_DELETE(pTexture);
    }

	bool Init()
		{
		Vector3f Pos(0.0f, 0.0f, -3.0f);
        Vector3f Target(0.0f, 0.0f, 1.0f);
        Vector3f Up(0.0, 1.0f, 0.0f);

        pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);
		//pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT,Pos, Target, Up);
		glEnable(GL_DEPTH_TEST);

        m_pEffect = new LightingTechnique();

        if (!m_pEffect->Init())
			{
			printf("Error initializing the lighting technique\n");
            return false;
			}
        m_pEffect->Enable();
        m_pEffect->SetTextureUnit(0);
		
		// Load Mesh
		bool isLoaded = m_pMesh.LoadMesh("resources//models//RiggedModel//Hand_v5.md5mesh");
		if (!isLoaded) 
			{
            printf("Mesh load failed \n");
            return false;            
			}
#ifndef WIN32
        if (!m_fontRenderer.InitFontRenderer()) {
            return false;
        }
#endif        	
        m_glutTime = glutGet(GLUT_ELAPSED_TIME);
        return true;
    }
	void Run()
		{
        GLUTBackendRun(this);
		}


	virtual void RenderSceneCB()
		{
		CalcFPS();
		pGameCamera->OnRender();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		m_pEffect->Enable();

		// Resole bone transformation
		vector<Matrix4f> Transforms;
		for(uint i = 0; i < TOTAL_NUM_OF_BONES; i++)
			m_pMesh.Transform(i, deform[i].angle_Flexion,deform[i].angle_Abduction );
		m_pMesh.BoneTransform(Transforms);
		for (uint i = 0 ; i < Transforms.size() ; i++) 
            m_pEffect->SetBoneTransform(i, Transforms[i]);
			
		Pipeline p;
		p.WorldPos(0.0f, 0.0f, 2.0f);
		Vector3f dir = p.getposition()  - pGameCamera->GetPos();
		
		p.SetCamera(pGameCamera->GetPos(), pGameCamera->GetTarget(), pGameCamera->GetUp());
		Vector3f dir3 = p.getposition() - m_pMesh.getoff();
		Vector3f dir4 = dir3 - pGameCamera->GetPos() ;
		dir4.Normalize();
		
		Vector3f dir2 = pGameCamera->GetTarget();
		dir.Normalize();
		p.SetPerspectiveProj(m_persProjInfo);


		m_pEffect->SetWVP(p.GetWVPTrans());
		m_pEffect->Setoffset(m_pMesh.getoff());
		const Matrix4f& WorldTransformation = p.GetWorldTrans();
        m_pEffect->SetWorldMatrix(WorldTransformation);
        m_pEffect->SetDirectionalLight(m_directionalLight);
	
		m_pMesh.Render();

		RenderFPS();

		if (increM)
			{
			imPos += 1;
			cout << "Frame: " << imPos << endl << endl;
  			writeColor(true);
			pGameCamera->Shift(0.02);
			increM = 0;
			writeDepth();
			writeJointAngle(Transforms);
			}
	

		glutSwapBuffers();
		}
	
	void writeColor( bool left)
		{
		unsigned char* buffer = (unsigned char*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * 3);
		glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, buffer);
		char name[1024];
		if (left)
			{
			sprintf(name, "D:\\Users\\abmn455\\Desktop\\thesis\\Matlab\\synthRGBL%d.txt", imPos);

			}
		else
			{
			sprintf(name, "D:\\Users\\abmn455\\Desktop\\thesis\\Matlab\\synthRGBR%d.txt", imPos);

			}
		FILE* fp = fopen(name, "wb");
		if (!fp)
			{
			fprintf(stderr, "ERROR: could not open %s for writing\n", name);
			}
		int bytes_in_row = WINDOW_WIDTH * 3;
		size_t  dddd;
		int bytes_left = WINDOW_WIDTH * WINDOW_HEIGHT * 3;
		int total_num_bytes_written = 0;
		while (bytes_left > 0)
			{
			int start_of_row = bytes_left - bytes_in_row;
			total_num_bytes_written += fwrite(&buffer[start_of_row], 1, bytes_in_row, fp);
			bytes_left -= bytes_in_row;
			}
		fclose(fp);
		free(buffer);
		}

	void writeDepth()
		{
		float* myDepthBuffer = (float*)malloc(WINDOW_WIDTH * WINDOW_HEIGHT * sizeof(float));
		glReadPixels(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, GL_DEPTH_COMPONENT, GL_FLOAT, myDepthBuffer);
		char namef[1024];
		sprintf(namef, "D:\\Users\\abmn455\\Desktop\\thesis\\Matlab\\synthDepth%d.txt", imPos);

		FILE* fpf = fopen(namef, "wb");
		if (!fpf)
			{
			fprintf(stderr, "ERROR: could not open %s for writing\n", namef);
			return;
			}

		unsigned int bytes_in_row = WINDOW_WIDTH;
		unsigned int bytes_left = WINDOW_WIDTH * WINDOW_HEIGHT;
		int total_num_bytes_written = 0;
		while (bytes_left > 0)
			{
			int start_of_row = bytes_left - bytes_in_row;
			total_num_bytes_written += fwrite(&myDepthBuffer[start_of_row], sizeof(float), bytes_in_row, fpf);
			bytes_left -= bytes_in_row;
			}

		fclose(fpf);
		free(myDepthBuffer);
		}

	void writeJointAngle(vector<Matrix4f> Transforms)
		{
		char fname[100];

		sprintf_s(fname, "D:\\Users\\abmn455\\Desktop\\thesis\\Matlab\\jointAngle%d.txt", imPos);
		FILE *fp = fopen(fname, "w");
		if (!fp)
			{
			fprintf(stderr, "ERROR: could not open %s for writing \n", fname);
			return;
			}
		
		int num_test = 0;
		for (uint i = 0; i < TOTAL_NUM_OF_BONES; i++)
			{
			num_test += fwrite(&deform[i].angle_Flexion, sizeof(float), 1, fp);
			num_test += fwrite(&deform[i].angle_Abduction, sizeof(float), 1, fp);
			}
		
		num_test += fwrite(&pGameCamera->GetPos().x, sizeof(float), 1, fp);
		num_test += fwrite(&pGameCamera->GetPos().y, sizeof(float), 1, fp);
		num_test += fwrite(&pGameCamera->GetPos().z, sizeof(float), 1, fp);

		fclose(fp);
		}



	virtual void IdleCB()
		{
        RenderSceneCB();
		}

    virtual void SpecialKeyboardCB(int Key, int x, int y)
		{
		pGameCamera->OnKeyboard(Key, m_pMesh.getoff());
		}

    virtual void KeyboardCB(unsigned char Key, int x, int y)
		{
      
		switch (Key) 
			{
            case 'q':
                glutLeaveMainLoop();
                break;

   //         case 'a':
   //             m_directionalLight.AmbientIntensity += 0.05f;
   //             break;

   //         case 's':
   //             m_directionalLight.AmbientIntensity -= 0.05f;
   //             break;
			//case 'z':
   //             m_directionalLight.DiffuseIntensity += 0.05f;
   //             break;

   //         case 'x':
   //             m_directionalLight.DiffuseIntensity -= 0.05f;
   //             break;
			//case 'd':
   //             m_directionalLight.AmbientIntensity = 1.00f;
			//	m_directionalLight.DiffuseIntensity = 0.00f;
   //             break;
			case 'r':
				imPos = 0;
				break;
			case 'v':
                incre=incre+0.05;
                break;
			case 'o':
				deform[con].angle_Flexion=deform[con].angle_Flexion+3.0f;
				//cout<<endl<<deform[con].angle_Flexion;
                break;
			case 'p':
				deform[con].angle_Flexion=deform[con].angle_Flexion-3.0f;
				//cout<<endl<<deform[con].angle_Flexion;
                break;
			case 'k':
				deform[con].angle_Abduction=deform[con].angle_Abduction+3.0f;
				//cout<<endl<<deform[con].angle_Abduction;
                break;
			case 'l':
				deform[con].angle_Abduction=deform[con].angle_Abduction-3.0f;
				//cout<<endl<<deform[con].angle_Abduction;
                break;
			case 'm':
				con++;
				break;
			case 'n':
				con--;
				break;

			case 'w':		
					pGameCamera->Shift(0.01f);
				break;
				}
		}
	

    virtual void PassiveMouseCB(int x, int y)
		{
        //pGameCamera->OnMouse(x, y);
		}

private:
	void CalcNormals(const unsigned int* pIndices, unsigned int IndexCount,
                     Vertex* pVertices, unsigned int VertexCount)
    {
        // Accumulate each triangle normal into each of the triangle vertices
        for (unsigned int i = 0 ; i < IndexCount ; i += 3) {
            unsigned int Index0 = pIndices[i];
            unsigned int Index1 = pIndices[i + 1];
            unsigned int Index2 = pIndices[i + 2];
            Vector3f v1 = pVertices[Index1].m_pos - pVertices[Index0].m_pos;
            Vector3f v2 = pVertices[Index2].m_pos - pVertices[Index0].m_pos;
            Vector3f Normal = v1.Cross(v2);
            Normal.Normalize();

            pVertices[Index0].m_normal += Normal;
            pVertices[Index1].m_normal += Normal;
            pVertices[Index2].m_normal += Normal;
        }

        // Normalize all the vertex normals
        for (unsigned int i = 0 ; i < VertexCount ; i++) {
            pVertices[i].m_normal.Normalize();
        }
    }



	void CreateVertexBuffer(const unsigned int* pIndices, unsigned int IndexCount)
		{
        Vertex Vertices[4] = { Vertex(Vector3f(-1.0f, -1.0f, 0.5773f), Vector2f(0.0f, 0.0f)),
                               Vertex(Vector3f(0.0f, -1.0f, -1.15475f), Vector2f(0.5f, 0.0f)),
                               Vertex(Vector3f(1.0f, -1.0f, 0.5773f),  Vector2f(1.0f, 0.0f)),
                               Vertex(Vector3f(0.0f, 1.0f, 0.0f),      Vector2f(0.5f, 1.0f)) };
        unsigned int VertexCount = ARRAY_SIZE_IN_ELEMENTS(Vertices);

        CalcNormals(pIndices, IndexCount, Vertices, VertexCount);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
		}

    void CreateIndexBuffer(const unsigned int* pIndices, unsigned int SizeInBytes)
		{
        glGenBuffers(1, &IBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, SizeInBytes, pIndices, GL_STATIC_DRAW);
		}

	void CalcFPS()
		{
        m_frameCount++;
        int time = glutGet( GLUT_ELAPSED_TIME );
        if (time - m_glutTime > 1000) 
			{
            m_fps = (float)m_frameCount * 1000.0f / (time - m_glutTime);
            m_glutTime = time;
            m_frameCount = 0;
			}
		}
        
    void RenderFPS()
    {
        char text[32];
        ZERO_MEM(text);        
        SNPRINTF(text, sizeof(text), "FPS: %.2f", m_fps);
#ifndef WIN32
        m_fontRenderer.RenderText(10, 10, text);        
#endif
    }

	GLuint VBO;
	GLuint IBO;
    LightingTechnique* m_pEffect;
    CTexture* pTexture;
    Camera* pGameCamera;
	float m_scale;
	DirectionalLight m_directionalLight;
	
	Mesh m_pMesh ;
	Vector3f m_position;            
    PersProjInfo m_persProjInfo;
	Deformation deform[TOTAL_NUM_OF_BONES];
#ifndef WIN32
    FontRenderer m_fontRenderer;
#endif    
    
	unsigned int i;

	int m_glutTime;
    long long m_startTime;
    int m_frameCount;
    float m_fps;
	
};
















int main(int argc, char** argv)
{
   
	//Magick::InitializeMagick(*argv);   
	GLUTBackendInit(argc, argv);
    if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, 32, false, "Rigged Hand Model")) 
		{
        return 1;
		}

	//SRANDOM;

	Rigger *pHdApp = new Rigger();


	bool success = pHdApp->Init();

	if(!success)
		{
		return 1;
		}

	pHdApp->Run();
	
	delete pHdApp;
	
	
	//system ("pause");
    return 0;
}

