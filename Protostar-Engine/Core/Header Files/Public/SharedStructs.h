#pragma once
#include "GlobalDefs.h"
#include <utility>
#include <vector>
#include "SharedEnums.h"
#include "DirectXMath.h"
#include "d3d12.h"
#include "Json.h"
#include "Guid.h"

namespace Protostar
{
	struct SPSOInfo
	{
		std::vector<D3D12_INPUT_ELEMENT_DESC> InputElementDescs;
		std::pair<SGuid, ID3D12RootSignature*> RootSignature;
		std::pair<SGuid, ID3DBlob*> VertexShader;
		std::pair<SGuid, ID3DBlob*> PixelShader;
		u32 NodeMask;
		D3D12_BLEND_DESC BlendDesc;
		D3D12_RASTERIZER_DESC RasterizerDesc;
		D3D12_DEPTH_STENCIL_DESC DepthStencilDesc;
		ID3D12PipelineState** PipelineState;

#define JSONPSOINFOINPUTELEDESCS					"input_element_descs"
#define JSONPSOINFOROOTSIGNATUREGUID				"root_signature_guid"	
#define JSONPSOINFOVTXSHADERGUID					"vtx_shader_guid"
#define JSONPSOINFOPIXELSHADERGUID					"pixel_shader_guid"
#define JSONPSOINFONODEMASK							"node_mask"
#define JSONPSOINFOBLENDDESC						"blend_desc"
#define JSONPSOINFORASTERIZERDESC					"rasterizer_desc"
#define JSONPSOINFODEPTHSTENCILDESC					"depth_stencil_desc"

		Json ToJson() const
		{
			std::vector<Json> inputDescsJson = {};
			for (const D3D12_INPUT_ELEMENT_DESC& inputElemendDesc : InputElementDescs)
			{
				inputDescsJson.push_back(InputElementDescToJson(inputElemendDesc));
			}

			return Json
			{
				{JSONPSOINFOINPUTELEDESCS, inputDescsJson},
				{JSONPSOINFOROOTSIGNATUREGUID, RootSignature.first.ToString()}, 
				{JSONPSOINFOVTXSHADERGUID, VertexShader.first.ToString()},
				{JSONPSOINFOPIXELSHADERGUID, PixelShader.first.ToString()},
				{JSONPSOINFONODEMASK, NodeMask},
				{JSONPSOINFOBLENDDESC, BlendDescToJson(BlendDesc)},
				{JSONPSOINFORASTERIZERDESC, RasterizerDescToJson(RasterizerDesc)},
				{JSONPSOINFODEPTHSTENCILDESC, DepthStencilDescToJson(DepthStencilDesc)}
			};
		}

		void FromJson(const Json& _json)
		{
			for (const Json& json : _json[JSONPSOINFOINPUTELEDESCS].get<std::vector<Json>>())
			{
				InputElementDescs.push_back(InputElementDescFromJson(json));
			}
			RootSignature = std::make_pair(_json[JSONPSOINFOROOTSIGNATUREGUID].get<std::string>(), nullptr);
			VertexShader = std::make_pair(_json[JSONPSOINFOVTXSHADERGUID].get<std::string>(), nullptr);
			PixelShader = std::make_pair(_json[JSONPSOINFOPIXELSHADERGUID].get<std::string>(), nullptr);
			NodeMask = _json[JSONPSOINFONODEMASK].get<u32>();
			BlendDesc = BlendDescFromJson(_json[JSONPSOINFOBLENDDESC].get<Json>());
			RasterizerDesc = RasterizerDescFromJson(_json[JSONPSOINFORASTERIZERDESC].get<Json>());
			DepthStencilDesc = DepthStencilDescFromJson(_json[JSONPSOINFODEPTHSTENCILDESC].get<Json>());
		}

#undef JSONPSOINFOINPUTELEDESCS	
#undef JSONPSOINFOROOTSIGNATUREGUID	
#undef JSONPSOINFOVTXSHADERGUID	
#undef JSONPSOINFOPIXELSHADERGUID	
#undef JSONPSOINFONODEMASK			

