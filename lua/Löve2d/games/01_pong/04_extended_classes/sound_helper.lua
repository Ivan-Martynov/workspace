local sounds = {
    -- https://freesound.org/people/xsgianni/sounds/388079/
    backbround = love.audio.newSource("sounds/marios_way.mp3", "stream"),
    hit_paddle = love.audio.newSource("sounds/paddle_hit.wav", "static"),
    hit_wall = love.audio.newSource("sounds/wall_hit.wav", "static"),
    score = love.audio.newSource("sounds/score.wav", "static"),
}

sounds.backbround:setVolume(0.1)
sounds.backbround:setLooping(true)

sounds.hit_paddle:setVolume(0.5)
sounds.hit_wall:setVolume(0.3)
sounds.score:setVolume(0.2)

return sounds
