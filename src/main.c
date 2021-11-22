#include <stdio.h>
#include <unistd.h>
#include <malloc.h>

#define INITIAL_SIZE 256

struct AnalyzerResult {
  char characters[INITIAL_SIZE];
  int characters_count[INITIAL_SIZE];
  size_t length;
};

struct FileContent {
  char* contents;
  size_t length;
};

int char_array_includes(char* array, size_t size, char c){
  for (size_t i = 0; i < size; i++){
    if (c == *(array+i)) return i;
  }
  return -1;
}


struct AnalyzerResult analyze_contents(struct FileContent file_content){
  size_t current_length = 0;
  struct AnalyzerResult result = {
    {},
    {},
    current_length
  };
  for (size_t i = 0; i < file_content.length; i++) {
    char c_char = file_content.contents[i];
    int index = char_array_includes(result.characters, INITIAL_SIZE, c_char);
    if (index != -1) result.characters_count[index]++;
    else {
      result.characters[current_length] = c_char;
      result.characters_count[current_length] = 1;
      current_length++;
    }
  }
  result.length = current_length;
  return result;
}

size_t read_file_length(FILE* ptr){
  size_t pos = ftell(ptr);
  fseek(ptr, 0, SEEK_END);
  size_t l = ftell(ptr);
  fseek(ptr, pos, SEEK_SET);
  return l;
}

struct FileContent read_file_contents(char* filepath){
  FILE* fptr = fopen(filepath, "r");
  size_t length = read_file_length(fptr);
  char* contents = (char*) malloc(sizeof(char)*length);
  fread(contents, 1, length, fptr);  
  fclose(fptr);
  struct FileContent res = { contents, length };
  return res;
}

int check_if_file_exists(char* filepath){
  return access(filepath, R_OK) == 0;
}

void print_result(struct AnalyzerResult result){
  for (size_t i = 0; i < result.length; i++){
    char c = result.characters[i];
    if (c == '\n') printf("%1$d) Character \"<new line>\" occurred %2$d times [#<new line>=%2$d]\n", (int) i+1, result.characters_count[i]);
    else if (c == ' ') printf("%1$d) Character \"<space>\" occurred %2$d times [#<space>=%2$d]\n", (int) i+1, result.characters_count[i]);
    else printf("%1$d) Character \"%2$c\" occurred %3$d times [#%2$c=%3$d]\n", (int) i+1, c, result.characters_count[i]);
  }
}

int main(int argc, char** argv){

  if (argc < 2) {
    printf("Please provide the path to the text file\n");
    return 1;
  }
  
  char* filepath = argv[1];
  int exists = check_if_file_exists(filepath);
  if (!exists) {
    printf("Given file does not exist (path: %s)\n", filepath);
    return 1;
  }

  struct FileContent file_content = read_file_contents(filepath);

  struct AnalyzerResult result = analyze_contents(file_content);

  print_result(result);

  return 0;
}
