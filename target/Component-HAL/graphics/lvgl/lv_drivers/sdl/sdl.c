/**
 * @file sdl.h
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "sdl.h"
#if USE_MONITOR || USE_SDL

#if LV_USE_GPU_SDL
# error "LV_USE_GPU_SDL must not be enabled"
#endif

#if USE_MONITOR
# warning "MONITOR is deprecated, use SDL instead. See lv_drivers/sdl/sdl.c"
#endif

#if USE_KEYBOARD
# warning "KEYBOARD is deprecated, use SDL instead. See lv_drivers/sdl/sdl.c"
#endif

#if USE_MOUSE
# warning "MOUSE is deprecated, use SDL instead. See lv_drivers/sdl/sdl.c"
#endif

#if USE_MOUSEWHEEL
# warning "MOUSEWHEEL is deprecated, use SDL instead that. See lv_drivers/sdl/sdl.c"
#endif

#if USE_MONITOR && USE_SDL
# error "Cannot enable both MONITOR and SDL at the same time. "
#endif

#if USE_MONITOR
# define SDL_HOR_RES            MONITOR_HOR_RES
# define SDL_VER_RES            MONITOR_VER_RES
# define SDL_ZOOM               MONITOR_ZOOM
# define SDL_DOUBLE_BUFFERED    MONITOR_DOUBLE_BUFFERED
# define SDL_INCLUDE_PATH       MONITOR_SDL_INCLUDE_PATH
# define SDL_VIRTUAL_MACHINE    MONITOR_VIRTUAL_MACHINE
# define SDL_DUAL_DISPLAY       MONITOR_DUAL
#endif

#ifndef SDL_FULLSCREEN
# define SDL_FULLSCREEN        0
#endif

#include "sdl_common_internal.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#endif
#include SDL_INCLUDE_PATH

/*********************
 *      DEFINES
 *********************/
#ifndef KEYBOARD_BUFFER_SIZE
#define KEYBOARD_BUFFER_SIZE SDL_TEXTINPUTEVENT_TEXT_SIZE
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef struct {
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Texture * texture;
    volatile bool sdl_refr_qry;
#if SDL_DOUBLE_BUFFERED
    uint32_t * tft_fb_act;
#else
    uint32_t * tft_fb;
#endif
}monitor_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void window_create(monitor_t * m);
static void window_update(monitor_t * m);
static void monitor_sdl_clean_up(void);
static void sdl_event_handler(lv_timer_t * t);
static void monitor_sdl_refr(lv_timer_t * t);

/***********************
 *   GLOBAL PROTOTYPES
 ***********************/

/**********************
 *  STATIC VARIABLES
 **********************/
monitor_t monitor;

#if SDL_DUAL_DISPLAY
monitor_t monitor2;
#endif

static volatile bool sdl_inited = false;

static bool left_button_down = false;
static int16_t last_x = 0;
static int16_t last_y = 0;

static int16_t wheel_diff = 0;
static lv_indev_state_t wheel_state = LV_INDEV_STATE_RELEASED;

static char buf[KEYBOARD_BUFFER_SIZE];

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