	private:
#define JSONINPUTELEDESCSEMNAME						"semantic_name"
#define JSONINPUTELEDESCSEMIDX						"semantic_index"
#define JSONINPUTELEDESCFORMAT						"format"
#define JSONINPUTELEDESCINPUTSLOT					"input_slot"
#define JSONINPUTELEDESCALIGNEDBYTEOFFSET			"aligned_byte_offset"
#define JSONINPUTELEDESCINPUTCLASSIFICATION			"input_classification"
#define JSONINPUTELEDESCINSTANCEDATASTEPRATE		"instace_data_step_rate"

		Json InputElementDescToJson(const D3D12_INPUT_ELEMENT_DESC& _inputDesc) const
		{
			return Json
			{
				{JSONINPUTELEDESCSEMNAME, _inputDesc.SemanticName}, 
				{JSONINPUTELEDESCSEMIDX, _inputDesc.SemanticIndex}, 
				{JSONINPUTELEDESCFORMAT, _inputDesc.Format}, 
				{JSONINPUTELEDESCINPUTSLOT, _inputDesc.InputSlot}, 
				{JSONINPUTELEDESCALIGNEDBYTEOFFSET, _inputDesc.AlignedByteOffset}, 
				{JSONINPUTELEDESCINPUTCLASSIFICATION, _inputDesc.InputSlotClass}, 
				{JSONINPUTELEDESCINSTANCEDATASTEPRATE, _inputDesc.InstanceDataStepRate}
			};
		}

		D3D12_INPUT_ELEMENT_DESC InputElementDescFromJson(const Json& _json) const
		{
			return D3D12_INPUT_ELEMENT_DESC
			{
				_json[JSONINPUTELEDESCSEMNAME].get<std::string>().c_str(),
				_json[JSONINPUTELEDESCSEMIDX].get<u32>(),
				(DXGI_FORMAT)_json[JSONINPUTELEDESCFORMAT].get<u32>(),
				_json[JSONINPUTELEDESCINPUTSLOT].get<u32>(),
				_json[JSONINPUTELEDESCALIGNEDBYTEOFFSET].get<u32>(),
				(D3D12_INPUT_CLASSIFICATION)_json[JSONINPUTELEDESCINPUTCLASSIFICATION].get<u32>(),
				_json[JSONINPUTELEDESCINSTANCEDATASTEPRATE].get<u32>()
			};
		}

#define JSONBLENDDESCALPHATOCOVENABLE				"alpha_to_cov_enable"
#define JSONBLENDDESCINDEPENDENTBLENDENABLE			"independent_blend_enable"
#define JSONBLENDDESCRENDERTARGETBLENDDESCS			"render_target_blend_descs"

		Json BlendDescToJson(const D3D12_BLEND_DESC& _blendDesc) const
		{
			Json rtBlendDescs[8] = 
			{
				RTBlendDescToJson(_blendDesc.RenderTarget[0]),
				RTBlendDescToJson(_blendDesc.RenderTarget[1]),
				RTBlendDescToJson(_blendDesc.RenderTarget[2]),
				RTBlendDescToJson(_blendDesc.RenderTarget[3]),
				RTBlendDescToJson(_blendDesc.RenderTarget[4]),
				RTBlendDescToJson(_blendDesc.RenderTarget[5]),
				RTBlendDescToJson(_blendDesc.RenderTarget[6]),
				RTBlendDescToJson(_blendDesc.RenderTarget[7])
			};

			return Json 
			{
				{JSONBLENDDESCALPHATOCOVENABLE, _blendDesc.AlphaToCoverageEnable}, 
				{JSONBLENDDESCINDEPENDENTBLENDENABLE, _blendDesc.IndependentBlendEnable},
				{JSONBLENDDESCRENDERTARGETBLENDDESCS, rtBlendDescs}
			};
		}

