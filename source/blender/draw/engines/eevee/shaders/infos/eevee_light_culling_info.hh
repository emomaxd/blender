/* SPDX-FileCopyrightText: 2023 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

#ifdef GPU_SHADER
#  pragma once
#  include "gpu_glsl_cpp_stubs.hh"

#  include "draw_object_infos_info.hh"
#  include "draw_view_info.hh"
#  include "eevee_common_info.hh"
#  include "eevee_shader_shared.hh"
#  include "gpu_shader_fullscreen_info.hh"

#  define SPHERE_PROBE
#endif

#include "eevee_defines.hh"
#include "gpu_shader_create_info.hh"

/* -------------------------------------------------------------------- */
/** \name Culling
 * \{ */

GPU_SHADER_CREATE_INFO(eevee_light_culling_select)
DO_STATIC_COMPILATION()
ADDITIONAL_INFO(eevee_shared)
ADDITIONAL_INFO(draw_view)
ADDITIONAL_INFO(draw_view_culling)
LOCAL_GROUP_SIZE(CULLING_SELECT_GROUP_SIZE)
STORAGE_BUF(0, read_write, LightCullingData, light_cull_buf)
STORAGE_BUF(1, read, LightData, in_light_buf[])
STORAGE_BUF(2, write, LightData, out_light_buf[])
STORAGE_BUF(3, write, float, out_zdist_buf[])
STORAGE_BUF(4, write, uint, out_key_buf[])
UNIFORM_BUF(0, LightData, sunlight_buf)
COMPUTE_SOURCE("eevee_light_culling_select_comp.glsl")
GPU_SHADER_CREATE_END()

GPU_SHADER_CREATE_INFO(eevee_light_culling_sort)
DO_STATIC_COMPILATION()
ADDITIONAL_INFO(eevee_shared)
ADDITIONAL_INFO(draw_view)
STORAGE_BUF(0, read, LightCullingData, light_cull_buf)
STORAGE_BUF(1, read, LightData, in_light_buf[])
STORAGE_BUF(2, write, LightData, out_light_buf[])
STORAGE_BUF(3, read, float, in_zdist_buf[])
STORAGE_BUF(4, read, uint, in_key_buf[])
LOCAL_GROUP_SIZE(CULLING_SORT_GROUP_SIZE)
COMPUTE_SOURCE("eevee_light_culling_sort_comp.glsl")
GPU_SHADER_CREATE_END()

GPU_SHADER_CREATE_INFO(eevee_light_culling_zbin)
DO_STATIC_COMPILATION()
ADDITIONAL_INFO(eevee_shared)
ADDITIONAL_INFO(draw_view)
LOCAL_GROUP_SIZE(CULLING_ZBIN_GROUP_SIZE)
STORAGE_BUF(0, read, LightCullingData, light_cull_buf)
STORAGE_BUF(1, read, LightData, light_buf[])
STORAGE_BUF(2, write, uint, out_zbin_buf[])
COMPUTE_SOURCE("eevee_light_culling_zbin_comp.glsl")
GPU_SHADER_CREATE_END()

GPU_SHADER_CREATE_INFO(eevee_light_culling_tile)
DO_STATIC_COMPILATION()
ADDITIONAL_INFO(eevee_shared)
ADDITIONAL_INFO(draw_view)
ADDITIONAL_INFO(draw_view_culling)
LOCAL_GROUP_SIZE(CULLING_TILE_GROUP_SIZE)
STORAGE_BUF(0, read, LightCullingData, light_cull_buf)
STORAGE_BUF(1, read, LightData, light_buf[])
STORAGE_BUF(2, write, uint, out_light_tile_buf[])
COMPUTE_SOURCE("eevee_light_culling_tile_comp.glsl")
GPU_SHADER_CREATE_END()

GPU_SHADER_CREATE_INFO(eevee_light_shadow_setup)
DO_STATIC_COMPILATION()
ADDITIONAL_INFO(eevee_shared)
ADDITIONAL_INFO(eevee_sampling_data)
ADDITIONAL_INFO(eevee_global_ubo)
LOCAL_GROUP_SIZE(CULLING_SELECT_GROUP_SIZE)
STORAGE_BUF(0, read, LightCullingData, light_cull_buf)
STORAGE_BUF(1, read_write, LightData, light_buf[])
STORAGE_BUF(2, read_write, ShadowTileMapData, tilemaps_buf[])
STORAGE_BUF(3, read_write, ShadowTileMapClip, tilemaps_clip_buf[])
COMPUTE_SOURCE("eevee_light_shadow_setup_comp.glsl")
GPU_SHADER_CREATE_END()

/** \} */

/* -------------------------------------------------------------------- */
/** \name Debug
 * \{ */

GPU_SHADER_CREATE_INFO(eevee_light_culling_debug)
DO_STATIC_COMPILATION()
FRAGMENT_OUT_DUAL(0, float4, out_debug_color_add, SRC_0)
FRAGMENT_OUT_DUAL(0, float4, out_debug_color_mul, SRC_1)
FRAGMENT_SOURCE("eevee_light_culling_debug_frag.glsl")
ADDITIONAL_INFO(eevee_shared)
ADDITIONAL_INFO(draw_view)
ADDITIONAL_INFO(gpu_fullscreen)
ADDITIONAL_INFO(eevee_light_data)
ADDITIONAL_INFO(eevee_hiz_data)
GPU_SHADER_CREATE_END()

/** \} */
