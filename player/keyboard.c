/**
* Copyright (c) 2013 Franklin Wong, Ben Duong
* All Rights Reserved
*/
/**
* @file
*
* @brief Capture user input from the keyboard
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>

#include "keyboard.h"
#include "player.h"
#include "playlist.h"


/**
* @brief Captures the user input from standard input.

void *keyboard(void * arg)
{
	while (1)
	{
		int res = fgetc(stdin);
		if (res == 'p') //pause/play
		{
			pause_play_flag = !pause_play_flag;
		}
		else if (res == 'b') { //back
			stop_flag = STOP;
			pause_play_flag = PLAY;
			song_choice = (song_choice - 1);
			if (song_choice < 0) song_choice = number_of_songs - 1;	
		}
		else if (res == 'n') { //next
			stop_flag = STOP;
			pause_play_flag = PLAY;
			song_choice = (song_choice + 1) % number_of_songs;		
		}
		else if (res == 's') //stop
		{
			stop_flag=STOP;
			pause_play_flag = PAUSE;
		} 
		else if (res == 'q') { //quit
			total_stop = 1;
			stop_flag = STOP;
			pause_play_flag;
		}
	}
}*/

//Capture from /proc/blabla

void *keyboard(void)
{

	char buf[1024]={0,};
	int score=0;
	int prev_score=0;
	
	while(1){

		FILE *fd=fopen("/proc/hcsr04","r");

		while(fgets(buf, 1024, fd)!=NULL){ 
			// Just search for the latest line, do nothing in the loop
		} 
		//printf("Last line %s\n", buf); //<this is just a log... you can remove it
		fclose(fd);
		sscanf(buf, "%d", &score);
		//printf("scoreIgot: %d\n", score);
		//end of the game
		if(score==-1 && prev_score!=-1){
printf("scoreIgot: %d %d\n", prev_score, score);
			stop_flag = STOP;
			song_choice = 0; //0--champions.mp3//(song_choice + 1) % 2;//number of songs
			pause_play_flag = PLAY;
			//sleep(10);
			//stop_flag=STOP;
			//pause_play_flag = PAUSE;	
		
		}
		else if(prev_score==0 && score==-2){
			printf("scoreIgot: %d %d\n", prev_score, score);
			stop_flag = STOP;
			pause_play_flag = PAUSE;
			stop_flag = STOP;
			song_choice = 1; // 1--letsgo.mp3
			pause_play_flag = PLAY;		
		}
		else if(prev_score==-1 && score!=-1){
printf("scoreIgot: %d %d\n", prev_score, score);
			stop_flag = STOP;
			pause_play_flag = PAUSE;
			stop_flag = STOP;
			song_choice = 1; // 1--letsgo.mp3
			pause_play_flag = PLAY;
		}
		
		//restart or start
		/*else if(score==-2 && prev_score!=-2){

			stop_flag = STOP;
			song_choice = 1; // 1--letsgo.mp3
			pause_play_flag = PLAY;
			//sleep(5);
			//stop_flag=STOP;
			//pause_play_flag = PAUSE;	
		}*/
		else if(prev_score==-2 && score!=-2){
			stop_flag = STOP;
			pause_play_flag = PAUSE;
		}
	
		//quit program
		else if(score==-3){
			total_stop = 1;
			stop_flag = STOP;
			pause_play_flag;
		}
		//a basket!
		else if(prev_score < score && score!=0){
			stop_flag = STOP;
			song_choice = 2; // 2--clap.mp3
			pause_play_flag = PLAY;
			//play the song for 3 secs
			sleep(3);
			stop_flag=STOP;
			pause_play_flag = PAUSE;		
		}
	

		sscanf(buf, "%d", &prev_score);
		//sleep(1);
	}
	
}












