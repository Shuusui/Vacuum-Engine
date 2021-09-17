#pragma once

#include "GlobalDefs.h"
#include "SharedEnums.h"
#include "DirectXMath.h"
#include "d3d12.h"
#include "Json.h"
#include "Guid.h"

import <utility>;
import <vector>;
import <algorithm>;

namespace Protostar
{
	namespace JsonKeys
	{
		constexpr const char* JSONPSOINFOINPUTELEDESCS					="input_element_descs";
		constexpr const char* JSONPSOINFOROOTSIGNATUREGUID				="root_signature_guid";
		constexpr const char* JSONPSOINFOVTXSHADERGUID					="vtx_shader_guid";
		constexpr const char* JSONPSOINFOPIXELSHADERGUID				="pixel_shader_guid";
		constexpr const char* JSONPSOINFONODEMASK						="node_mask";
		constexpr const char* JSONPSOINFOBLENDDESC						="blend_desc";
		constexpr const char* JSONPSOINFORASTERIZERDESC					="rasterizer_desc";
		constexpr const char* JSONPSOINFODEPTHSTENCILDESC				="depth_stencil_desc";
		constexpr const char* JSONINPUTELEDESCSEMNAME					="semantic_name";
		constexpr const char* JSONINPUTELEDESCSEMIDX					="semantic_index";
		constexpr const char* JSONINPUTELEDESCFORMAT					="format";
		constexpr const char* JSONINPUTELEDESCINPUTSLOT					="input_slot";
		constexpr const char* JSONINPUTELEDESCALIGNEDBYTEOFFSET			="aligned_byte_offset";
		constexpr const char* JSONINPUTELEDESCINPUTCLASSIFICATION		="input_classification";
		constexpr const char* JSONINPUTELEDESCINSTANCEDATASTEPRATE		="instace_data_step_rate";
		constexpr const char* JSONBLENDDESCALPHATOCOVENABLE				="alpha_to_cov_enable";
		constexpr const char* JSONBLENDDESCINDEPENDENTBLENDENABLE		="independent_blend_enable";
		constexpr const char* JSONBLENDDESCRENDERTARGETBLENDDESCS		="render_target_blend_descs";
		constexpr const char* JSONRTBLENDDESCBLENDENABLE				="blend_enable";
		constexpr const char* JSONRTBLENDDESCLOGICOPENABLE				="logic_op_enable";
		constexpr const char* JSONRTBLENDDESCSRCBLEND					="src_blend";
		constexpr const char* JSONRTBLENDDESCDESTBLEND					="dest_blend";
		constexpr const char* JSONRTBLENDDESCBLENDOP					="blend_op";
		constexpr const char* JSONRTBLENDDESCSRCBLENDALPHA				="src_blend_alpha";
		constexpr const char* JSONRTBLENDDESCDESTBLENDALPHA				="dest_blend_alpha";
		constexpr const char* JSONRTBLENDDESCBLENDOPALPHA				="blend_op_alpha";
		constexpr const char* JSONRTBLENDDESCLOGICOP					="logic_op";
		constexpr const char* JSONRTBLENDDESCRTWRITEMASK				="render_target_write_mask";
		constexpr const char* JSONRASTERIZERDESCFILLMODE				="fill_mode";
		constexpr const char* JSONRASTERIZERDESCCULLMODE				="cull_mode";
		constexpr const char* JSONRASTERIZERDESCFRONTCOUNTERCLOCKWISE	="front_counter_clockwise";
		constexpr const char* JSONRASTERIZERDESCDEPTHBIAS				="depth_bias";
		constexpr const char* JSONRASTERIZERDESCDEPTHBIASCLAMP			="depth_bias_clamp";
		constexpr const char* JSONRASTERIZERDESCSLOPESCALEDEPTHBIAS		="slope_scale_depth_bias";
		constexpr const char* JSONRASTERIZERDESCDEPTHCLIPENABLE			="depth_clip_enable";
		constexpr const char* JSONRASTERIZERDESCMULTISAMPLEENABLE		="multisample_enable";
		constexpr const char* JSONRASTERIZERDESCANTIALIASEDLINEENABLE	="antialiased_line_enable";
		constexpr const char* JSONRASTERIZERDESCFORCEDSAMPLECOUNT		="forced_sample_count";
		constexpr const char* JSONRASTERIZERDESCCONSERVATIVERASTER		="conservative_raster";
		constexpr const char* JSONDEPTHSTENCILDESCDEPTHENABLE			="depth_enable";
		constexpr const char* JSONDEPTHSTENCILDESCDEPTHWRITEMASK		="depth_write_mask";
		constexpr const char* JSONDEPTHSTENCILDESCDEPTHFUNC				="depth_func";
		constexpr const char* JSONDEPTHSTENCILDESCSTENCILENABLE			="stencil_enable";
		constexpr const char* JSONDEPTHSTENCILDESCSTENCILREADMASK		="stencil_read_mask";
		constexpr const char* JSONDEPTHSTENCILDESCSTENCILWRITEMASK		="stencil_write_mask";
		constexpr const char* JSONDEPTHSTENCILDESCFRONTFACE				="front_face";
		constexpr const char* JSONDEPTHSTENCILDESCBACKFACE				="back_face";
		constexpr const char* JSONDEPTHSTENCILOPDESCSTENCILFAILOP		="stencil_fail_op";
		constexpr const char* JSONDEPTHSTENCILOPDESCSTENCILDEPTHFAILOP	="stencil_depth_fail_op";
		constexpr const char* JSONDEPTHSTENCILOPDESCSTENCILPASSOP		="stencil_Pass_op";
		constexpr const char* JSONDEPTHSTENCILOPDESCSTENCILFUNC			="stencil_func";
	}


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


