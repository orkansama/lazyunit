#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char Project[256];
  char ProjectPart2[256];
  char File[256];
  char Test[256];
} UnitTest;

int main(int argc, char *argv[]) {
  FILE *dotnetTestCommand = popen("cat ~/lazyunit/tests.txt", "r");
  if (dotnetTestCommand == NULL) {
    printf("ERROR");
    return -1;
  }

  const int MAX_LINE_SIZE = 4096;

  char line[MAX_LINE_SIZE];
  int row = 1;

  // char *currentProject = NULL;
  // char *currentFile = NULL;

  char currentProject[256] = "";
  char currentFile[256] = "";

  while (fgets(line, sizeof(line), dotnetTestCommand) != NULL) {

    char a[256], b[256], c[256], d[256];
    bool lineIsValidTest =
        sscanf(line, "%255[^.].%255[^.].%255[^.].%255[^.]", a, b, c, d) == 4;

    if (lineIsValidTest && strstr(line, "Test run for") == NULL) {
      UnitTest testObject;

      sscanf(line, "%255[^.].%255[^.].%255[^.].%255[^.]", testObject.Project,
             testObject.ProjectPart2, testObject.File, testObject.Test);

      strcat(testObject.Project, testObject.ProjectPart2);

      if (strcmp(currentProject, testObject.Project) != 0) {
        strcpy(currentProject, testObject.Project);
        printf("1 %s\n", currentProject);
      }

      if (strcmp(currentFile, testObject.File) != 0) {
        strcpy(currentFile, testObject.File);
        printf("2 %s\n", currentFile);
      }

      printf("3 %s\n", testObject.Test);
    }
  }

  pclose(dotnetTestCommand);
}
