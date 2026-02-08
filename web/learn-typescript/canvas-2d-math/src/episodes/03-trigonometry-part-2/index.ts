import CanvasRenderer2D from "../../shared/Renderer/CanvasRenderer2D";
import eachFrame from "./each-frame";
import fixedDeltaTime from "./fixed-delta-time";
import semiFixedDeltaTime from "./semi-fixed-delta-time";
import variableDeltaTime from "./variable-delta-time";

const renderer1 = new CanvasRenderer2D("canvas1");
const renderer2 = new CanvasRenderer2D("canvas2");
const renderer3 = new CanvasRenderer2D("canvas3");
const renderer4 = new CanvasRenderer2D("canvas4");

for (const r of [renderer1, renderer2, renderer3, renderer4]) {
    r.canvas.width = window.innerWidth / 2 - 30;
    r.canvas.height = window.innerHeight / 2;
}

type methodType =
    | "each-frame"
    | "variable-delta-time"
    | "semi-fixed-delta-time"
    | "fixed-delta-time";

const runMethod = (renderer: CanvasRenderer2D, method: methodType): void => {
    console.log(method);
    switch (method) {
        case "each-frame":
            eachFrame.run(renderer);
            break;
        case "semi-fixed-delta-time":
            semiFixedDeltaTime.run(renderer);
            break;
        case "fixed-delta-time":
            fixedDeltaTime.run(renderer);
            break;
        case "variable-delta-time":
            variableDeltaTime.run(renderer);
            break;
    }
};

runMethod(renderer1, "fixed-delta-time");
runMethod(renderer2, "each-frame");
runMethod(renderer3, "semi-fixed-delta-time");
runMethod(renderer4, "variable-delta-time");
