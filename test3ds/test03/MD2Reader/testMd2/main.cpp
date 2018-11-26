#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <GL/gl.h>										
#include <GL/glu.h>	
//#include <FreeImage.h>
#include <math.h>

#include "c24bitmap.h"

#include "Md2.h"

typedef unsigned int UINT;
 
#define FILE_NAME  "TRIS.MD2"							
#define TEXTURE_NAME "RHINO.BMP"

//UINT g_Texture[MAX_TEXTURES] = {0};						
unsigned int g_Texture[MAX_TEXTURES] = {0};	

CLoadMD2 g_LoadMd2;										
t3DModel g_3DModel;									

int   g_ViewMode	  = GL_TRIANGLES;			
bool  g_bLighting     = true;				
float g_RotateX		  = 0.0f;				
float g_RotationSpeed = 0.5f;
//C24BitMap pBitmap;

struct AUX_RGBImageRec {
unsigned long sizeX;
unsigned long sizeY;
unsigned char *data;
};

/*static bool LoadTexture(const char* filename,
AUX_RGBImageRec* texture_image) {

FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(filename, 0);
FIBITMAP *dib = FreeImage_Load(fifmt, filename, 0);
dib = FreeImage_ConvertTo24Bits(dib);

int width = FreeImage_GetWidth(dib);
int height = FreeImage_GetHeight(dib);

BYTE *pixels = (BYTE*) FreeImage_GetBits(dib);
int pix = 0;

if (texture_image == NULL)
return FALSE;

texture_image->data = (BYTE *) malloc(width * height * 3);
texture_image->sizeX = width;
texture_image->sizeY = height;

for (pix = 0; pix < width * height; pix++) {
texture_image->data[pix * 3 + 0] = pixels[pix * 3 + 2];
texture_image->data[pix * 3 + 1] = pixels[pix * 3 + 1];
texture_image->data[pix * 3 + 2] = pixels[pix * 3 + 0];

}

FreeImage_Unload(dib);

return TRUE;
}*/

C24BitMap pBitmap;
void CreateTexture(UINT textureArray[], char* strFileName, int textureID)
{
	//AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// 如果无此文件，则直接返回
		return;

	pBitmap.Load(strFileName);				// 装入位图，并保存数据
	
	// 生成纹理
	glGenTextures(1, &textureArray[textureID]);

	// 设置像素对齐格式
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	/*gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap.Width, pBitmap.Height, GL_BGR, GL_UNSIGNED_BYTE, pBitmap.Buffer);
    //   glTexImage2D(GL_TEXTURE_2D,0,3,
	//	pBitmap.Width, pBitmap.Height,0,
	//	GL_BGR_EXT,GL_UNSIGNED_BYTE, pBitmap.Buffer);

	
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);*/
	
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
	//	GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glTexImage2D(GL_TEXTURE_2D,0,3,
		pBitmap.Width, pBitmap.Height,0,
		GL_BGR_EXT,GL_UNSIGNED_BYTE, pBitmap.Buffer);

}

/*void CreateTexture(UINT textureArray[], char* strFileName, int textureID)
{
	//AUX_RGBImageRec *pBitmap = NULL;
	//g_ColorPic  pBitmap;
	
	if(!strFileName)									// 如果无此文件，则直接返回
		return;
    AUX_RGBImageRec pBitmap;
	//pBitmap = auxDIBImageLoad(strFileName);				// 装入位图，并保存数据
	//pBitmap.Load(strFileName);
	//if(pBitmap == NULL)									// 如果装入位图失败，则退出
	LoadTexture(strFileName,
    &pBitmap);
	//	exit(0);

	// 生成纹理
	glGenTextures(1, &textureArray[textureID]);

	// 设置像素对齐格式
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap.Width, pBitmap.Height, GL_RGB, GL_UNSIGNED_BYTE, pBitmap.Buffer);
  //  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap.sizeX, pBitmap.sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap.data);
	//glTexImage2D( GL_TEXTURE_2D,0, GL_RGB, pBitmap.sizeX, pBitmap.sizeY ,0, GL_RGB, GL_UNSIGNED_BYTE, pBitmap.data );
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glTexImage2D( GL_TEXTURE_2D,0, GL_RGB, pBitmap.sizeX, pBitmap.sizeY ,0, GL_RGB, GL_UNSIGNED_BYTE, pBitmap.data );
	//gluBuild2DMipmaps( GL_TEXTURE_2D, 3, width,   height, GL_RGB, GL_UNSIGNED_BYTE, data );
	//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap.sizeX, pBitmap.sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap.data);
	
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	
 

}*/

//  从文件中创建纹理
/*void CreateTexture(UINT textureArray[], char* strFileName, int textureID)
{
	//AUX_RGBImageRec *pBitmap = NULL;
	//g_ColorPic  pBitmap;
	
	if(!strFileName)									// 如果无此文件，则直接返回
		return;

	//pBitmap = auxDIBImageLoad(strFileName);				// 装入位图，并保存数据
	pBitmap.Load(strFileName);
	//if(pBitmap == NULL)									// 如果装入位图失败，则退出
	//	exit(0);

	// 生成纹理
	glGenTextures(1, &textureArray[textureID]);

	// 设置像素对齐格式
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap.Width, pBitmap.Height, GL_RGB, GL_UNSIGNED_BYTE, pBitmap.Buffer);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

 
}*/

