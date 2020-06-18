
#ifndef TECHNIQUE_H
#define	TECHNIQUE_H

#include <list>
#include <GL/glew.h>

class Technique
{
public:

    Technique();

    ~Technique();

    virtual bool Init();

    void Enable();

protected:

    bool AddShader(GLenum ShaderType,const char* pFilename);

    bool Finalize();

    GLint GetUniformLocation(const char* pUniformName);

private:
	const char* m_pEffectFile;
    GLuint m_shaderProg;

    typedef std::list<GLuint> ShaderObjList;
    ShaderObjList m_shaderObjList;
};

#define INVALID_UNIFORM_LOCATION 0xFFFFFFFF

#endif	/* TECHNIQUE_H */

