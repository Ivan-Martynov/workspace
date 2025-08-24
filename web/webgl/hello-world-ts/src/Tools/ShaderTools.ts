const ShaderTools = {
    createShader(
        gl: WebGLRenderingContext,
        type: number,
        source: string
    ): WebGLShader | null {
        const shader = gl.createShader(type);
        if (!shader) {
            console.error("Failed to create shader");
            return null;
        }
        gl.shaderSource(shader, source);
        gl.compileShader(shader);

        return shader;
        //if (gl.getShaderParameter(shader, gl.COMPILE_STATUS)) {
        //    return shader;
        //} else {
        //    console.log(gl.getShaderInfoLog(shader));
        //    gl.deleteShader(shader);
        //    return null;
        //}
    },

    async makeProgramAsync(
        gl: WebGLRenderingContext,
        shadersSources: { type: number; source: string }[]
    ): Promise<WebGLProgram> {
        const shaders: (WebGLShader | null)[] = [];

        const funs: (() => Promise<void>)[] = [];

        const program = gl.createProgram();

        for (let i = 0; i < shadersSources.length; ++i) {
            shaders.push(gl.createShader(shadersSources[i].type));
            funs.push(async () => {
                new Promise<void>((resolve) => {
                    const shader = shaders[i];
                    if (shader) {
                        gl.shaderSource(shader, shadersSources[i].source);
                        gl.compileShader(shader);
                        gl.attachShader(program, shader);
                        resolve();
                    }
                });
            });
        }

        await Promise.all(funs.map((fn) => fn()));

        gl.linkProgram(program);
        if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
            console.error(
                `Link program failed: ${gl.getProgramInfoLog(program)}`
            );
            shaders.forEach((shader) =>
                console.error(`Shader info: ${gl.getShaderInfoLog(shader!)}`)
            );
            gl.deleteProgram(program);
        }
        return program;
    },

    createProgram(
        gl: WebGLRenderingContext,
        vertexShaderParam: WebGLShader | string,
        fragmentShaderParam: WebGLShader | string
    ): WebGLProgram | null {
        const getShader = (
            type: number,
            param: WebGLShader | string
        ): WebGLShader | null => {
            if (param instanceof WebGLShader) {
                return param;
            } else {
                return ShaderTools.createShader(gl, type, param);
            }
        };

        const vertexShader = getShader(gl.VERTEX_SHADER, vertexShaderParam);
        ////if (!vertexShader) {
        ////    return null;
        ////}

        const fragmentShader = getShader(
            gl.FRAGMENT_SHADER,
            fragmentShaderParam
        );
        //if (!fragmentShader) {
        //    return null;
        //}

        const program = gl.createProgram();

        if (vertexShader) {
            gl.attachShader(program, vertexShader);
        }
        if (fragmentShader) {
            gl.attachShader(program, fragmentShader);
        }
        gl.linkProgram(program);

        if (gl.getProgramParameter(program, gl.LINK_STATUS)) {
            return program;
        } else {
            console.error(
                `Link program failed: ${gl.getProgramInfoLog(program)}`
            );
            console.error(
                `Vertex shader info: ${gl.getShaderInfoLog(vertexShader!)}`
            );
            console.error(
                `Fragment shader info: ${gl.getShaderInfoLog(fragmentShader!)}`
            );
            gl.deleteProgram(program);
            return null;
        }
    },
} as const;

export default ShaderTools;
