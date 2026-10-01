/**
 * @file pimusiic.c
 * @details Application principale
 * @version 1.0
 * @author Tomas Salvado Robalo & Lukas Grando
*/

#include "common.h"

#include "core/core.h"
#include "core/logic_event_loop.h"
#include "core/ui_event_loop.h"
#include "core/app_logger.h"

#include "event/event.h"
#include "ui/base/ui_manager.h"
#include "inet/request.h"
#include "inet/mysyscall.h"
#include "sound/engine/mixer.h"
#include "sound/io/io_alsa.h"

#include "views.h"

#include "music/music.h"

volatile bool isRunning = true;
event_queue_t *uiQueue = NULL;
event_queue_t *logicQueue = NULL;
app_context_t *appContext = NULL;
logger_t appLogger;

static io_alsa_t *ioAudio = NULL;


static void signal_handler(int signal) {
    if (signal == SIGINT) {
        isRunning = false;
        // todo: nettoyer les ressources, fermer les threads, etc.
    }
}

/**
 * \fn static void clean_up()
 * \brief Fonction de nettoyage de l'application
 * \details Cette fonction permet de nettoyer l'application avant de quitter
 */
static void clean_up() {
    ui_manager_cleanup();
    logger_destroy(&appLogger);

    if(uiQueue) {
        destroy_event_queue(uiQueue);
        uiQueue = NULL;
    }
    if(logicQueue) {
        destroy_event_queue(logicQueue);
        logicQueue = NULL;
    }
    if(ioAudio) {
        io_alsa_destroy(ioAudio);
        ioAudio = NULL;
    }
    if(GET_APP_CONTEXT()->mixer) {
        mixer_destroy(GET_APP_CONTEXT()->mixer);
        GET_APP_CONTEXT()->mixer = NULL;
    }
}

int main() {
    atexit(clean_up);

    logger_init(&appLogger, LOG_LEVEL_DEBUG, file_log_callback);
    LOG_INFO_ASYNC(LOG_MOD_MAIN, "Application starting...");

    LOG_DEBUG_ASYNC(LOG_MOD_MAIN, "Installing signal handler");
    //install_signal_handler(SIGINT, signal_handler, 0);
    
    // Forcer la fenêtre à être de la taille APP_LINES x APP_COLS
    LOG_DEBUG_ASYNC(LOG_MOD_MAIN, "Resizing terminal to %dx%d", APP_COLS, APP_LINES);
    char cmd[100];
    sprintf(cmd, "resize -s %d %d", APP_LINES, APP_COLS);
    system(cmd);

    LOG_DEBUG_ASYNC(LOG_MOD_MAIN, "Initializing application context");
    app_context_t ctx;
    ctx.credentials.username[0] = '\0';
    ctx.credentials.password[0] = '\0';
    ctx.isConnected = false;
    music_init(&ctx.music, 120, "Untitled");

    mixer_config_t config = {
        .numChannels = MUSIC_MAX_CHANNELS, 
        .sampleRate = 44100, 
        .maxFramesPerBuffer = 512, 
        .masterVolume = 0.8
    };
    ctx.mixer = mixer_create(&config);
    if (!ctx.mixer) {
        LOG_FATAL_ASYNC(LOG_MOD_MAIN, "Failed to initialize mixer");
        return EXIT_FAILURE;
    }


    LOG_DEBUG_ASYNC(LOG_MOD_MAIN, "Creating event queues");
    uiQueue = create_event_queue();
    logicQueue = create_event_queue();

    LOG_DEBUG_ASYNC(LOG_MOD_MAIN, "Initializing audio output");
    ioAudio = io_alsa_create(ctx.mixer, "default");
    if (!ioAudio || !io_alsa_start(ioAudio)) {
        LOG_FATAL_ASYNC(LOG_MOD_MAIN, "Failed to initialize ALSA");
        mixer_destroy(ctx.mixer);
        return EXIT_FAILURE;
    }

    LOG_DEBUG_ASYNC(LOG_MOD_MAIN, "Initializing core");
    core_init(uiQueue, logicQueue, &appLogger, &ctx);

    LOG_DEBUG_ASYNC(LOG_MOD_MAIN, "Initializing UI manager");
    ui_manager_init();

    LOG_INFO_ASYNC(LOG_MOD_MAIN, "Entering main event loop");
    pthread_t logic_thread;
    pthread_create(&logic_thread, NULL, logic_event_loop_run, &ctx);
    ui_event_loop_run(&ctx);

    LOG_INFO_ASYNC(LOG_MOD_MAIN, "Shutting down application");
    pthread_join(logic_thread, NULL);

    return EXIT_SUCCESS;
}
