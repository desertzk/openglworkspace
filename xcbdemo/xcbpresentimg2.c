#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <xcb/present.h>
#include <xcb/xcb_image.h>

#include <png.h>

void write_png_file(const char *filename, xcb_connection_t *connection, xcb_pixmap_t pixmap, uint16_t width, uint16_t height) {
    // Open the PNG file for writing
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        fprintf(stderr, "Error: Failed to open PNG file for writing\n");
        return;
    }

    // Initialize PNG structures
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png_ptr) {
        fclose(fp);
        fprintf(stderr, "Error: Failed to create PNG write structure\n");
        return;
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        fclose(fp);
        png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
        fprintf(stderr, "Error: Failed to create PNG info structure\n");
        return;
    }

    // Set up error handling
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fprintf(stderr, "Error: Error during PNG write\n");
        return;
    }

    // Initialize PNG IO
    png_init_io(png_ptr, fp);

    // Set PNG header information
    png_set_IHDR(png_ptr, info_ptr, width, height,
                 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    // Allocate memory for row pointers
    png_bytep row_pointers[height];
    for (int i = 0; i < height; i++) {
        row_pointers[i] = malloc(3 * width * sizeof(png_byte));
    }

    // Read image data from pixmap
    xcb_image_t *image = xcb_image_get(connection, pixmap, 0, 0, width, height, 0xFFFFFFFF, XCB_IMAGE_FORMAT_Z_PIXMAP);
    if (!image) {
        fclose(fp);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fprintf(stderr, "Error: Failed to get image data from pixmap\n");
        return;
    }

    // Copy image data to PNG row pointers
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint32_t pixel = *(uint32_t*)(image->data + y * image->stride + x * 4);
            row_pointers[y][x * 3] = (pixel >> 16) & 0xFF;    // Red
            row_pointers[y][x * 3 + 1] = (pixel >> 8) & 0xFF; // Green
            row_pointers[y][x * 3 + 2] = pixel & 0xFF;        // Blue
        }
    }

    // Write PNG data
    png_write_info(png_ptr, info_ptr);
    png_write_image(png_ptr, row_pointers);
    png_write_end(png_ptr, NULL);

    // Clean up
    xcb_image_destroy(image);
    for (int i = 0; i < height; i++) {
        free(row_pointers[i]);
    }
    fclose(fp);
    png_destroy_write_struct(&png_ptr, &info_ptr);
}

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

    // Fill the pixmap with green color
    xcb_render_color_t color = {0, 0xFFFF, 0, 0xFFFF}; // Green
    xcb_rectangle_t rect = {0, 0, 400, 300};
    xcb_render_fill_rectangles(connection, XCB_RENDER_PICT_FORMAT_RGB_16, pixmap, color, 1, &rect);

    // Dump pixmap to PNG file
    write_png_file("output.png", connection, pixmap, 400, 300);

    // Clean up
    xcb_free_pixmap(connection, pixmap);
    xcb_disconnect(connection);
    return 0;
}
