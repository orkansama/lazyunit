#include <ncurses.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

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

  char line[4096];
  int row = 1;

  // First argument: where to put the line
  // Second argument: How big can the line be
  // Third argument: Input Stream
  while (fgets(line, sizeof(line), dotnetTestCommand) != NULL) {
    char a[256], b[256], c[256], d[256];
    bool lineIsValidTest =
        sscanf(line, "%255[^.].%255[^.].%255[^.].%255[^.]", a, b, c, d) == 4;

    if (lineIsValidTest) {
      char *newLine = removeSpaces(line);
      mvwprintw(windowToPrintAt, row, 2, "%s", newLine);
      wrefresh(windowToPrintAt);
      row++;
    }
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
