#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H

#include <map>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>

#include "Definitions.h"

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    void playSound(PlaySound sound);
    void stopAllSound();
    void stopCurrentSound();

private:

    void playSoundCallback(const uint8_t* data, uint32_t length, std::atomic<bool> running);

    std::vector<uint8_t> loadFile(const char* path);

    std::map<PlaySound, std::vector<uint8_t>> soundMap;
    std::queue<std::pair<const uint8_t*, uint32_t>> soundQueue;
    std::atomic<bool> stopSoundAtomic;
    std::atomic<bool> stopThreadAtomic;
    std::mutex queueMutex;
    std::thread audioThread;


    static void audioThreadFunction(SoundManager* parent);
};


#endif // SOUND_MANAGER_H