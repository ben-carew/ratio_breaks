// CPP program to run the "Rational Breaks" method of work management
// as suggested here:
// https://www.lesswrong.com/posts/RWu8eZqbwgB9zaerh/rational-breaks-a-better-way-to-work
// Compile: g++ ratio_break.cpp -o ratio_break -lncurses

#include <chrono>
#include <ctime>
#include <curses.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
using namespace std;

int menu();
int displayTime(int seconds, int break_seconds);
void statistics(int total, int accum_break, int rest, int num_breaks);
void overTime(int accum_break);

int main(int argc, char *argv[]) {
  int ratio;
  if (argc == 1) {
    ratio = 3;
  } else {
    ratio = strtod(argv[1], NULL);
  }
  int total = 0;
  int accum_break = 0;
  int rest = 0;
  int num_breaks = 0;
  int choice = 0;
  int ch = 0;
  while (true) {
    choice = menu();
    switch (choice) {
    case 1:
      while (true) {
        total++;
        if (total % ratio == 0) {
          accum_break++;
        }
        ch = displayTime(total, accum_break);
        if (ch == 10) {
          break;
        }
      }
      break;
    case 2:
      num_breaks++;
      while (true) {
        total++;
        accum_break--;
        rest++;
        ch = displayTime(total, accum_break);
        if (ch == 10) {
          break;
        }
        if (accum_break <= 0) {
          overTime(accum_break);
          accum_break = 0;
          break;
        }
      }
      break;
    case 3:
      statistics(total, accum_break, rest, num_breaks);
      break;
    }
    if (choice == 3) {
      break;
    }
  }
}

int menu() {
  int choice = 0;
  int highlight = 1;
  int ch;
  initscr();
  clear();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  attron(A_STANDOUT);
  printw("Work\n");
  attroff(A_STANDOUT);
  printw("Break\n");
  printw("Finish\n");
  refresh();
  while (true) {
    ch = getch();
    switch (ch) {
    case KEY_DOWN:
      if (highlight == 1) {
        move(0, 0);
        printw("Work\n");
        attron(A_STANDOUT);
        printw("Break\n");
        attroff(A_STANDOUT);
        printw("Finish\n");
        highlight = 2;
        break;
      } else if (highlight == 2) {
        move(0, 0);
        printw("Work\n");
        printw("Break\n");
        attron(A_STANDOUT);
        printw("Finish\n");
        attroff(A_STANDOUT);
        highlight = 3;
        break;
      } else if (highlight == 3) {
        break;
      }
    case KEY_UP:
      if (highlight == 1) {
        break;
      } else if (highlight == 2) {
        move(0, 0);
        attron(A_STANDOUT);
        printw("Work\n");
        attroff(A_STANDOUT);
        printw("Break\n");
        printw("Finish\n");
        highlight = 1;
        break;
      } else if (highlight == 3) {
        move(0, 0);
        printw("Work\n");
        attron(A_STANDOUT);
        printw("Break\n");
        attroff(A_STANDOUT);
        printw("Finish\n");
        highlight = 2;
        break;
      }
    case 10:
      choice = highlight;
      break;
    }
    refresh();
    if (choice != 0) {
      break;
    }
  }
  endwin();
  return (choice);
}

int displayTime(int seconds, int break_seconds) {
  int ch;
  initscr();
  clear();
  noecho();
  halfdelay(10);
  keypad(stdscr, TRUE);
  move(0, 0);
  printw("Ratio Break Timer\n");
  printw("-----------------\n");
  int hour, min, sec;
  min = seconds / 60;
  sec = seconds % 60;
  hour = min / 60;
  min = min % 60;
  printw("Total Time: %d:%d:%d\n", hour, min, sec);
  min = break_seconds / 60;
  sec = break_seconds % 60;
  hour = min / 60;
  min = min % 60;
  printw("Accumulated Break: %d:%d:%d\n", hour, min, sec);
  printw("Press Enter to return to Menu\n");
  refresh();
  ch = getch();
  endwin();
  return (ch);
}

void overTime(int accum_break) {
  system("spd-say 'your break is over, get back to work'");
  initscr();
  clear();
  noecho();
  halfdelay(10);
  keypad(stdscr, TRUE);
  while (true) {
    int ch;
    accum_break--;
    int over = accum_break * -1;
    move(0, 0);
    printw("Ratio Break Timer\n");
    printw("-----------------\n");
    printw("You are %d seconds over your break limit!\n", over);
    ch = getch();
    if (ch == 10) {
      break;
    }
    refresh();
  }
  endwin();
}

void statistics(int total, int accum_break, int rest, int num_breaks) {
  int worktime = total - rest;
  initscr();
  clear();
  noecho();
  cbreak();
  keypad(stdscr, TRUE);
  move(0, 0);
  printw("Ratio Break Timer\n");
  printw("-----------------\n");
  int hour, min, sec;
  min = total / 60;
  sec = total % 60;
  hour = min / 60;
  min = min % 60;
  printw("Total Session Time: %d:%d:%d\n", hour, min, sec);
  min = worktime / 60;
  sec = worktime % 60;
  hour = min / 60;
  min = min % 60;
  printw("Total Working Time: %d:%d:%d\n", hour, min, sec);
  min = rest / 60;
  sec = rest % 60;
  hour = min / 60;
  min = min % 60;
  printw("Total Break Time: %d:%d:%d\n", hour, min, sec);
  printw("Number of Breaks Taken: %d\n", num_breaks);
  printw("Good job!");
  refresh();
  getch();
  endwin();
  time_t now = time(0);
  tm *ltm = localtime(&now);
  ofstream outfile;
  outfile.open("work_log.txt", ios::app);
  outfile << ltm->tm_mday << "/" << 1 + ltm->tm_mon << "/"
          << 1900 + ltm->tm_year << " " << total << " " << worktime << " "
          << rest << " " << num_breaks << endl;
}
