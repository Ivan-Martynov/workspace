import TrigonometryConstants from "../../shared/constants/TrigonometryConstants";
import CanvasRenderer2D from "../../shared/Renderer/CanvasRenderer2D";
import type { Point2D } from "../../shared/types/Point2D";
import Draw from "./drawModule";

const semiFixedDeltaTime = {
    run: (renderer: CanvasRenderer2D) => {
        const center: Point2D = {
            x: renderer.width * 0.5,
            y: renderer.height * 0.5,
        };

        let lastTime = performance.now();

        let angle = 0;

        // Using fixed value to ensure the same speed.
        const fraction = 12;
        const fixed_dt = fraction / 60;

        const render = (): void => {
            // Calculate current frame delta time and remember time point.
            const now = performance.now();
            // Now using mutable variable to adjust to the limits.
            let frameTime = (now - lastTime) * 1e-3;
            lastTime = now;

            while (frameTime > 0) {
                const deltaTime = Math.min(frameTime, fixed_dt);

                angle += deltaTime;
                if (angle > TrigonometryConstants.Pi2) {
                    angle -= TrigonometryConstants.Pi2;
                }

                frameTime -= deltaTime;
            }

            renderer.clear();
            Draw.moveUpDown(renderer, center, angle);
            Draw.pulse(renderer, center, angle);
            Draw.fade(renderer, center, angle);

            renderer
                .fill("black")
                .font("24px sans-serif")
                .text(`Fixed dt: ${fraction}/60`, renderer.width / 2, 30);

            requestAnimationFrame(render);
        };
        render();
    },
};

export default semiFixedDeltaTime;
