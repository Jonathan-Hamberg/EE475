
#include <alsa/asoundlib.h>
#include <alsa/pcm.h>

#include "SoundManager.h"
#include <stdio.h>
#include <iostream>
#include <vorbis/vorbisfile.h>
#include <algorithm>

void SoundManager::audioThreadFunction(SoundManager *parent) {
    int err;
    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
    bool shouldPrepare = false;
    bool isQueueEmpty;

    const uint8_t *buffer = nullptr;
    size_t buffer_remaining = 0;

    if ((err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
    }

    if ((err = snd_pcm_set_params(handle, // pcm handle
                                  SND_PCM_FORMAT_S16, // format
                                  SND_PCM_ACCESS_RW_INTERLEAVED, // access
                                  1, // channels
                                  48000, // rate
                                  1, // soft re-sample
                                  500000) // latency (us)
        ) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
    }

    while (!parent->stopThreadAtomic) {
        if (parent->stopSoundAtomic) {
            buffer = nullptr;
            buffer_remaining = 0;
            parent->stopSoundAtomic = false;
        }

        // Get the data buffer for a new sound.
        parent->queueMutex.lock();
        // Determine if there is a sound in the message queue.
        if (!parent->soundQueue.empty() && buffer_remaining == 0) {
            // Get the sound data buffer.
            buffer = parent->soundQueue.front().first;
            // Get the length of the sound.
            buffer_remaining = parent->soundQueue.front().second;
            // Pop the front from the queue.
            parent->soundQueue.pop();
        }
        // Determine if the queue is now empty.
        isQueueEmpty = parent->soundQueue.empty();
        parent->queueMutex.unlock();


        // If there is data to play then send it to the sound card.
        if (buffer_remaining > 0) {
            parent->soundDoneAtomic = false;

            // Buffer size.
            size_t frames_remaining = std::min(size_t(4410), buffer_remaining / 2);

            if (shouldPrepare) {
                snd_pcm_prepare(handle);
                shouldPrepare = false;
            }

            // Write data to the sound buffer.
            frames = snd_pcm_writei(handle, buffer, frames_remaining);

            if (frames < 0)
                frames = snd_pcm_recover(handle, frames, 0);
            if (frames < 0) {
                printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
                break;
            }

            // Update the current place in the buffer.
            buffer += frames * 2;
            buffer_remaining -= frames * 2;


            // Drop the sound if no data is left.
            if (buffer_remaining <= 0 && isQueueEmpty) {
                snd_pcm_drain(handle);
                shouldPrepare = true;
            }


        } else {
            parent->soundDoneAtomic = true;
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

    }
}


SoundManager::SoundManager() {
    soundMap.emplace(PlaySound::Defused, loadFile("Resources/defused.ogg"));
    soundMap.emplace(PlaySound::Detonate, loadFile("Resources/detonate.ogg"));
    soundMap.emplace(PlaySound::Key1, loadFile("Resources/key01.ogg"));
    soundMap.emplace(PlaySound::Key2, loadFile("Resources/key02.ogg"));
    soundMap.emplace(PlaySound::Key3, loadFile("Resources/key03.ogg"));
    soundMap.emplace(PlaySound::Key4, loadFile("Resources/key04.ogg"));
    soundMap.emplace(PlaySound::Lose, loadFile("Resources/lose.ogg"));
    soundMap.emplace(PlaySound::Strike, loadFile("Resources/strike.ogg"));
    soundMap.emplace(PlaySound::Win, loadFile("Resources/win.ogg"));
    soundMap.emplace(PlaySound::BeepLow, loadFile("Resources/beepLow.ogg"));
    soundMap.emplace(PlaySound::BeepHigh, loadFile("Resources/beepHigh.ogg"));
    soundMap.emplace(PlaySound::Setup, loadFile("Resources/setup.ogg"));
    soundMap.emplace(PlaySound::LightBuzz, loadFile("Resources/lightBuzz.ogg"));

    // Print debug message with the size of the sound data loaded.
    uint64_t bytes = 0;
    for (const auto &kv : soundMap) {
        bytes += kv.second.size();
    }
    std::cout << "Loaded " << bytes << " bytes of sound data." << std::endl;

    // Start the audio thread.
    audioThread = std::thread(audioThreadFunction, this);
    // Disable the stopSound flag.
    stopSoundAtomic = false;
    // Disable the stopThread flag.
    stopThreadAtomic = false;
    // No sounds are playing.
    soundDoneAtomic = true;
}

SoundManager::~SoundManager() {
    stopThreadAtomic = true;
    audioThread.join();
}

void SoundManager::playSound(PlaySound sound) {
    // Determine if there is data for the requested sound.
    if (soundMap.count(sound) != 0) {
        queueMutex.lock();
        // Send the message to the audio thread to with the buffer address and size.
        soundQueue.push({soundMap[sound].data(), soundMap[sound].size()});
        queueMutex.unlock();
    }
}

void SoundManager::stopAllSound() {

    // Remove all queued sounds.
    queueMutex.lock();
    while (!soundQueue.empty()) {
        soundQueue.pop();
    }
    queueMutex.unlock();

    // Stop the current sound.
    stopSoundAtomic = true;
}

void SoundManager::stopCurrentSound() {
    stopSoundAtomic = true;
}

std::vector<uint8_t> SoundManager::loadFile(const char *path) {
    OggVorbis_File vf;
    FILE *f = fopen(path, "r");

    if (ov_open_callbacks(f, &vf, nullptr, 0, OV_CALLBACKS_NOCLOSE) < 0) {
        fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
    }

    vorbis_info *vi = ov_info(&vf, -1);

    std::vector<uint8_t> soundData;

    if (vi->channels != 1) {
        std::cout << "loadFile() only 2 channel audio streams supported." << std::endl;
        return soundData;
    }

    if (vi->rate != 44100) {
        std::cout << "loadfile() only supports 44100 Hz audio streams." << std::endl;
        return soundData;
    }

    uint8_t buffer[4096];
    long ret;
    int current_section;
    // Read data from the Vorbis stream until the end of the file.
    while ((ret = ov_read(&vf, (char *) buffer, sizeof(buffer), 0, 2, 1, &current_section)) > 0) {
        for (uint16_t i = 0; i < ret; i++) {
            soundData.push_back(buffer[i]);
        }
    }

    return soundData;
}

bool SoundManager::isSoundDone() {
    return soundDoneAtomic;
}
