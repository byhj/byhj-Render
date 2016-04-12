cd /d "E:\GitHub\byhj-Render\src\Vulkan-Window" &msbuild "Vulkan-Window.vcxproj" /t:sdvViewer /p:configuration="Debug" /p:platform=Win32
exit %errorlevel% 