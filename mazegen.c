/*
 * File:   mazegen.c
 * Author: matt
 *
 * Created on March 5, 2011, 2:10 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <sys/time.h>

#include "graph.h"

/* global maze window */
WINDOW* maze;
WINDOW* gw;
int rtimeout;

/*
 * initialize the ncurses library
 */
void
init_curses()
{
    initscr();
    cbreak();
    noecho();
    if (has_colors() == 0) {
        endwin();
        fprintf(stderr,"You terminal does not support color\n");
        exit(1);
    }
    start_color();

    maze = NULL;
    gw = NULL;
}

/*
 * draw game window box
 */
void
init_screen()
{
    int width,height,i,j;

    /* del if they already exist*/
    if (maze) delwin(maze);
    if (gw) delwin(gw);

    /* create game window + border */
    getmaxyx(stdscr,height,width);
    gw = newwin(0,0,1,0);
    box(gw,0,0);
    mvwprintw(stdscr,0,0,"Press 'q' to quit.");
    mvwprintw(stdscr,0,width-strlen("Speed: press + or -")-2
              ,"Speed: press + or -");

    wrefresh(stdscr);
    wrefresh(gw);

    /* get the number of rows and columns */
    getmaxyx(gw,height,width);
    maze = newwin(height-2,width-2,2,1);

    init_pair(1, COLOR_RED, COLOR_BLACK);
    wattron(maze,COLOR_PAIR(1));
    for (i=0; i<height; i++) {
        for (j=0; j<width; j++)
            mvwaddch(maze,i,j,' '|A_REVERSE);
    }
    wrefresh(maze);
    rtimeout = 0;
}

/*
 *  draw a section of the maze
 */
void
draw_maze(int x,int y)
{
    int ch;

    mvwaddch(maze,y,x,' ');
    wrefresh(maze);

    if ((ch = wgetch(maze))!=ERR) {
        if (ch == 'q') {
            endwin();
            exit(EXIT_SUCCESS);
        }
        if (ch == '+') {
            rtimeout-= 5; /* make it 5ms faster*/
            if (rtimeout<5) rtimeout = 5;
            wtimeout(maze,rtimeout);
        }
        if (ch == '-') {
            rtimeout+= 5; /* make it 5ms slower*/
            wtimeout(maze,rtimeout);
        }
    }
}

/*
 *
 */
int main()
{
    int width,height,ch;
    graph_t* mgraph;

    /* init random num generator */
    struct timeval tp;
    gettimeofday(&tp,NULL);
    srand((unsigned int)tp.tv_sec%tp.tv_usec);

    /* init ncurses display */
    init_curses();

    while (1) {
        /* create display windows */
        init_screen();

        /**** create graph ****/
        wtimeout(maze,0);
        getmaxyx(maze,height,width);
        mgraph = graph_init();
        graph_create(mgraph,width,height);

        /* create maze */
        rtimeout = 15;
        wtimeout(maze,rtimeout); /* so we can see what is drawn */
        graph_genmaze(mgraph);

        /* remove graph */
        graph_free(mgraph);

        /* draw restart request */
        mvwprintw(stdscr,0,width/2-(strlen("Press 'space' to restart")/2)
                  ,"Press 'space' to restart");
        wrefresh(stdscr);

        /* wait for correct input */
        wtimeout(maze,-1); /* blocking */
        while ((ch = wgetch(maze)) != 'q' && ch != ' ');

        /* quit */
        if (ch == 'q') break;
    }

    /* end curses mode */
    endwin();

    return (EXIT_SUCCESS);
}
