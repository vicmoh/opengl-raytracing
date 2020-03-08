// Calculates a ray that passes through each pixel of the viewing window.
// Draws randomly coloured pixel in the viewing window.

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

// MacOS OpenGL Headers.
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include "array_map.h"
#include "dynamic_string.h"
#include "file_reader.h"
#include "parsed_file.h"
#include "point.h"
#include "splitter.h"

// Show print if debug is true.
#define DEBUG false

// Window size.
#define SIZE 512

// The parsed file at the start.
ParsedFile *_pf;

/**
 * Calculate the length vector.
 */
float length(float *x, float *y, float *z) {
  return (sqrtf((*x * *x) + (*y * *y) + (*z * *z)));
}

/**
 * Creates a unit vector. Divide a vector
 * by it's own length.
 */
void normalize(float *xd, float *yd, float *zd) {
  float len;
  len = length(xd, yd, zd);
  *xd = *xd / len;
  *yd = *yd / len;
  *zd = *zd / len;
}

/**
 * OpenGL calls this to draw the screen
 */
void display() {
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
  for (int i = 0; i < SIZE; i++) {
    for (int j = 0; j < SIZE; j++) {
      /* screen location to draw pixel */
      /* used with glVertexf() but not in calculations for vectors */
      float x = (float)(i - (SIZE / 2)) / (SIZE / 2);
      float y = (float)(j - (SIZE / 2)) / (SIZE / 2);

      /* calculate the ray direction (xd, yd, zd) */
      /* ray starts at the origin (x0, y0, z0) and ends at
         a point on the viewing window (xd, yd, zd) */
      /* screen corner coordinates are (-1,-1) to (1,1) */
      xd = ((float)(i - (SIZE / 2)) / (SIZE / 2));
      yd = ((float)(j - (SIZE / 2)) / (SIZE / 2));
      zd = -1.0;
      normalize(&xd, &yd, &zd);

      // Calculate -----------------------------------------------------------
      Point origin = new_PointOf(x0, y0, z0);

      // Calculate B, C.  A == 1.0
      float A = 1.0;
      float B =
          2 * (xd * (x0 - _pf->sphereData.x) + yd * (y0 - _pf->sphereData.y) +
               zd * (z0 - _pf->sphereData.z));
      float C = powf(x0 - _pf->sphereData.x, 2) +
                powf(y0 - _pf->sphereData.y, 2) +
                powf(z0 - _pf->sphereData.z, 2) - powf(_pf->sphereData.r, 2);

      // calculate the discriminant.
      float dis = (B * B) - (4 * A * C);
      Point ri = new_Point();
      Point ri0 = new_Point();
      Point ri1 = new_Point();
      if (DEBUG) print("______________________________");

      // Determine the intersection.
      float t0 = 0;
      float t1 = 0;
      if (dis < 1) {
        continue;
      } else if (dis == 0) {
        ri = new_PointOf((x0 + xd * t0), (y0 + yd * t0), (z0 + zd * t0));
      } else if (dis > 0) {
        t0 = (((-1.0) * B) - sqrtf(dis)) / (2 * A);
        t1 = (((-1.0) * B) + sqrtf(dis)) / (2 * A);
        ri0 = new_PointOf((x0 + xd * t0), (y0 + yd * t0), (z0 + zd * t0));
        ri1 = new_PointOf((x0 + xd * t1), (y0 + yd * t1), (z0 + zd * t1));
        // float dist1 =
        float dist0 = calcDistForIntersection(ri, ri0);
        float dist1 = calcDistForIntersection(ri, ri1);
        ri = (dist1 < dist0) ? ri1 : ri0;
        if (DEBUG) print("t0: ", _(t0), ", t1: ", _(t1));
      }
      if (DEBUG) print("ri -> ", ri.toString);

      // Calculate normal vector (nx, ny, nz) to the intersection point.
      Point n = new_PointOf((ri.x - _pf->sphereData.x) / _pf->sphereData.r,
                            (ri.y - _pf->sphereData.y) / _pf->sphereData.r,
                            (ri.z - _pf->sphereData.z) / _pf->sphereData.r);
      normalize(&(n.x), &(n.y), &(n.z));
      if (DEBUG) print("n -> ", n.toString);

      // Calculate viewing vector.
      Point v = calcDistVector(origin, ri);
      if (DEBUG) print("v -> ", v.toString);

      // calculate the light vector (lx, ly, lz).
      Point r0 = new_PointOf(x0, y0, z0);
      Point lightPoint =
          new_PointOf(_pf->lightData.x, _pf->lightData.y, _pf->lightData.z);
      normalize(&(r0.x), &(r0.y), &(r0.z));
      Point lv = calcDistVector(lightPoint, ri);
      if (DEBUG) print("lv -> ", lv.toString);

      // calculate the dot product N.L, using the
      // normal vector and the light vector.
      float nl = calcDotProduct(n, lv);
      if (DEBUG) print("nl -> ", _(nl));

      // Calculate the reflection vector (rx, ry, rz).
      Point r = calcReflectVector(nl, n, lv);
      if (DEBUG) print("r -> ", r.toString);

      // Calculate the dot product R.V, using the reflection
      // vector and the viewing vector.
      float rv = calcDotProduct(r, v);
      if (DEBUG) print("rv -> ", _(rv));

      // calculate illumination using the parameters read from
      // the file and N.L and R.V
      float ir = _pf->sphereData.ar + (_pf->sphereData.dr * nl) +
                 (_pf->sphereData.sr * rv);
      float ig = _pf->sphereData.ag + (_pf->sphereData.dg * nl) +
                 (_pf->sphereData.sg * rv);
      float ib = _pf->sphereData.ab + (_pf->sphereData.db * nl) +
                 (_pf->sphereData.sg * rv);

      /* Set the colour of the point - calculate the r,g,b
         values using ambient, diffuse, and specular light
         parameters and the dot products N.L and R.V.  */
      // Draw the point on the display window at point (x, y).
      if (DEBUG) print("red: ", _(ir), ", green: ", _(ig), ", blue: ", _(ib));
      glColor3f(ir, ig, ib);
      glVertex2f(x, y);
    }  // end loop j
  }    // end loop i

  glEnd();
  glFlush();
}

/**
 * Read the keyboard, when q is pressed, exit the program.
 * @param key of the keyboard key that is pressed.
 * @param x
 * @param y
 */
void keyboard(unsigned char key, int x, int y) {
  switch (key) {
    case 27:
    case 'q':
      exit(0);
      break;
  }
}

/* -------------------------------------------------------------------------- */
/*                      Main Function to run the program                      */
/* -------------------------------------------------------------------------- */

int main(int argc, char **argv) {
  print("Running script...\n");

  // If no argument print the feedback.
  if (argc == 0 || argv[1] == null || strcmp(argv[1], "") == 0) {
    print(
        "\nNO ARGUMENT FOUND! PLEASE SPECIFY ARGUMENT. Please read the README "
        "provided for more information.");
    print("\nScript complete.\n");
    return 0;
  }

  // Parse and print if it failed to parse.
  _pf = new_ParsedFile(argv[1]);
  if (_pf->hasError) {
    print(
        "\nCould not parse the file. Please make sure it is in the correct"
        "format. File path might be incorrect or does not exist.");
    print("\nScript complete.\n");
    return 0;
  }

  // Initialize OpenGL and GLUT
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowSize(SIZE, SIZE);
  glutCreateWindow("2D Test");
  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutMainLoop();
  return 0;
}
