#ifndef VulkanApp_H
#define VulkanApp_H

#include "app.h"

#include <windows.h>
#include <fcntl.h>
#include <io.h>

#include <iostream>
#include <chrono>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <string>
#include <array>

#include <vulkan/vulkan.h>

namespace byhj {

   class VulkanApp : public App {
     public:
		 VulkanApp() {};
		 virtual ~VulkanApp(){};

		 void v_run();

		 virtual void v_init() {}
		 virtual void v_render() {}
		 virtual void v_update() {}
		 virtual void v_shutdown() {}

		 LRESULT CALLBACK handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		 int getClientWidth() const;
		 int getClientHeight() const;
		 HWND getHwnd() {
		 return m_hWnd;
		 }
		 HINSTANCE getHinstance() {
		   return m_hInstance;
		  }

     private:
		 int m_clientWidth;
		 int m_clientHeight;
		 std::string m_title = "Vulkan App";
		 std::string m_name  = "vulkanExample";
		 bool prepared = false;
		 uint32_t width = 1280;
		 uint32_t height = 720;

		 VkClearColorValue defaultClearColor ={ { 0.025f, 0.025f, 0.025f, 1.0f } };

		 float zoom = 0;

		 // Defines a frame rate independent timer value clamped from -1.0...1.0
		 // For use in animations, rotations, etc.
		 float timer = 0.0f;
		 // Multiplier for speeding up (or slowing down) the global timer
		 float timerSpeed = 0.25f;

		 bool paused = false;

		 // Use to adjust mouse rotation speed
		 float rotationSpeed = 1.0f;
		 // Use to adjust mouse zoom speed
		 float zoomSpeed = 1.0f;
		 // Last frame time, measured using a high performance timer (if available)
		 float frameTimer = 1.0f;
		 glm::vec3 rotation = glm::vec3();
		 glm::vec2 mousePos;
		 // OS specific 

		 HWND m_hWnd;
		 HINSTANCE m_hInstance;

		 void setupConsole();
		 void setupWindow();


   };

}

#endif