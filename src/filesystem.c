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
