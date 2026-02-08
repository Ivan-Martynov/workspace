import CanvasRenderer2D from "../../shared/Renderer/CanvasRenderer2D";

const renderer = new CanvasRenderer2D("canvas");

// Translate the context closer to the center.
renderer.translate(0, renderer.height * 0.5);
// Flip the y-axis for the context as by default the y-axis points
// downwards.
renderer.scale(1, -1);

const graphScale = 200;
for (let angle = 0; angle < Math.PI * 2; angle += 0.01) {
    const x = angle * graphScale;
    const y = Math.sin(angle) * graphScale;

    renderer.rect(x, y, 3, 3);
}
