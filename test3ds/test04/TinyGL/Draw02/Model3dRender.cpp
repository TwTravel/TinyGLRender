#include <stdio.h>
#include <stdlib.h>
#include "read_cfg.h"
#include "Scene.h"

 string trim(const string& str)
{
    string::size_type pos = str.find_first_not_of(' ');
    if (pos == string::npos)
    {
        return str;
    }
    string::size_type pos2 = str.find_last_not_of(' ');
    if (pos2 != string::npos)
    {
        return str.substr(pos, pos2 - pos + 1);
    }
    return str.substr(pos);
}

int ui_loop(int argc, char **argv, const char *name);

void init( void )
{
   static GLfloat pos[4] = {5.0, 5.0, 10.0, 0.0 };
   static GLfloat red[4] = {0.8, 0.1, 0.0, 1.0 };
   static GLfloat green[4] = {0.0, 0.8, 0.2, 1.0 };
   static GLfloat blue[4] = {0.2, 0.2, 1.0, 1.0 };

   glLightfv( GL_LIGHT0, GL_POSITION, pos );
   glEnable( GL_CULL_FACE );
   glEnable( GL_LIGHTING );
   glEnable( GL_LIGHT0 );
   glEnable( GL_DEPTH_TEST );

   /* make the gears */
   /*gear1 = glGenLists(1);
   glNewList(gear1, GL_COMPILE);
   glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, red );
   gear( 1.0, 4.0, 1.0, 20, 0.7 );
   glEndList();

   gear2 = glGenLists(1);
   glNewList(gear2, GL_COMPILE);
   glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, green );
   gear( 0.5, 2.0, 2.0, 10, 0.7 );
   glEndList();

   gear3 = glGenLists(1);
   glNewList(gear3, GL_COMPILE);
   glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, blue );
   gear( 1.3, 2.0, 0.5, 10, 0.7 );
   glEndList();*/

   glEnable( GL_NORMALIZE );
}

