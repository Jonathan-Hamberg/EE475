#ifndef DEFUSER_ALSASOUND_H
#define DEFUSER_ALSASOUND_H


class AlsaSound {
    explicit AlsaSound(const char*path);

    void load();

    void play();
};


#endif //DEFUSER_ALSASOUND_H
