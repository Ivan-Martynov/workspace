import type { Point2D } from "../types/Point2D";

class CanvasRenderer2D {
    #canvas: HTMLCanvasElement;
    #context: CanvasRenderingContext2D;

    public constructor(canvas: HTMLCanvasElement);
    public constructor(canvas: string);
    public constructor(canvas: HTMLCanvasElement | string) {
        if (typeof canvas === "string") {
            const cvs = document.getElementById(canvas) as HTMLCanvasElement;
            if (!cvs) {
                throw new Error("Couldn't find canvas element!");
            }
            this.#canvas = cvs;
            // this.#canvas.width = window.innerWidth;
            // this.#canvas.height = window.innerHeight;
        } else {
            this.#canvas = canvas;
        }

        const ctx = this.#canvas.getContext("2d");
        if (!ctx) {
            throw new Error("Canvas 2D is not supported!");
        }
        this.#context = ctx;
    }

    public get canvas() {
        return this.#canvas;
    }

    public get context() {
        return this.#context;
    }

    public get width(): number {
        return this.#canvas.width;
    }

    public get height(): number {
        return this.#canvas.height;
    }

    clearRect(
        x: number,
        y: number,
        width: number,
        height: number,
    ): CanvasRenderer2D {
        this.#context.clearRect(x, y, width, height);
        return this;
    }

    clear(): CanvasRenderer2D {
        this.clearRect(0, 0, this.#canvas.width, this.#canvas.height);
        return this;
    }

    fill(color: CanvasFillStrokeStyles["fillStyle"]): CanvasRenderer2D {
        this.#context.fillStyle = color;
        return this;
    }

    stroke(
        color: CanvasFillStrokeStyles["strokeStyle"],
        width = 1,
    ): CanvasRenderer2D {
        this.#context.strokeStyle = color;
        this.#context.lineWidth = width;
        return this;
    }

    rect(
        x: number,
        y: number,
        width: number,
        height: number,
        doFill = true,
    ): CanvasRenderer2D {
        doFill
            ? this.#context.fillRect(x, y, width, height)
            : this.#context.strokeRect(x, y, width, height);
        return this;
    }

    circle(
        x: number,
        y: number,
        radius: number,
        doFill = true,
    ): CanvasRenderer2D {
        this.#context.beginPath();
        this.#context.arc(x, y, radius, 0, Math.PI * 2, false);
        doFill ? this.#context.fill() : this.#context.stroke();

        return this;
    }

    line(x0: number, y0: number, x1: number, y1: number): CanvasRenderer2D {
        this.#context.beginPath();
        this.#context.moveTo(x0, y0);
        this.#context.lineTo(x1, y1);
        this.#context.stroke();

        return this;
    }

    bezierLine(
        p0: Point2D,
        p1: Point2D,
        p2: Point2D,
        p3: Point2D,
    ): CanvasRenderer2D {
        this.#context.beginPath();
        this.#context.moveTo(p0.x, p0.y);
        this.#context.bezierCurveTo(p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);
        this.#context.stroke();

        return this;
    }

    font(desc: string): CanvasRenderer2D {
        this.#context.font = desc;
        return this;
    }

    text(text: string, x: number, y: number, doFill = true): CanvasRenderer2D {
        doFill
            ? this.#context.fillText(text, x, y)
            : this.#context.strokeText(text, x, y);

        return this;
    }

    applyShadow(shadow: {
        color: CanvasShadowStyles["shadowColor"];
        offsetX: number;
        offsetY: number;
        blur: number;
    }): CanvasRenderer2D {
        if (shadow) {
            this.#context.shadowColor = shadow.color;
            this.#context.shadowOffsetX = shadow.offsetX;
            this.#context.shadowOffsetY = shadow.offsetY;
            this.#context.shadowBlur = shadow.blur;
        } else {
            this.#context.shadowColor = "transparent";
            this.#context.shadowOffsetX = 0;
            this.#context.shadowOffsetY = 0;
            this.#context.shadowBlur = 0;
        }

        return this;
    }

    save(): CanvasRenderer2D {
        this.#context.save();
        return this;
    }

    restore(): CanvasRenderer2D {
        this.#context.restore();
        return this;
    }

    translate(x: number, y: number): CanvasRenderer2D {
        this.#context.translate(x, y);
        return this;
    }

    scale(x: number, y: number): CanvasRenderer2D {
        this.#context.scale(x, y);
        return this;
    }

    rotate(angle: number): CanvasRenderer2D {
        this.#context.rotate(angle);
        return this;
    }

    image(src: CanvasImageSource): CanvasRenderer2D {
        this.#context.drawImage(src, 0, 0);
        return this;
    }
}

export default CanvasRenderer2D;
