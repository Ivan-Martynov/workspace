import vertexShaderSource from "./shaders/vertex.glsl?raw";
import fragmentShaderSource from "./shaders/fragment.glsl?raw";
import ShaderTools from "./Tools/ShaderTools";

const canvas = document.getElementById("app") as HTMLCanvasElement;
if (!canvas) {
    throw new Error("Canvas element not found");
}

const gl = canvas.getContext("webgl");
if (!gl) {
    throw new Error("Webgl not supported!");
}

const program = await ShaderTools.makeProgramAsync(gl, [
    { type: gl.VERTEX_SHADER, source: vertexShaderSource },
    { type: gl.FRAGMENT_SHADER, source: fragmentShaderSource },
]);

//const program = ShaderTools.createProgram(
//    gl,
//    vertexShaderSource,
//    fragmentShaderSource
//);
if (!program) {
    throw new Error("Failed to create webgl program.");
}

const positionAttributeLocation = gl.getAttribLocation(program, "a_position");
const positionBuffer = gl.createBuffer();
gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

// three 2d points
const positions = [0, 1, -1, -1, 1, -1];
gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(positions), gl.STATIC_DRAW);
//webglUtils.resizeCanvasToDisplaySize(gl.canvas);
gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);

// Clear the canvas
gl.clearColor(0, 0, 0, 0);
gl.clear(gl.COLOR_BUFFER_BIT);

// Tell it to use our program (pair of shaders)
gl.useProgram(program);

gl.enableVertexAttribArray(positionAttributeLocation);

// Bind the position buffer.
gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

// Tell the attribute how to get data out of positionBuffer (ARRAY_BUFFER)
// 2 components per iteration
const size = 2;
// the data is 32bit floats
const type = gl.FLOAT;
// don't normalize the data
const normalize = false;
// 0 = move forward size * sizeof(type) each iteration to get the next position
const stride = 0;
// start at the beginning of the buffer
const offset = 0;
gl.vertexAttribPointer(
    positionAttributeLocation,
    size,
    type,
    normalize,
    stride,
    offset
);
gl.drawArrays(gl.TRIANGLES, 0, 3);
