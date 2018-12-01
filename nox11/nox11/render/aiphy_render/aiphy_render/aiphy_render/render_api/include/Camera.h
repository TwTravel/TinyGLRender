//Camera.h
#ifndef CAMERA_H
#define CAMERA_H

#include "Node.h"

void gluLookAt(GLdouble eyex, GLdouble eyey, GLdouble eyez, GLdouble centerx,
								    GLdouble centery, GLdouble centerz, GLdouble upx, GLdouble upy,
								    GLdouble upz);

class Camera: public Node
{
 public:
  Camera(Enum CType);

  void Render();
  void SetValuev(Enum PName, float *v);
  void SetValue(Enum PName, float v1, float v2, float v3);
  void SetValue(Enum PName, float v);
  
 private:
  Enum Type;
  bool Changed[2];
  float Position[3];
  float AimAt[3];
  float UpDirection[3];
  float Near;
  float Far;
  float Height;
  float Aspect;
  float YAngle;
  
};

inline Camera::Camera(Enum CType)
{
  int i;
  
  Type=CType;
  KeepMatrix=true;

  for(i=0; i<3; i++)
    {
      if(i<2)
	Changed[i]=false;
      Position[i]=0.0;
      AimAt[i]=0.0;
      UpDirection[i]=0.0;
    }
  UpDirection[1]=1.0;
  AimAt[2]=-100.0;  

}

void inline 
Camera::SetValuev(Enum PName, float *v)
{
  float *temp=NULL;
  int i;

  switch(PName)
    {
    case POSITION:
      Changed[0]=true;
      temp=Position;
      break;
    case AIMAT:
      Changed[0]=true;
      temp=AimAt;
      break;
    case UPDIRECTION:
      Changed[0]=true;
      temp=UpDirection;
      break;
    default:
      break;
    }

  for(i=0; i<3; i++)
    temp[i]=v[i];
}

void inline 
Camera::SetValue(Enum PName, float v1, float v2, float v3)
{
  float temp[]={v1, v2, v3};
  
  SetValuev(PName, temp);
}

void inline 
Camera::SetValue(Enum PName, float v)
{
  switch(PName)
    {
    case ASPECT:
      Changed[1]=true;
      Aspect=v;
      break;
    case NEAR:
      Changed[1]=true;
      Near=v;
      break;
    case FAR:
      Changed[1]=true;
      Far=v;
      break;
    case HEIGHT:
      if(Type==ORTHO)
	{
	  Changed[1]=true;
	  Height=v;
	}
      break;
    case YANGLE:
      if(Type==PERSPECTIVE)
	{
	  Changed[1]=true;
	  YAngle=v;
	}
      break;
    default:
      break;
    }
}

void inline  glMultMatrixd( const GLdouble *m )
{
   GLint i;
   GLfloat f[16];
   if (!m) return;
   for (i = 0; i < 16; i++)
      f[i] = (GLfloat) m[i];
    glMultMatrixf( f );
}


  void inline  _TgluMakeIdentityd(GLdouble m[16])
{
    m[0+4*0] = 1; m[0+4*1] = 0; m[0+4*2] = 0; m[0+4*3] = 0;
    m[1+4*0] = 0; m[1+4*1] = 1; m[1+4*2] = 0; m[1+4*3] = 0;
    m[2+4*0] = 0; m[2+4*1] = 0; m[2+4*2] = 1; m[2+4*3] = 0;
    m[3+4*0] = 0; m[3+4*1] = 0; m[3+4*2] = 0; m[3+4*3] = 1;
}

void inline TgluPerspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	#define __glPi 3.14159265358979323846
    GLdouble m[4][4];
    double sine, cotangent, deltaZ;
    double radians = fovy / 2 * __glPi / 180;

    deltaZ = zFar - zNear;
    sine = sin(radians);
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0)) {
	return;
    }
    //cotangent = COS(radians) / sine;

    cotangent = cos(radians) / sine;

    _TgluMakeIdentityd(&m[0][0]);
    m[0][0] = cotangent / aspect;
    m[1][1] = cotangent;
    m[2][2] = -(zFar + zNear) / deltaZ;
    m[2][3] = -1;
    m[3][2] = -2 * zNear * zFar / deltaZ;
    m[3][3] = 0;
    glMultMatrixd(&m[0][0]);
}

void inline Camera::Render()
{
  printf("%s name:%s\n",(char*)nodespace.c_str(),(char*)nodename.c_str());
  float x2=Height*Aspect/2;
  float x1=(-1)*x2;
  float y2=Height/2;
  float y1=(-1)*y2;

  if(Changed[1])
    {
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
    //  if(Type==ORTHO)
	//glOrtho(x1, x2, y1, y2, Near, Far);
    //  else
	TgluPerspective(YAngle, Aspect, Near, Far);
      glMatrixMode(GL_MODELVIEW);
    }
  if(Changed[0])
    {
      glLoadIdentity();
      gluLookAt(Position[0], Position[1], Position[2], AimAt[0],
		AimAt[1], AimAt[2], UpDirection[0], UpDirection[1],
		UpDirection[2]);
    }
}
#endif
