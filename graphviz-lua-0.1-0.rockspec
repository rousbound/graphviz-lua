package = "graphviz-lua"
version = "0.1-0"
source = {
    url = "git://github.com/rousbound/graphviz-lua"
}

description = {
    summary = "Graphviz Lua Binding",
    detailed = "Graphviz Lua Binding",
    license = "MIT"
}

dependencies = {
    "lua = 5.3",
    "luafilesystem"
}

external_dependencies = {
    GRAPHVIZ = {
        header = "gvc.h"
    }
}

build = {
    type = "builtin",
    modules = {
        ["graphviz"] = {
            sources = { "main.c" },
            libraries = { "gvc", "cgraph" },
            incdirs = {  "$(GRAPHVIZ_INCDIR)" },
            libdirs = {  "$(GRAPHVIZ_LIBDIR)" }
        }
    }
}
