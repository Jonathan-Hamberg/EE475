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

static char *device = "default";                        /* playback device */
snd_output_t *output = NULL;
unsigned char buffer[2 * 16 * 1024];                          /* some random data */
int main(void) {


    soundManager.playSound(PlaySound::Key1);
    soundManager.playSound(PlaySound::Key2);
    soundManager.playSound(PlaySound::Key3);
    soundManager.playSound(PlaySound::Key4);
    soundManager.playSound(PlaySound::Win);
    soundManager.playSound(PlaySound::Lose);
    soundManager.playSound(PlaySound::Win);


    std::string input;
    std::cin >> input;

    soundManager.stopAllSound();

    std::cin >> input;

    return 0;

    int err;
    unsigned int i;
    snd_pcm_t *handle;
    snd_pcm_sframes_t frames;
    for (i = 0; i < sizeof(buffer); i++)
        buffer[i] = random() & 0xff;

    if ((err = snd_pcm_open(&handle, device, SND_PCM_STREAM_PLAYBACK, 0)) < 0) {
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    if ((err = snd_pcm_set_params(handle,
                                  SND_PCM_FORMAT_S16,
                                  SND_PCM_ACCESS_RW_INTERLEAVED,
                                  2,
                                  48000,
                                  1,
                                  500000)) < 0) {   /* 0.5sec */
        printf("Playback open error: %s\n", snd_strerror(err));
        exit(EXIT_FAILURE);
    }

    OggVorbis_File vf;
    FILE *f = fopen("win.ogg", "r");

    if (ov_open_callbacks(f, &vf, NULL, 0, OV_CALLBACKS_NOCLOSE) < 0) {
        fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
    }

    vorbis_info *vi = ov_info(&vf, -1);

    std::vector<uint8_t> data;
    uint8_t buffer[4096];
    long ret;
    int current_section, eof;
    while ((ret = ov_read(&vf, (char *) buffer, sizeof(buffer), 0, 2, 1, &current_section)) > 0) {
        for (uint16_t i = 0; i < ret; i++) {
            data.push_back(buffer[i]);
        }
    }

    uint8_t *dataBuffer = data.data();

    snd_pcm_sframes_t numFrames = data.size() / 4;
    while (numFrames > 0) {
        snd_pcm_sframes_t frames = snd_pcm_writei(handle, dataBuffer, numFrames);
        dataBuffer += frames * 4;
        numFrames -= frames;
    }

    return 0;
    while (!eof) {
        long ret = ov_read(&vf, (char *) buffer, sizeof(buffer), 0, 2, 1, &current_section);
        if (ret == 0) {
            /* EOF */
            eof = 1;
        } else if (ret < 0) {
            /* error in the stream.  Not a problem, just reporting it in
           case we (the app) cares.  In this case, we don't. */
        } else {
            /* we don't bother dealing with sample rate changes, etc, but
           you'll have to*/
            snd_pcm_sframes_t frames = snd_pcm_writei(handle, buffer, ret / 2);
            std::cout << frames << std::endl;
        }
    }
//
//    for (i = 0; i < 16; i++) {
//        frames = snd_pcm_writei(handle, buffer, sizeof(buffer) / 4);
//        if (frames < 0)
//            frames = snd_pcm_recover(handle, frames, 0);
//        if (frames < 0) {
//            printf("snd_pcm_writei failed: %s\n", snd_strerror(frames));
//            break;
//        }
//        if (frames > 0 && frames < (long) sizeof(buffer))
//            printf("Short write (expected %li, wrote %li)\n", (long) sizeof(buffer), frames);
//    }
    snd_pcm_close(handle);
    return 0;
}
