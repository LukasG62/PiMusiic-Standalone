/**
 * @file main.c
 * @brief Banc d'essai du moteur audio avec Gnuplot
 */
#include <stdio.h>
#include <stdlib.h>

#include "sound/engine/instrument.h"
#include "sound/engine/voice.h"
#include "sound/engine/mixer.h"
#include "sound/plot/gnuwave.h"

#define SAMPLE_RATE 48000.0
#define FRAMES 480 

int main() {
    printf("Audio test : \n");

    instrument_t *inst_bass = instrument_create_default();
    inst_bass->oscType = OSC_TYPE_SIN;
    inst_bass->adsrConfig.attackTime = 0.0;
    inst_bass->adsrConfig.sustainLevel = 1.0;

    instrument_t *inst_lead = instrument_create_default();
    inst_lead->oscType = OSC_TYPE_TRIANGLE;
    inst_lead->adsrConfig.attackTime = 0.0;
    inst_lead->adsrConfig.sustainLevel = 0.6;
    inst_lead->fxCount = 1;
    inst_lead->fxTypes[0] = FX_TYPE_FUZZ;
    inst_lead->fxConfigs[0].fuzz.drive = 5.0;

    double buf_bass[FRAMES];
    double buf_lead[FRAMES];
    double buf_mixer[FRAMES];
    voice_t *voice_bass = voice_create(inst_bass);
    voice_t *voice_lead = voice_create(inst_lead);

    voice_note_on(voice_bass, 220.0);
    voice_note_on(voice_lead, 440.0);

    voice_process(voice_bass, SAMPLE_RATE, buf_bass, FRAMES);
    voice_process(voice_lead, SAMPLE_RATE, buf_lead, FRAMES);

    voice_destroy(voice_bass);
    voice_destroy(voice_lead);

    mixer_config_t mix_cfg = {
        .sampleRate = SAMPLE_RATE,
        .masterVolume = 0.8,
        .numChannels = 3,
        .maxFramesPerBuffer = 1024
    };
    mixer_t *mixer = mixer_create(&mix_cfg);

    mixer_play_on_channel(mixer, 0, inst_bass, 220.0);
    mixer_play_on_channel(mixer, 1, inst_lead, 440.0);
    mixer_process(mixer, buf_mixer, FRAMES);

    printf("Generate plot...\n");
    gnuplot_t *plot = gnuplot_create("Test mixer (10ms)", GNUPLOT_TYPE_TIME);

    gnuplot_add_curve(plot, buf_bass, FRAMES, SAMPLE_RATE, "Channel 0 (Sine Basse 220Hz)");
    gnuplot_add_curve(plot, buf_lead, FRAMES, SAMPLE_RATE, "Channel 1 (Triangle Fuzz 440Hz)");
    gnuplot_add_curve(plot, buf_mixer, FRAMES, SAMPLE_RATE, "Sortie Mixeur audio");

	if (gnuplot_render_live(plot) != 0) {
        printf("Erreur : Impossible de lancer Gnuplot.\n");
    }

    gnuplot_destroy(plot);
    mixer_destroy(mixer);
    instrument_destroy(inst_bass);
    instrument_destroy(inst_lead);

    printf("Done!\n");
    return 0;
}