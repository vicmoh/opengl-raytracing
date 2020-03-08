#include "dynamic_string.h"
#include "file_reader.h"
#include "parsed_file.h"

ParsedFile* new_ParsedFile(String filePath) {
  // Initialize parsing.
  FileReader* fr = new_FileReader(filePath);
  String lightString = FileReader_getLineAt(fr, 0);
  String sphereString = FileReader_getLineAt(fr, 1);

  // Check if parsing is successfull.
  ParsedFile* new = malloc(sizeof(ParsedFile));
  new->hasError = true;
  if (fr == null) return new;

  /// Read and assign to the object.
  char buffer[256];
  sscanf(lightString, "%s %f %f %f", buffer, &new->lightData.x,
         &new->lightData.y, &new->lightData.z);
  sscanf(sphereString, "%s %f %f %f %f %f %f %f %f %f %f %f %f %f", buffer,
         &new->sphereData.x, &new->sphereData.y, &new->sphereData.z,
         &new->sphereData.r, &new->sphereData.ar, &new->sphereData.ag,
         &new->sphereData.ab, &new->sphereData.dr, &new->sphereData.dg,
         &new->sphereData.db, &new->sphereData.sr, &new->sphereData.sg,
         &new->sphereData.sb);

  // Print and show what has been parsed.
  printf("Parsed light: %f %f %f\n", new->lightData.x, new->lightData.y,
         new->lightData.z);
  printf("Parsed sphere: %f %f %f %f %f %f %f %f %f %f %f %f %f\n",
         new->sphereData.x, new->sphereData.y, new->sphereData.z,
         new->sphereData.r, new->sphereData.ar, new->sphereData.ag,
         new->sphereData.ab, new->sphereData.dr, new->sphereData.dg,
         new->sphereData.db, new->sphereData.sr, new->sphereData.sg,
         new->sphereData.sb);

  // Dispose and return
  dispose(lightString, sphereString);
  free_FileReader(fr);
  new->hasError = false;
  return new;
}

void free_ParsedFile(ParsedFile* this) { free(this); }