#include <stdio.h>
#include <stdlib.h>

#include "Md2.h"


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
/**/
int main(int argc, char* argv[])
{
	CLoadMD2 LMld;
	t3DModel  pModel;
	char *strFileName,   *strTexture;
    LMld.ImportMD2( &pModel, "TRIS.MD2",  "RHINO.BMP");
	printf("%i\n", pModel.pObject.size());
	
	convert_obj(&pModel, "dump.obj");
	return 1;
}