		D3D12_BLEND_DESC BlendDescFromJson(const Json& _json) const
		{
			D3D12_BLEND_DESC blendDesc = {};
			for (s32 i = 0; i < _json[JSONBLENDDESCRENDERTARGETBLENDDESCS].get<std::vector<Json>>().size(); ++i)
			{
				blendDesc.RenderTarget[i] = RTBlendDescFromJson(_json[i]);
			}

			blendDesc.AlphaToCoverageEnable = _json[JSONBLENDDESCALPHATOCOVENABLE].get<bool>();
			blendDesc.IndependentBlendEnable = _json[JSONBLENDDESCINDEPENDENTBLENDENABLE].get<bool>();

			return blendDesc;
		}

#define JSONRTBLENDDESCBLENDENABLE					"blend_enable"
#define JSONRTBLENDDESCLOGICOPENABLE				"logic_op_enable"
#define JSONRTBLENDDESCSRCBLEND						"src_blend"
#define JSONRTBLENDDESCDESTBLEND					"dest_blend"
#define JSONRTBLENDDESCBLENDOP						"blend_op"
#define JSONRTBLENDDESCSRCBLENDALPHA				"src_blend_alpha"
#define JSONRTBLENDDESCDESTBLENDALPHA				"dest_blend_alpha"
#define JSONRTBLENDDESCBLENDOPALPHA					"blend_op_alpha"
#define JSONRTBLENDDESCLOGICOP						"logic_op"
#define JSONRTBLENDDESCRTWRITEMASK					"render_target_write_mask"

		Json RTBlendDescToJson(const D3D12_RENDER_TARGET_BLEND_DESC& _rtBlendDesc) const
		{
			return Json
			{
				{JSONRTBLENDDESCBLENDENABLE, _rtBlendDesc.BlendEnable},
				{JSONRTBLENDDESCLOGICOPENABLE, _rtBlendDesc.LogicOpEnable}, 
				{JSONRTBLENDDESCSRCBLEND, _rtBlendDesc.SrcBlend},
				{JSONRTBLENDDESCDESTBLEND, _rtBlendDesc.DestBlend}, 
				{JSONRTBLENDDESCBLENDOP, _rtBlendDesc.BlendOp}, 
				{JSONRTBLENDDESCSRCBLENDALPHA, _rtBlendDesc.SrcBlendAlpha},
				{JSONRTBLENDDESCDESTBLENDALPHA, _rtBlendDesc.DestBlendAlpha}, 
				{JSONRTBLENDDESCLOGICOP, _rtBlendDesc.LogicOp},
				{JSONRTBLENDDESCRTWRITEMASK, _rtBlendDesc.RenderTargetWriteMask}
			};
		}

		D3D12_RENDER_TARGET_BLEND_DESC RTBlendDescFromJson(const Json& _json) const
		{
			return D3D12_RENDER_TARGET_BLEND_DESC
			{
				_json[JSONRTBLENDDESCBLENDENABLE].get<bool>(),
				_json[JSONRTBLENDDESCLOGICOPENABLE].get<bool>(),
				(D3D12_BLEND)_json[JSONRTBLENDDESCSRCBLEND].get<u32>(),
				(D3D12_BLEND)_json[JSONRTBLENDDESCDESTBLEND].get<u32>(),
				(D3D12_BLEND_OP)_json[JSONRTBLENDDESCBLENDOP].get<u32>(),
				(D3D12_BLEND)_json[JSONRTBLENDDESCSRCBLENDALPHA].get<u32>(),
				(D3D12_BLEND)_json[JSONRTBLENDDESCDESTBLENDALPHA].get<u32>(),
				(D3D12_BLEND_OP)_json[JSONRTBLENDDESCBLENDOPALPHA].get<u32>(),
				(D3D12_LOGIC_OP)_json[JSONRTBLENDDESCLOGICOP].get<u32>(),
				_json[JSONRTBLENDDESCRTWRITEMASK].get<u8>()
			};
		}

#define JSONRASTERIZERDESCFILLMODE					"fill_mode"
#define JSONRASTERIZERDESCCULLMODE					"cull_mode"
#define JSONRASTERIZERDESCFRONTCOUNTERCLOCKWISE		"front_counter_clockwise"
#define JSONRASTERIZERDESCDEPTHBIAS					"depth_bias"
#define JSONRASTERIZERDESCDEPTHBIASCLAMP			"depth_bias_clamp"
#define JSONRASTERIZERDESCSLOPESCALEDEPTHBIAS		"slope_scale_depth_bias"
#define JSONRASTERIZERDESCDEPTHCLIPENABLE			"depth_clip_enable"
#define JSONRASTERIZERDESCMULTISAMPLEENABLE			"multisample_enable"
#define JSONRASTERIZERDESCANTIALIASEDLINEENABLE		"antialiased_line_enable"
#define JSONRASTERIZERDESCFORCEDSAMPLECOUNT			"forced_sample_count"
#define JSONRASTERIZERDESCCONSERVATIVERASTER		"conservative_raster"

