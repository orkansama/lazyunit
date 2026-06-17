#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char Project[256];
  char File[256];
  char Test[256];
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
  if (dotnetTestCommand == NULL) {
    printf("ERROR");
    return -1;
  }

  char line[4096];
  int row = 1;

  char *currentProject = NULL;
  char *currentFile = NULL;

  while (fgets(line, sizeof(line), dotnetTestCommand) != NULL) {

    char a[256], b[256], c[256], d[256];
    bool lineIsValidTest =
        sscanf(line, "%255[^.].%255[^.].%255[^.].%255[^.]", a, b, c, d) == 4;

    if (lineIsValidTest) {
      char *lineNoWhitespaces = removeSpaces(line);
      if (lineNoWhitespaces == NULL) {
        printf("Error");
        return -1;
      }

      UnitTest testObject;
      char temp[256];

      sscanf(lineNoWhitespaces, "%255[^.].%255[^.].%255[^.].%255[^.]", temp,
             testObject.Project, testObject.File, testObject.Test);

      if (currentProject == NULL ||
          strcmp(currentProject, testObject.Project) != 0) {

        static char savedProject[4096];
        static char savedFile[4096];

        strncpy(savedProject, testObject.Project, 4096);
        currentProject = savedProject;
        printf("%s\n", currentProject);
      }

      if (currentFile == NULL || strcmp(currentFile, testObject.File) != 0) {
        static char savedFile[4096];

        strncpy(savedFile, testObject.File, 4096);
        currentFile = savedFile;
        printf(" 󱞩 %s\n", currentFile);
      }

      printf("   󱞩 %s\n", testObject.Test);
    }
  }

  pclose(dotnetTestCommand);
}
