#include "Music.h"

// Music.c
// This program can use timer0A and timer1A ISR  
// playing your favorite song.
//
// For use with the TM4C123
// EE319K lab6 extra credit
// Program written by: George Koussa & Anuj Jain
// 3/24/21 

#define msToClockCycle(t) (t*80000) //gets num of clock cycles for t ms, assumes 80 Mhz clock

void Timer_Init(void);
void Timer_Stop(void);
void Timer0A_Handler(void);


Music* m;

// initialized the interrupt timer and the sound module, run once
Music::Music(void){
	Sound_Init();
	Timer0_Init();
	m = this;
	songLen = 0;
}

// plays the next note in the currentSong and updates the length
void Music::playNextNote(){
		Sound_SetVolume(currSong->volume);// set volume
		Sound_Start(currSong->pitch);			// set pitch
		currSong = currSong + 1;					// go to the next note
		songLen--;												// update length
}

// Play song, passed as a pointer to an array of notes along with its length
void Music::playSong(Note* song, uint16_t len){
	if(len > 0){
		playingMusic = 1;											// music is starting to be played
		songLen = len;
		currSong = song;
		play();																// start playing song
	} else {
		stopSong();														// dont play anything if there are no notes to play
	}
}

// Stop song
void Music::stopSong(void){
	Timer0_Stop();														// stop the timer
	songLen = 0;		
	Sound_SetVolume(63);										// set volume to default value
	Sound_Off();														// turn off sound
	playingMusic = 0;												// music is not longer being played
}

// Timer0A acts as a metronome for the song(s)
void Music::play(void){
	if(m->songLen > 0){	// more notes to play
		Timer0_Init(*play, msToClockCycle(m->currSong->time));				// set timer to the time that the note should be played for
		m->playNextNote();																						// play the note
	} else {
		m->stopSong();																					      // no more notes to play
	}
}


