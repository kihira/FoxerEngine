
#ifndef GAMEENGINE301CR_SOUNDMANAGER_H
#define GAMEENGINE301CR_SOUNDMANAGER_H

#include <soloud.h>
#include <spdlog/spdlog.h>
#include "../util/StringId.h"
#include "../event/EventHandler.h"


class Clip;

class SoundManager final : public EventHandler {
private:
    std::shared_ptr<spdlog::logger> logger;
    SoLoud::Soloud soloud;
public:
    SoundManager();

    ~SoundManager();

    /**
     * Initialises the SoundManager
     */
    void startUp();

    /**
     * De-initialises the SoundManager
     */
    void shutDown();

    void update(float deltaTime);

	Clip play(StringId id);

	/**
	 * Sets the pan value for a clip
	 * -1.f for full left, 0 for balanced and 1.f for full right
	 */
	void setPan(Clip &clip, float pan);

	/**
	 * Sets the volume for the current clip
	 */
	void setVolume(Clip &clip, float volume);

	/**
	 * Sets the global volume for all clips
	 */
	void setGlobalVolume(float volume);

	float getVolume(Clip &clip);

	float getGlobalVolume();

    float getPan(Clip& clip);

    bool onEvent(Event& event) override;
};


#endif //GAMEENGINE301CR_SOUNDMANAGER_H
