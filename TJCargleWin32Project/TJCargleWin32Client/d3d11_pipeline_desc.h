#pragma once

#include <d3d11_2.h>

// Currently unused

namespace fsgd
{
	struct pipeline_desc_t
	{
		void*						vs;
		SIZE_T						vs_length;
		void*						ps;
		SIZE_T						ps_length;
		void*						ds;
		SIZE_T						ds_length;
		void*						hs;
		SIZE_T						hs_length;
		void*						gs;
		SIZE_T						gs_length;
		D3D11_BLEND_DESC			blend_state;
		UINT						sample_mask;
		D3D11_RASTERIZER_DESC		rasterizer_state;
		D3D11_INPUT_ELEMENT_DESC	input_layout;
		D3D11_PRIMITIVE_TOPOLOGY	primitive_topology_type;
		UINT						num_render_targets;
		DXGI_FORMAT					rtv_formats[8];
		DXGI_FORMAT					dsv_formats;
		DXGI_SAMPLE_DESC			sample_desc;
	};
}