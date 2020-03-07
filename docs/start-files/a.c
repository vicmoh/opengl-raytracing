/* Starting code for assignment 3.
   Calculates a ray that passes through each pixel of the viewing window.
   Draws randomly coloured pixel in the viewing window. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Linux OpenGL Headers
/*
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
*/

// MacOS OpenGL Headers
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

	// window size
#define SIZE 512




	/* calculate the length of a vector */
float length(float *x, float *y, float *z) {
   return( sqrtf( (*x * *x) + (*y * *y) + (*z * *z)) );
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

 
	/* OoenGL calls this to draw the screen */
void display() {
int i, j;
float x, y;
float r, g, b;

float x0, y0, z0;   // viewpoint origin
float xd, yd, zd;   // view ray direction


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
   for(i=0; i<SIZE; i++) {
      for(j=0; j<SIZE; j++) {
		/* screen location to draw pixel */
		/* used with glVertexf() but not in calculations for vectors */
         x = (float) (i - (SIZE/2)) / (SIZE/2);
         y = (float) (j - (SIZE/2)) / (SIZE/2);

		/* calculate the ray direction (xd, yd, zd) */
		/* ray starts at the origin (x0, y0, z0) and ends at
		   a point on the viewing window (xd, yd, zd) */
        	/* screen corner coordinates are (-1,-1) to (1,1) */
         xd = ((float) (i - (SIZE/2)) / (SIZE/2));
         yd = ((float) (j - (SIZE/2)) / (SIZE/2));
         zd = -1.0;
         normalize(&xd, &yd, &zd);

			/* ray tracing code */
			/* your code starts here */

		/* calculate B, C.  A == 1.0 */

		/* calculate the discriminant */

		/* if there is one intersection point (discriminant == 0)
		   then calculate intersection of ray and sphere at (xi, yi, zi) */

		/* if there are two two intersection points (discriminant > 0) 
		   then determine which point is closer to the viewpoint (x0, y0, z0)
		   and calculate the intersection of point (xi, yi, zi) */

		/* calculate normal vector (nx, ny, nz) to the intersection point */

		/* calculate viewing vector (vx, vy, vz) */

		/* calculate the light vector (lx, ly, lz)  */

		/* calculate the dot product N.L, using the normal vector
		   and the light vector */

		/* calculate the reflection vector (rx, ry, rz) */

		/* calculate the dot product R.V, using the reflection vector
		   and the viewing vector */

		/* calculate illumination using the parameters read from the
		   file and N.L and R.V  */


		/* set the colour of the point - calculate the r,g,b
		   values using ambient, diffuse, and specular light parameters
		   and the dot products N.L and R.V  */

			/* end of ray tracing code */

		/* replace the following three lines with the illumination calculations */
         r =  (float) rand() / RAND_MAX ;    
         b =  (float) rand()  / RAND_MAX;
         g =  (float) rand()  / RAND_MAX;
         glColor3f(r, g, b);
		/* draw the point on the display window at point (x, y) */
         glVertex2f(x, y);


      }  // end loop j
   }     // end loop i

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
 
int main(int argc, char** argv) {

	/* read input file */

	/* Initialize OpenGL and GLUT */
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(SIZE,SIZE);
   glutCreateWindow("2D Test");
   glutDisplayFunc(display);
   glutKeyboardFunc (keyboard);

   glutMainLoop();
   return 0;
}
