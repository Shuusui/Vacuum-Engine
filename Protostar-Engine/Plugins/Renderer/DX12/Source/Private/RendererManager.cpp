#include "..\Header Files\Public\RendererManager.h"

Protostar::PRendererManager* Protostar::PRendererManager::s_rendererManager = nullptr;

void Protostar::PRendererManager::Create(const SRendererCreationInfo& _info)
{
	if (!s_rendererManager)
	{
		s_rendererManager = new PRendererManager(_info);
	}
}

void Protostar::PRendererManager::CreateFontsTexture(unsigned char* _pixels, const s32& _width, const s32& _height, u64& _texID)
{
	s_rendererManager->m_renderer->CreateFontsTexture(_pixels, _width, _height, _texID);
}

void Protostar::PRendererManager::PrepareRendering()
{
	s_rendererManager->m_renderer->PrepareRendering();
}

void Protostar::PRendererManager::UpdateGuiDrawData(SGuiDrawData* _drawData)
{
	s_rendererManager->m_renderer->UpdateGuiDrawData(_drawData);
}

void Protostar::PRendererManager::AddDrawData(SDrawData* _drawData)
{
	s_rendererManager->m_renderer->AddDrawData(_drawData);
}

void Protostar::PRendererManager::OnInit(const std::vector<std::filesystem::path>& _shaderPaths)
{
	s_rendererManager->m_renderer->SetShaderPaths(_shaderPaths);
	s_rendererManager->m_renderer->OnInit();
}

void Protostar::PRendererManager::OnUpdate()
{
	s_rendererManager->m_renderer->OnUpdate();
}

void Protostar::PRendererManager::OnRender()
{
	s_rendererManager->m_renderer->OnRender();
}

void Protostar::PRendererManager::Destroy()
{
	if (!s_rendererManager)
	{
		return;
	}
	s_rendererManager->m_renderer->OnDestroy();
	delete s_rendererManager->m_renderer;
	delete s_rendererManager;
	s_rendererManager = nullptr;
}

void Protostar::PRendererManager::RegisterAfterResizeCallback(const std::function<void(HWND, u32, WPARAM, LPARAM)>& _func)
{
	s_rendererManager->m_renderer->RegisterAfterResizeCallback(_func);
}

bool Protostar::PRendererManager::CreateRootSignature(void* _blob, void** _rootSignature)
{
	return s_rendererManager->m_renderer->CreateRootSignature((ID3DBlob*)_blob, (ID3D12RootSignature**)_rootSignature);
}

bool Protostar::PRendererManager::CreatePSO(void* _desc, void** _pso)
{
	return s_rendererManager->m_renderer->CreatePSO((D3D12_GRAPHICS_PIPELINE_STATE_DESC*)_desc, (ID3D12PipelineState**)_pso);
}
