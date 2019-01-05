#include "Clip.h"
#include "../Managers.h"
#include "SoundManager.h"

Clip::Clip(int handle) : handle(handle) {}

int Clip::getHandle() const {
	return handle;
}

void Clip::setPan(float pan) {
	gSoundManager.setPan(*this, pan);
}

float Clip::getPan() {
	return gSoundManager.getPan(*this);
}

void Clip::setVolume(float volume) {
	gSoundManager.setVolume(*this, volume);
}

float Clip::getVolume() {
	return gSoundManager.getVolume(*this);
}
