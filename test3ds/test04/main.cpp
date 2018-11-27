#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>										
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>
#include "3DS.h"
#include "c24bitmap.h"

#define FILE_NAME  "FACE.3DS"

typedef unsigned int UINT;
unsigned int g_Texture[MAX_TEXTURES] = {0};

CLoad3DS g_Load3ds;									
t3DModel g_3DModel;									
								

int   g_ViewMode	  = GL_TRIANGLES;			
bool  g_bLighting     = true;				
float g_RotateX		  = 0.0f;				
float g_RotationSpeed = 0.5f;
C24BitMap pBitmap;

//  从文件中创建纹理
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
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
	//	GL_BGR_EXT, GL_UNSIGNED_BYTE, pixels);
	glTexImage2D(GL_TEXTURE_2D,0,3,
		pBitmap.Width, pBitmap.Height,0,
		GL_RGB, GL_UNSIGNED_BYTE, pBitmap.Buffer);
		//GL_BGR_EXT,GL_UNSIGNED_BYTE, pBitmap.Buffer);

	/*if (pBitmap)										// 释放位图占用的资源
	{
		if (pBitmap->data)						
		{
			free(pBitmap->data);				
		}

		free(pBitmap);					
	}*/
}

void RenderScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();									

	gluLookAt(		0, 1.5f, 8,		0, 0.5f, 0,			0, 1, 0);
	
	glRotatef(g_RotateX, 0, 1.0f, 0);
	g_RotateX += g_RotationSpeed;	

	// 遍历模型中所有的对象
	for(int i = 0; i < g_3DModel.numOfObjects; i++)
	{
		// 如果对象的大小小于0，则退出
		if(g_3DModel.pObject.size() <= 0) break;

		// 获得当前显示的对象
		t3DObject *pObject = &g_3DModel.pObject[i];
			
		// 判断该对象是否有纹理映射
		if(pObject->bHasTexture) {

			// 打开纹理映射
			glEnable(GL_TEXTURE_2D);
			glColor3f(1.0, 1.0, 1.0);
			printf("matid:%i\n",pObject->materialID);
			glBindTexture(GL_TEXTURE_2D, g_Texture[pObject->materialID]);
		} else {

			// 关闭纹理映射
			glDisable(GL_TEXTURE_2D);
			glColor3f(1.0, 1.0, 1.0);
		}
		// 开始以g_ViewMode模式绘制
		glBegin(g_ViewMode);					
			// 遍历所有的面
			for(int j = 0; j < pObject->numOfFaces; j++)
			{
				// 遍历三角形的所有点
				for(int whichVertex = 0; whichVertex < 3; whichVertex++)
				{
					// 获得面对每个点的索引
					int index = pObject->pFaces[j].vertIndex[whichVertex];
			
					// 给出法向量
					glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);
				
					// 如果对象具有纹理
					if(pObject->bHasTexture) {

						// 确定是否有UVW纹理坐标
						if(pObject->pTexVerts) {
							glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
						}
					} else {

						if(g_3DModel.pMaterials.size() && pObject->materialID >= 0) 
						{
							BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;
							glColor3f(float(pColor[0])/255.0, float(pColor[1])/255.0, float(pColor[2])/255.0);
						}
					}
					glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
				}
			}

		glEnd();								// 绘制结束
	}

	//SwapBuffers(g_hDC);									// 交换缓冲区
	glFlush();
    //glutSwapBuffers();
}

void reshape(int width, int height)		
{
	if (height==0)										
	{
		height=1;										
	}

	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity();						
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height, .5f ,150.0f);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();									
}

#define WindowWidth  600
#define WindowHeight 600
#define WindowTitle  "OpenGL纹理测试"

//void model_init()
void init()
{

	g_Load3ds.Import3DS(&g_3DModel, FILE_NAME);			// 将3ds文件装入到模型结构体中

	// 遍历所有的材质
	for(int i = 0; i < g_3DModel.numOfMaterials; i++)
	{
		// 判断是否是一个文件名
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

}
int ui_loop(int argc, char **argv, const char *name);
void tkSwapBuffers(void);

int main(int argc,char* argv[])
{
	ui_loop(argc, argv, "models");
  	// GLUT初始化
	/*glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WindowTitle);*/	
	glEnable(GL_DEPTH_TEST);    
	glEnable(GL_TEXTURE_2D);    // 启用纹理
	//model_init();
	//init();
	reshape(WindowWidth, WindowHeight);
	//texGround = load_texture("ground.bmp");  //加载纹理
	//texWall = load_texture("wall.bmp");
	//glutDisplayFunc(&RenderScene);   //注册函数 
	//glutIdleFunc(&myIdle);  
	//glutMainLoop(); //循环调用
	RenderScene();
     tkSwapBuffers();
    return 1;
}