		Json ToJson() const
		{
			std::vector<Json> inputDescsJson = {};
			for (const D3D12_INPUT_ELEMENT_DESC& inputElemendDesc : InputElementDescs)
			{
				inputDescsJson.push_back(InputElementDescToJson(inputElemendDesc));
			}

			using namespace JsonKeys;

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
			using namespace JsonKeys;
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

	private:
		Json InputElementDescToJson(const D3D12_INPUT_ELEMENT_DESC& _inputDesc) const
		{
			using namespace JsonKeys;
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
			using namespace JsonKeys;
			std::string descName = _json[JSONINPUTELEDESCSEMNAME].get<std::string>();
			char* tempStr = new char[descName.size()];
			strcpy_s(tempStr, descName.size(), descName.c_str());
			return D3D12_INPUT_ELEMENT_DESC
			{
				tempStr,
				_json[JSONINPUTELEDESCSEMIDX].get<u32>(),
				(DXGI_FORMAT)_json[JSONINPUTELEDESCFORMAT].get<u32>(),
				_json[JSONINPUTELEDESCINPUTSLOT].get<u32>(),
				_json[JSONINPUTELEDESCALIGNEDBYTEOFFSET].get<u32>(),
				(D3D12_INPUT_CLASSIFICATION)_json[JSONINPUTELEDESCINPUTCLASSIFICATION].get<u32>(),
				_json[JSONINPUTELEDESCINSTANCEDATASTEPRATE].get<u32>()
			};
		}



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
			using namespace JsonKeys;
			return Json 
			{
				{JSONBLENDDESCALPHATOCOVENABLE, _blendDesc.AlphaToCoverageEnable}, 
				{JSONBLENDDESCINDEPENDENTBLENDENABLE, _blendDesc.IndependentBlendEnable},
				{JSONBLENDDESCRENDERTARGETBLENDDESCS, rtBlendDescs}
			};
		}

		D3D12_BLEND_DESC BlendDescFromJson(const Json& _json) const
		{
			using namespace JsonKeys;
			D3D12_BLEND_DESC blendDesc = {};
			for (s32 i = 0; i < _json[JSONBLENDDESCRENDERTARGETBLENDDESCS].get<std::vector<Json>>().size(); ++i)
			{
				blendDesc.RenderTarget[i] = RTBlendDescFromJson(_json[i]);
			}

			blendDesc.AlphaToCoverageEnable = _json[JSONBLENDDESCALPHATOCOVENABLE].get<bool>();
			blendDesc.IndependentBlendEnable = _json[JSONBLENDDESCINDEPENDENTBLENDENABLE].get<bool>();

			return blendDesc;
		}



