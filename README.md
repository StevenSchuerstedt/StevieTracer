# StevieTracer

StevieTracer is a simple path tracer written in C++.
I tried to stay really close to the mathematical formulation of path tracing, so you can see the corresponding equations in the code.

- only spheres supported
- light sources
- Next event estimation
- russian roulette to terminate paths
- ppm image format for output
- metal and lambertian materials
- based on the pbrt renderer

read more: https://stevenschuerstedt.github.io/post/computer-graphics-and-path-tracing/

## Building
no cmake or sln, so setup a project by yourself, only glm is needed as dependency. 

![alt text](https://github.com/StevenSchuerstedt/StevieTracer/blob/main/render2.png)
