import CanvasRenderer2D from "../../shared/Renderer/CanvasRenderer2D";

const renderer = new CanvasRenderer2D("canvas");

for (let i = 0; i < 256; ++i) {
    // Generate colors randomly.
    const red = Math.random() * 255;
    const green = Math.random() * 255;
    const blue = Math.random() * 255;
    const alpha = Math.random() * 0.1 + 0.9;

    // Apply color before drawing a line with random end points.
    renderer
        .stroke(`rgba(${red},${green},${blue},${alpha})`)
        .line(
            Math.random() * renderer.width,
            Math.random() * renderer.height,
            Math.random() * renderer.width,
            Math.random() * renderer.height,
        );
}
