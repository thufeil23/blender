/*
 * Adapted from Open Shading Language with this license:
 *
 * Copyright (c) 2009-2010 Sony Pictures Imageworks Inc., et al.
 * All Rights Reserved.
 *
 * Modifications Copyright 2013, Blender Foundation.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * * Neither the name of Sony Pictures Imageworks nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

CCL_NAMESPACE_BEGIN

/* Blackbody Node */

ccl_static_constant float h = 6.62607015e-34f;
ccl_static_constant float c = 2.99792458e+8f;
ccl_static_constant float kb = 1.380649e-23f;
ccl_static_constant float b = 2.897771955e-3f;

ccl_device_inline float blackbody_intensity(float temperature, float wavelength)
{
  float wavelength_i = 1.0f / wavelength;
  float wavelength_i2 = wavelength_i * wavelength_i;
  float wavelength_i5 = wavelength_i2 * wavelength_i2 * wavelength_i;

  return (2 * h * c * c * wavelength_i5) / (exp(h * c / (kb * wavelength * temperature)) - 1.0f);
}

ccl_device void svm_node_blackbody(
    KernelGlobals *kg, PathState *state, float *stack, uint temperature_offset, uint col_offset)
{
  /* Input */
  float temperature = stack_load_float(stack, temperature_offset);

  float peak_wavelength = clamp(b / temperature, 360.0e-9f, 730.0e-9f);
  float peak_intensity = blackbody_intensity(temperature, peak_wavelength);

  float3 spectral = make_float3(
      blackbody_intensity(temperature, state->wavelengths.x * 1e-9f) / peak_intensity,
      blackbody_intensity(temperature, state->wavelengths.y * 1e-9f) / peak_intensity,
      blackbody_intensity(temperature, state->wavelengths.z * 1e-9f) / peak_intensity);

  stack_store_float3(stack, col_offset, spectral);
}

CCL_NAMESPACE_END
