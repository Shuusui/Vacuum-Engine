#include "..\Header Files\Public\RendererManager.h"

Vacuum::CRendererManager* Vacuum::CRendererManager::s_rendererManager = nullptr;

void Vacuum::CRendererManager::Create(const SRendererCreationInfo& _info)
{
	if (!s_rendererManager)
	{
		s_rendererManager = new CRendererManager(_info);
	}
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

