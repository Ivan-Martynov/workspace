# Trigonometry examples, showing oscillation behaviour.

1. The moveUpDown function shows a circle moving up and down;
2. The pulse function shows a circle changing its size;
3. The fade function shows a circle changing its transparency.

# Timing issues

You can read more on the topic at
[gaffer-fix-timestep](https://gafferongames.com/post/fix_your_timestep/)).

The change/update happens when an angle changes. And we change with a fixed
value to maintain smooth animation.

1. The animation update happens on each `requestAnimationFrame` call. And that
   depends on how fast the computer refreshes the screen. That means, the speed
   will vary as some machines are slower and some are faster.

2. We can calculate the delta time between two consecutive frame, thus providing
   a flexible frame rate, which can be used to adjust the speed of angle change.
   This approach might be good for camera movement, UI animations. However, for
   games and physics it might produce different feel depending on the frame
   rate, as delta is not fixed and, in theory and practice, can change
   significantly, potentially breaking the logic.

3. We can set a maximum value for the delta time. Now we ensure that the delta
   time will not be bigger than the desire threshold. The drawback here is the
   need to run update method few times before rendering (in the case when frame
   rate is bigger than our max value), which might be an issue when the update
   part is time consuming, which can lead to lags, as the program cannot catch
   up with simulations. That is, it takes a lot of time to simulate the physics
   and on the next frame the simulations falls behind, and on the next frame
   falls even more... Eventually, leading to a program/browser freeze or crash.

4. Almost same as before, but setting a threshold for a frame time and using an
   accumulator to keep track of time, subtracting the fixed delta time (the one
   to be used for update logic), until the accumulated time is not less than the
   delta time. At the next frame, adding new frame time to the remaining
   accumulated time and repeat the process by reducing the accumulated time by
   the fixed delta time, updating the simulation. We also can interpolate
   between the previous and current states to blend into a yet smoother
   simulation.
