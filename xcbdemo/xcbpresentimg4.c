#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/present.h>

int main() {
    xcb_connection_t *connection;
    xcb_screen_t *screen;
    xcb_window_t window;
    xcb_generic_event_t *event;

    // Open a connection to the X server
    connection = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(connection)) {
        fprintf(stderr, "Error: Unable to open XCB connection\n");
        return 1;
    }

    // Get the default screen
    screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

    // Create a window
    window = xcb_generate_id(connection);
    xcb_create_window(connection, XCB_COPY_FROM_PARENT, window, screen->root,
                      0, 0, 400, 300, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT, 
                      screen->root_visual, 0, NULL);

    // Map the window on the screen
    xcb_map_window(connection, window);
    xcb_flush(connection);

    // Create a pixmap
    xcb_pixmap_t pixmap = xcb_generate_id(connection);
    xcb_create_pixmap(connection, screen->root_depth, pixmap, window, 400, 300);

    // Fill the pixmap with some color (e.g., white)
    xcb_gcontext_t gc = xcb_generate_id(connection);
    xcb_create_gc(connection, gc, pixmap, 0, NULL);
    xcb_rectangle_t rect = {0, 0, 400, 300};
    xcb_poly_fill_rectangle(connection, pixmap, gc, 1, &rect);

    // Present the pixmap on the window
    xcb_present_pixmap(connection, XCB_NONE, pixmap, window, 0, XCB_NONE, XCB_NONE, 0, 0, XCB_NONE, XCB_NONE, 0, 0, 0, XCB_PRESENT_OPTION_NONE, 0, NULL);

    // Wait for the present event
    while ((event = xcb_wait_for_event(connection))) {
        if (event->response_type == XCB_PRESENT_COMPLETE_NOTIFY) {
            xcb_present_generic_event_t *pe = (xcb_present_generic_event_t *)event;
            printf("Present event received: serial %d\n", pe->sequence);
            break;
        }
        free(event);
    }

    // Clean up
    xcb_free_pixmap(connection, pixmap);
    xcb_disconnect(connection);
    return 0;
}

