<h1 align="center">Silverlight Engine</h1>

<p align="center">
  <img src ="https://github.com/user-attachments/assets/59d3eefa-d47b-4818-ab9f-14efaa31aa9a" alt="image">
</p>

<p>Silverlight is a modern, high-performance C++ and Vulkan-based 3D rendering engine designed for real-time graphics, game development, and interactive applications. Built from the ground up for efficiency and flexibility, the engine provides a powerful foundation for rendering visuals with Vulkan.</p>

<h2>Key Features</h2>
<ul>
<li> Advanced Lighting System: Directional, point, and spotlights with customizable properties. </li>
<li> Dynamic Shadows: Uses shadow mapping to project shadows efficiently. </li>
<li> Custom Model Loading: Supports glTF/glb files with per-submesh texture mapping. </li>
<li> Primitive Shapes: Triangle, cube, and plane with custom textures and per-vertex coloring. </li>
<li> Optimized Vulkan Renderer: Efficient rendering pipeline for high-performance graphics. </li>
<li> Logging System: Outputs to console and external log files. </li>
<li> Camera System: Free movement and view matrix handling. </li>
<li> Input System: Keyboard and mouse input for interactive applications. </li>
</ul>

<h2>Upcoming Features</h2>
<p>The following features will be added in the next few updates:</p>

<ul>
<li> ✅ An improved lighting system to support more light casters, including directional lights, point lights, and spotlights </li>
<li> ✅ Support for multiple lights in a scene </li>
<li> ✅ Add shadows </li>
</ul>

<h1>Getting Started</h1>
<p>To get started
<ul>
<li> 1️⃣ Clone the repository. </li>
<li> 2️⃣ Open the solution in Microsoft Visual Studio 2022.</li>
<li> 3️⃣ Install the Vulkan SDK from LunarG's website.</li>
<li> 4️⃣ Build & run the engine to start rendering!</li>
</ul>

Please note that a project build system is not yet in place, but one will be coming soon. Additionally, Silverlight has only been tested on Windows, so compatibility with other operating systems is not guaranteed.</p>

<h1>Architecture Overview</h1>
<p>Silverlight follows a modular layered architecture for maintainability and scalability.</p>
<ul>
<li>Foundation Layer: Core utilities (logging, input system, ECS, timers, resource management).</li>
<li>Graphics Layer: Vulkan-based rendering, shaders, materials, lighting, mesh and camera systems.</li>
<li>Application Layer: The high-level entry point, connecting engine and components for execution.</li>
</ul>

<h1>Why Silverlight Engine?</h1>
<p>Silverlight is built with modern rendering techniques in mind, focusing on performance, flexibility, and extensibility. The engine uses modern C++ (C++20) to maximize performance and code security. Whether you're developing a game, simulation, or visualization tool, Silverlight provides a robust foundation for real-time 3D graphics!</p>

<p>Copyright (c) 2023-2025 Rony Hanna. All rights reserved.</p>
