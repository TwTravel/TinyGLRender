#include "Python.h"
#include "c24bitmap.h"
#include "c256bitmap.h"
#include <vector>
using namespace std;
#include "mregion.h"
#include "region.h"
#include <numpy/arrayobject.h>

#define ColorDist(R1,G1,B1,R2,G2,B2)  ((R1-R2)*(R1-R2) + (G1-G2)*(G1-G2) + (B1-B2)*(B1-B2))
#include "Model3dRender.h"

void TransGray(C24BitMap&Sr,C256BitMap&Ds)
{
	IJT;
	Ds.FormatF(Sr.Width,Sr.Height);
	LoopPic_ij(Ds){
		C24PixVal Tmp=get_pix_color(Sr,i,j);
    int diff = ColorDist( (*Tmp.r), (*Tmp.g) ,(*Tmp.b) ,
                           255    ,         0,      0  );
    if(diff > 30*30)
      *get_pix_color(Ds,i,j) = 255;
    else
      *get_pix_color(Ds,i,j) = 0;
		//  *get_pix_color(Ds,i,j) = float(*Tmp.r);
		//	float(*Tmp.r)*.30+float(*Tmp.g)*.59+float(*Tmp.b)*.11;
  }
}

void GetBlackPicRegion(C256BitMap &Pic,C24BitMap&CPic,vector<ObjRegion>&RegionVec)
{
	int i,j;
	int ww,hh;
	ww = Pic.Width;
	hh = Pic.Height;
	ConnectedComponents Cp;
		
	Cp.scan_width  = Pic.LineWidth;
	Cp.scan_height = Pic.Height;
	
	Cp.left  = 0;
	Cp.right = ww-1;
	Cp.top   = 0;
	Cp.bottom= hh-1; 
	Cp.pix_threshold =128;		
	Cp.alloc_space();
	
	//Cp.label_image(Pic.Buffer,1);
	Cp.label_image(Pic.Buffer,1);
	int obnum=Cp.relabel_image(); 
	Cp.GetResultGroup(); 	 
	CPic.FormatF(Pic.Width,Pic.Height);
	CPic.CleanPic(0);
	
	RegionVec.clear();
	RegionVec.resize(Cp.ResultVec.size());
	
	for( i=0;i<Cp.ResultVec.size();i++)
	{
		if(Cp.ResultVec[i].PtVec.size()< 10 )
			continue;
		
		CPic.RandPenColor();
		Loopj(Cp.ResultVec[i].PtVec.size())
		{
			CPic.SigDot(Cp.ResultVec[i].PtVec[j].x,
				CPic.Height-1-Cp.ResultVec[i].PtVec[j].y);
			
			RPoint Pt;
			Pt.x = Cp.ResultVec[i].PtVec[j].x;
			Pt.y = CPic.Height-1-Cp.ResultVec[i].PtVec[j].y;
			RegionVec[i].PtVec.push_back(Pt);
		}
	}

}

/* Call double avg(double *, int) */
static PyObject *py_imgcpt(PyObject *self, PyObject *args) {
  PyObject *bufobj;
  Py_buffer view;
  double result;
  /* Get the passed Python object */
  if (!PyArg_ParseTuple(args, "O", &bufobj)) {
    return NULL;
  }

  /* Attempt to extract buffer information from it */
  if (PyObject_GetBuffer(bufobj, &view, PyBUF_ANY_CONTIGUOUS | PyBUF_FORMAT) == -1) {
    return NULL;
  }

  printf("========================\n",view.ndim);
  printf("the opencv obj's dimensions are %i\n",view.ndim);
  printf("========================\n",view.ndim);

  printf("============ok0=============\n");
  printf(" shape:== %d, %d, %d =\n",    view.shape[0], view.shape[1], view.shape[2]);
  printf("stride:== %d, %d, %d =\n",  view.strides[0], view.strides[1], view.strides[2]);
  printf("====data format:%s=====\n",view.format);
  printf("============ok1=============\n");
  
  C24BitMap CPic;
  CPic.FormatF(view.shape[1], view.shape[0]);
  BYTE* ImgPtr = (BYTE*)view.buf;
 
  int i,j;

  Loopi(view.shape[0])
    { 
      BYTE * LineStart = CPic.GetRowStartPt(i);
      BYTE * DataSrc   = &ImgPtr[i*view.strides[0]];
     Loopj(view.shape[1]*3)
     {
       LineStart[j] =  DataSrc[j];
     }/* */
   }
   
   C256BitMap GrayPic;
   TransGray(CPic, GrayPic);
   GrayPic.Save("gray.bmp");

//=================================================================================
   vector<ObjRegion> RegionVec;
   C24BitMap CPicObjs;
   GetBlackPicRegion(GrayPic, CPicObjs ,  RegionVec);
  
   CPicObjs.Save("target_objs.bmp");
  
 


  /* Pass the raw buffer and size to the C function */
  result = 10;//imgcpt(view.buf, view.shape[0]);

  /* Indicate we're done working with the buffer */
  PyBuffer_Release(&view);
  return Py_BuildValue("d", result);
}

int Render3dModel(int argc, char* argv[] );

static PyObject* py_Render3DObj(PyObject* self, PyObject* args) {
    
	Render3dModel(0, NULL );
	printf("rendering finish\n" );
    Py_RETURN_NONE;
}

//static PyObject *py_imgcpt(PyObject *self, PyObject *args) {
static PyObject* py_setModelParam(PyObject* self, PyObject* args) {
   /* int iNum; double fNum;  char* str;
    int iNum2 = 4;  double fNum2 = 5.0;  char *str2 = "hello";
	*/
	char* file_stl;
	double colorH,  colorS,   colorV,  posX,   posY,   posZ,
	  rotAngle,   rotX,   rotY,  rotZ,  model_scale;
	printf("start loading\n");
   /* if (!PyArg_ParseTuple(args, "ids|ids", &iNum, &fNum, &str,&iNum2, &fNum2, &str2)) {
        return NULL;
    }*/
	
	 if (!PyArg_ParseTuple(args, "sddddddddddd", &file_stl, &colorH,  &colorS,   &colorV,  &posX,   &posY,   &posZ,
	                             &rotAngle,  &rotX,  &rotY, &rotZ, &model_scale )) 
	{
        return NULL;
    }
	
	AddStlModel( file_stl, colorH, colorS,  colorV, posX,  posY,  posZ,
	                         rotAngle,   rotX,   rotY,  rotZ,  model_scale);
	printf("%s loaded\n", file_stl);
	//Render3dModel(0, NULL);
	//printf("%i,%lf, %s (optional), %i,%lf, %s\n", iNum, fNum, str, iNum2, fNum2, str2);
    Py_RETURN_NONE;
}

/* Module method table */
static PyMethodDef SampleMethods[] = {
  {"imgcpt",  py_imgcpt, METH_VARARGS, "cpp_ext_calculation"},
  {"modelset",  py_setModelParam, METH_VARARGS, "cpp_ext_calculation"},
  {"modelrender", py_Render3DObj, METH_VARARGS, "cpp_ext_calculation"},
  { NULL, NULL, 0, NULL}
};

/* Module structure */
static struct PyModuleDef samplemodule = {
  PyModuleDef_HEAD_INIT,
  "sample1",           /* name of module */
  "A sample module",  /* Doc string (may be NULL) */
  -1,                 /* Size of per-interpreter state or -1 */
  SampleMethods       /* Method table */
};

/* Module initialization function */
PyMODINIT_FUNC
PyInit_sample1(void) {
  return PyModule_Create(&samplemodule);
}
