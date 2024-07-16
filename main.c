#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <gvc.h>

// Function to render a DOT string to PNG bytes
unsigned char* render_dot_to_png(const char *dot_string, unsigned int *length) {
    GVC_t *gvc;
    Agraph_t *g;
    char *png_data;
    unsigned int png_length;
    unsigned char *result;

    // Initialize a Graphviz context
    gvc = gvContext();

    // Read the DOT string into a graph
    g = agmemread(dot_string);
    if (g == NULL) {
        fprintf(stderr, "Error: could not parse DOT string\n");
        return NULL;
    }

    // Layout the graph
    gvLayout(gvc, g, "dot");

    // Render the graph to a PNG buffer
    gvRenderData(gvc, g, "png", &png_data, &png_length);

    // Allocate memory for the result
    result = (unsigned char*) malloc(png_length);
    if (result == NULL) {
        fprintf(stderr, "Error: could not allocate memory for PNG data\n");
        gvFreeRenderData(png_data);
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);
        return NULL;
    }

    // Copy the PNG data to the result buffer
    memcpy(result, png_data, png_length);

    // Set the output length
    *length = png_length;

    // Clean up
    gvFreeRenderData(png_data);
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);

    return result;
}

// Lua binding function
int render(lua_State *L) {
    const char *dot_string = luaL_checkstring(L, 1); // Get the DOT string from Lua
    unsigned int png_length;
    unsigned char *png_data;

    png_data = render_dot_to_png(dot_string, &png_length);

    if (png_data != NULL) {
        lua_pushlstring(L, (const char *)png_data, png_length); // Push PNG data as a Lua string
        free(png_data); // Free the PNG data after pushing it to Lua
        return 1; // Number of return values
    } else {
        lua_pushnil(L);
        return 1;
    }
}

// Register the function with Lua
int luaopen_graphviz(lua_State *L) {
    lua_newtable(L); // Create a new table

    // Register the render function in the table
    lua_pushcfunction(L, render);
    lua_setfield(L, -2, "render");

    return 1; // Return the table
}
