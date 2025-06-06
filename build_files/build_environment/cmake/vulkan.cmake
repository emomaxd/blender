# SPDX-FileCopyrightText: 2022 Blender Authors
#
# SPDX-License-Identifier: GPL-2.0-or-later

set(VULKAN_HEADERS_EXTRA_ARGS)

ExternalProject_Add(external_vulkan_headers
  URL file://${PACKAGE_DIR}/${VULKAN_HEADERS_FILE}
  URL_HASH ${VULKAN_HEADERS_HASH_TYPE}=${VULKAN_HEADERS_HASH}
  PREFIX ${BUILD_DIR}/vulkan_headers

  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${LIBDIR}/vulkan_headers
    -Wno-dev ${DEFAULT_CMAKE_FLAGS}
    ${VULKAN_HEADERS_EXTRA_ARGS}

  INSTALL_DIR ${LIBDIR}/vulkan_headers
)

set(VULKAN_LOADER_EXTRA_ARGS
  -DVULKAN_HEADERS_INSTALL_DIR=${LIBDIR}/vulkan_headers
)

set(VULKAN_UTILITY_LIBRARIES_EXTRA_ARGS
  -DVulkanHeaders_DIR=${LIBDIR}/vulkan_headers/share/cmake/VulkanHeaders)

ExternalProject_Add(external_vulkan_utility_libraries
  URL file://${PACKAGE_DIR}/${VULKAN_UTILITY_LIBRARIES_FILE}
  URL_HASH ${VULKAN_UTILITY_LIBRARIES_HASH_TYPE}=${VULKAN_UTILITY_LIBRARIES_HASH}
  PREFIX ${BUILD_DIR}/vulkan_utility_libraries

  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${LIBDIR}/vulkan_headers
    -Wno-dev ${DEFAULT_CMAKE_FLAGS}
    ${VULKAN_UTILITY_LIBRARIES_EXTRA_ARGS}

  INSTALL_DIR ${LIBDIR}/vulkan_headers
)

add_dependencies(
  external_vulkan_utility_libraries
  external_vulkan_headers
)

set(SPIRV_HEADERS_EXTRA_ARGS)

ExternalProject_Add(external_spirv_headers
  URL file://${PACKAGE_DIR}/${SPIRV_HEADERS_FILE}
  URL_HASH ${SPIRV_HEADERS_HASH_TYPE}=${SPIRV_HEADERS_HASH}
  PREFIX ${BUILD_DIR}/spirv_headers

  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${LIBDIR}/vulkan_headers
    -Wno-dev ${DEFAULT_CMAKE_FLAGS}
    ${SPIRV_HEADERS_EXTRA_ARGS}

  INSTALL_DIR ${LIBDIR}/vulkan_headers
)

if(UNIX AND NOT APPLE)
  # These are used in `cmake/FindWayland.cmake` from `external_vulkan_loader`.
  # NOTE: When upgrading to CMAKE 3.22 we it would be cleaner to use: `PKG_CONFIG_ARGN`,
  # so `pkgconfig` would find wayland.
  set(VULKAN_LOADER_EXTRA_ARGS
    ${VULKAN_LOADER_EXTRA_ARGS}
    -DPKG_WAYLAND_INCLUDE_DIRS=${LIBDIR}/wayland/include
    -DPKG_WAYLAND_LIBRARY_DIRS=${LIBDIR}/wayland/lib64
  )
elseif(BLENDER_PLATFORM_WINDOWS_ARM)
  set(VULKAN_LOADER_EXTRA_ARGS
    -DUSE_MASM=OFF
    -DVulkanHeaders_DIR=${LIBDIR}/vulkan_headers/share/cmake/VulkanHeaders
  )
endif()

ExternalProject_Add(external_vulkan_loader
  URL file://${PACKAGE_DIR}/${VULKAN_LOADER_FILE}
  URL_HASH ${VULKAN_LOADER_HASH_TYPE}=${VULKAN_LOADER_HASH}
  PREFIX ${BUILD_DIR}/vulkan_loader

  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${LIBDIR}/vulkan_loader
    -Wno-dev
    ${DEFAULT_CMAKE_FLAGS}
    ${VULKAN_LOADER_EXTRA_ARGS}

  INSTALL_DIR ${LIBDIR}/vulkan_loader
)

add_dependencies(
  external_vulkan_loader
  external_vulkan_headers
)

if(UNIX AND NOT APPLE)
  add_dependencies(
    external_vulkan_loader
    external_wayland
  )
endif()

if(WIN32)
  if(BUILD_MODE STREQUAL Release)
    ExternalProject_Add_Step(external_vulkan_loader after_install
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${LIBDIR}/vulkan_loader/
        ${HARVEST_TARGET}/vulkan
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${LIBDIR}/vulkan_headers/
        ${HARVEST_TARGET}/vulkan
      DEPENDEES install
    )
  endif()
else()
  harvest(external_vulkan_loader vulkan_headers vulkan "*")
  harvest_rpath_lib(external_vulkan_loader vulkan_loader/lib vulkan/lib "*${SHAREDLIBEXT}*")
  if(APPLE)
    harvest(external_vulkan_loader vulkan_loader/loader vulkan/loader "*")
  endif()
endif()
