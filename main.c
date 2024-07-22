#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include <gvc.h>

// Function to render a DOT string to the specified format bytes
unsigned char* render_dot_to_format(const char *dot_string, const char *format, unsigned int *length) {
    GVC_t *gvc;
    Agraph_t *g;
    char *output_data;
    unsigned int output_length;
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

    // Render the graph to the specified format buffer
    gvRenderData(gvc, g, format, &output_data, &output_length);

    // Allocate memory for the result
    result = (unsigned char*) malloc(output_length);
    if (result == NULL) {
        fprintf(stderr, "Error: could not allocate memory for output data\n");
        gvFreeRenderData(output_data);
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);
        return NULL;
    }

    // Copy the output data to the result buffer
    memcpy(result, output_data, output_length);

    // Set the output length
    *length = output_length;

    // Clean up
    gvFreeRenderData(output_data);
    gvFreeLayout(gvc, g);
    agclose(g);
    gvFreeContext(gvc);

    return result;
}

// Lua binding function
int render(lua_State *L) {
    const char *dot_string = luaL_checkstring(L, 1); // Get the DOT string from Lua
    const char *format = luaL_optstring(L, 2, "png"); // Get the format string from Lua, default to "png"
    unsigned int output_length;
    unsigned char *output_data;

    output_data = render_dot_to_format(dot_string, format, &output_length);

    if (output_data != NULL) {
        lua_pushlstring(L, (const char *)output_data, output_length); // Push output data as a Lua string
        free(output_data); // Free the output data after pushing it to Lua
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