void convert_obj(t3DModel *pModel, char*objmdl_name  )
{
	int i;
	
	FILE*file= fopen(objmdl_name,"wt+");
   
   int nPoints    = pModel->pObject[0].numOfVerts;
   int nTriangles = pModel->pObject[0].numOfFaces;
 //int cout;

 for (i=0; i< nPoints; i++)
{
	fprintf(file, "v %6.2f  %6.2f  %6.2f\n",
	pModel->pObject[0].pVerts[i].x,
  	pModel->pObject[0].pVerts[i].y,
	pModel->pObject[0].pVerts[i].z);
}

  for (i=0; i<nTriangles; i++)
{
	fprintf(file, "f %i  %i  %i\n", 
	pModel->pObject[0].pFaces[i].vertIndex[0]+1,
	pModel->pObject[0].pFaces[i].vertIndex[1]+1,
	pModel->pObject[0].pFaces[i].vertIndex[2]+1);
}

 fclose(file);
}

//########################################################
//########################################################
#include <GL/glut.h>
#include <stdlib.h>

void init(void) 
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_FLAT);
   
   //====================================================
   g_LoadMd2.ImportMD2(&g_3DModel, FILE_NAME, TEXTURE_NAME);		
	// 遍历所有的材质
	for(int i = 0; i < g_3DModel.numOfMaterials; i++)
	{
		// 判断是否是一个文件名
		printf("%s\n",g_3DModel.pMaterials[i].strFile);
		if(strlen(g_3DModel.pMaterials[i].strFile) > 0)
		{
			//  使用纹理文件名称来装入位图
			CreateTexture(g_Texture, g_3DModel.pMaterials[i].strFile, i);			
		}

		// 设置材质的纹理ID
		g_3DModel.pMaterials[i].texureId = i;
	}

	glEnable(GL_LIGHT0);								
	glEnable(GL_LIGHTING);								
	glEnable(GL_COLOR_MATERIAL);					

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
}

void draw_triangle(void)
{
   glBegin (GL_LINE_LOOP);
   glVertex2f(0.0, 25.0);
   glVertex2f(25.0, -25.0);
   glVertex2f(-25.0, -25.0);
   glEnd();
}

void display()//RenderScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();	
	gluLookAt(   0, 1.5f, 90,   0, 0.5f, 0,	  0, 1, 0);
	
	glRotatef(g_RotateX, 0, 1.0f, 0);
	g_RotateX += g_RotationSpeed;	


	if(g_3DModel.pObject.size() <= 0) return;

	// 获得当前显示的对象
	t3DObject *pObject = &g_3DModel.pObject[0];

	glBegin(g_ViewMode);

		// 遍历对象中所有的面
		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			// 遍历三角形中的每个顶点
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				int index = pObject->pFaces[j].vertIndex[whichVertex];

				int index2 = pObject->pFaces[j].coordIndex[whichVertex];
			
				glNormal3f(-pObject->pNormals[ index ].x, -pObject->pNormals[ index ].y, -pObject->pNormals[ index ].z);
					
				// 判断是否有纹理坐标
				//if(pObject->pTexVerts) 
				{
					glTexCoord2f(pObject->pTexVerts[ index2 ].x, pObject->pTexVerts[ index2 ].y);
				}
				
				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
			}
		}

	glEnd();

	glFlush ();									// 交换缓冲区
}


/*void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f (1.0, 1.0, 1.0);

   glLoadIdentity ();
   glColor3f (1.0, 1.0, 1.0);
   draw_triangle ();

   glEnable (GL_LINE_STIPPLE);
   glLineStipple (1, 0xF0F0);
   glLoadIdentity ();
   glTranslatef (-20.0, 0.0, 0.0);
   draw_triangle ();

   glLineStipple (1, 0xF00F);
   glLoadIdentity ();
   glScalef (1.5, 0.5, 1.0);
   draw_triangle ();

   glLineStipple (1, 0x8888);
   glLoadIdentity ();
   glRotatef (90.0, 0.0, 0.0, 1.0);
   draw_triangle ();
   glDisable (GL_LINE_STIPPLE);

   glFlush ();
}*/

void reshape (int w, int h)
{
	int width  =w;
	int height =h;
	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity();						
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,150.0f);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();	
	
  /* glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   if (w <= h)
      glOrtho (-50.0, 50.0, -50.0*(GLfloat)h/(GLfloat)w,
         50.0*(GLfloat)h/(GLfloat)w, -1.0, 1.0);
   else
      glOrtho (-50.0*(GLfloat)w/(GLfloat)h,
         50.0*(GLfloat)w/(GLfloat)h, -50.0, 50.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);*/
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
	 case 'a':								// 按下向左键
		g_RotationSpeed -= 0.05f;
        display();		
	 break;

	 case 'd':								// 按下向右键
		g_RotationSpeed += 0.05f;
		display();
	 break;
   }
}

int main(int argc, char** argv)
{
   //init();
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (800, 800); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display); 
   glutReshapeFunc(reshape);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0;
}

/*
int main(int argc, char* argv[])
{
	CLoadMD2 LMld;
	t3DModel  pModel;
	char *strFileName,   *strTexture;
    LMld.ImportMD2( &pModel, "TRIS.MD2",  "RHINO.BMP");
	printf("%i\n", pModel.pObject.size());
	
	convert_obj(&pModel, "dump.obj");
	return 1;
}*/