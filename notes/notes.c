#include <ctype.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char Project[999];
  char ProjectPart2[999];
  char File[999];
  char Test[999];
} UnitTest;

const char *ltrim(const char *s) {
    while (isspace((unsigned char)*s))
        s++;
    return s;
}

int main(int argc, char *argv[]) {
  FILE *dotnetTestCommand = popen("cat ~/lazyunit/tests.txt", "r");
  if (dotnetTestCommand == NULL) {
    printf("ERROR");
    return -1;
  }

  const int MAX_LINE_SIZE = 4096;

  char line[MAX_LINE_SIZE];

  char currentProject[256] = "";
  char currentFile[256] = "";

  while (fgets(line, sizeof(line), dotnetTestCommand) != NULL) {
    UnitTest testObject = {0};

    if (strstr(line, "Test run") == NULL) {
      if (sscanf(line, "%255[^.].%255[^.].%255[^.].%255[^\n]",
                 testObject.Project, testObject.ProjectPart2, testObject.File,
                 testObject.Test) == 4) {

        strcat(testObject.Project, ".");
        strcat(testObject.Project, testObject.ProjectPart2);

        // 󱞩
        if (strcmp(currentProject, testObject.Project) != 0) {
          const char *trim = ltrim(testObject.Project);
          printf("1=>%s\n", trim);
          strcpy(currentProject, testObject.Project);
        }

        if (strcmp(currentFile, testObject.File) != 0) {
          printf("2=>%s\n", testObject.File);
          strcpy(currentFile, testObject.File);
        }

        printf("3=>%s\n", testObject.Test);
      }
    };
  }

  pclose(dotnetTestCommand);
}
