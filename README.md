# WebAssembly C++ Canvas Demo

This project is a **WebAssembly demo** that showcases interactive pan, zoom, and drawing functionality on an HTML5 canvas. All core logic (drawing, transform handling, and event processing) is implemented in **C++**, compiled to WebAssembly, while JavaScript only handles event forwarding to the C++ side.

## Features

- Written in C++ and compiled to WebAssembly.
- Pan and zoom support on the canvas.
- Dot placement via mouse clicks.
- All application logic is handled in C++, with minimal JavaScript glue code.
- Cross-platform build system powered by CMake.

## Project Structure

    ├── CMakeLists.txt     # Build configuration
    ├── draw.cpp           # C++ source code
    ├── index.html         # Demo web page with JS glue code
    └── README.md          # This file

## Building on Ubuntu

### Install dependencies

First, install the required packages:

    sudo apt update
    sudo apt install cmake build-essential emscripten

### Configure and build

Create a build directory and run CMake with the Emscripten toolchain:

    mkdir build
    cd build
    emcmake cmake ..
    cmake --build .

This will produce a `.wasm` module and corresponding JavaScript glue code.

## Running the Demo

To run locally, you need a web server (browsers block `file://` access for WebAssembly):

    cd build
    ../run.sh

Now open [http://localhost:8080/index.html](http://localhost:8080/index.html) in your browser.

## AI Assistance

This project was heavily developed with the help of AI, specifically **ChatGPT**, which assisted in designing, implementing, and refining the WebAssembly C++ demo code.

## Usage

- **Left-click + drag** → pan the canvas.
- **Scroll wheel** → zoom in/out around the cursor.
- **Click (without dragging)** → add a point.
- **Click + drag on a point** → reposition that point.

## Notes

- This is a **WebAssembly demo**: the browser is executing your C++ code natively via WASM.
- No interaction logic is implemented in JavaScript — the canvas state and drawing pipeline live fully in C++.
- The canvas clears only the **visible region** before redraws, ensuring performance.

## License

This project is released under the **Unlicense**, placing the code in the public domain. You are free to use, modify, and distribute this software without restriction. See [LICENSE](https://unlicense.org/) for details.
