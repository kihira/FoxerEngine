#include "Sample.h"
#include <spdlog/spdlog.h>

Sample::Sample(const char *file) {
	if (sample.load(file) != 0) {
		spdlog::get("sound")->error("Failed to load sound file {}", file);
	}
}

SoLoud::Wav& Sample::getSample() {
	return sample;
}
