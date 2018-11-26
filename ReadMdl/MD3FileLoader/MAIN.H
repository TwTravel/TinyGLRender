#ifndef _MAIN_H
#define _MAIN_H

#pragma warning(disable: 4786)

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>
#include <vector>
#include <gl\gl.h>	
#include <gl\glu.h>	
#include <gl\glaux.h>
#include <crtdbg.h>
using namespace std;

#define SCREEN_WIDTH 400
#define SCREEN_HEIGHT 300
#define SCREEN_DEPTH 16	

#define MAX_TEXTURES 100								// ����������Ŀ


// ����3D����࣬���ڱ���ģ���еĶ���
class CVector3 
{
public:
	float x, y, z;
};

// ����2D���࣬���ڱ���ģ�͵�UV��������
class CVector2 
{
public:
	float x, y;
};

//  ��Ľṹ����
struct tFace
{
	int vertIndex[3];			// ��������
	int coordIndex[3];			// ������������
};

//  ������Ϣ�ṹ��
struct tMaterialInfo
{
	char  strName[255];			// ��������
	char  strFile[255];			// �����������ӳ�䣬���ʾ�����ļ�����
	BYTE  color[3];				// �����RGB��ɫ

	int   texureId;				// ����ID
	float uTile;				// u �ظ�
	float vTile;				// v �ظ�
	float uOffset;			    // u ����ƫ��
	float vOffset;				// v ����ƫ��
} ;

//  ������Ϣ�ṹ��
struct t3DObject 
{
	int  numOfVerts;			// ģ���ж������Ŀ
	int  numOfFaces;			// ģ���������Ŀ
	int  numTexVertex;			// ģ���������������Ŀ
	int  materialID;			// ����ID
	bool bHasTexture;			// �Ƿ��������ӳ��
	char strName[255];			// ���������
	CVector3  *pVerts;			// ����Ķ���
	CVector3  *pNormals;		// ����ķ�����
	CVector2  *pTexVerts;		// ����UV����
	tFace *pFaces;				// ���������Ϣ
};

//  ģ����Ϣ�ṹ��
struct t3DModel 
{
	int numOfObjects;					// ģ���ж������Ŀ
	int numOfMaterials;					// ģ���в��ʵ���Ŀ
	vector<tMaterialInfo> pMaterials;	// ����������Ϣ
	vector<t3DObject> pObject;			// ģ���ж���������Ϣ
	
	int numOfTags;						// ģ���б�ǩ����Ŀ
	t3DModel	**pLinks;
	struct tMd3Tag		*pTags;
};


extern bool  g_bFullScreen;									
extern HWND  g_hWnd;										
extern RECT  g_rRect;										
extern HDC   g_hDC;											
extern HGLRC g_hRC;											
extern HINSTANCE g_hInstance;								

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hprev, PSTR cmdline, int ishow);
LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
WPARAM MainLoop();

// ���������������䱣��������������
void CreateTexture(UINT &texture, LPSTR strFileName);

// �ı���ʾΪȫ��ģʽ
void ChangeToFullScreen();

// �����û�����
HWND CreateMyWindow(LPSTR strWindowName, int width, int height, DWORD dwStyle, bool bFullScreen, HINSTANCE hInstance);

// �������ظ�ʽ
bool bSetupPixelFormat(HDC hdc);

// ��ʼ��ͶӰ�任
void SizeOpenGLScreen(int width, int height);

// ��ʼ��OpenGL
void InitializeOpenGL(int width, int height);

// ��ʼ����������
void Init(HWND hWnd);

// ��Ⱦ����
void RenderScene();

// �ͷų���ռ�õ��ڴ�ռ�
void DeInit();

#endif 


/////////////////////////////////////////////////////////////////////////////////
//
// * QUICK NOTES * 
//
// This file stores all the structures needed to just load a Quake3 character.
// To animate the character, we will need to add a bit more structures and such.
// These structures should all be the same pretty much as the other loader tutorials.
//
// I would like to note that in our t3DModel structure, we put a "struct" in front
// of the tMD3Tag so that we don't get an error when compiling because it doesn't
// know what a tMD3Tag is yet.  You can think of this as a "forward declaration", where
// it allows you to use it, even if it's hasn't been defined yet.  If you have problems
// with this in your compiler, just rip the structure tMD3Tag out of md3.h and stick
// it right above the t3DModel structure.
//
// * What's An STL (Standard Template Library) Vector? *
//
// Let me quickly explain the STL vector for those of you who are not familiar with them.
// To use a vector you must include <vector> and use the std namespace: using namespace std;
// A vector is an array based link list.  It allows you to dynamically add and remove nodes.
// This is a template class so it can be a list of ANY type.  To create a vector of type
// "int" you would say:  vector<int> myIntList;
// Now you can add a integer to the dynamic array by saying: myIntList.push_back(10);
// or you can say:  myIntList.push_back(num);.  The more you push back, the larger
// your array gets.  You can index the vector like an array.  myIntList[0] = 0;
// To get rid of a node you use the pop_back() function.  To clear the vector use clear().
// It frees itself so you don't need to worry about it, except if you have data
// structures that need information freed from inside them, like our objects.
//
//
// Ben Humphrey (DigiBen)
// Game Programmer
// DigiBen@GameTutorials.com
// Co-Web Host of www.GameTutorials.com
//
// The Quake3 .Md3 file format is owned by ID Software.  This tutorial is being used 
// as a teaching tool to help understand model loading and animation.  This should
// not be sold or used under any way for commercial use with out written consent
// from ID Software.
//
// Quake, Quake2 and Quake3 are trademarks of ID Software.
// Lara Croft is a trademark of Eidos and should not be used for any commercial gain.
// All trademarks used are properties of their respective owners. 
//
//