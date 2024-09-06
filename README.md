<h1 align="center">Banshee Engine</h1>

<p align="center">
<img src="https://github.com/Rony-Hanna/banshee/assets/62596512/896ba616-c9d9-460e-b240-b05d47ba0984" alt="image">
</p>

<p>Banshee is a robust C++ and Vulkan-based 3D rendering engine designed to easily create interactive applications. The engine is still in an early development stage and much work is yet to be carried out. Currenty, the engine supports the following features:</p>

<ul>
<li> Custom model loading with the ability to load textures for each sub-mesh within a complex model </li>
<li> Input handling </li>
<li> Logging (console and external file) </li>
<li> Camera system </li>
<li> Efficient rendering system with the Vulkan API </li>
<li> Lighting (basic phong shading implementation) </li>
<li> Functional but currently minimal material system </li>
</ul>

<h2>Upcoming Features</h2>
<p>The next update will bring the following features:</p>

<ul>
<li> An improved lighting system to support for more light casters, including directional lights, point lights, and spotlights </li>
<li> Support for multiple lights in a scene </li>
</ul>

<h1>Getting Started</h1>
<p>To get started, clone the repository and open the solution file in Microsoft Visual Studio 2022. After that, build and run the project. Please note that a project build system is not yet in place, but one will be coming soon. Additionally, Banshee has only been tested on Windows, so compatibility with other operating systems is not guaranteed.</p>

<h1>The Plan</h1>
<p>Banshee aims to be a powerful easy-to-use, plug-and-play rendering engine. It outputs core functionality to a Dynamic-Link Library (DLL) file, enabling users to link their projects with the Banshee library and start rendering.</p>

<h1>Future Features</h1>
<p>Planned features include:</p>
<ul>
<li> Normal mapping </li>
<li> Shadow mapping </li>
<li> Ambient occlusion </li>
<li> Improved input system to handle joysticks </li>
<li> Improved lighting system to efficiently handle a huge amount of light sources (deferred rendering) </li>
<li> Physics integration (e.g., Bullet Physics for collision detection) </li>
<li> Audio system </li>
<li> Post-processing effects </li>
</ul>

<h1>Banshee's Architecture</h1>
<p>Banshee utilizes a layered architecture approach, this is to ensure modularity and ease of maintenance. Currently, there are 3 layers that make up the engine's architecture.</p>

<h2>Foundation Layer</h2>
<p>The Foundation layer contains core functionalities essential for the engine, including:</p>

<ul>
<li> Logging system </li>
<li> Resource loading mechanisms to load files and images </li>
<li> INI file parsing </li>
<li> Input handling (utilizes the GLFW library) </li>
<li> Entity-Component System </li>
<li> Observer pattern to easily notify for when a component is changed </li>
<li> Timer and other core utilities </li>
</ul>

<h2>Graphics Layer</h2>
<p>Built on top of the Foundation layer, the Graphics layer manages all graphics-related operations, including:</p>

<ul>
<li> Vulkan renderer </li>
<li> Mesh system </li>
<li> Light system </li>
<li> Camera class </li>
<li> Window management (utilizes the GLFW library) </li>
</ul>

<h2>Application Layer</h2>
<p>The Application layer integrates components from the Foundation and Graphics layers. It includes the main entry point of the program, orchestrating the initialization and execution of the engine.</p>


<p>Copyright (c) 2023-2024 Rony Hanna. All rights reserved.</p>