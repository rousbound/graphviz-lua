# graphviz-lua


Lua binding to [graphviz](https://gitlab.com/graphviz/graphviz),
an open source graph visualization software.

## Installation

```bash
luarocks install graphviz-lua GRAPHVIZ_INCDIR=/usr/include/graphviz
```

## Usage/Docs

```lua
local gv = require"graphviz"

----------------------------------------
-- Render dot graph to bytes of determined format
-- @param string dot graph
-- @param string output format
-- @return string|nil bytes
-- @return string|nil error mensage|n
local bytes, err = gv.render("digraph G{A -> B}", "png")
```

## License

This work is released under the Apache-2.0 license. A copy of the license is provided in the [LICENSE](./LICENSE) file.