void sdl_init(void)
{
    /* libdrm 2.4.113+ (Debian 12) requires XDG_RUNTIME_DIR for KMS/DRM device access.
     * When the app is started outside a login session (e.g. init script, systemd service)
     * this variable is unset and SDL's KMS backend silently fails to open the display. */
#ifdef __linux__
    if (getenv("XDG_RUNTIME_DIR") == NULL) {
        char path[64];
        snprintf(path, sizeof(path), "/run/user/%d", (int)getuid());
        mkdir(path, 0700); /* no-op if already exists */
        setenv("XDG_RUNTIME_DIR", path, 0);
    }

#ifdef EHS_LVGL_LINUX_DISPLAY_BACKEND_WAYLAND
    /* Wayland backend: connect to a running compositor (e.g. Weston).
     * Force the SDL video driver and locate the compositor socket.
     * SDL2 connects to $XDG_RUNTIME_DIR/$WAYLAND_DISPLAY, so both must
     * point to wherever the compositor (e.g. Weston) actually placed it. */
    if (getenv("SDL_VIDEODRIVER") == NULL)
        setenv("SDL_VIDEODRIVER", "wayland", 0);

    if (getenv("WAYLAND_DISPLAY") == NULL) {
        /* Locate the Wayland compositor socket by reading /proc/<pid>/environ
         * for every running process.  Whichever process (weston, weston-terminal,
         * foot, …) has WAYLAND_DISPLAY set is connected to the compositor —
         * inherit its socket path and XDG_RUNTIME_DIR.  This is reliable
         * regardless of where the compositor placed the socket. */
        bool found = false;
        DIR *proc_d = opendir("/proc");
        if (proc_d) {
            struct dirent *pent;
            while (!found && (pent = readdir(proc_d)) != NULL) {
                /* Only numeric directories are PIDs. */
                bool is_pid = (pent->d_name[0] >= '1' && pent->d_name[0] <= '9');
                for (const char *p = pent->d_name + 1; is_pid && *p; p++)
                    if (*p < '0' || *p > '9') is_pid = false;
                if (!is_pid) continue;

                char env_path[64];
                snprintf(env_path, sizeof(env_path), "/proc/%s/environ", pent->d_name);
                FILE *f = fopen(env_path, "r");
                if (!f) continue;

                /* environ is null-separated; read in one chunk. */
                char buf[8192];
                size_t n = fread(buf, 1, sizeof(buf) - 1, f);
                fclose(f);
                buf[n] = '\0';

                const char *wayland_val = NULL, *xdg_val = NULL;
                for (const char *p = buf; p < buf + n; p += strlen(p) + 1) {
                    if (strncmp(p, "WAYLAND_DISPLAY=", 16) == 0)
                        wayland_val = p + 16;
                    else if (strncmp(p, "XDG_RUNTIME_DIR=", 16) == 0)
                        xdg_val = p + 16;
                }

                if (wayland_val && xdg_val) {
                    /* Strip trailing slashes from XDG_RUNTIME_DIR so
                     * libwayland constructs "dir/socket" not "dir//socket". */
                    static char xdg_clean[256];
                    strncpy(xdg_clean, xdg_val, sizeof(xdg_clean) - 1);
                    xdg_clean[sizeof(xdg_clean) - 1] = '\0';
                    size_t xdg_len = strlen(xdg_clean);
                    while (xdg_len > 1 && xdg_clean[xdg_len - 1] == '/')
                        xdg_clean[--xdg_len] = '\0';

                    /* Verify the socket actually exists at this path. */
                    char sock_path[320];
                    snprintf(sock_path, sizeof(sock_path), "%s/%s", xdg_clean, wayland_val);
                    struct stat st;
                    if (stat(sock_path, &st) != 0 || !S_ISSOCK(st.st_mode)) {
                        fprintf(stderr,
                                "sdl_init: PID %s has WAYLAND_DISPLAY=%s but "
                                "socket %s not found, continuing scan\n",
                                pent->d_name, wayland_val, sock_path);
                        continue;
                    }

                    setenv("WAYLAND_DISPLAY",  wayland_val, 0);
                    setenv("XDG_RUNTIME_DIR",  xdg_clean,   1);
                    fprintf(stderr,
                            "sdl_init: inherited Wayland env from PID %s: "
                            "XDG_RUNTIME_DIR=%s WAYLAND_DISPLAY=%s (socket OK)\n",
                            pent->d_name, xdg_clean, wayland_val);
                    found = true;
                }
            }
            closedir(proc_d);
        }

        if (!found) {
            fprintf(stderr,
                    "sdl_init: no running process has WAYLAND_DISPLAY set.\n"
                    "sdl_init: set WAYLAND_DISPLAY and XDG_RUNTIME_DIR manually "
                    "before launching, e.g. from a Weston terminal:\n"
                    "  export WAYLAND_DISPLAY XDG_RUNTIME_DIR && ./your_app\n");
        }
    }
#endif
#endif

    /* Log which video backends are compiled into this SDL2 build. */
    int ndrv = SDL_GetNumVideoDrivers();
    fprintf(stderr, "sdl_init: %d SDL video driver(s) compiled in:", ndrv);
    for (int i = 0; i < ndrv; i++) fprintf(stderr, " %s", SDL_GetVideoDriver(i));
    fprintf(stderr, "\n");

    /*Initialize the SDL*/
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "sdl_init: SDL_Init failed: %s\n", SDL_GetError());
        fprintf(stderr, "sdl_init: hint – ensure the process has read/write access to /dev/dri/card* "
                        "(add user to the 'video' group, or run as root)\n");
        return;
    }
    fprintf(stderr, "sdl_init: using video driver: %s\n", SDL_GetCurrentVideoDriver());

    SDL_SetEventFilter(quit_filter, NULL);

    window_create(&monitor);
