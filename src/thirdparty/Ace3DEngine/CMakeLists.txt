cmake_minimum_required(VERSION 3.10)

if (APPLE AND NOT IOS)
    set(PLATFORM_ABI x86_64)
    message(STATUS "### ACE_3D PLATFORM_ABI x86_64")
endif ()

if (IOS)
    set(PLATFORM_ABI iOS)
    message(STATUS "### ACE_3D PLATFORM_ABI iOS")
endif ()

if (WIN32)
    set(PLATFORM_ABI win/${CMAKE_BUILD_TYPE}/x86_64)
    message(STATUS "### ACE_3D PLATFORM_ABI win")
endif()

if (ANDROID)
    set(PLATFORM_ABI android/${ANDROID_ABI})
    message(STATUS "### ACE_3D PLATFORM_ABI ${PLATFORM_ABI}")
endif ()

set(ACE_3D_ENGINE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(ACE_3D_ENGINE_INCLUDE_DIR
        ${ACE_3D_ENGINE_DIR}/include
        ${ACE_3D_ENGINE_DIR}/third_party/include
        ${ACE_3D_ENGINE_DIR}/third_party/robin-map
        CACHE INTERNAL "ACE_3D_ENGINE_INCLUDE_DIR")

link_directories(${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI})

add_library(filament STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(filament PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libfilament.a)
else()
    set_target_properties(filament PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/filament.lib)
endif()

add_library(backend STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(backend PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libbackend.a)
else()
    set_target_properties(backend PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/backend.lib)
endif()

add_library(utils STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(utils PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libutils.a)
else()
    set_target_properties(utils PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/utils.lib)
endif()


add_library(filaflat STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(filaflat PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libfilaflat.a)
else()
    set_target_properties(filaflat PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/filaflat.lib)
endif()

add_library(ibl STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(ibl PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libibl.a)
else()
    set_target_properties(ibl PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/ibl.lib)
endif()

add_library(image STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(image PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libimage.a)
else()
    set_target_properties(image PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/image.lib)
endif()

add_library(geometry STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(geometry PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libgeometry.a)
else()
    set_target_properties(geometry PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/geometry.lib)
endif()

add_library(filabridge STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(filabridge PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libfilabridge.a)
else()
    set_target_properties(filabridge PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/filabridge.lib)
endif()

add_library(gltfio_core STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(gltfio_core PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libgltfio_core.a)
else()
    set_target_properties(gltfio_core PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/gltfio_core.lib)
endif()

add_library(gltfio_resources STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(gltfio_resources PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libgltfio_resources.a)
else()
    set_target_properties(gltfio_resources PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/gltfio_resources.lib)
endif()

if (NOT WIN32)
    add_library(bluevk STATIC IMPORTED GLOBAL)
    set_target_properties(bluevk PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libbluevk.a)
endif()


if (NOT ANDROID AND NOT IOS AND NOT WIN32)
    add_library(gltfio STATIC IMPORTED GLOBAL)
    set_target_properties(gltfio PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libgltfio.a)

    add_library(bluegl STATIC IMPORTED GLOBAL)
    set_target_properties(bluegl PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libbluegl.a)
elseif(WIN32)
    add_library(gltfio STATIC IMPORTED GLOBAL)
    set_target_properties(gltfio PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/gltfio.lib)

    add_library(bluegl STATIC IMPORTED GLOBAL)
    set_target_properties(bluegl PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/bluegl.lib)
elseif(ANDROID)
    #    add_library(libfilament SHARED IMPORTED GLOBAL)
    #    set_target_properties(libfilament PROPERTIES IMPORTED_LOCATION
    #            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libfilament-jni.so)
    #
    #    add_library(libgltfio SHARED IMPORTED GLOBAL)
    #    set_target_properties(libgltfio PROPERTIES IMPORTED_LOCATION
    #            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libgltfio-jni.so)
endif()

add_library(smol-v STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(smol-v PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libsmol-v.a)
else()
    set_target_properties(smol-v PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/smol-v.lib)
endif()

add_library(shaders STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(shaders PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libshaders.a)
else()
    set_target_properties(shaders PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/shaders.lib)
endif()

add_library(filamat STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(filamat PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libfilamat.a)
else()
    set_target_properties(filamat PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/filamat.lib)
endif()

add_library(draco STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(draco PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libdracodec.a)
else()
    set_target_properties(draco PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/dracodec.lib)
endif()

add_library(camutil STATIC IMPORTED GLOBAL)
if (NOT WIN32)
    set_target_properties(camutil PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/libcamutils.a)
else()
    set_target_properties(camutil PROPERTIES IMPORTED_LOCATION
            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/camutils.lib)
endif()

if (WIN32)
#    add_library(SPIRV STATIC IMPORTED GLOBAL)
#    set_target_properties(SPIRV PROPERTIES IMPORTED_LOCATION
#            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/SPIRV.lib)

#    add_library(SPIRV_Tools_opt STATIC IMPORTED GLOBAL)
#    set_target_properties(SPIRV_Tools_opt PROPERTIES IMPORTED_LOCATION
#            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/SPIRV-Tools-opt.lib)

#    add_library(SPIRV_Tools STATIC IMPORTED GLOBAL)
#    set_target_properties(SPIRV_Tools PROPERTIES IMPORTED_LOCATION
#            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/SPIRV-Tools.lib)

#    add_library(SPVRemapper STATIC IMPORTED GLOBAL)
#    set_target_properties(SPVRemapper PROPERTIES IMPORTED_LOCATION
#            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/SPVRemapper.lib)

#    add_library(OSDependent STATIC IMPORTED GLOBAL)
#    set_target_properties(OSDependent PROPERTIES IMPORTED_LOCATION
#            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/OSDependent.lib)

#    add_library(OGLCompiler STATIC IMPORTED GLOBAL)
#    set_target_properties(OGLCompiler PROPERTIES IMPORTED_LOCATION
#            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/OGLCompiler.lib)

#    add_library(glslang STATIC IMPORTED GLOBAL)
#    set_target_properties(glslang PROPERTIES IMPORTED_LOCATION
#            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/glslang.lib)

#    add_library(spirv_cross_core STATIC IMPORTED GLOBAL)
#    set_target_properties(spirv_cross_core PROPERTIES IMPORTED_LOCATION
#            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/spirv-cross-core.lib)

#    add_library(spirv_cross_glsl STATIC IMPORTED GLOBAL)
#    set_target_properties(spirv_cross_glsl PROPERTIES IMPORTED_LOCATION
#            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/spirv-cross-glsl.lib)

#    add_library(spirv_cross_msl STATIC IMPORTED GLOBAL)
#    set_target_properties(spirv_cross_msl PROPERTIES IMPORTED_LOCATION
#            ${ACE_3D_ENGINE_DIR}/lib/${PLATFORM_ABI}/spirv-cross-msl.lib)
endif()

if (NOT ANDROID AND NOT IOS)
    SET(ACE_3D_LIBS
            gltfio
            gltfio_core
            gltfio_resources

            filament
            backend
            filaflat
            filabridge
            geometry
            image
            ibl
            utils
            shaders
            filamat
            bluegl
            smol-v
            draco
            camutil
            CACHE INTERNAL "ACE_3D_LIBS"
            )
    if (WIN32)
        SET(ACE_3D_LIBS
                ${ACE_3D_LIBS}
                CACHE INTERNAL "ACE_3D_LIBS"
                )
    else()
        SET(ACE_3D_LIBS
                ${ACE_3D_LIBS}
                bluevk
                CACHE INTERNAL "ACE_3D_LIBS"
                )
    endif()
else()
    SET(ACE_3D_LIBS
            #            gltfio
            gltfio_core
            gltfio_resources

            filament
            filamat
            backend
            filaflat
            filabridge
            geometry
            image
            ibl
            utils
            shaders
            bluevk
            #            bluegl
            smol-v
            draco

            CACHE INTERNAL "ACE_3D_LIBS"
            )
endif()