/* Starting code for assignment 3.
   Calculates a ray that passes through each pixel of the viewing window.
   Draws randomly coloured pixel in the viewing window. */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Linux OpenGL Headers
/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

// MacOS OpenGL Headers
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "dynamic_string.h"
#include "file_reader.h"
#include "splitter.h"
#include "array_map.h"

// window size
#define SIZE 512

typedef struct {
  float x, y, z;
} Light;

typedef struct {
  float x, y, z;
  float ar, ag, ab;
  float dr, dg, db;
  float sr, sg, sb;
  float r;
} Sphere;

typedef struct {
  float x, y, z;
} Point;

Light lightData;
Sphere sphereData;

bool parseFile(String filePath) {
  print("Parsing files");
  FileReader *fr = new_FileReader(filePath);
  String lightString = FileReader_getLineAt(fr, 0);
  String sphereString = FileReader_getLineAt(fr, 1);
  print("light string: ", lightString);
  print("sphere string: ", sphereString);
  Splitter *lightSplits = new_Splitter(lightString, " \t\n");
  Splitter *sphereSplits = new_Splitter(sphereString, " \t\n");
  if (fr == null) return false;

  int next = 0;
  print("light splits length: ", _(lightSplits->length));
  print("sphere splits length: ", _(sphereSplits->length));
  for_in(next, lightSplits) print("light data: ", lightSplits->list[next]);
  for_in(next, sphereSplits) print("sphere data: ", sphereSplits->list[next]);

  // init light data
  lightData.x = atof(lightSplits->list[1]);
  lightData.y = atof(lightSplits->list[2]);
  lightData.z = atof(lightSplits->list[3]);
  // init sphere split
  sphereData.x = atof(sphereSplits->list[1]);
  sphereData.y = atof(sphereSplits->list[2]);
  sphereData.z = atof(sphereSplits->list[3]);
  sphereData.r = atof(sphereSplits->list[4]);
  // init the a for sphere.
  sphereData.ar = atof(sphereSplits->list[6]);
  sphereData.ag = atof(sphereSplits->list[7]);
  sphereData.ab = atof(sphereSplits->list[8]);
  // init the d for sphere.
  sphereData.dr = atof(sphereSplits->list[9]);
  sphereData.dg = atof(sphereSplits->list[10]);
  sphereData.db = atof(sphereSplits->list[11]);
  // init the s for sphere
  sphereData.sr = atof(sphereSplits->list[12]);
  sphereData.sg = atof(sphereSplits->list[13]);
  sphereData.sb = atof(sphereSplits->list[14]);

  // free objects
  dispose(lightString, sphereString);
  free_Splitter(lightSplits);
  free_Splitter(sphereSplits);
  free_FileReader(fr);
  return true;
}

/* calculate the length of a vector */
float length(float *x, float *y, float *z) {
  return (sqrtf((*x * *x) + (*y * *y) + (*z * *z)));
}

/* creates a unit vector */
/* divide a vector by its own length */
void normalize(float *xd, float *yd, float *zd) {
  float len;
  len = length(xd, yd, zd);
  *xd = *xd / len;
  *yd = *yd / len;
  *zd = *zd / len;
}

Point calcViewingAngle(Point r0, Point ri) {
  /*.   V = ro - ri
          = (vx vy vz)
          = (x0-xi  y0-yi  z0-zi) */
  Point v;
  v.x = r0.x - ri.x;
  v.y = r0.y - ri.x;
  v.z = r0.z - ri.z;
  normalize(&(v.x), &(v.y), &(v.z));
  return v;
}

Point calcLightVector(Point l0, Point ri) {
  // L = l0 - ri
  //   = (lvx  lvy  lvz)
  //   = (lx-xi  ly-yi  lz-zi)
  Point l;
  l.x = l0.x - ri.x;
  l.y = l0.y - ri.y;
  l.z = l0.z - ri.z;
  normalize(&(l.x), &(l.y), &(l.z));
  return l;
}

float calcDotProduct(Point n, Point l) {
  // N.L = ( nx*lvx +  ny*lvy +  nz*lvz)
  return (n.x * l.x + n.y * l.y + n.z * l.z);
}

Point calcReflectVector(float nl, Point n, Point l) {
  // R = 2 * (N.L) * N - L
  //  = (rx ry rz)
  Point r;
  r.x = 2 * (nl)*n.x - l.x;
  r.y = 2 * (nl)*n.y - l.y;
  r.z = 2 * (nl)*n.z - l.z;
  normalize(&(r.x), &(r.y), &(r.z));
  return r;
}

