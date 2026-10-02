# 3D Renderer — C++

A basic **3D renderer built from scratch in C++** to understand the fundamentals of computer graphics and how 3D objects can be represented and rendered without using a graphics engine.

## 🚀 About the Project

This project implements a simple 3D rendering pipeline using C++. 3D objects are represented using **vertices and triangles**, which are then processed and converted into a 2D pixel-based output.

The project was built as a hands-on exploration of how rendering works at a lower level rather than relying on an existing 3D graphics engine.

## ✨ Features

* 3D vertices and coordinate representation
* Triangle-based 3D objects
* Basic 3D-to-2D projection
* Face-based coloring
* Pixel-level rendering
* PBM image output
* Built using C++ without a dedicated graphics engine

## 🧠 Concepts Explored

* 3D coordinate systems
* Vertices and triangles
* Vector representation
* Projection
* Rasterization
* Pixel-based image generation
* Basic rendering pipeline

## 🛠️ Technologies Used

* **C++**
* **Windows API**
* **PBM (Portable Bitmap)**
* **MinGW / GCC**

## 📂 Project Structure

```text
3D-Renderer/
│
├── main.cpp
├── renderer.cpp
├── renderer.h
└── README.md
```

> The exact file structure may vary depending on the current version of the project.

## ▶️ How to Run

### 1. Clone the repository

```bash
git clone <your-repository-link>
cd 3D-Renderer
```

### 2. Compile

Using MinGW/G++:

```bash
g++ main.cpp -o renderer3dextra.exe
```

If the project contains multiple `.cpp` files:

```bash
g++ *.cpp -o renderer3dextra.exe
```

### 3. Run

```bash
renderer3dextra.exe
```

The renderer generates the corresponding output based on the implementation.

## 📸 Output

The renderer produces a pixel-based representation of the 3D object.

*Add your rendered output image here once you upload it.*

## 🎯 Purpose

The main goal of this project was to understand the fundamentals behind a 3D rendering pipeline by implementing the core concepts manually in C++.

Rather than using an existing graphics library or engine, the project focuses on understanding **how 3D geometry eventually becomes a 2D image**.

## 🔮 Future Improvements

* Perspective projection
* Depth buffering (Z-buffer)
* Camera movement
* Object rotation
* Lighting and shading
* Texture mapping
* Interactive real-time rendering

## 👩‍💻 Author

**Joshika**

Built as a learning project to explore **C++, computer graphics, and low-level rendering concepts**.
