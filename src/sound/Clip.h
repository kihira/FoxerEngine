#pragma once

class Clip {
private:
	const int handle;
public:
	explicit Clip(int handle);

	/**
	 * Returns the handle used internally by SoLoud
	 */
	int getHandle() const;

	/**
	 * Sets the pan for the clip
	 * -1.f for full left, 0 for balanced and 1.f for full right
	 */
	void setPan(float pan);

	float getPan();

	void setVolume(float volume);

	float getVolume();
};
