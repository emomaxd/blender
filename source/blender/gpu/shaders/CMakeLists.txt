# SPDX-FileCopyrightText: 2024 Blender Authors
#
# SPDX-License-Identifier: GPL-2.0-or-later

set(INC_GLSL
  .
  ..
  ../intern

  common
  infos
)

set(SRC_GLSL_VERT
  gpu_shader_2D_area_borders_vert.glsl
  gpu_shader_2D_image_rect_vert.glsl
  gpu_shader_2D_image_vert.glsl
  gpu_shader_2D_node_socket_vert.glsl
  gpu_shader_2D_nodelink_vert.glsl
  gpu_shader_2D_point_uniform_size_aa_vert.glsl
  gpu_shader_2D_point_uniform_size_outline_aa_vert.glsl
  gpu_shader_2D_point_varying_size_varying_color_vert.glsl
  gpu_shader_3D_polyline_vert.glsl
  gpu_shader_2D_vert.glsl
  gpu_shader_2D_widget_base_vert.glsl
  gpu_shader_2D_widget_shadow_vert.glsl
  gpu_shader_3D_clipped_uniform_color_vert.glsl
  gpu_shader_3D_flat_color_vert.glsl
  gpu_shader_3D_image_vert.glsl
  gpu_shader_3D_line_dashed_uniform_color_vert.glsl
  gpu_shader_3D_normal_vert.glsl
  gpu_shader_3D_point_uniform_size_aa_vert.glsl
  gpu_shader_3D_point_varying_size_varying_color_vert.glsl
  gpu_shader_3D_point_flat_color_vert.glsl
  gpu_shader_3D_smooth_color_vert.glsl
  gpu_shader_display_fallback_vert.glsl
  gpu_shader_gpencil_stroke_vert.glsl
  gpu_shader_icon_multi_vert.glsl
  gpu_shader_icon_vert.glsl
  gpu_shader_keyframe_shape_vert.glsl
  gpu_shader_sequencer_strips_vert.glsl
  gpu_shader_sequencer_thumbs_vert.glsl
  gpu_shader_text_vert.glsl
)

set(SRC_GLSL_FRAG
  gpu_shader_2D_area_borders_frag.glsl
  gpu_shader_2D_line_dashed_frag.glsl
  gpu_shader_2D_node_socket_frag.glsl
  gpu_shader_2D_nodelink_frag.glsl
  gpu_shader_2D_widget_base_frag.glsl
  gpu_shader_2D_widget_shadow_frag.glsl
  gpu_shader_3D_polyline_frag.glsl
  gpu_shader_3D_smooth_color_frag.glsl
  gpu_shader_checker_frag.glsl
  gpu_shader_depth_only_frag.glsl
  gpu_shader_diag_stripes_frag.glsl
  gpu_shader_display_fallback_frag.glsl
  gpu_shader_flat_color_frag.glsl
  gpu_shader_gpencil_stroke_frag.glsl
  gpu_shader_icon_frag.glsl
  gpu_shader_image_color_frag.glsl
  gpu_shader_image_desaturate_frag.glsl
  gpu_shader_image_frag.glsl
  gpu_shader_image_overlays_merge_frag.glsl
  gpu_shader_image_overlays_stereo_merge_frag.glsl
  gpu_shader_image_shuffle_color_frag.glsl
  gpu_shader_keyframe_shape_frag.glsl
  gpu_shader_point_uniform_color_aa_frag.glsl
  gpu_shader_point_uniform_color_outline_aa_frag.glsl
  gpu_shader_point_varying_color_frag.glsl
  gpu_shader_sequencer_strips_frag.glsl
  gpu_shader_sequencer_thumbs_frag.glsl
  gpu_shader_simple_lighting_frag.glsl
  gpu_shader_text_frag.glsl
  gpu_shader_uniform_color_frag.glsl
)

set(SRC_GLSL_COMP
  # TODO rename them properly to enable compilation.
  # gpu_shader_index_2d_array_lines.glsl
  # gpu_shader_index_2d_array_points.glsl
  # gpu_shader_index_2d_array_tris.glsl
)

set(SRC_GLSL_LIB
  common/gpu_shader_print_lib.glsl
)

# Compile shaders with shader code.
if(WITH_GPU_SHADER_CPP_COMPILATION)
  with_shader_cpp_compilation_config()

  compile_sources_as_cpp(gpu_cpp_shaders_vert "${SRC_GLSL_VERT}" "GPU_VERTEX_SHADER")
  compile_sources_as_cpp(gpu_cpp_shaders_frag "${SRC_GLSL_FRAG}" "GPU_FRAGMENT_SHADER")
  # Only enable to make sure they compile on their own.
  # Otherwise it creates a warning about `pragma once`.
  # compile_sources_as_cpp(gpu_cpp_shaders_lib "${SRC_GLSL_LIB}" "GPU_LIBRARY_SHADER")
endif()