#if SDL_DUAL_DISPLAY
    window_create(&monitor2);
    int x, y;
    SDL_GetWindowPosition(monitor2.window, &x, &y);
    SDL_SetWindowPosition(monitor.window, x + (SDL_HOR_RES * SDL_ZOOM) / 2 + 10, y);
    SDL_SetWindowPosition(monitor2.window, x - (SDL_HOR_RES * SDL_ZOOM) / 2 - 10, y);
#endif

    SDL_StartTextInput();

    lv_timer_create(sdl_event_handler, 10, NULL);
    sdl_inited = true;
}

/**
 * Flush a buffer to the marked area
 * @param disp_drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixels to copy to the `area` part of the screen
 */
void sdl_display_flush(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    if (!sdl_inited) {
        lv_disp_flush_ready(disp_drv);
        return;
    }

    const lv_coord_t hres = disp_drv->physical_hor_res == -1 ? disp_drv->hor_res : disp_drv->physical_hor_res;
    const lv_coord_t vres = disp_drv->physical_ver_res == -1 ? disp_drv->ver_res : disp_drv->physical_ver_res;

//    printf("x1:%d,y1:%d,x2:%d,y2:%d\n", area->x1, area->y1, area->x2, area->y2);

    /*Return if the area is out the screen*/
    if(area->x2 < 0 || area->y2 < 0 || area->x1 > hres - 1 || area->y1 > vres - 1) {
        lv_disp_flush_ready(disp_drv);
        return;
    }

#if SDL_DOUBLE_BUFFERED
    monitor.tft_fb_act = (uint32_t *)color_p;
#else /*SDL_DOUBLE_BUFFERED*/

    int32_t y;
#if LV_COLOR_DEPTH != 24 && LV_COLOR_DEPTH != 32    /*32 is valid but support 24 for backward compatibility too*/
    int32_t x;
    for(y = area->y1; y <= area->y2 && y < vres; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            monitor.tft_fb[y * hres + x] = lv_color_to32(*color_p);
            color_p++;
        }

    }
#else
    uint32_t w = lv_area_get_width(area);
    for(y = area->y1; y <= area->y2 && y < vres; y++) {
        memcpy(&monitor.tft_fb[y * hres + area->x1], color_p, w * sizeof(lv_color_t));
        color_p += w;
    }
#endif
#endif /*SDL_DOUBLE_BUFFERED*/

    monitor.sdl_refr_qry = true;

    /* TYPICALLY YOU DO NOT NEED THIS
     * If it was the last part to refresh update the texture of the window.*/
    if(lv_disp_flush_is_last(disp_drv)) {
        monitor_sdl_refr(NULL);
    }

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);

}


#if SDL_DUAL_DISPLAY

/**
 * Flush a buffer to the marked area
 * @param disp_drv pointer to driver where this function belongs
 * @param area an area where to copy `color_p`
 * @param color_p an array of pixels to copy to the `area` part of the screen
 */
