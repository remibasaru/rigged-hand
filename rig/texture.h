
#ifndef TEXTURE_H
#define	TEXTURE_H
#endif

#ifdef WIN32
#include <Windows.h>
#endif

#include <string>

#include <GL/glew.h>
#include <gl/GL.h>



// Class that provides a texture for texture mapping in OpenGL
class CTexture
	{
	public:
		void CreateFromData(BYTE* bData, int iWidth, int iHeight, int iBPP, GLenum format, bool bGenerateMipMaps = false);
		bool Load(std::string sPath, bool bGenerateMipMaps = false);
		void Bind(int iTextureUnit = 0);
		//void Bind2(GLenum TextureUnit);
		void SetFiltering(int tfMagnification, int tfMinification);

		void SetSamplerParameter(GLenum parameter, GLenum value);

		int GetMinificationFilter();
		int GetMagnificationFilter();

		int GetWidth();
		int GetHeight();
		int GetBPP();
		std::string GetPath();
		void SetTarget(GLenum TextureTarget);
		void Release();
	
		CTexture();
	private:
		int m_iWidth, m_iHeight, m_iBPP; // Texture width, height, and bytes per pixel
		UINT m_uiTexture; // Texture name
		UINT m_uiSampler; // Sampler name
		bool m_bMipMapsGenerated;
		GLenum m_textureTarget;
		int m_tfMinification, m_tfMagnification;

		std::string m_sPath;
	};





