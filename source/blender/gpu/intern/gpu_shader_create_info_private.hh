/* SPDX-FileCopyrightText: 2021 Blender Authors
 *
 * SPDX-License-Identifier: GPL-2.0-or-later */

/** \file
 * \ingroup gpu
 *
 * Descriptor type used to define shader structure, resources and interfaces.
 *
 * Some rule of thumb:
 * - Do not include anything else than this file in each descriptor file.
 */

#pragma once

#include "GPU_shader.hh"

void gpu_shader_create_info_init();
void gpu_shader_create_info_exit();

bool gpu_shader_create_info_compile(const char *name_starts_with_filter);

/**
 * Runtime created shader information is not registered in the dictionary
 * and cannot be searched.
 */
const GPUShaderCreateInfo *gpu_shader_create_info_get(const char *info_name);
