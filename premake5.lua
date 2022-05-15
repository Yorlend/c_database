workspace "lab_db"
   configurations { "Debug", "Release" }

project "lab_db"
   kind "ConsoleApp"
   language "C"
   targetdir "build/%{cfg.buildcfg}"

   includedirs { "src" }
   files { "src/**.h", "src/**.c" }

   filter "configurations:Debug"
      defines { "DEBUG" }
      symbols "On"

   filter "configurations:Release"
      defines { "NDEBUG" }
      optimize "On"