		Json RasterizerDescToJson(const D3D12_RASTERIZER_DESC& _rasterizerDesc) const
		{
			return Json
			{
				{JSONRASTERIZERDESCFILLMODE, _rasterizerDesc.FillMode},
				{JSONRASTERIZERDESCCULLMODE, _rasterizerDesc.CullMode},
				{JSONRASTERIZERDESCFRONTCOUNTERCLOCKWISE, _rasterizerDesc.FrontCounterClockwise},
				{JSONRASTERIZERDESCDEPTHBIAS, _rasterizerDesc.DepthBias},
				{JSONRASTERIZERDESCDEPTHBIASCLAMP, _rasterizerDesc.DepthBiasClamp},
				{JSONRASTERIZERDESCSLOPESCALEDEPTHBIAS, _rasterizerDesc.SlopeScaledDepthBias},
				{JSONRASTERIZERDESCDEPTHCLIPENABLE, _rasterizerDesc.DepthClipEnable},
				{JSONRASTERIZERDESCMULTISAMPLEENABLE, _rasterizerDesc.MultisampleEnable},
				{JSONRASTERIZERDESCANTIALIASEDLINEENABLE, _rasterizerDesc.AntialiasedLineEnable},
				{JSONRASTERIZERDESCFORCEDSAMPLECOUNT, _rasterizerDesc.ForcedSampleCount},
				{JSONRASTERIZERDESCCONSERVATIVERASTER, _rasterizerDesc.ConservativeRaster}
			};
		}

		D3D12_RASTERIZER_DESC RasterizerDescFromJson(const Json& _json) const
		{
			return D3D12_RASTERIZER_DESC
			{
				(D3D12_FILL_MODE)_json[JSONRASTERIZERDESCFILLMODE].get<u32>(),
				(D3D12_CULL_MODE)_json[JSONRASTERIZERDESCCULLMODE].get<u32>(),
				_json[JSONRASTERIZERDESCFRONTCOUNTERCLOCKWISE].get<bool>(),
				_json[JSONRASTERIZERDESCDEPTHBIAS].get<s32>(),
				_json[JSONRASTERIZERDESCDEPTHBIASCLAMP].get<float>(),
				_json[JSONRASTERIZERDESCSLOPESCALEDEPTHBIAS].get<float>(),
				_json[JSONRASTERIZERDESCDEPTHCLIPENABLE].get<bool>(),
				_json[JSONRASTERIZERDESCMULTISAMPLEENABLE].get<bool>(),
				_json[JSONRASTERIZERDESCANTIALIASEDLINEENABLE].get<bool>(),
				_json[JSONRASTERIZERDESCFORCEDSAMPLECOUNT].get<u32>(),
				(D3D12_CONSERVATIVE_RASTERIZATION_MODE)_json[JSONRASTERIZERDESCCONSERVATIVERASTER].get<u32>()
			};
		}

#define JSONDEPTHSTENCILDESCDEPTHENABLE				"depth_enable"
#define JSONDEPTHSTENCILDESCDEPTHWRITEMASK			"depth_write_mask"
#define JSONDEPTHSTENCILDESCDEPTHFUNC				"depth_func"
#define JSONDEPTHSTENCILDESCSTENCILENABLE			"stencil_enable"
#define JSONDEPTHSTENCILDESCSTENCILREADMASK			"stencil_read_mask"
#define JSONDEPTHSTENCILDESCSTENCILWRITEMASK		"stencil_write_mask"
#define JSONDEPTHSTENCILDESCFRONTFACE				"front_face"
#define JSONDEPTHSTENCILDESCBACKFACE				"back_face"

