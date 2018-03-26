include(BuildHelpers.CMakeLists.txt)

include_directories(
        ${CMAKE_SOURCE_DIR}/gplay3d/src
        ${CMAKE_SOURCE_DIR}/external-deps/include
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    find_package(OpenGL REQUIRED)
    FIND_LIBRARY(AGL_LIBRARY AGL)
    FIND_LIBRARY(APP_SERVICES_LIBRARY ApplicationServices )
    FIND_LIBRARY(ATBOX_LIBRARY AudioToolbox)
    FIND_LIBRARY(CARBON_LIBRARY Carbon)
    FIND_LIBRARY(CAUDIO_LIBRARY CoreAudio)
    FIND_LIBRARY(COREVIDEO_LIBRARY CoreVideo)
    FIND_LIBRARY(CFOUNDATION_LIBRARY CoreFoundation)
    FIND_LIBRARY(CSERVICES_LIBRARY CoreServices)
    FIND_LIBRARY(OPENGL_LIBRARY OpenGL)
    FIND_LIBRARY(QUICKTIME_LIBRARY QuickTime )
    FIND_LIBRARY(IOKIT_LIBRARY IOKit )
    FIND_LIBRARY(AVF_LIBRARY AVFoundation)
    FIND_LIBRARY(OAL_LIBRARY OpenAL)
    FIND_LIBRARY(GKIT_LIBRARY GameKit)
    SET(FRAMEWORK_LIBRS
            ${AGL_LIBRARY}
            ${APP_SERVICES_LIBRARY}
            ${ATBOX_LIBRARY}
            ${CARBON_LIBRARY}
            ${CAUDIO_LIBRARY}
            ${COREVIDEO_LIBRARY}
            ${CFOUNDATION_LIBRARY}
            ${CSERVICES_LIBRARY}
            ${OAL_LIBRARY}
            ${OPENGL_LIBRARIES}
            ${GKIT_LIBRARY}
            ${IOKIT_LIBRARY}
            "-framework Foundation"
            "-framework Cocoa")
    link_directories(${CMAKE_SOURCE_DIR}/external-deps/lib/macosx/x86_64)
    set(GPLAY3D_LIBRARIES
            stdc++
            gplay3d
            gplay3d-deps
            m
            dl
            pthread
            ${FRAMEWORK_LIBRS}
            )
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    add_definitions(-D__linux__)

    IF(ARCH_DIR STREQUAL "x64")
        link_directories(${CMAKE_SOURCE_DIR}/external-deps/lib/linux/x86_64)
    ELSE()
        link_directories(${CMAKE_SOURCE_DIR}/external-deps/lib/linux/x86)
    ENDIF(ARCH_DIR STREQUAL "x64")


    set(GPLAY3D_LIBRARIES
            stdc++
            gplay3d
            gplay3d-deps
            m
            GL
            rt
            dl
            X11
            pthread
            gtk-x11-2.0
            glib-2.0
            gobject-2.0
            sndio
            )
ELSEIF(CMAKE_SYSTEM_NAME MATCHES "Windows")

	include_directories(${CMAKE_SOURCE_DIR}/external-deps/include/compat/msvc)
	
	IF(ARCH_DIR STREQUAL "x64")
		IF(CMAKE_BUILD_TYPE MATCHES Debug)
			link_directories(${CMAKE_SOURCE_DIR}/external-deps/lib/windows/x86_64/Debug)
		ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
			link_directories(${CMAKE_SOURCE_DIR}/external-deps/lib/windows/x86_64/Release)
		ENDIF()        
    ELSE()
        IF(CMAKE_BUILD_TYPE MATCHES Debug)
			link_directories(${CMAKE_SOURCE_DIR}/external-deps/lib/windows/x32/Debug)
		ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
			link_directories(${CMAKE_SOURCE_DIR}/external-deps/lib/windows/x32/Release)
		ENDIF() 
    ENDIF()
	
	set(GPLAY3D_LIBRARIES
            gplay3d
            gplay3d-deps            
			opengl32
			kernel32
			user32
			winmm
			gdi32
			winspool
			comdlg32
			advapi32
			shell32
			imm32
			ole32
			oleaut32
			uuid
			version
			odbc32
			odbccp32
    )
	
ENDIF()

add_definitions(-std=c++11)

add_subdirectory(browser)
add_subdirectory(examples)
add_subdirectory(character)
add_subdirectory(racer)
add_subdirectory(spaceship)
