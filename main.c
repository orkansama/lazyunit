#include <ctype.h>
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

const char *ltrim(const char *s);
void ExecuteDotnetTest(WINDOW *pad, int max_x, int max_y);

int main(void) {
  initscr();
  if (!has_colors()) {
    // TODO: SEPERATE ERROR WINDOW / OVERLAY
  }
  start_color();
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

  WINDOW *mypad = newpad(max_y * 5, max_x * 5);
  UnitTest testObjects[500] = {0};
  ExecuteDotnetTest(mypad, max_x, max_y);

  int ch;
  int scroll = 0;

  while ((ch = getch()) != EOF && ch != 'q') {
    if (ch == KEY_DOWN) {
      scroll++;
      prefresh(mypad, scroll, 0, 2, 2, 0 + max_y - 2, 0 + max_x - 2);
    }
    if (ch == KEY_UP) {
      if (scroll > 0) {
        scroll--;
      }
      prefresh(mypad, scroll, 0, 2, 2, 0 + max_y - 2, 0 + max_x - 2);
    }
  };

  delwin(border);
  endwin();
  return 0;
}

const char *ltrim(const char *s) {
  while (isspace((unsigned char)*s))
    s++;
  return s;
}

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
  int itrCount = 0;

  while (fgets(line, sizeof(line), dotnetTestCommand) != NULL) {
    UnitTest testObject = {0};

    if (strstr(line, "Test run") == NULL) {
      if (sscanf(line, "%255[^.].%255[^.].%255[^.].%255[^\n]",
                 testObject.Project, testObject.ProjectPart2, testObject.File,
                 testObject.Test) == 4) {

        strcat(testObject.Project, ".");
        strcat(testObject.Project, testObject.ProjectPart2);

        if (strcmp(currentProject, testObject.Project) != 0) {
          const char *trim = ltrim(testObject.Project);
          strcpy(currentProject, testObject.Project);
          wprintw(pad, "=> %s\n", trim);

          // ┌────────────────┐
          // │                │
          // │                │
          // └────────────────●  ← (start_y + height - 2,  start_x + width - 2)
          prefresh(pad, 0, 0, 2, 2, 0 + max_y - 2, 0 + max_x - 2);
        }

        if (strcmp(currentFile, testObject.File) != 0) {
          wprintw(pad, " => %s\n", testObject.File);
          prefresh(pad, 0, 0, 2, 2, 0 + max_y - 2, 0 + max_x - 2);
          strcpy(currentFile, testObject.File);
        }

        wprintw(pad, "  => %s\n", testObject.Test);
        prefresh(pad, 0, 0, 2, 2, 0 + max_y - 2, 0 + max_x - 2);

        testObjects[itrCount] = testObject;
        itrCount++;
      }
    };
  }

  pclose(dotnetTestCommand);
}
