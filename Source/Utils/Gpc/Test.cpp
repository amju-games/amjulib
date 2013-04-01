/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Test.cpp,v $
Revision 1.2  2008/05/08 10:58:08  jay
New memory management code

Revision 1.1  2004/09/08 15:43:21  jay
Added to repository
  
*/

// Test.cpp
// Test the polygon clipping library.

extern "C"
{
#include <AmjuFirst.h>
#include "gpc.h"
#include <AmjuFinal.h>
}

int main(int, char**)
{
  AMJU_CALL_STACK;

  // First poly: a square
  gpc_vertex v1[5]; // close the poly ?
  v1[0].x = 1;
  v1[0].y = 1;
  v1[1].x = -1;
  v1[1].y = 1;
  v1[2].x = -1;
  v1[2].y = -1;
  v1[3].x = 1;
  v1[3].y = -1;
  v1[4].x = 1;
  v1[4].y = 1;


  gpc_vertex_list vl1;
  vl1.num_vertices = 5;
  vl1.vertex = v1;

  gpc_polygon p1;
  p1.num_contours = 1;
  p1.contour = &vl1;
  int h1 = 0;
  p1.hole = &h1;


  // Second poly: 
  gpc_vertex v2[5];
  v2[0].x = 3;
  v2[0].y = 1.5;
  v2[1].x = 0.5;
  v2[1].y = 1.5;
  v2[2].x = 0.5;
  v2[2].y = -0.75;
  v2[3].x = 3;
  v2[3].y = -0.75;
  v2[4].x = 3;
  v2[4].y = 1.5;

  gpc_vertex_list vl2;
  vl2.num_vertices = 5;
  vl2.vertex = v2;

  gpc_polygon p2;
  p2.num_contours = 1;
  p2.contour = &vl2;
  int h2 = 0;
  p2.hole = &h2;

  // Get intersection
  gpc_polygon result;

  gpc_polygon_clip(GPC_INT, &p1, &p2, &result);

  printf("Contours: %d\n", result.num_contours);
  for (int i = 0; i < result.num_contours; i++)
  {
      printf("Contour %d\n", i);
      gpc_vertex_list& vlist = result.contour[i];

      printf("Vertices: %d\n", vlist.num_vertices);
      for (int j = 0; j < vlist.num_vertices; j++)
      {
          gpc_vertex& v = vlist.vertex[j];
          printf("(%f, %f)\n", v.x, v.y);
      }
  }
  return 0;

}
