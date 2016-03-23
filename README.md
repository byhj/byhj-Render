# byhj-Render

　　　Welcome，this is a render by byhj. I write it by using OpenGL3+ and DirectX11. I classify this project by the book of real-time rending chapter.

---

# Introduction
　　The all project will be three part:

## Render:
   This part include the render base code. It has resource manager and other.

## Sample:     
   The sample has OpenGL3+ and DirectX11 Sample code. I also write the Next GPU API
base part(DirectX12 & Vulkan).

## Utility:
   The utility show the render some part how it work. It include camera, gui, font, audio, mesh, texture, window.

---

# Requirements

## Compiler
  - Visual Studio 2015 (I will write the cmake file in the future). Your can build the dependences
  by yourself if your want.

## Dependences:
  - Texture (DevIL, FreeImage, SOIL, DirectXTK, gli)
  - Model (Assimp)
  - GUI (AntWeakbar, CEGUI, DXUT)
  - Winodw (glfw3)
  - Audio (irrKlang, XAudio)
  - Font (Freetype, FW1FontWrapper, SpriteFont)
  - Memory (VLD)

## Next GPU API Sample
    -  Vulkan (SPIRV, glslang)
    -  DiectX12
---

# How to Buil

 1. Make sure you develop environment is right. I write some code by c++ 11, so the compiler should support the

 2. You can build the environment by yourself according your target.
 3. Project Organization:

		  |Root

              |-- include
              |-- src
              |-- media
              |-- lib
              |-- extern
 Notice: I use the Visual Studio project props file to share the project setting.

         The output dir:

              |-- bin(Execute Dir)
              |-- temp(temp file we possibly not need)

---

# Sample

## Billboard (OpenGL3+)
　　Use Geometry Shader to make billboard. The shader coed is base the OpenGL Step by Step
[Tutorial27](http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html)

<img width = "400" src="http://7xs5mn.com1.z0.glb.clouddn.com/Billboard.png">

---
## Bloom (OpenGL3+)
　　Use gaussian blur to make the bloom effect. The detail tutorial read on
[Learning OpenGL](http://www.learnopengl.com/#!Advanced-Lighting/Bloom)

<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/bloom.png">

---
## Terrain (DirectX11)
　　Use height map to make the terrain. Then, use slope based texturing to make grass and rock effect.
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/Terrain-height.png">

---
## Normal Mapping (OpenGL3+)
　　Use normal texture to make the normal mapping effect.
<img width = "400" src= "http://7xs5mn.com1.z0.glb.clouddn.com/ogl-normal-map.png">

---
## Skybox (OpenGL3+ & DirectX11)
Use normal texture to make the normal mapping effect.

---
## Fog (OpenGL3+)

---
## Environment Mapping (OpenGL3+)

---
## Bezier (OpenGL3+)

---
## GUI

---
## Rim-Light (OpenGL3+)

---
## SSAO (OpenGL3+)

---
## Shadow Mapping (OpenGL3+)

---
## Sprites (OpenGL3+)

---
## Tessellation (OpenGL3+)

---
## Fur (OpenGL3+)

---
## Instance Draw (OpenGL3+)
---
## Water
---
## Screen-Fades (DirectX11)
---
## MSAA
---
