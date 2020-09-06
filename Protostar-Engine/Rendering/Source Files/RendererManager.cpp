#include "..\Header Files\Public\RendererManager.h"

Protostar::CRendererManager* Protostar::CRendererManager::s_rendererManager = nullptr;

void Protostar::CRendererManager::Create(const SRendererCreationInfo& _info)
{
	if (!s_rendererManager)
	{
		s_rendererManager = new CRendererManager(_info);
	}
}

void Protostar::CRendererManager::CreateFontsTexture(unsigned char* _pixels, const s32& _width, const s32& _height, u64& _texID)
{
	s_rendererManager->m_renderer->CreateFontsTexture(_pixels, _width, _height, _texID);
}

void Protostar::CRendererManager::PrepareRendering()
{
	s_rendererManager->m_renderer->PrepareRendering();
}

void Protostar::CRendererManager::UpdateDrawData(SGuiDrawData* _drawData)
{
	s_rendererManager->m_renderer->UpdateDrawData(_drawData);
}

void Protostar::CRendererManager::OnInit(const std::vector<std::filesystem::path>& _shaderPaths)
{
	s_rendererManager->m_renderer->SetShaderPaths(_shaderPaths);
	s_rendererManager->m_renderer->OnInit();
}

void Protostar::CRendererManager::OnUpdate()
{
	s_rendererManager->m_renderer->OnUpdate();
}

void Protostar::CRendererManager::OnRender()
{
	s_rendererManager->m_renderer->OnRender();
}

void Protostar::CRendererManager::Destroy()
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

void Protostar::CRendererManager::RegisterAfterResizeCallback(const std::function<void(HWND, u32, WPARAM, LPARAM)>& _func)
{
	s_rendererManager->m_renderer->RegisterAfterResizeCallback(_func);
}
