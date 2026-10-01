/**
 * @file logic_event_loop.c
 * @brief Implémentation de la boucle d'événements logique et du Séquenceur
 */
#include "core/logic_event_loop.h"
#include "event/event.h"

#include "music/music.h"
#include "music/note.h"
#include "music/step.h"
#include "sound/engine/instrument.h"

#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define DEFAULT_BPM 120
#define TICKS_PER_BEAT MUSIC_TIME_NOIRE

#define IDLE_SLEEP_NS (5 * NS_PER_MS)

extern const note_scale_t note_scale; 

/**
 * @brief État interne du séquenceur de lecture
 */
typedef struct {
    bool isPlaying;
    uint16_t currentBpm;
    int currentLines[MUSIC_MAX_CHANNELS];  
    int ticksRemaining[MUSIC_MAX_CHANNELS]; 
} player_state_t;

/**
 * @brief Calcule la durée d'un tick en nanosecondes
 */
static inline long calculate_tick_ns(uint16_t bpm) {
    if (bpm == 0) bpm = DEFAULT_BPM;
    return (NS_PER_MINUTE / (bpm * TICKS_PER_BEAT));
}

/**
 * @brief Notifie l'UI qu'une note a été déclenchée
 */
static void notify_ui_note_played(int channel, int line) {
    event_note_played_data_t *payload = malloc(sizeof(event_note_played_data_t));
    if (payload) {
        payload->channelId = channel;
        payload->lineIndex = line;
        event_t *ev = create_event(UI_EVENT_NOTE_PLAYED, payload, free);
        notify_event(GET_UI_QUEUE(), ev);
    }
}

/**
 * @fn void *logic_event_loop_run(void *arg)
 * @brief Point d'entrée du thread logique
 * @param arg Arguments du thread
 */
void *logic_event_loop_run(void *arg) {
    app_context_t *ctx = (app_context_t *)arg;
    mixer_t *mixer = ctx->mixer;
    music_t *music = &(ctx->music);

    player_state_t player = {0};
    player.currentBpm = music->baseBpm > 0 ? music->baseBpm : DEFAULT_BPM;

    struct timespec next_tick_time;
    long tick_duration_ns = calculate_tick_ns(player.currentBpm);

    while (isRunning) {
        event_t *ev;
        while ((ev = trywait_event(GET_LOGIC_QUEUE())) != NULL) {
            switch (ev->type) {
                case UI_EVENT_MUSICPLAYBACK_STARTED:
                    player.isPlaying = true;
                    player.currentBpm = music->baseBpm;
                    tick_duration_ns = calculate_tick_ns(player.currentBpm);
                    for (int i = 0; i < MUSIC_MAX_CHANNELS; i++) {
                        player.currentLines[i] = 0;
                        player.ticksRemaining[i] = 0;
                    }
                    clock_gettime(CLOCK_MONOTONIC, &next_tick_time);
                    break;

                case UI_EVENT_MUSICPLAYBACK_STOPPED:
                    player.isPlaying = false;
                    for (int i = 0; i < MUSIC_MAX_CHANNELS; i++) {
                        mixer_stop_channel(mixer, i);
                    }
                    break;

                case LOGIC_EVENT_NONE: /*!< Emis quand il n'y a pas d'event (sert à diviser les types d'events) */
                    break;
                case LOGIC_EVENT_REQUEST_SENT: /*!< Emis quand une requête a été envoyé au serveur */
                    break;
                case LOGIC_EVENT_RESPONSE_RECEIVED: /*!< Emis quand une reponse a été reçue du serveur */
                    break;
                case LOGIC_EVENT_REQUEST_FAILED: /*!< Emis quand une requête a échoué */
                    break;
                case LOGIC_EVENT_NOTE_PLAYED: /*!< Emis quand une note a été jouée sur la carte son */
                    break;
                case LOGIC_EVENT_SOUNDCARD_ERROR: /*!< Emis quand il y a une erreur avec la carte son */
                    player.isPlaying = false;
                    break;
                case LOGIC_EVENT_AUTOSAVE_STARTED: /*!< Emis quand l'autosave commence */
                    break;
                case LOGIC_EVENT_MUSIC_SAVED: /*!< Emis quand la musique a été sauvegardée sur le serveur */
                    break;
                default:
                    break;
            }
            destroy_event(ev);
        }

        if (player.isPlaying) {
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);

            if (now.tv_sec > next_tick_time.tv_sec || 
               (now.tv_sec == next_tick_time.tv_sec && now.tv_nsec >= next_tick_time.tv_nsec)) {
                
                for (int c = 0; c < MUSIC_MAX_CHANNELS; c++) {
                    if (player.ticksRemaining[c] > 0) {
                        player.ticksRemaining[c]--;
                        continue;
                    }

                    int currentLine = player.currentLines[c];
                    if (currentLine >= MUSIC_CHANNEL_MAX_STEPS) continue;

                    music_step_t step = music->channels[c].steps[currentLine];

                    if (step.type == MUSIC_STEP_TYPE_NOTE) {
                        double freq = note_get_frequency(step.data.note.noteId, step.data.note.octave);
                        instrument_t *mock_inst = NULL;
                        
                        mixer_play_on_channel(mixer, c, mock_inst, freq);
                        notify_ui_note_played(c, currentLine);
                        
                        player.ticksRemaining[c] = step.duration > 0 ? step.duration - 1 : 0;
                    } 
                    else if (step.type == MUSIC_STEP_TYPE_REST) {
                        mixer_stop_channel(mixer, c);
                        player.ticksRemaining[c] = step.duration > 0 ? step.duration - 1 : 0;
                    }
                    else if (step.type == MUSIC_STEP_TYPE_COMMAND) {
                        if (step.data.cmd.type == MUSIC_CMD_SET_BPM) {
                            player.currentBpm = step.data.cmd.param.bpm.bpm;
                            tick_duration_ns = calculate_tick_ns(player.currentBpm);
                        }
                        else if (step.data.cmd.type == MUSIC_CMD_RESET_BPM) {
                            player.currentBpm = music->baseBpm;
                            tick_duration_ns = calculate_tick_ns(player.currentBpm);
                        }
                        player.ticksRemaining[c] = step.duration > 0 ? step.duration - 1 : 0;
                    }
                    player.currentLines[c]++;
                }

                next_tick_time.tv_nsec += tick_duration_ns;
                while (next_tick_time.tv_nsec >= NS_PER_SEC) {
                    next_tick_time.tv_nsec -= NS_PER_SEC;
                    next_tick_time.tv_sec += 1;
                }
            }

            clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &next_tick_time, NULL);
            
        } else {
            struct timespec pause_sleep = {0, IDLE_SLEEP_NS};
            nanosleep(&pause_sleep, NULL);
        }
    }
    return NULL;
}