#include <stdio.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <xcb/present.h>

#define WIDTH 400
#define HEIGHT 300

int main() {
    // Open connection to the X server
    xcb_connection_t *connection = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(connection)) {
        printf("Error opening display\n");
        return 1;
    }

    // Create a window
    xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
    xcb_window_t window = xcb_generate_id(connection);
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root,
                      0, 0, WIDTH, HEIGHT, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual,
                      0, NULL);

    // Map the window to the screen
    xcb_map_window(connection, window);
    xcb_flush(connection);

    // Create graphics context
    xcb_gcontext_t gc = xcb_generate_id(connection);
    uint32_t white_pixel = screen->white_pixel;
    xcb_create_gc(connection, gc, window, XCB_GC_FOREGROUND | XCB_GC_GRAPHICS_EXPOSURES, &white_pixel);

    // Create a pixmap
    xcb_pixmap_t pixmap = xcb_generate_id(connection);
    xcb_create_pixmap(connection, screen->root_depth, pixmap, window, WIDTH, HEIGHT);

    // Draw a solid triangle on the pixmap
    xcb_point_t points[] = {
        {100, 200},
        {300, 200},
        {200, 50}
    };
    xcb_fill_poly(connection, pixmap, gc, XCB_POLY_SHAPE_CONVEX, XCB_COORD_MODE_ORIGIN, 3, points);

    // Present the pixmap
    xcb_void_cookie_t cookie = xcb_present_pixmap(connection, window, pixmap, 0, XCB_NONE, XCB_NONE, 0, 0, XCB_NONE, XCB_NONE, 0, 0, 0, 0, 0, 0, NULL);
    xcb_generic_error_t *error = xcb_request_check(connection, cookie);
    if (error) {
        printf("Error presenting pixmap: %d\n", error->error_code);
        free(error);
    } else {
        printf("Pixmap presented successfully\n");
    }

    xcb_flush(connection);

    // Main loop
    xcb_generic_event_t *event;
    while ((event = xcb_wait_for_event(connection))) {
        switch (event->response_type & ~0x80) {
            case XCB_KEY_PRESS:
                goto done;
        }
    }

done:
    // Cleanup
    xcb_free_pixmap(connection, pixmap);
    xcb_disconnect(connection);
    return 0;
}
