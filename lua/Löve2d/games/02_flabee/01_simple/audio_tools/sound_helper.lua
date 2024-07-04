local sounds_path = "assets/audio/"

local sounds = {
    -- https://freesound.org/people/mika55/sounds/191725/
    background = love.audio.newSource(sounds_path .. "background.wav",
        "stream"),
    score = love.audio.newSource(sounds_path .. "score.wav", "static"),
    bump = love.audio.newSource(sounds_path .. "bump.wav", "static"),
    hit = love.audio.newSource(sounds_path .. "hit.wav", "static"),
}

sounds.bump:setVolume(0.3)

sounds.background:setVolume(0.25)
sounds.background:setLooping(true)

local SoundHelper = {
    sounds = sounds,
}

function SoundHelper:play(name)
    self.sounds[name]:play()
end

function SoundHelper:pause(name)
    self.sounds[name]:pause()
end

function SoundHelper:stop(name)
    self.sounds[name]:stop()
end

return SoundHelper
