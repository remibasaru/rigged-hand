
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "util.h"
#include "glut_backend.h"

// Points to the object implementing the ICallbacks interface which was delivered to
// GLUTBackendRun(). All events are forwarded to this object.

static ICallbacks* s_pCallbacks = NULL;

static void SpecialKeyboardCB(int Key, int x, int y)
    {
    s_pCallbacks->SpecialKeyboardCB(Key, x, y);
    }

static void KeyboardCB(unsigned char Key, int x, int y)
    {
    s_pCallbacks->KeyboardCB(Key, x, y);
    }


static void PassiveMouseCB(int x, int y)
    {
    s_pCallbacks->PassiveMouseCB(x, y);
    }


static void RenderSceneCB()
    {
    s_pCallbacks->RenderSceneCB();
    }


static void IdleCB()
    {
    s_pCallbacks->IdleCB();
    }


static void InitCallbacks()
    {
    glutDisplayFunc(RenderSceneCB);
    glutIdleFunc(IdleCB);
    glutSpecialFunc(SpecialKeyboardCB);
    glutPassiveMotionFunc(PassiveMouseCB);
    glutKeyboardFunc(KeyboardCB);
    }

void GLUTBackendInit(int argc, char** argv)
    {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    }

bool GLUTBackendCreateWindow(unsigned int Width, unsigned int Height, unsigned int bpp, bool isFullScreen, const char* pTitle)
    {
    if (isFullScreen)
        {
        char ModeString[64] = { 0 };
#ifdef WIN32
        _snprintf_s(ModeString, sizeof(ModeString), "%dx%d@%d", Width, Height, bpp);
#else
        snprintf(ModeString, sizeof(ModeString), "%dx%d@%d", Width, Height, bpp);
#endif
        glutGameModeString(ModeString);
        glutEnterGameMode();
        }
    else
        {
        glutInitWindowSize(Width, Height);
        glutCreateWindow(pTitle);
        }

    // Must be done after glut is initialized!
    GLenum res = glewInit();
    if (res != GLEW_OK) 
        {
        fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
        return false;
        }
	printf("GL version: %s\n", glGetString(GL_VERSION));
    return true;
    }

void GLUTBackendRun(ICallbacks* pCallbacks)
    {
    if (!pCallbacks)
        {
        fprintf(stderr, "%s : callbacks not specified!\n", __FUNCTION__);
        return;
        }

	glClearColor(0.5, 0.5, 0.5, 1);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    s_pCallbacks = pCallbacks;
    InitCallbacks();
    glutMainLoop();
    }