		Json RTBlendDescToJson(const D3D12_RENDER_TARGET_BLEND_DESC& _rtBlendDesc) const
		{
			using namespace JsonKeys;
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
			using namespace JsonKeys;
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



		Json RasterizerDescToJson(const D3D12_RASTERIZER_DESC& _rasterizerDesc) const
		{
			using namespace JsonKeys;
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
			using namespace JsonKeys;
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



		Json DepthStencilDescToJson(const D3D12_DEPTH_STENCIL_DESC& _depthStencilDesc) const
		{
			using namespace JsonKeys;
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
			using namespace JsonKeys;
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



		Json DepthStencilOpDescToJson(const D3D12_DEPTH_STENCILOP_DESC& _depthStencilOpDesc) const
		{
			using namespace JsonKeys;
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
			using namespace JsonKeys;
			return D3D12_DEPTH_STENCILOP_DESC
			{
				(D3D12_STENCIL_OP)_json[JSONDEPTHSTENCILOPDESCSTENCILFAILOP].get<u32>(),
				(D3D12_STENCIL_OP)_json[JSONDEPTHSTENCILOPDESCSTENCILDEPTHFAILOP].get<u32>(),
				(D3D12_STENCIL_OP)_json[JSONDEPTHSTENCILOPDESCSTENCILPASSOP].get<u32>(),
				(D3D12_COMPARISON_FUNC)_json[JSONDEPTHSTENCILOPDESCSTENCILFUNC].get<u32>()
			};
		}
	};	

	struct SRendererCreationInfo
	{
		ERenderAPIs RenderApi;
		u32 Width;
		u32 Height;
		bool bVSync;
		void* WndHandle;

		SRendererCreationInfo()
			:RenderApi(ERenderAPIs::DX12)
			,Width(-1)
			,Height(-1)
			,bVSync(false)
			,WndHandle(nullptr)
		{
		}

		SRendererCreationInfo(ERenderAPIs _renderApi, u32 _width, u32 _height, bool _bVsync, void* _wndHandle)
			:RenderApi(_renderApi)
			,Width(_width)
			,Height(_height)
			,bVSync(_bVsync)
			,WndHandle(_wndHandle)
		{
		}

		SRendererCreationInfo(const SRendererCreationInfo&) = default;

		SRendererCreationInfo(SRendererCreationInfo&& _other) noexcept
			:RenderApi(std::move(_other.RenderApi))
			,Width(std::move(_other.Width))
			,Height(std::move(_other.Height))
			,bVSync(std::move(_other.bVSync))
			,WndHandle(std::move(_other.WndHandle))
		{
			_other = SRendererCreationInfo();
		}

		SRendererCreationInfo& operator=(const SRendererCreationInfo& _other)
		{
			RenderApi = _other.RenderApi;
			Width = _other.Width;
			Height = _other.Height;
			bVSync = _other.bVSync;
			WndHandle = _other.WndHandle;
			return *this;
		}
	};

	struct SVertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT2 TextureCoordinate;
	};

	struct SMesh
	{
		SMesh() = default;

		SMesh(const SMesh& _other) = default;

		SMesh(SMesh&& _other) noexcept
			:Vertices(std::move(_other.Vertices))
			,Indices(std::move(_other.Indices))
		{
		}

		SMesh& operator=(const SMesh&) = default;

		std::vector<SVertex> Vertices;
		std::vector<u32> Indices;
	};

	struct SWindowDimParams
	{
		SWindowDimParams()
			:Width(-1)
			,Height(-1)
			,LeftTopCornerX(-1)
			,LeftTopCornerY(-1)
		{
		};

		SWindowDimParams(const SWindowDimParams& _other) = default;

		SWindowDimParams(SWindowDimParams&& _other) noexcept
			: Width(std::move(_other.Width))
			, Height(std::move(_other.Height))
			, LeftTopCornerX(std::move(_other.LeftTopCornerX))
			, LeftTopCornerY(std::move(_other.LeftTopCornerY))
		{
			_other = SWindowDimParams();
		}

		SWindowDimParams& operator=(const SWindowDimParams& _other)
		{
			Width = _other.Width;
			Height = _other.Height;
			LeftTopCornerX = _other.LeftTopCornerX;
			LeftTopCornerY = _other.LeftTopCornerY;
			return *this;
		}

		s64 Width;
		s64 Height;
		s32 LeftTopCornerX;
		s32 LeftTopCornerY;
	};
}