
DX_SDK_PATH = string.gsub(os.getenv("DXSDK_DIR"), "\\", "/")


solution "Iso"
    language "C++"
    flags { "StaticRuntime" }
    
    location "."
    debugdir "."
    includedirs {
        "../Ferrite/Code/",
        "./Code/",
        path.join(DX_SDK_PATH, "Include")
    }
    libdirs {
        path.join(DX_SDK_PATH, "Lib", "x86")
    }
    defines { "UNICODE", "_UNICODE" }
    
    startproject "Game"
    
    
    
    -- CONFIGURATIONS ------------------------
    configurations { "Debug", "Release" }
    
    configuration "Debug"
        defines { "CONFIG_DEBUG", "_DEBUG" }
        objdir "./Build/Obj/Debug/"
        flags { "Symbols" }
        targetsuffix ".Debug"

    configuration "Release"
        defines { "CONFIG_RELEASE" }
        objdir "./Build/Obj/Release/"
        flags { "Optimize" }



    -- GAME ----------------------------------
    project "Game"
        kind "WindowedApp"
        flags { "WinMain" }
        
        location "./Build/Projects/"
        targetdir "./Bin/"
            
        files {
            "./Code/**.h",
            "./Code/**.cpp",
            "./Code/**.inl",
        }
        vpaths {
            ["*"] = "./Code/**"
        }

        links {
            "Ferrite"
        }
    
    
    
    -- ENGINE -------------------------------

    include "../Ferrite"