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
int OccupiedRowCount();

int main(void) {
  initscr();
  if (!has_colors()) {
    // TODO: SEPERATE ERROR WINDOW / OVERLAY
  }
  start_color();
  init_pair(999, COLOR_RED, COLOR_GREEN);
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

  int occupiedRowCount = OccupiedRowCount();
  WINDOW *myPad = newpad(occupiedRowCount, max_x);
  ExecuteDotnetTest(myPad, max_x, max_y);

  int ch;
  int scroll = 0;
  int paddingTop = 0;
  int triggerScrollDistance = max_y - 12;

  mvwchgat(myPad, scroll, 0, -1, A_REVERSE, 0, NULL);
  prefresh(myPad, paddingTop, 0, 2, 2, max_y - 2, max_x - 2);

  while ((ch = getch()) != EOF && ch != 'q') {
    if (scroll <= occupiedRowCount - 2) {
      if (ch == 'j') {
        scroll++;
        mvwchgat(myPad, scroll - 1, 0, -1, A_NORMAL, 0, NULL);
        mvwchgat(myPad, scroll, 0, -1, A_REVERSE, 0, NULL);

        if (scroll >= paddingTop + triggerScrollDistance) {
          paddingTop = scroll - triggerScrollDistance;
        }

        prefresh(myPad, paddingTop, 0, 2, 2, max_y - 2, max_x - 2);
      }
    }
    if (scroll > 0) {
      if (ch == 'k') {
        scroll--;
        mvwchgat(myPad, scroll + 1, 0, -1, A_NORMAL, 0, NULL);
        mvwchgat(myPad, scroll, 0, -1, A_REVERSE, 0, NULL);

        if (scroll < paddingTop) {
          paddingTop = scroll;
        }

        prefresh(myPad, paddingTop, 0, 2, 2, max_y - 2, max_x - 2);
      }
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
  int testCount = 0;

  // TODO: Setting to set build true/false
  FILE *dotnetTestCommand = popen("cat ~/lazyunit/tests.txt", "r");
  if (dotnetTestCommand == NULL) {
    printf("ERROR");
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

        if (strcmp(currentProject, testObject.Project) != 0) {
          testCount++;
          const char *trim = ltrim(testObject.Project);
          strcpy(currentProject, testObject.Project);
          wprintw(pad, "=> %s\n", trim);

          // ┌────────────────┐
          // │                │
          // │                │
          // └────────────────●  ← (start_y + height - 2,  start_x + width -
          // 2)
          prefresh(pad, 0, 0, 2, 2, 0 + max_y - 2, 0 + max_x - 2);
        }

        if (strcmp(currentFile, testObject.File) != 0) {
          testCount++;
          wprintw(pad, " => %s\n", testObject.File);
          prefresh(pad, 0, 0, 2, 2, 0 + max_y - 2, 0 + max_x - 2);
          strcpy(currentFile, testObject.File);
        }

        testCount++;
        wprintw(pad, "  => %s\n", testObject.Test);
        prefresh(pad, 0, 0, 2, 2, 0 + max_y - 2, 0 + max_x - 2);
      }
    };
  }

  pclose(dotnetTestCommand);
}

int OccupiedRowCount() {
  int rowCount = 0;

  // TODO: Setting to set build true/false
  FILE *dotnetTestCommand = popen("cat ~/lazyunit/tests.txt", "r");
  if (dotnetTestCommand == NULL) {
    printf("ERROR");
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
          rowCount++;
          strcpy(currentProject, testObject.Project);
        }

        if (strcmp(currentFile, testObject.File) != 0) {
          rowCount++;
          strcpy(currentFile, testObject.File);
        }

        rowCount++;
      }
    };
  }

  pclose(dotnetTestCommand);
  return rowCount;
}
