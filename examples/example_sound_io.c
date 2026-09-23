#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "sound/engine/instrument.h"
#include "sound/engine/voice.h"
#include "sound/engine/mixer.h"
#include "sound/io/io_alsa.h"
#include "sound/io/io_pmsd.h" 
#include "sound/osc/osc_resampler.h"

#define SAMPLE_FILE "data/overdriven_guitar.pmsd"

#define FREQ_BB2 116.54
#define FREQ_B2  123.47
#define FREQ_C3  130.81
#define FREQ_D3  146.83
#define FREQ_F3  174.61
#define FREQ_G3  196.00
#define FREQ_GS3 207.65
#define FREQ_A3  220.00
#define FREQ_D4  293.66

typedef struct {
    double frequency;
    double timeStart;
    double timeValue;
} example_note_t;

int main() {
    io_pmsd_t harpsichordPmsd;
    if (!io_pmsd_load(SAMPLE_FILE, &harpsichordPmsd)) {
        return 1;
    }
    
    instrument_t *instLead = instrument_create_default();
    osc_resampler_config_from_pmsd(&instLead->oscConfig.resampler, &harpsichordPmsd);
    
    instLead->oscConfig.resampler.loopEnabled = true;
    instLead->oscConfig.resampler.loopStart = 0;
    instLead->oscConfig.resampler.loopEnd = instLead->oscConfig.resampler.sampleLength;
    instLead->oscType = OSC_TYPE_RESAMPLER;
    
    instLead->adsrConfig.enabled = true;
    instLead->adsrConfig.attackTime = 0.05;
    instLead->adsrConfig.sustainLevel = 0.5;
    instLead->adsrConfig.releaseTime = 0.2;

    mixer_config_t mixCfg = {
        .sampleRate = 48000.0,
        .masterVolume = 0.8,
        .numChannels = 1,
        .maxFramesPerBuffer = 512
    };
    mixer_t *mixer = mixer_create(&mixCfg);

    io_alsa_t *io = io_alsa_create(mixer, "default");
    if (!io || !io_alsa_start(io)) {
        return 1;
    }

    usleep(500000);

    example_note_t sequence[] = {
        {FREQ_D3, 0.0, 0.25}, {FREQ_D3, 0.25, 0.25}, {FREQ_D4, 0.5, 0.25}, {FREQ_A3, 1.0, 0.25}, 
        {FREQ_GS3, 1.75, 0.25}, {FREQ_G3, 2.25, 0.25}, {FREQ_F3, 2.75, 0.5}, {FREQ_D3, 3.25, 0.25}, {FREQ_F3, 3.5, 0.25}, {FREQ_G3, 3.75, 0.25},
        {FREQ_C3, 4.0, 0.25}, {FREQ_C3, 4.25, 0.25}, {FREQ_D4, 4.5, 0.25}, {FREQ_A3, 5.0, 0.25}, 
        {FREQ_GS3, 5.75, 0.25}, {FREQ_G3, 6.25, 0.25}, {FREQ_F3, 6.75, 0.5}, {FREQ_D3, 7.25, 0.25}, {FREQ_F3, 7.5, 0.25}, {FREQ_G3, 7.75, 0.25},
        {FREQ_B2, 8.0, 0.25}, {FREQ_B2, 8.25, 0.25}, {FREQ_D4, 8.5, 0.25}, {FREQ_A3, 9.0, 0.25}, 
        {FREQ_GS3, 9.75, 0.25}, {FREQ_G3, 10.25, 0.25}, {FREQ_F3, 10.75, 0.5}, {FREQ_D3, 11.25, 0.25}, {FREQ_F3, 11.5, 0.25}, {FREQ_G3, 11.75, 0.25},
        {FREQ_BB2, 12.0, 0.25}, {FREQ_BB2, 12.25, 0.25}, {FREQ_D4, 12.5, 0.25}, {FREQ_A3, 13.0, 0.25}, 
        {FREQ_GS3, 13.75, 0.25}, {FREQ_G3, 14.25, 0.25}, {FREQ_F3, 14.75, 0.5}, {FREQ_D3, 15.25, 0.25}, {FREQ_F3, 15.5, 0.25}, {FREQ_G3, 15.75, 0.25}
    };

    int bpm = 120;
    double beatDurationUs = (60.0 / bpm) * 1000000.0;
    int numNotes = sizeof(sequence) / sizeof(sequence[0]);

    for (int loop = 0; loop < 2; loop++) {
        double currentTimeBeats = 0.0;
        
        for (int i = 0; i < numNotes; i++) {
            double waitBeats = sequence[i].timeStart - currentTimeBeats;
            if (waitBeats > 0.0) {
                usleep((useconds_t)(waitBeats * beatDurationUs));
                currentTimeBeats += waitBeats;
            }
            
            mixer_play_on_channel(mixer, 0, instLead, sequence[i].frequency);
            
            usleep((useconds_t)(sequence[i].timeValue * beatDurationUs));
            currentTimeBeats += sequence[i].timeValue;
            
            mixer_stop_channel(mixer, 0);
        }
        
        double loopRemainder = 16.0 - currentTimeBeats;
        if (loopRemainder > 0.0) {
            usleep((useconds_t)(loopRemainder * beatDurationUs));
        }
    }

    io_alsa_stop(io);
    io_alsa_destroy(io);
    mixer_destroy(mixer);
    instrument_destroy(instLead);
    io_pmsd_destroy(&harpsichordPmsd); 

    return 0;
}