#include "../inc/filesystem.h"

// filters out 'dot' and 'dot-dot' entries within every directory
// passed to scandir as it lets u pass sorting functions which is neato-burrito
static int one(const struct dirent* entry) { 
  if (!strcmp(entry->d_name, ".") || !strcmp(entry->d_name, ".."))
    return 0;
  else
    return 1;
}


int save_data_to_file(char* path, void* data, size_t size) {
  if (!path) return 0;
  
  FILE* file = NULL;
  file = fopen(path, "wb");
  if (!file) return 0;

  if (fwrite(data, 1, size, file) != size)
    return 0;
  
  fclose(file);
  return 1;
}


void list_dir_unsorted(char* dir_path) {
  if (!dir_path) return; // retard

  DIR* dir = opendir(dir_path);
  if (!dir) return; // opendir error

  struct dirent* df = readdir(dir);
  for (long int i = 0; df; i = telldir(dir)) {
    printf("filename: %s\n", df->d_name);
    df = readdir(dir);
  }

  closedir(dir);
}


void list_dir_alpha_sorted(char* path, Sort_Option option) {
  if (!path) return;

  struct dirent** df;
  int n;

  n = scandir(path, &df, one, alphasort);
  if (n == -1) return; //scandir error

  switch (option) {
    case SORT_ASCENDING:
      while (n--) {
        printf("filename: %s\n", df[n]->d_name);
        free(df[n]);
      }
      break;
    case SORT_DESCENDING:
      for (int i = 0; i < n; i++) {
        printf("filename: %s\n", df[i]->d_name);
        free(df[i]);
      }
      break;  
  }

  free(df);
}


size_t num_of_lines(char* src, size_t size) {
  if (!src) return 0;
  size_t num = 0;

  for (size_t i = 0; i < size; i++)
    if (src[i] == '\n') num++;

  return num;
}


// shifts the chars in a string to the left by one n times
// make sure the size accounts for the \0
void str_shift (char* target, size_t size, int n) {
  if (!target) return;

  for (int i = 0; i < n; i++) {
    for (size_t i = 1; i < size; i++) {
      target[i - 1] = target[i];
    }
  }
}


// return value must be freed
char* get_delimited_value(char* src, char deliminator, int n) {
  // n is basically how many deliminators we have to pass
  // firstly, scan src to see how big the value is
  size_t vsize = 0;
  size_t delims_seen = 0;
  for (size_t i = 0; i < strlen(src); i++) {
    if (delims_seen == n) vsize++; // we are at the correct value in src, so count

    if (src[i] == deliminator) { // skip the entry if it's the deliminator
      if (delims_seen == n) vsize--; // don't count the deliminator in total size of value
      delims_seen++;
      continue; 
    }
  } delims_seen = 0; 

  if (vsize < 1) return NULL; // caller was a retard
  char* value = calloc(sizeof(char), vsize + 1);

  size_t j = 0; // counter for value
  for (size_t i = 0; i < strlen(src); i++) {
    if (delims_seen == n) {
      value[j] = src[i];
      j++;
    }

    if (src[i] == deliminator) {
      if (delims_seen == n) value[vsize] = '\0';
      delims_seen++;
      continue;
    }
  }

  return value;
}


// must free return value
char* get_line_from_str(char* src, size_t line) {
  if (!src) return NULL;

  size_t nlines = num_of_lines(src, strlen(src));
  
  int cursor = 0;
  while (line) {
    if (cursor >= strlen(src) - 1) return NULL; // if the offset does not exist, return NULL
    if (src[cursor++] == '\n') line--;
  }
  
  // calculate the lines size 
  size_t lsize = 0;
  for (size_t i = cursor; src[i] != '\n'; i++) lsize++;

  // allocate return value and assign line to it
  char *ret = calloc(sizeof(char), lsize + 1);
  for (size_t i = 0; i < lsize; i++) ret[i] = src[i + cursor];

  return ret;
}


// must free return value
char* get_line_offset(char* path, size_t offset) {
  if (!path) return NULL;
  FILE* file = NULL;
  file = fopen(path, "r");
  if (!file) return NULL;
  
  // get size of file and dump contents into buffer
  fseek(file, 0, SEEK_END);
  size_t fsize = ftell(file);
  rewind(file);
  char* buff = calloc(sizeof(char), fsize + 1);
  if (fread(buff, sizeof(char), fsize, file) < 1)
    printf("error: could not read file %s", path);
  fclose(file);
 
  // move buffer cursor to the start of the specified line
  // should not call segfault anymore
  int cursor = 0;
  while (offset) {
    if (cursor >= fsize - 1) return NULL; // if the offset does not exist, return NULL
    if (buff[cursor++] == '\n') offset--;
  }
  
  // calculate the lines size 
  size_t lsize = 0;
  for (size_t i = cursor; buff[i] != '\n'; i++) lsize++;

  // allocate return value and assign line to it
  char *ret = calloc(sizeof(char), lsize + 1);
  for (size_t i = 0; i < lsize; i++) ret[i] = buff[i + cursor];

  free(buff);
  return ret;
}


char* get_line_match(char* path, char* match) {
  if (!path || !match) return NULL;
  FILE* file = NULL;
  file = fopen(path, "r");
  if (!file) return NULL;
  
  // get size of file and dump contents into buffer
  fseek(file, 0, SEEK_END);
  size_t fsize = ftell(file);
  rewind(file);
  char* buff = calloc(sizeof(char), fsize + 1);
  if (fread(buff, sizeof(char), fsize, file) < 1)
    printf("error: could not read file %s", path);
  fclose(file);

  // prepare a buffer to hold one line of the file at a time
  size_t nlines = num_of_lines(buff, fsize + 1);
  char* ret = malloc(1);

  // characters to discard after shift: match_len + 1
  size_t match_len = strlen(match);
  if (match_len < 1) return NULL;

  size_t line_found = -1;

  // loop through every line in the file and search for the match
  for (size_t i = 0; i < nlines; i++) {
    ret = realloc(ret, strlen(get_line_from_str(buff, i)));
    ret = get_line_from_str(buff, i);
    if (strncmp(ret, match, match_len) == 0) {
      line_found = 0;
      break;
    }
  }

  // now filter out the match from the line, as that's included in what was found
  str_shift(ret, strlen(ret) + 1, match_len + 1);
 
  free(buff);
  return ret;
}
