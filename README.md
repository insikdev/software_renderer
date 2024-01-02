# software_renderer

### dependency
- SDL
- glm

### vertex shader
- local space -> clip space

### rasterizer
1. perspective division
	- clip space -> NDC
2. viewport transform
3. depth test
4. interpolation using barycentric coordinate

### fragment shader