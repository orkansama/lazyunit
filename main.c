#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
  char Project[256];
  char ProjectPart2[256];
  char File[256];
  char Test[256];
} UnitTest;

void ExecuteDotnetTest(WINDOW *pad, int max_x, int max_y) {
  FILE *dotnetTestCommand = popen("cat ~/lazyunit/tests.txt", "r");
  if (dotnetTestCommand == NULL) {
    printf("ERROR");
  }

  const int MAX_LINE_SIZE = 4096;
  int row = 1;

  char line[MAX_LINE_SIZE];

  char currentProject[256] = "";
  char currentFile[256] = "";

  while (fgets(line, sizeof(line), dotnetTestCommand) != NULL) {
    UnitTest testObject = {0};

    if (strstr(line, "Test run") == NULL) {
      if (sscanf(line, "%255[^.].%255[^.].%255[^.].%255[^.]",
                 testObject.Project, testObject.ProjectPart2, testObject.File,
                 testObject.Test) == 4) {

        strcat(testObject.Project, testObject.ProjectPart2);

        if (strcmp(currentProject, testObject.Project) != 0) {
          wprintw(pad, "=> %s\n", testObject.Project);
          prefresh(pad, 1, 0, 3, 11, 20, 68);
          strcpy(currentProject, testObject.Project);
        }

        if (strcmp(currentFile, testObject.File) != 0) {
          wprintw(pad, " => %s\n", testObject.File);
          prefresh(pad, 1, 0, 3, 11, 20, 68);
          strcpy(currentFile, testObject.File);
        }

        wprintw(pad, "  => %s\n", testObject.Test);
        prefresh(pad, 1, 0, 3, 11, 20, 68);
      }
    };
  }

  pclose(dotnetTestCommand);
}

int main(void) {
  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  curs_set(0);

  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  WINDOW *border = newwin(max_y, max_x, 0, 0);
  box(border, 0, 0);
  refresh();
  wrefresh(border);

  WINDOW *mypad = newpad(max_y * 2, 1000);
  ExecuteDotnetTest(mypad, max_x, max_y);

  while (getch() != 'q') {
    switch (getch()) {
    case KEY_DOWN:
      break;
      prefresh(mypad, 0, 0, 0, 0, 0, 20);
    };
  };

  delwin(border);
  endwin();
  return 0;
}