/* OoenGL calls this to draw the screen */
void display() {
  int i, j;
  float x, y;
  float r, g, b;

  float x0, y0, z0;  // viewpoint origin
  float xd, yd, zd;  // view ray direction

  glClearColor(0.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glPointSize(1.0);

  /* ray tracing code starts here */
  /* set viewer position (x0, y0, z0) */
  x0 = 0.0;
  y0 = 0.0;
  z0 = 0.0;

  /* draw individual pixels */
  glBegin(GL_POINTS);

  /* nested loops move over the viewing window */
  /* i and j move through the x and y coordinates respectively */
  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      /* screen location to draw pixel */
      /* used with glVertexf() but not in calculations for vectors */
      x = (float)(i - (SIZE / 2)) / (SIZE / 2);
      y = (float)(j - (SIZE / 2)) / (SIZE / 2);

      /* calculate the ray direction (xd, yd, zd) */
      /* ray starts at the origin (x0, y0, z0) and ends at
         a point on the viewing window (xd, yd, zd) */
      /* screen corner coordinates are (-1,-1) to (1,1) */
      xd = ((float)(i - (SIZE / 2)) / (SIZE / 2));
      yd = ((float)(j - (SIZE / 2)) / (SIZE / 2));
      zd = -1.0;
      normalize(&xd, &yd, &zd);

      /* ray tracing code */
      /* your code starts here */

      /* calculate B, C.  A == 1.0 */
      float A = 1.0;
      float B = 2 * (xd * (x0 - sphereData.x) + yd * (y0 - sphereData.y) +
                     zd * (z0 - sphereData.z));
      float C = pow((x0 - sphereData.x), 2) + pow((y0 - sphereData.y), 2) +
                pow((z0 - sphereData.z), 2) - (sphereData.r);

      /* calculate the discriminant */
      float dis = pow(B, 2) - 4 * A * C;

      float t0 = ((-1.0) * B - sqrtf(dis)) / (2 * A);
      float t1 = ((-1.0) * B + sqrtf(dis)) / (2 * A);
      Point ri;
      Point ri0;
      Point ri1;

      /* if there is one intersection point (discriminant == 0)
         then calculate intersection of ray and sphere at (xi, yi, zi) */
      if (dis == 0) {
        // float  ri = (xi yi, zi) = ((x0 + xd * t0)(y0 + yd * t0)(z0 + zd *
        // t0));
        ri.x = (x0 + xd * t0);
        ri.y = (y0 + yd * t0);
        ri.z = (z0 + zd * t0);
      }

      /* if there are two two intersection points (discriminant > 0)
         then determine which point is closer to the viewpoint (x0, y0, z0)
         and calculate the intersection of point (xi, yi, zi) */
      // if (dis > 0) {
      //   // ri0 = ( (x0 + xd*t0)    (y0 + yd*t0)    (z0 + zd*t0) )
      //   // ri1 = ( (x0 + xd*t1)    (y0 + yd*t1)    (z0 + zd*t1) )
      //   // First intersection
      //   ri0.x = (x0 + xd * t0);
      //   ri0.y = (y0 + yd * t0);
      //   ri0.z = (z0 + zd * t0);
      //   //  Second intersection
      //   ri1.x = (x0 + xd * t1);
      //   ri1.y = (y0 + yd * t1);
      //   ri1.z = (z0 + zd * t1);
      // }

      /* calculate normal vector (nx, ny, nz) to the intersection point */
      //    N = (nx ny nz)  =  ( (xi - xc)/Sr    (yi - yc)/Sr    (zi - zc)/Sr  )
      Point n;
      n.x = (ri.x - sphereData.x) / sphereData.r;
      n.y = (ri.y - sphereData.y) / sphereData.r;
      n.z = (ri.z - sphereData.z) / sphereData.r;
      normalize(&(n.x), &(n.y), &(n.z));

      /* calculate viewing vector (vx, vy, vz) */
      /*.   V = ro - ri
              = (vx vy vz)
              = (x0-xi  y0-yi  z0-zi) */
      Point v = calcViewingAngle(ri0, ri1);

      /* calculate the light vector (lx, ly, lz)  */
      Point r0 = {.x = x0, .y = y0, .z = z0};
      normalize(&(r0.x), &(r0.y), &(r0.z));
      Point lv = calcLightVector(n, r0);

      /* calculate the dot product N.L, using the normal vector
         and the light vector */
      float nl = calcDotProduct(n, lv);

      /* calculate the reflection vector (rx, ry, rz) */
      Point r = calcReflectVector(nl, n, lv);

      /* calculate the dot product R.V, using the reflection
         vector and the viewing vector */
      float rv = calcDotProduct(r, v);

      /* calculate illumination using the parameters read from
         the file and N.L and R.V  */
      float ir = sphereData.ar + sphereData.dr * nl + sphereData.sr * rv;
      float ig = sphereData.ag + sphereData.dg * nl + sphereData.sg * rv;
      float ib = sphereData.ab + sphereData.db * nl + sphereData.sg * rv;

      /* set the colour of the point - calculate the r,g,b
         values using ambient, diffuse, and specular light
         parameters and the dot products N.L and R.V  */

      /* end of ray tracing code */

      /* replace the following three lines with the
         illumination calculations */

      glColor3f(ir, ib, ig);
      // print("red: ", _(ir), ", green: ", _(ig), ", blue: ", _(ib));
      /* draw the point on the display window at point (x, y) */
      glVertex2f(x, y);

    }  // end loop j
  }    // end loop i

  glEnd();
  glFlush();
}

/* read the keyboard, when q pressed exit the program */
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27:
    case 'q':
      exit(0);
      break;
  }
}

int main(int argc, char **argv) {
  print("Running script...\n");
  // Check if argument exist.

  if (argc == 0 || argv[1] == null || strcmp(argv[1], "") == 0) {
    print(
        "NO ARGUMENT FOUND! PLEASE SPECIFY ARGUMENT. Please read the README "
        "provided for more information.\n");
    print("\nScript complete.\n");
    return 0;
  }

  /* read input file */
  bool parseRes = parseFile(argv[1]);
  if (!parseRes) {
    print(
        "Could not parse the file. Please make sure it is in the correct"
        "format. File path might be incorrect or does not exist.");
    print("\nScript complete.\n");
    return 0;
  }

  /* Initialize OpenGL and GLUT */
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(SIZE, SIZE);
  glutCreateWindow("2D Test");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);

  glutMainLoop();
  return 0;
}
