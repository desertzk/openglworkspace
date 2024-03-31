#include <xcb/xcb.h>
#include <xcb/present.h>
#include <png.h>
#include <stdlib.h>

// void save_pixmap_to_png(xcb_connection_t* connection, xcb_pixmap_t pixmap, int width, int height, const char* filename) {
//     xcb_get_image_reply_t* reply = xcb_get_image_reply(
//         connection,
//         xcb_get_image(
//             connection, XCB_IMAGE_FORMAT_Z_PIXMAP, pixmap,
//             0, 0, width, height, (uint32_t)(-1)
//         ),
//         NULL
//     );

//     if (reply) {
//         png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
//         png_infop info = png_create_info_struct(png);

//         FILE* fp = fopen(filename, "wb");
//         png_init_io(png, fp);

//         png_set_IHDR(
//             png, info, width, height, 8,
//             PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
//             PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT
//         );
//         png_write_info(png, info);

//         uint8_t* data = xcb_get_image_data(reply);
//         for (int y = 0; y < height; y++) {
//             png_write_row(png, data + y * reply->stride);
//         }

//         png_write_end(png, NULL);
//         png_destroy_write_struct(&png, &info);
//         fclose(fp);

//         free(reply);
//     }
// }

void save_pixmap_to_png(xcb_connection_t* connection, xcb_pixmap_t pixmap, int width, int height, const char* filename) {
    xcb_get_image_reply_t* reply = xcb_get_image_reply(
        connection,
        xcb_get_image(
            connection, XCB_IMAGE_FORMAT_Z_PIXMAP, pixmap,
            0, 0, width, height, (uint32_t)(-1)
        ),
        NULL
    );

    if (reply) {
        png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
        png_infop info = png_create_info_struct(png);

        FILE* fp = fopen(filename, "wb");
        png_init_io(png, fp);

        png_set_IHDR(
            png, info, width, height, 8,
            PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT
        );
        png_write_info(png, info);

        uint8_t* data = xcb_get_image_data(reply);
        int stride = width * 4; // Assuming 32-bit pixels (4 bytes per pixel)
        for (int y = 0; y < height; y++) {
            png_write_row(png, data + y * stride);
        }

        png_write_end(png, NULL);
        png_destroy_write_struct(&png, &info);
        fclose(fp);

        free(reply);
    }
}


int main() {
    // Connect to the X server
    xcb_connection_t* connection = xcb_connect(NULL, NULL);

    // Get the default screen
    xcb_screen_t* screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;

    // Create a window
    xcb_window_t window = xcb_generate_id(connection);
    uint32_t values[] = {screen->black_pixel, XCB_EVENT_MASK_EXPOSURE};
    xcb_create_window(
        connection, XCB_COPY_FROM_PARENT, window, screen->root, 0, 0, 500, 500, 0,
        XCB_WINDOW_CLASS_INPUT_OUTPUT, screen->root_visual, XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK, values);

    // Create a pixmap
    xcb_pixmap_t pixmap = xcb_generate_id(connection);
    xcb_create_pixmap(connection, screen->root_depth, pixmap, window, 500, 500);

    // Create a graphics context
    xcb_gcontext_t gc = xcb_generate_id(connection);
    xcb_create_gc(connection, gc, pixmap, 0, NULL);

    // Draw a white rectangle on the pixmap
    xcb_rectangle_t rect = {100, 100, 300, 200};
    values[0] = screen->white_pixel;
    xcb_change_gc(connection, gc, XCB_GC_FOREGROUND, values);
    xcb_poly_fill_rectangle(connection, pixmap, gc, 1, &rect);

    // Map the window
    xcb_map_window(connection, window);

    // Present the pixmap
    xcb_present_pixmap(
        connection, window, pixmap, 0, XCB_NONE, XCB_NONE, 0, 0, XCB_NONE, XCB_NONE,
        XCB_NONE, XCB_PRESENT_OPTION_NONE, 0, 1, 0, 0, NULL
    );

    // Flush the connection
    xcb_flush(connection);

    // Save the pixmap to a PNG file
    save_pixmap_to_png(connection, pixmap, 500, 500, "rectangle.png");

    // Event loop
    xcb_generic_event_t* event;
    while ((event = xcb_wait_for_event(connection))) {
        switch (event->response_type & ~0x80) {
            case XCB_EXPOSE:
                // Re-present the pixmap on expose events
                xcb_present_pixmap(
                    connection, window, pixmap, 0, XCB_NONE, XCB_NONE, 0, 0, XCB_NONE, XCB_NONE,
                    XCB_NONE, XCB_PRESENT_OPTION_NONE, 0, 1, 0, 0, NULL
                );
                xcb_flush(connection);
                break;
        }
        free(event);
    }

    // Disconnect from the X server
    xcb_disconnect(connection);

    return 0;
}