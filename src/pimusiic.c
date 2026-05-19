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
#include "event/event.h"
#include "ui/base/ui_manager.h"
#include "views.h"
#include "inet/request.h"
#include "music/sound.h"
#include "inet/mysyscall.h"
#include "core/app_logger.h"

volatile bool isRunning = true;
event_queue_t *uiQueue = NULL;
event_queue_t *logicQueue = NULL;
logger_t appLogger;


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
}

int main() {
    atexit(clean_up);
    //install_signal_handler(SIGINT, signal_handler, 0);
    
    // Forcer la fenêtre à être de la taille APP_LINES x APP_COLS
    char cmd[100];
    sprintf(cmd, "resize -s %d %d", APP_LINES, APP_COLS);
    system(cmd);

    app_context_t ctx;
    ctx.credentials.username[0] = '\0';
    ctx.credentials.password[0] = '\0';
    ctx.isConnected = false;
    init_music(&ctx.music, 120);

    uiQueue = create_event_queue();
    logicQueue = create_event_queue();
    logger_init(&appLogger, LOG_LEVEL_DEBUG, file_log_callback);
    core_init(uiQueue, logicQueue, &appLogger, &ctx);

    ui_manager_init();

    pthread_t logic_thread;
    pthread_create(&logic_thread, NULL, logic_event_loop_run, &ctx);

    ui_event_loop_run(&ctx);

    pthread_join(logic_thread, NULL);
    return EXIT_SUCCESS;
}