		Json DepthStencilDescToJson(const D3D12_DEPTH_STENCIL_DESC& _depthStencilDesc) const
		{
			return Json
			{
				{JSONDEPTHSTENCILDESCDEPTHENABLE, _depthStencilDesc.DepthEnable},
				{JSONDEPTHSTENCILDESCDEPTHWRITEMASK, _depthStencilDesc.DepthWriteMask},
				{JSONDEPTHSTENCILDESCDEPTHFUNC, _depthStencilDesc.DepthFunc},
				{JSONDEPTHSTENCILDESCSTENCILENABLE, _depthStencilDesc.StencilEnable},
				{JSONDEPTHSTENCILDESCSTENCILREADMASK, _depthStencilDesc.StencilReadMask},
				{JSONDEPTHSTENCILDESCSTENCILWRITEMASK, _depthStencilDesc.StencilWriteMask},
				{JSONDEPTHSTENCILDESCFRONTFACE, DepthStencilOpDescToJson(_depthStencilDesc.FrontFace)},
				{JSONDEPTHSTENCILDESCBACKFACE, DepthStencilOpDescToJson(_depthStencilDesc.BackFace)}
			};
		}

		D3D12_DEPTH_STENCIL_DESC DepthStencilDescFromJson(const Json& _json) const 
		{
			return D3D12_DEPTH_STENCIL_DESC
			{
				_json[JSONDEPTHSTENCILDESCDEPTHENABLE].get<bool>(),
				(D3D12_DEPTH_WRITE_MASK)_json[JSONDEPTHSTENCILDESCDEPTHWRITEMASK].get<u32>(),
				(D3D12_COMPARISON_FUNC)_json[JSONDEPTHSTENCILDESCDEPTHFUNC].get<u32>(),
				_json[JSONDEPTHSTENCILDESCSTENCILENABLE].get<bool>(),
				_json[JSONDEPTHSTENCILDESCSTENCILREADMASK].get<u8>(),
				_json[JSONDEPTHSTENCILDESCSTENCILWRITEMASK].get<u8>(),
				DepthStencilOpDescFromJson(_json[JSONDEPTHSTENCILDESCFRONTFACE].get<Json>()),
				DepthStencilOpDescFromJson(_json[JSONDEPTHSTENCILDESCBACKFACE].get<Json>())
			};
		}

#define JSONDEPTHSTENCILOPDESCSTENCILFAILOP			"stencil_fail_op"
#define JSONDEPTHSTENCILOPDESCSTENCILDEPTHFAILOP	"stencil_depth_fail_op"
#define JSONDEPTHSTENCILOPDESCSTENCILPASSOP			"stencil_Pass_op"
#define JSONDEPTHSTENCILOPDESCSTENCILFUNC			"stencil_func"

		Json DepthStencilOpDescToJson(const D3D12_DEPTH_STENCILOP_DESC& _depthStencilOpDesc) const
		{
			return Json
			{
				{JSONDEPTHSTENCILOPDESCSTENCILFAILOP, _depthStencilOpDesc.StencilFailOp},
				{JSONDEPTHSTENCILOPDESCSTENCILDEPTHFAILOP, _depthStencilOpDesc.StencilDepthFailOp},
				{JSONDEPTHSTENCILOPDESCSTENCILPASSOP, _depthStencilOpDesc.StencilPassOp},
				{JSONDEPTHSTENCILOPDESCSTENCILFUNC, _depthStencilOpDesc.StencilFunc}
			};
		}

