#ifndef __FILESYSTEM__
#define __FILESYSTEM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>


/* library for helping out with file system stuff
 * mostly for saving data ig
 */

 typedef enum Sort_Option {
  SORT_ASCENDING,
  SORT_DESCENDING,
 } Sort_Option;


// might change this later, just drawing stuff out
int save_data_to_file(char* path, void* data, size_t size);

// just stuff for testing dirent.h
void list_dir_unsorted(char* dir_path);

void list_dir_alpha_sorted(char* dir_path, Sort_Option option);


#endif
