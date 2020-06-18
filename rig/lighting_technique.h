
#ifndef LIGHTING_TECHNIQUE_H
#define	LIGHTING_TECHNIQUE_H



#include "technique.h"
#include "math_3d.h"

struct DirectionalLight
{
    Vector3f Color;
    float AmbientIntensity;
    Vector3f Direction;
    float DiffuseIntensity;
};

class LightingTechnique : public Technique
{
public:

	static const uint MAX_BONES = 100;

    LightingTechnique();

    virtual bool Init();

    void SetWVP(const Matrix4f& WVP);
    void SetWorldMatrix(const Matrix4f& WVP);
    void SetTextureUnit(unsigned int TextureUnit);
    void SetDirectionalLight(const DirectionalLight& Light);
	void SetBoneTransform(uint Index, const Matrix4f& Transform);
	void Setoffset(const Vector3f& WVP);
private:

    GLuint m_WVPLocation;
    GLuint m_WorldMatrixLocation;
    GLuint m_samplerLocation;

    struct {
        GLuint Color;
        GLuint AmbientIntensity;
        GLuint Direction;
        GLuint DiffuseIntensity;
    } m_dirLightLocation;
	GLuint offsetvecloc;
	GLuint m_boneLocation[MAX_BONES];
};


#endif	/* LIGHTING_TECHNIQUE_H */
