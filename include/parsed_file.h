#ifndef PARSED_FILE_H
#define PARSED_FILE_H

#include "dynamic_string.h"
#include "file_reader.h"

#define DEBUG true

/* -------------------------------------------------------------------------- */
/*                              Class attributes                              */
/* -------------------------------------------------------------------------- */

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
  bool hasError;
  Light lightData;
  Sphere sphereData;
} ParsedFile;

/* -------------------------------------------------------------------------- */
/*                                  Functions                                 */
/* -------------------------------------------------------------------------- */

/**
 * Parse the file for the Ray tracing data.
 * Which contains the lighting and the sphere data.
 * @param filePath of the file.
 * @return ParsedFile object.
 */
ParsedFile* new_ParsedFile(String filePath);

/**
 * Free the object.
 * @param self object to be freed.
 */
void free_ParsedFile(ParsedFile* self);

#endif
