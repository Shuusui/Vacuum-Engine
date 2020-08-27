#include "..\Header Files\Public\RendererManager.h"

Vacuum::CRendererManager* Vacuum::CRendererManager::s_rendererManager = nullptr;

void Vacuum::CRendererManager::Create(const SRendererCreationInfo& _info)
{
	if (!s_rendererManager)
	{
		s_rendererManager = new CRendererManager(_info);
	}
}

void Vacuum::CRendererManager::CreateFontsTexture(unsigned char* _pixels, const int32& _width, const int32& _height, uint64& _texID)
{
	s_rendererManager->m_renderer->CreateFontsTexture(_pixels, _width, _height, _texID);
}

void Vacuum::CRendererManager::PrepareRendering()
{
	s_rendererManager->m_renderer->PrepareRendering();
}

void Vacuum::CRendererManager::UpdateDrawData(SDrawData* _drawData)
{
	s_rendererManager->m_renderer->UpdateDrawData(_drawData);
}

void Vacuum::CRendererManager::OnInit(const std::vector<std::filesystem::path>& _shaderPaths)
{
	s_rendererManager->m_renderer->SetShaderPaths(_shaderPaths);
	s_rendererManager->m_renderer->OnInit();
}

void Vacuum::CRendererManager::OnRender()
{
	s_rendererManager->m_renderer->OnRender();
}

void Vacuum::CRendererManager::Destroy()
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

void Vacuum::CRendererManager::RegisterAfterResizeCallback(const std::function<void(HWND, uint32, WPARAM, LPARAM)>& _func)
{
	s_rendererManager->m_renderer->RegisterAfterResizeCallback(_func);
}