/* new window size or exposure */
void reshape( int width, int height )
{
   GLfloat  h = (GLfloat) height / (GLfloat) width;

   glViewport(0, 0, (GLint)width, (GLint)height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum( -1.0, 1.0, -h, h, 5.0, 60.0 );
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glTranslatef( 0.0, 0.0, -40.0 );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void tkSwapBuffers(void);

class VWordModel
{
  public:
  VWordModel();
  ~VWordModel();
  Camera *Camera1 ;
  Transformation *SysTrans  ;
  Light *Light1 ;
  vector< StlShape *      > StlElements;// = new StlShape[ element_num ];
  vector< Transformation *> StlTrans;//    = new Transformation[ element_num ];
  Polygon  *AxisXX  ;                              
  Polygon  *AxisYY  ;                              
  Polygon  *AxisZZ  ;                              
  Node *Root ;
  GLViewer *MyViewer ;
  TextureSurface* groundface  ;
};

VWordModel::VWordModel()
{
  Camera1  = new Camera(PERSPECTIVE);
  SysTrans = new Transformation;
  Light1   = new Light;
  //StlShape       *StlElements = new StlShape[ element_num ];
  //Transformation *StlTrans    = new Transformation[ element_num ];
  AxisXX = new Polygon;  AxisXX->nodename ="AxisXX = new Polygon";                             
  AxisYY = new Polygon;  AxisYY->nodename ="AxisYY = new Polygon";                             
  AxisZZ = new Polygon;  AxisZZ->nodename ="AxisZZ = new Polygon";                             
  Root   = new Node;     Root  ->nodename ="Root";
  MyViewer = new GLViewer;// MyViewer ->nodename = "Viewer";
  groundface = new TextureSurface;
}

VWordModel::~VWordModel()
{
	return;
}

VWordModel world_model;

void ReadModelFile(int argc, char *argv[])
{
	 printf("hello\n");
  std::vector<std::string> lines;
  readlistfile(argv[1],  lines); 
  /*CAMERA_POS, 2.2, 0.9, 3, 50 , #posx posy posz angle
   SYSROTATION, -90,  1, 0 ,0, 0
   SYSTRANSLTN, -0.5, 0, 0, 2, 0
bunny.stl, 10.0, 1.0, 1.0, 0, 0, 0, 0, 0, 0, 0, 4 , #OBJ(color3)(pos3)(rot4)(scale1)
bunny.stl, 40.0, 1.0, 1.0, -10, 0, 0, 0, 0, 0, 0, 2,#OBJ(color3)(pos3)(rot4)(scale1)  
bunny.stl, 50.0, 1.0, 1.0, 10, 0, 0, 0, 0, 0,  0,  3,#OBJ(color3)(pos3)(rot4)(scale1)   
  */
    float vx[][3]={{0,  -0.02, 0.0},{100, -0.02, 0.0},
		         {100, 0.02, 0.0},{  0,  0.02, 0.0} };

    float vy[][3]={ {-0.02,  0, 0.0 } ,{  0.02,   0, 0.0}, 
	                {0.02 ,100, 0.0},  {-0.02, 100, 0.0}};		
		
    float vz[][3]={{-0.02,  0, 0.0},{ -0.02,  0, 100.0}, 
                 	{0.02 ,0, 100.0},{  0.02,   0, 0.0} };	
  
   int i;
   double  camx, camy, camz, campov;
   double  rot_angle,rot_x,rot_y,rot_z;
   double  trans_x,trans_y,trans_z;
   
   std::vector<std::string> strvec;
 
//##################################################################
//################################################################## 
   strvec.clear(); splitString(lines[0], strvec, ",");
   
   camx   = atof( trim(strvec[1]).c_str());
   camy   = atof( trim(strvec[2]).c_str() );
   camz   = atof( trim(strvec[3]).c_str() );
   campov = atof( trim(strvec[4]).c_str() );  
   
   
   
   world_model.Camera1->SetValue(POSITION, camx, camy, camz);
   world_model.Camera1->SetValue(AIMAT, 0, 0, 0);
   world_model.Camera1->SetValue(UPDIRECTION, 0, 1, 0);
   world_model.Camera1->SetValue(ASPECT, 1);
   world_model.Camera1->SetValue(NEAR, 0.1);
   world_model.Camera1->SetValue(FAR, 200);
   world_model.Camera1->SetValue(YANGLE, campov); 
   world_model.Camera1->nodename="Camera1";
   
//##################################################################
//##################################################################  
   strvec.clear(); splitString(lines[1], strvec, ","); 
   rot_angle   = atof( trim(strvec[1]).c_str() );
   rot_x       = atof( trim(strvec[2]).c_str() );
   rot_y       = atof( trim(strvec[3]).c_str() );
   rot_z       = atof( trim(strvec[4]).c_str() ); 
   strvec.clear(); splitString(lines[2], strvec, ","); 
   trans_x = atof( trim(strvec[1]).c_str() );
   trans_y = atof( trim(strvec[2]).c_str() );
   trans_z = atof( trim(strvec[3]).c_str() );
   
  
   
   world_model.SysTrans->SetValue(ROTATION, rot_angle,rot_x, rot_y, rot_z,0);
   world_model.SysTrans->SetValue(TRANSLATION, trans_x,  trans_y, trans_z, 2);
//##################################################################
//##################################################################  
   int element_num = lines.size() - 3 ;
   world_model.StlElements.resize(element_num); 
      Loopi(element_num) world_model.StlElements[i] = new StlShape;
   world_model.StlTrans.resize(element_num);    
       Loopi(element_num) world_model.StlTrans[i]    = new Transformation;
     //StlShape       *StlElements = new StlShape[ element_num ];
  //Transformation *StlTrans    = new Transformation[ element_num ];
   
   
   world_model.Light1->SetValue(POSITION,  2, -1,  1.8, 1);
   world_model.Light1->SetValue(SPOT_DIRECTION, -2, 1, -1.8);
   world_model.Light1->SetValue(CUTOFFANGLE, 180.0);
   world_model.Light1->TurnOn();     world_model.Light1->nodename="Light1";
  

  
   world_model.AxisXX->SetVerticesv(vx, 4); world_model.AxisXX->SetMaterial(GetColorMat(360*0.0/8.0, 1.0, 1.0 ));  
   world_model.AxisYY->SetVerticesv(vy, 4); world_model.AxisYY->SetMaterial(GetColorMat(360*4.0/8.0, 1.0, 1.0 ));
   world_model.AxisZZ->SetVerticesv(vz, 4); world_model.AxisZZ->SetMaterial(GetColorMat(360*6.0/8.0, 1.0, 1.0 ));
   
   //===================================================================================
  
   
   float vground[][3]={{0,  -0.5, -0.8},{5,  -0.5,  -0.8},
                      {5,  0.5, -0.8},{  0,  0.5,  -0.8} };
     
  /* float vground[][3]={{-200,  200, 0.0},{-200, -200, 0.0},
                    { 200,  -200, 0.0},{ 200, 200, 0.0} };*/
   world_model.groundface->SetVerticesv(vground,4);
   // groundface->SetVerticesv(vx, 4);
   world_model.groundface->SetMaterial(GetColorMat(360*0.0/8.0, 1.0, 1.0 ));
   //====================================================================================
  
   for( i = 0; i <  element_num; i++ )
   {
	   strvec.clear(); splitString(lines[3+i], strvec, ",");
	   
	   double  mat_h, mat_s, mat_v;
	   double  obj_x, obj_y, obj_z;
       double  rot_angle, rot_x, rot_y, rot_z , obj_scale;	   
	   string  fname;
	   fname = strvec[0];
	   mat_h = atof(trim(strvec[1]).c_str());
	   mat_s = atof(trim(strvec[2]).c_str());
	   mat_v = atof(trim(strvec[3]).c_str());
	   
	   obj_x = atof(trim(strvec[4]).c_str());
	   obj_y = atof(trim(strvec[5]).c_str());
	   obj_z = atof(trim(strvec[6]).c_str());
	   
	   rot_angle =  atof(trim(strvec[ 7]).c_str());
	   rot_x     =  atof(trim(strvec[ 8]).c_str());
	   rot_y     =  atof(trim(strvec[ 9]).c_str());
	   rot_z     =  atof(trim(strvec[10]).c_str());
	   obj_scale =  atof(trim(strvec[11]).c_str()); 
	   
	   printf("##########################trans: %.2f,  %.2f,  %.2f\n", obj_x ,    obj_y,  obj_z);
	   //StlTrans[i].SetValue(ROTATION    , rot_angle, rot_x,  rot_y, rot_z, 0);
	   world_model.StlTrans[i]->SetValue(TRANSLATION , obj_x ,    obj_y,  obj_z  , 1);
	   world_model.StlTrans[i]->SetValue(SCALE       , obj_scale, obj_scale, obj_scale,2); 
	   
	   world_model.StlElements[i]->SetTransform( world_model.StlTrans[i]);
     world_model.StlElements[i]->LoadStl((char*)trim(fname).c_str());
     world_model.StlElements[i]->SetMaterial(GetColorMat(mat_h, mat_s, mat_v ));  //GetColorMat(360*0.0/8.0, 1.0, 1.0 ));//
       
	   world_model.SysTrans->AddChild( world_model.StlElements[i]);
	   printf( "%s\n", trim(lines[i]).c_str() );
   }
  
  world_model.Light1->AddChild( world_model.AxisXX);
  world_model.Light1->AddChild( world_model.AxisYY);
  world_model.Light1->AddChild( world_model.AxisZZ);
  world_model.Light1->AddChild( world_model.groundface);
   
  
  
  world_model.Root->AddChild( world_model.SysTrans);
  world_model.SysTrans->AddChild( world_model.Light1);
  
  //Root->AddChild(Light1);
  //Light1->AddChild(SysTrans);
  world_model.Root->AddChild( world_model.Camera1);  world_model.Root ->nodename="root node";
  
  //Viewer:
}
int main(int argc, char *argv[])
{
  ui_loop(argc, argv, "models");
  
  ReadModelFile( argc,  argv );
  world_model.MyViewer->Init(argc, argv);
  world_model.MyViewer->SetValue(BACKCOLOR, GREY);
  world_model.MyViewer->SetValue(BUFFER, DOUBLE);
  world_model.MyViewer->CreateWin("Working Hard", 800, 800);
   
  world_model.MyViewer->Show( world_model.Root);
  world_model.MyViewer->Display0();
  tkSwapBuffers();
  return 1;
}
