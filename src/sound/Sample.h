#pragma once
#include <soloud_wav.h>

class Sample {
private:
	SoLoud::Wav sample;
public:
	explicit Sample(const char *file);


	SoLoud::Wav& getSample();
};
