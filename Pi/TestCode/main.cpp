/*
 *  This extra small demo sends a random samples to your speakers.
 */
#include <alsa/asoundlib.h>
#include <vorbis/vorbisfile.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include "../src/SoundManager.h"
#include <thread>

SoundManager soundManager;

int main(void) {

    for (uint8_t i = 0; i < 5; i++) {

        std::cout << "PlaySound: " << std::endl;
        soundManager.playSound(PlaySound::BeepLow);
        soundManager.playSound(PlaySound::BeepHigh);

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    };

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    return 0;

}
