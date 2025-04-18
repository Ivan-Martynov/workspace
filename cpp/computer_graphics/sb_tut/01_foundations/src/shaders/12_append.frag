#version 450 core

// atomic counter for filled size
layout (binding = 0, offset = 0) uniform atomic_uint fill_counter;

// 2D image to store head pointers
layout (binding = 0, r32ui) coherent uniform uimage2D head_pointer;

// shader storage buffer containing appended fragments
struct list_item {
    vec4 color;
    float depth;
    int facing;
    uint next;
};

layout (binding = 0, std430) buffer list_item_block {
    list_item item[];
};

in VS_OUT {
    vec4 pos;
    vec4 color;
} fs_in;

void main() {
    const ivec2 p = ivec2(gl_FragCoord.xy);

    const uint idx = atomicCounterIncrement(fill_counter);

    const uint old_head = imageAtomicExchange(head_pointer, p, idx);

    item[idx].color = fs_in.color;
    item[idx].depth = gl_FragCoord.z;
    item[idx].facing = gl_FrontFacing ? 1 : 0;
    item[idx].next = old_head;
}

