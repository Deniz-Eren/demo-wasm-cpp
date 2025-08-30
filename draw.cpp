/**
 * \file    draw.cpp
 *
 * Copyright (C) 2025 Deniz Eren (deniz.eren@outlook.com)
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *
 * For more information, please refer to <https://unlicense.org/>
 */

#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <vector>
#include <cmath>

using namespace emscripten;

struct Point {
    int x, y;
};

std::vector<Point> points;
val ctx = val::undefined();
val canvas = val::undefined();

int dragIndex = -1;
int circleRadius = 8;

// --- Pan / zoom ---
float offsetX = 0, offsetY = 0;
float zoom = 1.0f;
bool panning = false;
bool didDrag = false;
int lastMouseX = 0, lastMouseY = 0;

void init() {
    canvas = val::global("document").call<val>("getElementById", std::string("canvas"));
    ctx = canvas.call<val>("getContext", std::string("2d"));
}

void draw() {
    int w = canvas["width"].as<int>();
    int h = canvas["height"].as<int>();

    // Save transform
    ctx.call<void>("save");

    // Reset transform to identity to clear in pixel space
    ctx.call<void>("setTransform", 1, 0, 0, 1, 0, 0);
    ctx.call<void>("clearRect", 0, 0, w, h);

    // Restore pan/zoom transform
    ctx.call<void>("restore");
    ctx.call<void>("setTransform", zoom, 0, 0, zoom, offsetX, offsetY);

    // Draw lines
    if(points.size() > 1) {
        ctx.call<void>("beginPath");
        ctx.call<void>("moveTo", points[0].x, points[0].y);
        for (size_t i = 1; i < points.size(); i++) {
            ctx.call<void>("lineTo", points[i].x, points[i].y);
        }
        ctx.call<void>("stroke");
    }

    // Draw filled circles
    ctx.set("fillStyle", std::string("red"));
    for(auto &p: points) {
        ctx.call<void>("beginPath");
        ctx.call<void>("arc", p.x, p.y, circleRadius, 0, 2*M_PI);
        ctx.call<void>("fill");
    }
}

int findPoint(int x, int y) {
    // convert mouse coords to logical coordinates
    float lx = (x - offsetX) / zoom;
    float ly = (y - offsetY) / zoom;
    for (size_t i = 0; i < points.size(); i++) {
        float dx = lx - points[i].x;
        float dy = ly - points[i].y;
        if (dx*dx + dy*dy <= circleRadius * circleRadius) {
            return (int)i;
        }
    }
    return -1;
}

void mouseDown(int x, int y) {
    dragIndex = findPoint(x, y);
    lastMouseX = x;
    lastMouseY = y;
    panning = (dragIndex == -1);
    didDrag = false;
}

void mouseMove(int x, int y) {
    if(dragIndex != -1) {
        points[dragIndex] = {(int)((x - offsetX)/zoom), (int)((y - offsetY)/zoom)};
    } else if(panning) {
        offsetX += x - lastMouseX;
        offsetY += y - lastMouseY;
        didDrag = true;
    }
    lastMouseX = x;
    lastMouseY = y;
    draw();
}

void mouseUp() {
    if (dragIndex == -1 && !didDrag) {
        if(dragIndex == -1) points.push_back({(int)((lastMouseX - offsetX)/zoom), (int)((lastMouseY - offsetY)/zoom)});
        draw();
    }

    dragIndex = -1;
    panning = false;
}

void wheel(int deltaY, int mouseX, int mouseY) {
    float factor = (deltaY > 0) ? 0.9f : 1.1f;
    float lx = (mouseX - offsetX) / zoom;
    float ly = (mouseY - offsetY) / zoom;
    zoom *= factor;
    offsetX = mouseX - lx * zoom;
    offsetY = mouseY - ly * zoom;
    draw();
}

EMSCRIPTEN_BINDINGS(my_module){
    function("init", &init);
    function("draw", &draw);
    function("mouseDown", &mouseDown);
    function("mouseMove", &mouseMove);
    function("mouseUp", &mouseUp);
    function("wheel", &wheel);
}
