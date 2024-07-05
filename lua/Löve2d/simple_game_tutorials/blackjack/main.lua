local deck

local dealer_hand
local player_hand

local images

local round_over

local button_hit
local button_stand
local button_play_again

local function deal_card(hand)
    table.insert(hand, table.remove(deck, love.math.random(#deck)))
end

local function reset()
    deck = {}
    for _, suit in ipairs({ "club", "diamond", "heart", "spade" }) do
        for rank = 1, 13 do
            table.insert(deck, { suit = suit, rank = rank })
        end
    end

    round_over = false

    player_hand = {}
    dealer_hand = {}
    for _ = 1, 2 do
        deal_card(player_hand)
        deal_card(dealer_hand)
    end
end

function love.load()
    love.window.setMode(500, 300)
    love.window.setTitle("Blackjack")
    love.graphics.setBackgroundColor(1, 1, 1)

    images = {}
    for _, name in ipairs({
        1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
        "pip_heart", "pip_diamond", "pip_club", "pip_spade",
        "mini_heart", "mini_diamond", "mini_club", "mini_spade",
        "card", "card_face_down",
        "face_jack", "face_queen", "face_king",
    }) do
        images[name] = love.graphics.newImage("images/" .. name .. ".png")
    end
    local button_y = 230
    local button_height = 25
    local text_offset_y = 6
    button_hit = {
        x = 10,
        y = button_y,
        width = 53,
        height = button_height,
        text = "Hit!",
        text_offset_x = 16,
        text_offset_y = text_offset_y,
    }

    button_stand= {
        x = 70,
        y = button_y,
        width = 53,
        height = button_height,
        text = "Stand",
        text_offset_x = 8,
        text_offset_y = text_offset_y,
    }

    button_play_again = {
        x = 10,
        y = button_y,
        width = 113,
        height = button_height,
        text = "Play again",
        text_offset_x = 24,
        text_offset_y = text_offset_y,
    }

    reset()
end

local function is_mouse_over_button(button)
    local mouse_x, mouse_y = love.mouse.getPosition()

    return (mouse_x >= button.x) and (mouse_x <= button.x + button.width)
        and (mouse_y >= button.y) and (mouse_y <= button.y + button.height)
end

local function get_total(hand)
    local total = 0

    for _, card in ipairs(hand) do
        if card.rank >= 10 then
            total = total +  10
        elseif (card.rank == 1) and (total < 11) then
            total = total + 11
        else
            total = total + card.rank
        end
    end

    return total
end

function love.mousereleased()
    if not round_over then
        if is_mouse_over_button(button_hit) then
            deal_card(player_hand)
        elseif is_mouse_over_button(button_stand) then
            round_over = true
        end
    elseif is_mouse_over_button(button_play_again) then
        reset()
    end
end

function love.keypressed(key)
    if key == "escape" then
        love.event.quit()
    end
end

function love.update(_)
    if get_total(player_hand) >= 21 then
        round_over = true
    end

    if round_over then
        while get_total(dealer_hand) < 17 do
            deal_card(dealer_hand)
        end
    end
end

local function hand_won(hand, opponent)
    local hand_score = get_total(hand)
    local opponent_score = get_total(opponent)

    return (hand_score <= 21) and ((opponent_score > 21)
        or (hand_score > opponent_score))
end

function love.draw()
    local function draw_card(card, x, y)
        love.graphics.setColor(1, 1, 1)
        love.graphics.draw(images.card, x, y)

        local card_width = images.card:getWidth()
        local card_height = images.card:getHeight()
        local offset_x = 3
        local number_offset_y = 4
        local suit_offset_y = number_offset_y + 10

        local function draw_corners(image, shift_x, shift_y)
            love.graphics.draw(image, x + shift_x, y + shift_y)
            love.graphics.draw(image, x + card_width - shift_x,
                y + card_height - shift_y, 0, -1)
        end

        if (card.suit == "heart") or (card.suit == "diamond") then
            love.graphics.setColor(0.89, 0.06, 0.39)
        else
            love.graphics.setColor(0.2, 0.2, 0.2)
        end
        draw_corners(images[card.rank], offset_x, number_offset_y)
        draw_corners(images["mini_" .. card.suit], offset_x, suit_offset_y)

        if card.rank > 10 then
            local face_image
            if card.rank == 11 then
                face_image = images.face_jack
            elseif card.rank == 12 then
                face_image = images.face_queen
            elseif card.rank == 13 then
                face_image = images.face_king
            end

            if face_image then
                love.graphics.setColor(1, 1, 1)
                love.graphics.draw(face_image, x + 12, y + 11)
            end
        else
            local function draw_pip(shift_x, shift_y, mirror_x, mirror_y)
                local pip_image = images["pip_" .. card.suit]
                local pip_width = pip_image:getWidth()

                love.graphics.draw(pip_image, x + shift_x, y + shift_y)
                if mirror_x then
                    love.graphics.draw(pip_image,
                        x + card_width - shift_x - pip_width, y + shift_y)
                end

                if mirror_y then
                    love.graphics.draw(pip_image, x + shift_x + pip_width,
                        y + card_height - shift_y, 0, -1)
                end

                if mirror_x and mirror_y then
                    love.graphics.draw(pip_image,
                        x + card_width - shift_x, y + card_height - shift_y,
                        0, -1)
                end
            end

            local x_left = 11
            local x_mid = 21
            local y_mid = 31
            local y_top = 7
            local y_third = 19
            local y_quarter = 23

            if card.rank == 1 then
                draw_pip(x_mid, y_mid)
            elseif card.rank == 2 then
                draw_pip(x_mid, y_top, false, true)
            elseif card.rank == 3 then
                draw_pip(x_mid, y_mid)
                draw_pip(x_mid, y_top, false, true)
            elseif card.rank == 4 then
                draw_pip(x_left, y_top, true, true)
            elseif card.rank == 5 then
                draw_pip(x_mid, y_mid)
                draw_pip(x_left, y_top, true, true)
            elseif card.rank == 6 then
                draw_pip(x_left, y_top, true, true)
                draw_pip(x_left, y_mid, true)
            elseif card.rank == 7 then
                draw_pip(x_mid, y_third)
                draw_pip(x_left, y_top, true, true)
                draw_pip(x_left, y_mid, true)
            elseif card.rank == 8 then
                draw_pip(x_left, y_top, true, true)
                draw_pip(x_left, y_mid, true)
                draw_pip(x_mid, y_third, false, true)
            elseif card.rank == 9 then
                draw_pip(x_left, y_top, true, true)
                draw_pip(x_left, y_quarter, true, true)
                draw_pip(x_mid, y_mid)
            elseif card.rank == 10 then
                draw_pip(x_left, y_top, true, true)
                draw_pip(x_left, y_quarter, true, true)
                draw_pip(x_mid, 16, false, true)
            end
        end
    end

    local card_spacing = 60
    local margin_x = 10

    for i, card in ipairs(dealer_hand) do
        local dealer_margin_y = 30
        if not round_over and (i == 1) then
            love.graphics.setColor(1, 1, 1)
            love.graphics.draw(images.card_face_down, margin_x, dealer_margin_y)
        else
            draw_card(card, (i - 1) * card_spacing + margin_x, 30)
        end
    end

    for i, card in ipairs(player_hand) do
        draw_card(card, (i - 1) * card_spacing + margin_x, 140)
    end

    love.graphics.setColor(0, 0 ,0)
    if round_over then
        love.graphics.print("Total: " .. get_total(dealer_hand), margin_x, 10)
    else
        love.graphics.print("Total: ?" , margin_x, 10)
    end

    love.graphics.print("Total: " .. get_total(player_hand), margin_x, 120)

    if round_over then
        local function draw_win_message(message)
            love.graphics.print(message, margin_x, 260)
        end

        if hand_won(player_hand, dealer_hand) then
            draw_win_message("Player wins!")
        elseif hand_won(dealer_hand, player_hand) then
            draw_win_message("Dealer wins!")
        else
            draw_win_message("Draw")
        end
    end

    local function draw_button(button)
        if is_mouse_over_button(button) then
            love.graphics.setColor(1, 0.8, 0.3)
        else
            love.graphics.setColor(1, 0.5, 0.2)
        end
        love.graphics.rectangle("fill", button.x, button.y,
            button.width, button.height)
        love.graphics.setColor(1, 1, 1)
        love.graphics.print(button.text, button.x + button.text_offset_x,
            button.y + button.text_offset_y)
    end

    if round_over then
        draw_button(button_play_again)
    else
        draw_button(button_hit)
        draw_button(button_stand)
    end
end
