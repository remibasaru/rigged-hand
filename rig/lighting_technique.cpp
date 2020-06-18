

#include <limits.h>
#include <string>
#include <glfx.h>
#include <assert.h>

#include "lighting_technique.h"
#include "util.h"


LightingTechnique::LightingTechnique()
    {   
    }

bool LightingTechnique::Init()
    {
    if (!Technique::Init())
        {
        return false;
        }

    if (!AddShader(GL_VERTEX_SHADER, "Shader/HandRigShader.vs")) 
        {
        return false;
        }

    if (!AddShader(GL_FRAGMENT_SHADER, "Shader/HandRigShader.fs")) 
        {
        return false;
        }

    if (!Finalize()) 
        {
        return false;
        }
	offsetvecloc = GetUniformLocation("gOffset");
    m_WVPLocation = GetUniformLocation("gWVP");
    m_WorldMatrixLocation = GetUniformLocation("gWorld");
    m_samplerLocation = GetUniformLocation("gSampler");
    m_dirLightLocation.Color = GetUniformLocation("gDirectionalLight.Color");
    m_dirLightLocation.AmbientIntensity = GetUniformLocation("gDirectionalLight.AmbientIntensity");
    m_dirLightLocation.Direction = GetUniformLocation("gDirectionalLight.Direction");
    m_dirLightLocation.DiffuseIntensity = GetUniformLocation("gDirectionalLight.DiffuseIntensity");
	
    if (m_dirLightLocation.AmbientIntensity == INVALID_UNIFORM_LOCATION ||
        m_WVPLocation == INVALID_UNIFORM_LOCATION ||
		//offsetvecloc == INVALID_UNIFORM_LOCATION ||
        m_WorldMatrixLocation == INVALID_UNIFORM_LOCATION ||
        m_samplerLocation == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.Color == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.DiffuseIntensity == INVALID_UNIFORM_LOCATION ||
        m_dirLightLocation.Direction == INVALID_UNIFORM_LOCATION) {
        return false;
    }
	for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(m_boneLocation) ; i++) 
        {
        char Name[128];
        memset(Name, 0, sizeof(Name));
        SNPRINTF(Name, sizeof(Name), "gBones[%d]", i);
        m_boneLocation[i] = GetUniformLocation(Name);
        }

    return true;
    }

void LightingTechnique::SetWVP(const Matrix4f& WVP)
    {
    glUniformMatrix4fv(m_WVPLocation, 1, GL_TRUE, (const GLfloat*)WVP.m);    
    }

void LightingTechnique::Setoffset(const Vector3f& off)
    {
    float v[3];
	v[0] = off.x;
	v[1] = off.y;
	v[2] = off.z;
	glUniform3fv(offsetvecloc, 1, (const GLfloat*)v);    
    }


void LightingTechnique::SetWorldMatrix(const Matrix4f& WorldInverse)
    {
    glUniformMatrix4fv(m_WorldMatrixLocation, 1, GL_TRUE, (const GLfloat*)WorldInverse.m);
    }


void LightingTechnique::SetTextureUnit(unsigned int TextureUnit)
    {
    glUniform1i(m_samplerLocation, TextureUnit);
    }


void LightingTechnique::SetDirectionalLight(const DirectionalLight& Light)
    {
    glUniform3f(m_dirLightLocation.Color, Light.Color.x, Light.Color.y, Light.Color.z);
    glUniform1f(m_dirLightLocation.AmbientIntensity, Light.AmbientIntensity);
    Vector3f Direction = Light.Direction;
    Direction.Normalize();
    glUniform3f(m_dirLightLocation.Direction, Direction.x, Direction.y, Direction.z);
    glUniform1f(m_dirLightLocation.DiffuseIntensity, Light.DiffuseIntensity);
    }

void LightingTechnique::SetBoneTransform(uint Index, const Matrix4f& Transform)
    {
    assert(Index < MAX_BONES);
    //Transform.Print();
    glUniformMatrix4fv(m_boneLocation[Index], 1, GL_TRUE, (const GLfloat*)Transform.m);       
    }