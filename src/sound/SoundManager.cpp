
#include "SoundManager.h"
#include "../Managers.h"
#include "../AssetManager.h"
#include "Clip.h"
#include "Sample.h"

SoundManager::SoundManager() = default; // noop

SoundManager::~SoundManager() = default; // noop

void SoundManager::startUp() {
    logger = spdlog::get("main")->clone("sound");
	auto result = soloud.init();
    if (result != 0) {
        logger->error("Failed to initialise SoLoud: {}", result);
		return;
    }

	// Setup lua
	sol::table engineTable = gAssetManager.getLua()["engine"];
	sol::table soundTable = engineTable.create_named("sound");

	soundTable["play"] = [](StringId id) -> Clip { return gSoundManager.play(id); };

	// Register clip type
	soundTable.new_usertype<Clip>(
		"clip",
		"", sol::no_constructor,
		"volume", sol::property(&Clip::getVolume, &Clip::setVolume),
		"pan", sol::property(&Clip::getPan, &Clip::setPan)
	);
}

void SoundManager::shutDown() {
    soloud.deinit();
}

void SoundManager::update(float deltaTime) {

}

Clip SoundManager::play(StringId id) {
	auto sample = gAssetManager.loadSound(id);
	auto clip = Clip(soloud.play(sample->getSample()));

	return clip;
}

void SoundManager::setPan(Clip& clip, float pan) {
	soloud.setPan(clip.getHandle(), pan);
}

void SoundManager::setVolume(Clip& clip, float volume) {
	soloud.setVolume(clip.getHandle(), volume);
}

void SoundManager::setGlobalVolume(float volume) {
	soloud.setGlobalVolume(volume);
}

float SoundManager::getVolume(Clip& clip) {
	return soloud.getVolume(clip.getHandle());
}

float SoundManager::getGlobalVolume() {
	return soloud.getGlobalVolume();
}

float SoundManager::getPan(Clip& clip) {
	return soloud.getPan(clip.getHandle());
}
