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

WINDOW *TopWin(int top_win_height, int max_x) {
  WINDOW *win_top = newwin(top_win_height, max_x, 0, 0);

  box(win_top, 0, 0);
  refresh();
  wrefresh(win_top);

  return win_top;
}

WINDOW *LeftWin(int top_win_height, int max_y, int max_x) {
  int half_x = max_x / 2;

  int winHeight = max_y - top_win_height;
  int winWidth = max_x / 2;
  int winStartTop = top_win_height;
  int winStartLeft = 0;

  WINDOW *win_left = newwin(winHeight, winWidth, winStartTop, winStartLeft);

  box(win_left, 0, 0);
  refresh();
  wrefresh(win_left);

  return win_left;
}

WINDOW *RightWin(int top_win_height, int max_y, int max_x) {
  int half_x = max_x / 2;

  int winHeight = max_y - top_win_height;
  int winWidth = max_x / 2;
  int winStartTop = top_win_height;
  int winStartLeft = max_x / 2;

  WINDOW *win_right = newwin(winHeight, winWidth, winStartTop, winStartLeft);

  box(win_right, 0, 0);
  refresh();
  wrefresh(win_right);

  return win_right;
}

void ExecuteDotnetTest(WINDOW *windowToPrintAt, int max_x, int max_y) {

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
          wprintw(windowToPrintAt, "=> %s\n", testObject.Project);
          wrefresh(windowToPrintAt);
          strcpy(currentProject, testObject.Project);
        }

        if (strcmp(currentFile, testObject.File) != 0) {
          wprintw(windowToPrintAt, " => %s\n", testObject.File);
          wrefresh(windowToPrintAt);
          strcpy(currentFile, testObject.File);
        }

        wprintw(windowToPrintAt, "  => %s\n", testObject.Test);
        wrefresh(windowToPrintAt);
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

  int TOP_WIN_HEIGHT = 3;
  WINDOW *win_top = TopWin(TOP_WIN_HEIGHT, max_x);
  WINDOW *win_left = LeftWin(TOP_WIN_HEIGHT, max_y, max_x);
  WINDOW *win_right = RightWin(TOP_WIN_HEIGHT, max_y, max_x);

  scrollok(win_left, true);
  wrefresh(win_left);

  ExecuteDotnetTest(win_left, max_x, max_y);

  while (getch() != 'q')
    ;

  delwin(win_top);
  delwin(win_left);
  delwin(win_right);
  endwin();
  return 0;
}
