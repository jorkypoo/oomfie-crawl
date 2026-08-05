#ifndef __FILESYSTEM__
#define __FILESYSTEM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* library for helping out with file system stuff
 * mostly for saving data ig
 */


// might change this later, just drawing stuff out
int save_data_to_file(char* path, void* data, size_t size);

void list_dir_contents(char* dir_path);

// etc, etc

#endif
