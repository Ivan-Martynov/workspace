import CanvasRenderer2D from "../../shared/Renderer/CanvasRenderer2D";
import type { Point2D } from "../../shared/types/Point2D";
import Draw from "./drawModule";

const variableDeltaTime = {
    run: (renderer: CanvasRenderer2D) => {
        const center: Point2D = {
            x: renderer.width * 0.5,
            y: renderer.height * 0.5,
        };

        let lastTime = performance.now();

        let angle = 0;

        const render = (): void => {
            // Calculate current frame delta time and remember time point.
            const now = performance.now();
            const deltaTime = (now - lastTime) * 1e-3;
            lastTime = now;

            // Using delta time in calculation to adapt to current frame rate.
            // angle += angleStep * deltaTime;
            angle += deltaTime;
            if (angle > Math.PI * 2) {
                angle -= Math.PI * 2;
            }

            renderer.clear();
            Draw.moveUpDown(renderer, center, angle);
            Draw.pulse(renderer, center, angle);
            Draw.fade(renderer, center, angle);

            renderer
                .fill("black")
                .font("24px sans-serif")
                .text("Variable dt", renderer.width / 2, 30);

            requestAnimationFrame(render);
        };
        render();
    },
};

export default variableDeltaTime;
