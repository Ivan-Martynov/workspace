import type CanvasRenderer2D from "../../shared/Renderer/CanvasRenderer2D";
import type { Point2D } from "../../shared/types/Point2D";

const Draw = {
    moveUpDown: (
        renderer: CanvasRenderer2D,
        center: Point2D,
        angle: number,
    ): void => {
        const baseRadius = 37;
        const offset = center.y - baseRadius;

        // Set circle's position using sine.
        const y = center.y + Math.sin(angle) * offset;

        renderer.fill("green").circle(center.x, y, baseRadius);
    },

    // Simple pulsation animation, making a circle change its size.
    pulse: (
        renderer: CanvasRenderer2D,
        center: Point2D,
        angle: number,
    ): void => {
        const baseRadius = 100;
        const offset = 50;

        // In this case, using sine to oscillate circle's radius.
        const radius = baseRadius + Math.sin(angle) * offset;

        renderer.fill("purple").circle(renderer.width / 4, center.y, radius);
    },

    // Make a circe appear and disappear in the sine rhythm.
    fade: (
        renderer: CanvasRenderer2D,
        center: Point2D,
        angle: number,
    ): void => {
        const baseAlpha = 0.5;
        const baseRadius = 100;
        const offset = 0.5;

        // Now changing alpha parameter using trigonometry.
        const alpha = baseAlpha + Math.sin(angle) * offset;

        renderer
            .fill(`rgba(255, 128, 0, ${alpha})`)
            .circle((renderer.width / 4) * 3, center.y, baseRadius);
    },
};

export default Draw;
