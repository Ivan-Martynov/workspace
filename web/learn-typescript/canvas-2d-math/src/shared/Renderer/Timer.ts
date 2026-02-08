class Timer {
    #lastTime: number = 0;
    #currentTime: number = 0;
    #deltaTime: number = 0;
    #fps: number = 0;
    #fpsCount: number = 0;
    #fpsTime: number = 0;

    public constructor() {
        this.reset();
    }

    public reset(): void {
        const now = performance.now();
        this.#lastTime = now;
        this.#currentTime = now;
        this.#deltaTime = 0;
        this.#fps = 0;
        this.#fpsCount = 0;
        this.#fpsTime = 0;
    }

    public tick(): void {
        this.#lastTime = this.#currentTime;
        this.#currentTime = performance.now();
        this.#deltaTime = this.#currentTime - this.#lastTime;

        // FPS calculation
        this.#fpsTime += this.#deltaTime;
        ++this.#fpsCount;

        if (this.#fpsTime >= 1e3) {
            // Update FPS every second
            this.#fps = (this.#fpsCount * 1e3) / this.#fpsTime;
            this.#fpsCount = 0;
            this.#fpsTime = 0;
        }
    }

    public get deltaTime(): number {
        return this.#deltaTime;
    }

    public get deltaTimeSeconds(): number {
        return this.#deltaTime * 1e-3;
    }

    public get FPS(): number {
        return this.#fps;
    }

    public get FPSCount(): number {
        return this.#fpsCount;
    }

    public get currentTime(): number {
        return this.#currentTime;
    }
}

export default Timer;
