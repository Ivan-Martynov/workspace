import CanvasRenderer2D from "../../shared/Renderer/CanvasRenderer2D";
import type { Point2D } from "../../shared/types/Point2D";
import Draw from "./drawModule";

const eachFrame = {
    run: (renderer: CanvasRenderer2D) => {
        const center: Point2D = {
            x: renderer.width * 0.5,
            y: renderer.height * 0.5,
        };

        const fixed_dt = 1 / 60;

        let angle = 0;

        const render = (): void => {
            angle += fixed_dt * 2;
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
                .text("Each frame", renderer.width / 2, 30);

            requestAnimationFrame(render);
        };
        render();
    },
};

export default eachFrame;
