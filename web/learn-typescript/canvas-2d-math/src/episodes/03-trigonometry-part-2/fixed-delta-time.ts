import TrigonometryConstants from "../../shared/constants/TrigonometryConstants";
import CanvasRenderer2D from "../../shared/Renderer/CanvasRenderer2D";
import type { Point2D } from "../../shared/types/Point2D";
import Draw from "./drawModule";

const fixedDeltaTime = {
    run: (renderer: CanvasRenderer2D) => {
        const center: Point2D = {
            x: renderer.width * 0.5,
            y: renderer.height * 0.5,
        };

        let lastTime = performance.now();

        let angle = 0;

        // Using fixed value to ensure the same speed.
        const fraction = 1;
        const fixed_dt = fraction / 60;

        // Using accumulator to adjust the time.
        let accumulator = 0;

        const render = (): void => {
            // Calculate current frame delta time and remember time point.
            const now = performance.now();
            // Now using mutable variable to adjust to the limits.
            // Add max threshold to avoid spiral of death?
            const threshold = 0.25; // One forth of a second.
            let frameTime = Math.min((now - lastTime) * 1e-3, threshold);
            lastTime = now;

            accumulator += frameTime;
            let prevAngle = -1;
            while (accumulator > fixed_dt) {
                prevAngle = angle;

                angle += fixed_dt;

                accumulator -= fixed_dt;
            }

            // Interpolate between the previous and current state
            const t = accumulator / fixed_dt;
            const nextAngle =
                prevAngle > -1 ? angle * t + prevAngle * (1 - t) : angle;

            if (angle > TrigonometryConstants.Pi2) {
                angle -= TrigonometryConstants.Pi2;
            }

            renderer.clear();
            Draw.moveUpDown(renderer, center, nextAngle);
            Draw.pulse(renderer, center, nextAngle);
            Draw.fade(renderer, center, nextAngle);

            renderer
                .fill("black")
                .font("24px sans-serif")
                .text(`Fixed dt: ${fraction}/60`, renderer.width / 2, 30);

            requestAnimationFrame(render);
        };
        render();
    },
};

export default fixedDeltaTime;