void sdl_display_flush2(lv_disp_drv_t * disp_drv, const lv_area_t * area, lv_color_t * color_p)
{
    const lv_coord_t hres = disp_drv->physical_hor_res == -1 ? disp_drv->hor_res : disp_drv->physical_hor_res;
    const lv_coord_t vres = disp_drv->physical_ver_res == -1 ? disp_drv->ver_res : disp_drv->physical_ver_res;

    /*Return if the area is out the screen*/
    if(area->x2 < 0 || area->y2 < 0 || area->x1 > hres - 1 || area->y1 > vres - 1) {
        lv_disp_flush_ready(disp_drv);
        return;
    }

#if SDL_DOUBLE_BUFFERED
    monitor2.tft_fb_act = (uint32_t *)color_p;

    monitor2.sdl_refr_qry = true;

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);
#else

    int32_t y;
#if LV_COLOR_DEPTH != 24 && LV_COLOR_DEPTH != 32    /*32 is valid but support 24 for backward compatibility too*/
    int32_t x;
    for(y = area->y1; y <= area->y2 && y < vres; y++) {
        for(x = area->x1; x <= area->x2; x++) {
            monitor2.tft_fb[y * hres + x] = lv_color_to32(*color_p);
            color_p++;
        }

    }
#else
    uint32_t w = lv_area_get_width(area);
    for(y = area->y1; y <= area->y2 && y < vres; y++) {
        memcpy(&monitor2.tft_fb[y * hres + area->x1], color_p, w * sizeof(lv_color_t));
        color_p += w;
    }
#endif

    monitor2.sdl_refr_qry = true;

    /* TYPICALLY YOU DO NOT NEED THIS
     * If it was the last part to refresh update the texture of the window.*/
    if(lv_disp_flush_is_last(disp_drv)) {
        monitor_sdl_refr(NULL);
    }

    /*IMPORTANT! It must be called to tell the system the flush is ready*/
    lv_disp_flush_ready(disp_drv);
#endif
}
#endif

/**********************
 *   STATIC FUNCTIONS
 **********************/


/**
 * SDL main thread. All SDL related task have to be handled here!
 * It initializes SDL, handles drawing and the mouse.
 */

static void sdl_event_handler(lv_timer_t * t)
{
    (void)t;

    /*Refresh handling*/
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        mouse_handler(&event);
        mousewheel_handler(&event);
        keyboard_handler(&event);
        //printf("(&event)->type=%d, (&event)->window.event=%d\n",(&event)->type, (&event)->window.event);
        if((&event)->type == SDL_WINDOWEVENT) {
            switch((&event)->window.event) {
#if SDL_VERSION_ATLEAST(2, 0, 5)
                case SDL_WINDOWEVENT_TAKE_FOCUS:
#endif
                case SDL_WINDOWEVENT_EXPOSED:
                    window_update(&monitor);
#if SDL_DUAL_DISPLAY
                    window_update(&monitor2);
#endif
                    break;
                default:
                    break;
            }
        }
    }

    /*Run until quit event not arrives*/
    if(sdl_quit_qry) {
        monitor_sdl_clean_up();
        exit(0);
    }
}

/**
 * SDL main thread. All SDL related task have to be handled here!
 * It initializes SDL, handles drawing and the mouse.
 */

static void monitor_sdl_refr(lv_timer_t * t)
{
    (void)t;

    /*Refresh handling*/
    if(monitor.sdl_refr_qry != false) {
        monitor.sdl_refr_qry = false;
        window_update(&monitor);
    }

#if SDL_DUAL_DISPLAY
    if(monitor2.sdl_refr_qry != false) {
        monitor2.sdl_refr_qry = false;
        window_update(&monitor2);
    }
#endif
}

static void monitor_sdl_clean_up(void)
{
    SDL_DestroyTexture(monitor.texture);
    SDL_DestroyRenderer(monitor.renderer);
    SDL_DestroyWindow(monitor.window);

#if SDL_DUAL_DISPLAY
    SDL_DestroyTexture(monitor2.texture);
    SDL_DestroyRenderer(monitor2.renderer);
    SDL_DestroyWindow(monitor2.window);

#endif

    SDL_Quit();
}

