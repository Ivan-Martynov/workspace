local color_base_names = {
    "black",
    "white",
    "red",
    "green",
    "blue",
    "yellow",
    "magenta",
    "cyan",
    "orange",
}

local schemes = {}

-- Chose indices from
-- https://learn.microsoft.com/en-us/office/vba/api/excel.colorindex
local function set_color_indices(scheme)
    for i, name in ipairs(color_base_names) do
        scheme[i] = scheme[name] or { 1, 1, 1, 1 }
    end
    
    scheme.background = scheme.black
    scheme.foreground = scheme.white
end

local function create_default(_)
    local default = {
        black = { 0, 0, 0, 1 },    -- black
        white = { 1, 1, 1, 1 },    -- white
        red = { 1, 0, 0, 1 },      -- red
        green = { 0, 1, 0, 1 },    -- green
        blue = { 0, 0, 1, 1 },     -- blue
        yellow = { 1, 1, 0, 1 }, -- blue
        magenta = { 1, 0, 1, 1 },  -- magenta
        cyan = { 0, 1, 1, 1 },     -- cyan
        orange = { 1, 0.5, 0, 1 },   -- orange
        violet = { 0.5, 0, 1, 1 },   -- violet
    }

    set_color_indices(default)

    return default
end

local function create_solarized_base()
    return {
        base03  = { 0 / 255, 43 / 255, 54 / 255, 1 },
        base02  = { 7 / 255, 54 / 255, 66 / 255, 1 },
        base01  = { 88 / 255, 110 / 255, 117 / 255, 1 },
        base00  = { 101 / 255, 123 / 255, 131 / 255, 1 },
        base0   = { 131 / 255, 148 / 255, 150 / 255, 1 },
        base1   = { 147 / 255, 161 / 255, 161 / 255, 1 },
        base2   = { 238 / 255, 232 / 255, 213 / 255, 1 },
        base3   = { 253 / 255, 246 / 255, 227 / 255, 1 },
        yellow  = { 181 / 255, 137 / 255, 0 / 255, 1 },
        orange  = { 203 / 255, 75 / 255, 22 / 255, 1 },
        red     = { 220 / 255, 50 / 255, 47 / 255, 1 },
        magenta = { 211 / 255, 54 / 255, 130 / 255, 1 },
        violet  = { 108 / 255, 113 / 255, 196 / 255, 1 },
        blue    = { 38 / 255, 139 / 255, 210 / 255, 1 },
        cyan    = { 42 / 255, 161 / 255, 152 / 255, 1 },
        green   = { 133 / 255, 153 / 255, 0 / 255, 1 },
    }
end

local function create_solarized(is_dark)
    local solarized = create_solarized_base()

    solarized.black = solarized.base03
    solarized.white = solarized.base3

    set_color_indices(solarized)

    if is_dark then
        solarized.background = solarized.black
        solarized.foreground = solarized.white
    else
        solarized.background = solarized.white
        solarized.foreground = solarized.black
    end

    return solarized
end

schemes.default = create_default()
schemes.solarized_light = create_solarized(false)
schemes.solarized_dark = create_solarized(true)

schemes.current = schemes.default

function schemes:set_scheme(scheme)
    self.current = scheme or self.default
end

return schemes
