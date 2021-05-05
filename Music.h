
#ifndef MUSIC_H
#define MUSIC_H
#include <stdint.h>
#include "Timer0.h"
#include "Sound.h"
#include "DAC.h"

// a single note, with a volume from 0 - 63, a pitch, and a time for how long the note is played
typedef const struct note {
	uint16_t pitch;
	uint8_t volume;
	uint32_t time;
} Note;

class Music{
	bool playingMusic;
	Note* currSong;
	uint16_t songLen;
	void playNextNote(); // plays the next note in the currentSong and updates the length
	static void play(void);
	public:
	// initializes the interrupt timer and the sound module
	Music();
	// Plays your favorite song
  // inputs: a pointer to an array of notes, the length of that array
	void playSong(Note* song, uint16_t len);
	// Stops song
	void stopSong(void);
	// returns true if music is currently playing
	bool isPlaying(void);
};

#endif