		D3D12_DEPTH_STENCILOP_DESC DepthStencilOpDescFromJson(const Json& _json) const
		{
			return D3D12_DEPTH_STENCILOP_DESC
			{
				(D3D12_STENCIL_OP)_json[JSONDEPTHSTENCILOPDESCSTENCILFAILOP].get<u32>(),
				(D3D12_STENCIL_OP)_json[JSONDEPTHSTENCILOPDESCSTENCILDEPTHFAILOP].get<u32>(),
				(D3D12_STENCIL_OP)_json[JSONDEPTHSTENCILOPDESCSTENCILPASSOP].get<u32>(),
				(D3D12_COMPARISON_FUNC)_json[JSONDEPTHSTENCILOPDESCSTENCILFUNC].get<u32>()
			};
		}
	};


#undef JSONINPUTELEDESCSEMNAME 
#undef JSONINPUTELEDESCSEMIDX 
#undef JSONINPUTELEDESCFORMAT 
#undef JSONINPUTELEDESCINPUTSLOT 
#undef JSONINPUTELEDESCALIGNEDBYTEOFFSET 
#undef JSONINPUTELEDESCINPUTCLASSIFICATION 
#undef JSONINPUTELEDESCINSTANCEDATASTEPRATE 
#undef JSONBLENDDESCALPHATOCOVENABLE			
#undef JSONBLENDDESCINDEPENDENTBLENDENABLE		
#undef JSONBLENDDESCRENDERTARGETBLENDDESCS		
#undef JSONRTBLENDDESCBLENDENABLE				
#undef JSONRTBLENDDESCLOGICOPENABLE			
#undef JSONRTBLENDDESCSRCBLEND					
#undef JSONRTBLENDDESCDESTBLEND				
#undef JSONRTBLENDDESCBLENDOP					
#undef JSONRTBLENDDESCSRCBLENDALPHA			
#undef JSONRTBLENDDESCDESTBLENDALPHA			
#undef JSONRTBLENDDESCBLENDOPALPHA				
#undef JSONRTBLENDDESCLOGICOP					
#undef JSONRTBLENDDESCRTWRITEMASK				
#undef JSONRASTERIZERDESCFILLMODE				
#undef JSONRASTERIZERDESCCULLMODE				
#undef JSONRASTERIZERDESCFRONTCOUNTERCLOCKWISE	
#undef JSONRASTERIZERDESCDEPTHBIAS				
#undef JSONRASTERIZERDESCDEPTHBIASCLAMP		
#undef JSONRASTERIZERDESCSLOPESCALEDEPTHBIAS	
#undef JSONRASTERIZERDESCDEPTHCLIPENABLE		
#undef JSONRASTERIZERDESCMULTISAMPLEENABLE		
#undef JSONRASTERIZERDESCANTIALIASEDLINEENABLE	
#undef JSONRASTERIZERDESCFORCEDSAMPLECOUNT		
#undef JSONRASTERIZERDESCCONSERVATIVERASTER	
#undef JSONDEPTHSTENCILDESCDEPTHENABLE			
#undef JSONDEPTHSTENCILDESCDEPTHWRITEMASK		
#undef JSONDEPTHSTENCILDESCDEPTHFUNC			
#undef JSONDEPTHSTENCILDESCSTENCILENABLE		
#undef JSONDEPTHSTENCILDESCSTENCILREADMASK		
#undef JSONDEPTHSTENCILDESCSTENCILWRITEMASK	
#undef JSONDEPTHSTENCILDESCFRONTFACE			
#undef JSONDEPTHSTENCILDESCBACKFACE			
#undef JSONDEPTHSTENCILOPDESCSTENCILFAILOP		
#undef JSONDEPTHSTENCILOPDESCSTENCILDEPTHFAILOP
#undef JSONDEPTHSTENCILOPDESCSTENCILPASSOP		
#undef JSONDEPTHSTENCILOPDESCSTENCILFUNC		

	struct SRendererCreationInfo
	{
		ERenderAPIs RenderApi;
		u32 Width;
		u32 Height;
		bool bVSync;
		void* WndHandle;
	};

	struct SVertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TextureCoordinate;
	};

	struct SMesh
	{
		SMesh()
			: Vertices({})
			, Indices({})
		{

		}

		SMesh(const SMesh& _other)
			: Vertices(_other.Vertices)
			, Indices(_other.Indices)
		{

		}

		std::vector<SVertex> Vertices;
		std::vector<u32> Indices;
	};

	struct SWindowDimParams
	{
		SWindowDimParams() = default;

		SWindowDimParams(const SWindowDimParams& _other)
			: Width(_other.Width)
			, Height(_other.Height)
			, LeftTopCornerX(_other.LeftTopCornerX)
			, LeftTopCornerY(_other.LeftTopCornerY)
		{
		}

		SWindowDimParams(SWindowDimParams&& _other) noexcept
			: Width(std::move(_other.Width))
			, Height(std::move(_other.Height))
			, LeftTopCornerX(std::move(_other.LeftTopCornerX))
			, LeftTopCornerY(std::move(_other.LeftTopCornerY))
		{
		}

		void operator=(const SWindowDimParams& _other)
		{
			Width = _other.Width;
			Height = _other.Height;
			LeftTopCornerX = _other.LeftTopCornerX;
			LeftTopCornerY = _other.LeftTopCornerY;
		}

		s64 Width;
		s64 Height;
		s32 LeftTopCornerX;
		s32 LeftTopCornerY;
	};
}