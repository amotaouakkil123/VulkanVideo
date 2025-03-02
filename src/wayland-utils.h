#include <iostream>
#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include "xdg-shell-client-protocol.h"

static const int width = 500;
static const int height = 500;

static bool configured = false;
static bool running = true;

static struct wl_compositor* compositor = nullptr;
static struct xdg_wm_base* xdg_wm_base = nullptr;

static struct wl_surface* surface = nullptr;
static struct xdg_toplevel* xdg_toplevel = nullptr;

static void
noop()
{
    // This space intentionally left blank
}

static void
xdg_wm_base_handle_ping(void* data,
                        struct xdg_wm_base* xdg_wm_base,
                        uint32_t serial)
{
    // The compositor will ping an event to verify response
    // Send back a pong
    xdg_wm_base_pong(xdg_wm_base, serial);
}

static const struct
xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_handle_ping,
};

static void
xdg_surface_handle_configure(void* data,
                             struct xdg_surface* xdg_surface,
                             uint32_t serial)
{
    // Once the compositor configures our surface, we need to acknowledge the change
    xdg_surface_ack_configure(xdg_surface, serial);

    if (configured) {
        // Commit the surface to apply the configure acknowledgement
        wl_surface_commit(surface);
    }

    configured = true;
}

static const struct
xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_handle_configure,
};

static void
xdg_toplevel_handle_close(void* data,
                          struct xdg_toplevel* xdg_toplevel)
{
    // Stop running if the user requests to close the toplevel
    running = false;
}

static const struct
xdg_toplevel_listener xdg_toplevel_listener = {
    .configure = noop,
    .close = xdg_toplevel_handle_close,
};

static void
pointer_handle_button(void* data,
                                  struct wl_pointer* pointer,
                                  uint32_t serial,
                                  uint32_t time,
                                  uint32_t button,
                                  uint32_t state)
{
    // Start an interactive move of the top level
    if (button == BTN_LEFT && state == WL_POINTER_BUTTON_STATE_PRESSED)
    {
        xdg_toplevel_move(xdg_toplevel, seat, serial);
    }
}

static const struct
wl_pointer_listener pointer_listener = {
    .enter = noop,
    .leave = noop,
    .motion = noop,
    .button = pointer_handle_button,
    .axis = noop,
}

static void
seat_handle_capabilities(void* data,
                                     struct wl_seat* seat,
                                     uint32_t capabilities)
{
    // If the wl_seat has pointer capability, start listening to pointer events
    if (capabilities & WL_SEAT_CAPABILITY_POINTER)
    {
        struct wl_pointer* pointer = wl_seat_get_pointer(seat);
        wl_pointer_add_listener(pointer, &pointer_listener, seat);
    }
}

static const struct
wl_seat_listener seat_listener = {
    .capabilities = seat_handle_capabilities,
};

static void
handle_global(void* data,
       struct wl_registry* registry,
       uint32_t name,
       const char* interface,
       uint32_t version)
{
    if (strcmp(interface, wl_seat_interface.name) == 0)
    {
        struct wl_seat* seat = wl_registry_bind(registry, name, &wl_seat_interface, 1);
        wl_seat_add_listener(seat, &seat_listener, nullptr);
    }
    else if (strcmp(interface, wl_compositor_interface.name) == 0)
    {
        compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 1);
    }
    else if (strcmp(interface, xdg_wm_base_interface.name) == 0)
    {
        xdg_wm_base = wl_registry_bind(registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(xdg_wm_base, &xdg_wm_base_listener, nullptr);
    }
}

static void
handle_global_remove(void* data,
                     struct wl_registry* registry,
                     uint32_t name)
{
    // Keep empty
}

static const struct wl_registry_listener registry_listener = {
    .global = handle_global,
    .global_remove = handle_global_remove,
};

