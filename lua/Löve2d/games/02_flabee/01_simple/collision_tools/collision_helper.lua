local CollisionHelper = {}

function CollisionHelper.AABB_collision(first_rectangle, second_rectangle)
    return (first_rectangle.x + first_rectangle.width >= second_rectangle.x)
        and (first_rectangle.x <= second_rectangle.x + second_rectangle.width)
        and (first_rectangle.y + first_rectangle.height >= second_rectangle.y)
        and (first_rectangle.y <= second_rectangle.y + second_rectangle.height)
end

return CollisionHelper