#ifndef CAMERA_H
#define	CAMERA_H

#include "math_3d.h"
#define SPACEBAR 32

class Camera
    {
    public:

        Camera(int WindowWidth, int WindowHeight);

        Camera(int WindowWidth, int WindowHeight, const Vector3f& Pos, const Vector3f& Target, const Vector3f& Up);

	    bool Camera::OnKeyboard(int key, Vector3f offSet);

        void OnMouse(int x, int y);
        void OnRender();
	    float reAdjust(Vector3f offSet);
	    float Revolution(float disp) ;

	    float Shift(float dir);

        const Vector3f& GetPos() const
            {
            return m_pos;
            }

        const Vector3f& GetTarget() const
            {
            return m_target;
            }
            
        const Vector3f& GetUp() const
            {
            return m_up;
            }

    private:

        void Init();
        void Update();
	
	    Vector3f m_pos;
        Vector3f m_target;
        Vector3f m_up;

	    int m_windowWidth;
        int m_windowHeight;

        float m_AngleH;
        float m_AngleV;

        bool m_OnUpperEdge;
        bool m_OnLowerEdge;
        bool m_OnLeftEdge;
        bool m_OnRightEdge;

        Vector2i m_mousePos;
	    Vector3f shiftVector; 
    };

#endif	/* CAMERA_H */