static void window_create(monitor_t * m)
{
    int flag = 0;
#if SDL_FULLSCREEN
#ifdef EHS_LVGL_LINUX_DISPLAY_BACKEND_WAYLAND
    /* On Wayland, passing SDL_WINDOW_FULLSCREEN_DESKTOP at creation time
     * causes SDL2 to call xdg_toplevel_resize with serial=0 during window
     * setup, which Weston rejects as a fatal protocol error.
     * Instead create a normal window first, then go fullscreen after. */
#else
    flag |= SDL_WINDOW_FULLSCREEN_DESKTOP;
#endif
#endif
    /* SDL_WINDOW_OPENGL is NOT set: we use the SDL renderer API, not direct GL.
     * Setting it can trigger EGL initialisation which fails on boards (e.g. Rubik Pi 3)
     * that have no full OpenGL driver, causing SDL_CreateRenderer to return NULL. */

    m->window = SDL_CreateWindow("eRT (F8 - Home)",
                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SDL_HOR_RES * SDL_ZOOM, SDL_VER_RES * SDL_ZOOM, flag);
    if (m->window == NULL) {
        fprintf(stderr, "sdl_init: SDL_CreateWindow failed: %s\n", SDL_GetError());
        return;
    }

    /* Try hardware-accelerated renderer; fall back to software on failure. */
    m->renderer = SDL_CreateRenderer(m->window, -1, SDL_RENDERER_ACCELERATED);
    if (m->renderer == NULL) {
        fprintf(stderr, "sdl_init: accelerated renderer failed (%s), trying software\n", SDL_GetError());
        m->renderer = SDL_CreateRenderer(m->window, -1, SDL_RENDERER_SOFTWARE);
    }
    if (m->renderer == NULL) {
        fprintf(stderr, "sdl_init: SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return;
    }
    m->texture = SDL_CreateTexture(m->renderer,
                                SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SDL_HOR_RES, SDL_VER_RES);
    SDL_SetTextureBlendMode(m->texture, SDL_BLENDMODE_BLEND);

#if SDL_FULLSCREEN
#ifdef EHS_LVGL_LINUX_DISPLAY_BACKEND_WAYLAND
    /* Deferred fullscreen for Wayland: set after renderer is up to avoid
     * the xdg_toplevel_resize protocol error that fires when fullscreen is
     * requested at window-creation time. */
    SDL_SetWindowFullscreen(m->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif
#endif

    /*Initialize the frame buffer to gray (77 is an empirical value) */
#if SDL_DOUBLE_BUFFERED
    SDL_UpdateTexture(m->texture, NULL, m->tft_fb_act, SDL_HOR_RES * sizeof(uint32_t));
#else
    m->tft_fb = (uint32_t *)malloc(sizeof(uint32_t) * SDL_HOR_RES * SDL_VER_RES);
    memset(m->tft_fb, 0x44, SDL_HOR_RES * SDL_VER_RES * sizeof(uint32_t));
#endif

    m->sdl_refr_qry = true;

}

static void window_update(monitor_t * m)
{
#if SDL_DOUBLE_BUFFERED == 0
    SDL_UpdateTexture(m->texture, NULL, m->tft_fb, SDL_HOR_RES * sizeof(uint32_t));
#else
    if(m->tft_fb_act == NULL) return;
    SDL_UpdateTexture(m->texture, NULL, m->tft_fb_act, SDL_HOR_RES * sizeof(uint32_t));
#endif
    SDL_RenderClear(m->renderer);
    lv_disp_t * d = _lv_refr_get_disp_refreshing();
    if(d->driver->screen_transp) {
        SDL_SetRenderDrawColor(m->renderer, 0xff, 0, 0, 0xff);
        SDL_Rect r;
        r.x = 0; r.y = 0; r.w = SDL_HOR_RES; r.h = SDL_VER_RES;
        SDL_RenderDrawRect(m->renderer, &r);
    }

    /*Update the renderer with the texture containing the rendered image*/
    SDL_RenderCopy(m->renderer, m->texture, NULL, NULL);
    SDL_RenderPresent(m->renderer);
}

#endif /*USE_MONITOR || USE_SDL*/
