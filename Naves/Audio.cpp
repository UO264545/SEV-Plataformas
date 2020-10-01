#include "Audio.h"

Audio::Audio(string filename, bool loop) {
	this->loop = loop;

	if (loop) {
		// Uso la libreria Mixer - mp3
		Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096); // 2 canales
		mix = Mix_LoadMUS(filename.c_str());
	} else {
		// Uso SDL audio standard
		SDL_LoadWAV(filename.c_str(), &wavSpec, &wavBuffer, &wavLength);
		deviceID = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);
	}
}

Audio::~Audio() {
	if (loop) {
		Mix_FreeMusic(mix);
		Mix_CloseAudio();
	}
	else {
		SDL_CloseAudioDevice(deviceID);
		SDL_FreeWAV(wavBuffer);
	}
}

void Audio::play() {
	if (loop) {
		Mix_PlayMusic(mix, -1);
		// -1 se repite sin parar
	}
	else {
		// hay más de 4 en cola
		if (SDL_GetQueuedAudioSize(deviceID) > wavLength * 4) {
			SDL_ClearQueuedAudio(deviceID); // limpiar
		}
		SDL_QueueAudio(deviceID, wavBuffer, wavLength);
		SDL_PauseAudioDevice(deviceID, 0);
	}
}
