#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "type.h"
#include "fileToString.h"

void colorch(char ch, int color){
  attron(COLOR_PAIR(color));
  addch(ch);
  attroff(COLOR_PAIR(color));
}

void dispSting(char * str, char * typed, unsigned int buffer)
{
  int attrib = 3;

  move(LINES/2, 0);

  for(int i = 0; i < buffer; i++)
    addch(' ');

  for(int i = 0; i < (COLS - buffer) && i < strlen(str) - 1; i++)
  {
    if(i > COLS/2 - buffer - 1 || strlen(typed) == 0){
      attrib = 3;
    }else{
      if(str[i] == typed[i])
      {
        attrib = 1;
      }
      else
        attrib = 2;
    }

    colorch(str[i], attrib);

  }
  addch(' ');
}

int main()
{
  initscr();
  start_color();
  raw();
  noecho();
  refresh();
  curs_set(0);
  keypad(stdscr, TRUE);
  int wrongfg = COLOR_RED;
  int correctfg = COLOR_GREEN;
  int nextfg = COLOR_WHITE;
  int bg = COLOR_BLACK;
  init_pair(1, correctfg, bg);
  init_pair(2, wrongfg, bg);
  init_pair(3, nextfg, bg);

  char * text = FileToString("res/para");

  char * typed = malloc(sizeof(char) * strlen(text) + 1);
  typed[0] = '\0';
  mvprintw(LINES/2 + 1, COLS/2, "^");
  dispSting(text, typed, COLS/2);
  refresh();

  time_t start = time(NULL);
  time_t ctime;

  bool newmax = FALSE;
  float acc = 1.0;
  float wpm = 0.0;
  int correct = 0;
  int ch;
  int i = 0;
  int max = 0;
  char tmp[2];
  tmp[1] = '\0';
  while(ch != 27){
    ch = getch();
    i++;
    if(i >= max)
    {
      max = i;
      newmax = TRUE;
    }
    switch (ch)
    {
      case 26:
        break;
      case KEY_BACKSPACE:
        i--;
        typed[i] = '\0';
        i--;
        typed[i] = '\0';

        if(COLS/2 - i < 0)
        {
          dispSting(&text[i - COLS/2], &typed[i - COLS/2], 0);
        }else{
          dispSting(text, typed, COLS/2 - i);
        }

        break;
      default:
        tmp[0] = ch;
        strcat(typed, tmp);
        if(COLS/2 - i < 0)
        {
          dispSting(&text[i - COLS/2], &typed[i - COLS/2], 0);
        }else{
          dispSting(text, typed, COLS/2 - i);
        }

        if(newmax)
          correct = typed[i-1] == text[i-1] ? correct + 1 : correct;

        if(newmax)
          acc = 100*(float)correct/max;
        mvprintw(LINES/2 + 2, COLS/2, "%%%d ", (int)roundf(acc));
        time(&ctime);

        wpm = (((float)i/6.1)/(int)(ctime - start))*60;
        mvprintw(LINES/2 + 3, COLS/2, "wpm:%f ", wpm);

        refresh();
        break;
    }
    newmax = FALSE;
  }

  endwin();
}
