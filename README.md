# byhj-Render

　　　Welcome，this is a gpu render by byhj. I write it by using OpenGL3+ and Direct3D11. I classify this project by the book of real-time rending.

---

# Introduction
　　The whole project will be three parts:

## Render:
   This part is the render base code. It has resource(texture, model, audio) manager and the render framework.

## Sample:     
   The sample part include OpenGL3+ and DirectX11 Sample code. I also write the base supprot for  Next GPU API
(Direct3D12 & Vulkan).

## Utility:
   The utility part show the important extensions : camera, gui, font, audio, meshload, texture, window, script, resource

## Notice:
OpenGL:
For the shader effect, the opengl3+ will need, the shader file will be:
  - .vert : GL_VERTEX_SHADER
  - .tcs  : GL_TESS_CONTROL_SHADER
  - .tes  : GL_TESS_EVALUATION_SHADER
  - .geom : GL_GEOMETRY_SHADER
  - .farg : GL_FRAGMENT_SHADER
  - .comp : GL_COMPUTE_SHADER
Your can use [NShader](https://nshader.codeplex.com/) to make shader file heightlight.

Direct3D:

　　The D3DX is not include since Windows8+, so the d3dx support part is use old directx11 sdk.
For more information about d3dx obsolete, read the　[Living without D3DX](https://blogs.msdn.microsoft.com/chuckw/2013/08/20/living-without-d3dx/).

The direct3d shader file will be:
   - .vsh: D3D_VERTEX_SHADER
   - .hsh: D3D_HULL_SHADER
   - .dsh: D3D_DOMAIN_SHADER
   - .gsh: D3D_GEOMETRY_SHADER
   - .psh: D3D_PIXEL_SHADER
   - .csh: D3D_COMPUTE_SHADER

Vulkan:
     Vulkan is the next gpu api, more information read the [Vulkan](https://www.khronos.org/vulkan/)

---

# Requirements

## Compiler
  Visual Studio 2015 (I will write the cmake file in the future). I provide the (x86) dependences file only.
  Your can build the dependences by yourself.

## Dependences

### OpenGL
  -  [OpenGL Mathematics (GLM)](http://glm.g-truc.net/0.9.7/index.html)
  -  [GLFW](http://www.glfw.org/)
  -  [The OpenGL Extension Wrangler Library(GLEW)](http://glew.sourceforge.net/)
  -  [OpenGL Image (GLI)](http://gli.g-truc.net/0.8.1/index.html)

###  DirectX
  - [DirectX Tool Kit(DirectXTK)](https://github.com/Microsoft/DirectXTK)
  - [Effects for Direct3D 11(FX11)](https://github.com/Microsoft/FX11)

### Texture
  - [SOIL](http://www.lonesock.net/soil.html)
  - [DevIL](http://openil.sourceforge.net/)
  - [FreeImage](http://freeimage.sourceforge.net/)

### Model
  -  [Open Asset Import Library(Assimp)](http://www.assimp.org/)

### GUI
   - [AntWeakbar](http://anttweakbar.sourceforge.net/doc/)
   - [Crazy Eddie’s GUI(CEGUI)](http://cegui.org.uk/)
   - [DXUT](https://github.com/Microsoft/DXUT)

### Audio
  - [irrKlang](http://www.ambiera.com/irrklang/)

### Font
  - [Freetype](http://www.freetype.org/)
  - [FW1FontWrapper](https://fw1.codeplex.com/)
  - [SpriteFont](https://directxtk.codeplex.com/)

### Memory
  - [Visual Leak Detector(VLD)](https://vld.codeplex.com/)

### other
  - [Lua](https://www.lua.org/)
  - [TinyXML2](http://www.grinninglizard.com/tinyxml2/)

## Next GPU API Sample
  - [Vulkan SDK](https://vulkan.lunarg.com/app/download)
  - [Direct3D12](https://msdn.microsoft.com/en-us/library/windows/desktop/dn899121(v=vs.85).aspx)
    For the DirectX12, your need the [Win10SDK](https://developer.microsoft.com/zh-cn/windows/downloads/windows-10-sdk)

---

# How to Buil

 1. Make sure you develop environment is right. I write some code by c++ 11, so the compiler should support the
 c++11, use vs2015 will be the best choice or modifed by yourself if you use other compilers.

 2. Project Organization:

```
		  |Root
              |-- include
              |-- src
              |-- media
              |-- lib
              |-- extern
              |-- doc
        |Output
              |-- bin(Execute Dir)
              |-- temp(temp file we possibly not need)
```

  Notice: .props file is the command properties file, it set up the develop environment easily.

---

# Sample

## Triangle (Vulkan & Direct3D12)
<img width = "400" src="http://7xs5mn.com1.z0.glb.clouddn.com/vulkanTriangle.png">
---

## Triangle (Direct3D12)
<img width = "400" src="http://7xs5mn.com1.z0.glb.clouddn.com/dx12-cube.jpg">
---

## Texture (Vulkan)
<img width = "400" src="http://7xs5mn.com1.z0.glb.clouddn.com/vulkanTexutre.png">
---

## Billboard (OpenGL3+)
　　Use Geometry Shader to make billboard. The shader code is base the OpenGL Step by Step
[Tutorial27](http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html)

<img width = "400" src="http://7xs5mn.com1.z0.glb.clouddn.com/Billboard.png">

---
## Bloom (OpenGL3+)
　　Use gaussian blur to make the bloom effect. The detail tutorial read on
[Learning OpenGL](http://www.learnopengl.com/#!Advanced-Lighting/Bloom)

<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/bloom.png">

---
## Terrain (Direct3D11)
　　Use height map to make the terrain. Then, use slope based texturing to make grass and rock effect.
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/Terrain-height.png">

---
## Fog (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/fog.png">

---
## Bezier (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/ber.png">
---

## Normal Mapping (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/ogl-normal-map.png">

---
## Skybox (OpenGL3+）
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/skybox.png">

---
## Rim-Light (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/rim.png">

---

## Sprites (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/sprite.png">
---

## Texture-Array (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/tessarray.png">

---

## Texture-Light (Direct3D11)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/textLight.png">

---

## Toon Mapping (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/toon.png">
---

## Shadow Mapping (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/shadow-mapping.png">

---
## Tessellation-Triangle (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/tess.png">

---

## Render To Texture (OpenGL3+ & Direct3D11)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/RTT.png">

---
## SSAO (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/ssao.png">
---
## Layer Rendering （OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/layer.png">

---
## Blinn-Phong （OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/phong.png">
---

## Point-Light （OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/point-light.png">

----
## Spot-Light （OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/spot.png">

---
## Instance Rendering （OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/instance.png">

---

## Model Blending  （Direct3D11)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/blend.png">

---
## Fur (OpenGL3+)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/fur.png">

---

## CEGUI
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/cegui.png">

---
## DXUT
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/dxut.png">

---

## Deffered-Shading
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/deferred.jpg">

---
## Motion-Blur
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/motionblur.png">

---
## Mesh Load (OpenGL3+ & Direct3D11)
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/meshload.png">

---
