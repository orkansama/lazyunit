#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char *Project;
  char *File;
  char *Test;
} UnitTest;

char *removeSpaces(char s[]) {
  int inputLenght = strlen(s);
  int currentChar = 0, iteration = 0;

  while (currentChar < inputLenght) {
    if (s[currentChar] != ' ') {
      s[iteration++] = s[currentChar];
    }
    currentChar++;
  }
  s[iteration] = '\0';
  return s;
}

int main(int argc, char *argv[]) {
  FILE *dotnetTestCommand = popen("cat ~/lazyunit/tests.txt", "r");

  char line[4096];
  int row = 1;

  while (fgets(line, sizeof(line), dotnetTestCommand) != NULL) {
    char a[256], b[256], c[256], d[256];
    bool lineIsValidTest =
        sscanf(line, "%255[^.].%255[^.].%255[^.].%255[^.]", a, b, c, d) == 4;

    if (lineIsValidTest) {
      char *lineWithNoWhitespaces = removeSpaces(line);

      char lineArray[] = { *lineWithNoWhitespaces };

      char Project[256], File[256], Test[256];
      sscanf(lineArray, "%255[^.].%255[^.].%255[^.]", Project, File, Test);

      char *currentProject = NULL;
      char *currentFile = NULL;

      int testArrayLenght = (sizeof tests / sizeof tests[0]);
      for (size_t i = 0; i < testArrayLenght; i++) {

        if (currentProject == NULL ||
            strcmp(currentProject, tests[i].Project) != 0) {

          currentProject = tests[i].Project;
          currentFile = NULL;
          printf("%s\n", currentProject);
        }

        if (currentFile == NULL || strcmp(currentFile, tests[i].File) != 0) {

          currentFile = tests[i].File;
          printf(" 󱞩 %s\n", currentFile);
        }

        printf("   󱞩 %s\n", tests[i].Test);
      }
    }
  }
}
