#ifndef __FILESYSTEM__
#define __FILESYSTEM__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>


/* library for helping out with file system stuff
 * mostly for internal use tbh
 */

 // allow for this to be changed in settings maybe
 #define DELIMITER ','

 typedef enum Sort_Option {
  SORT_ASCENDING,
  SORT_DESCENDING,
 } Sort_Option;


// might change this later, just drawing stuff out
int save_data_to_file(char* path, void* data, size_t size);

// just stuff for testing dirent.h
void list_dir_unsorted(char* dir_path);
void list_dir_alpha_sorted(char* dir_path, Sort_Option option);

// returns num of lines in a file, including extenuous new lines
size_t num_of_lines(char* src, size_t size);

// shifts characters to the left by n times in src
// size must account for the \0 character!
void str_shift(char* src, size_t size, int n);

/* make sure you free the return value of the following functions */

// returns the nth delimited value from src, or NULL on failure
// must free the returned value!
char* get_delimited_value(char* src, char delimiter, int n);

// similar to get_line_offset, but works with a given string, because its a very good bit of code
// must free the returned value!
char* get_line_from_str(char* src, size_t line);

// return an entire selected line from a csv ish file based on an offset, or NULL on failure
// 0 being first line, 1 being second, etc
// must free the returned value!
char* get_line_offset(char* path, size_t offset);

// same as above, but *assumes* a string match, looks for the first occurance
// of match, and returns the following line after the match; else, NULL
// must free the returned value!
char* get_line_match(char* path, char* match);

#endif
