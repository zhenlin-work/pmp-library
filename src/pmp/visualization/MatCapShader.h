//=============================================================================
// Copyright (C) 2011-2020 The pmp-library developers
//
// This file is part of the Polygon Mesh Processing Library.
// Distributed under a MIT-style license, see LICENSE.txt for details.
//
// SPDX-License-Identifier: MIT-with-employer-disclaimer
//=============================================================================

// clang-format off

// mat-cap shader: assume view=(0,0,-1), then the tex-coord for
// spherical environment mapping is just the normal's XY
// scaled by 0.5 and shifted by 0.5.
// scale by 0.49 to avoid artifacts at gracing angles
static const char* matcap_vshader =
#ifndef __EMSCRIPTEN__
    "#version 330"
#else
    "#version 300 es"
#endif
R"glsl(
layout (location=0) in vec4 v_position;
layout (location=1) in vec3 v_normal;
out vec3 v2f_normal;
uniform mat4 modelview_projection_matrix;
uniform mat3 normal_matrix;

void main()
{
    v2f_normal = normalize(normal_matrix * v_normal);
    gl_Position = modelview_projection_matrix * v_position;
}
)glsl";


static const char* matcap_fshader =
#ifndef __EMSCRIPTEN__
    "#version 330"
#else
    "#version 300 es"
#endif
R"glsl(
precision mediump float;

in vec3 v2f_normal;
uniform sampler2D matcap;
uniform float  alpha;
out vec4 f_color;

vec2 uv;
vec4 rgba;

void main()
{
    if (v2f_normal.z < 0.0) // back-facing?
    {
        // invert normal, damp color
        uv = normalize(-v2f_normal).xy * 0.49 + 0.5;
        rgba = texture(matcap, uv);
        rgba.rgb *= 0.5;
    }
    else // front-facing
    {
        uv = normalize(v2f_normal).xy * 0.49 + 0.5;
        rgba = texture(matcap, uv);
    }

    rgba.a *= alpha;
    f_color = rgba;
}
)glsl";


//=============================================================================
// clang-format on
//=============================================================================
