//#include "ShaderLibrary.h"
//
//#include <d3dcompiler.h>
//#include <unordered_set>
//#include <fstream>
//
//namespace Protostar::Core
//{
//	ShaderLibrary* ShaderLibrary::s_shaderLibrary = nullptr;
//
//	ShaderLibrary::ShaderLibrary(const std::filesystem::path& _projectPath)
//		: m_projectConfigPath(_projectPath / "Configs")
//		, m_shadersPath(_projectPath / "Content" / "Shaders")
//	{
//		m_shaderLibConfigPath = m_projectConfigPath / "shaderlib.ini";
//
//		if (std::filesystem::exists(m_shaderLibConfigPath))
//		{
//			LoadShaderJson();
//		}
//		LoadShaders(m_shadersPath);
//	}
//
//	ShaderLibrary::~ShaderLibrary()
//	{
//		if (m_vertexShaders.empty() && m_pixelShaders.empty())
//		{
//			return;
//		}
//
//		Save();
//	}
//
//	void ShaderLibrary::Create(const std::filesystem::path& _projectPath)
//	{
//		if (s_shaderLibrary)
//		{
//			return;
//		}
//
//		s_shaderLibrary = new ShaderLibrary(_projectPath);
//	}
//
//	void ShaderLibrary::Destroy()
//	{
//		if (s_shaderLibrary)
//		{
//			delete s_shaderLibrary;
//			s_shaderLibrary = nullptr;
//		}
//	}
//
//	void ShaderLibrary::Save()
//	{
//		Json json = {};
//		Json vertexShaderMaJson = {};
//		Json pixelShaderMaJson = {};
//
//		std::ofstream configFile(m_shaderLibConfigPath, std::ios::trunc);
//		configFile << json.dump();
//	}
//
//	void ShaderLibrary::LoadShaderJson()
//	{
//		//std::ifstream configFile(m_shaderLibConfigPath);
//
//		//Json json = {};
//		//configFile >> json;
//
//		//ID3DBlob* vertexShader = nullptr;
//		//ID3DBlob* pixelShader = nullptr;
//		//ID3DBlob* errorBlob = nullptr;
//
//		//Json vertexShaderMaJson = json[JSONVERTEXSHADERMAP].get<Json>();
//		//for (const auto& [guid, shaderInfoJson] : vertexShaderMaJson.items())
//		//{
//		//	Guid shaderGuid = Guid(guid);
//		//	if (!shaderGuid.IsValid())
//		//	{
//		//		continue;
//		//	}
//
//		//	PShaderInfo shaderInfo = {};
//		//	shaderInfo.Name = shaderInfoJson[JSONSHADERINFONAME].get<std::string>();
//		//	shaderInfo.ShaderPath = shaderInfoJson[JSONSHADERINFOPATH].get<std::string>();
//
//		//	if (D3DCompileFromFile(shaderInfo.ShaderPath.wstring().c_str(), nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vertexShader, &errorBlob) != S_OK)
//		//	{
//		//		if (!errorBlob)
//		//		{
//		//			continue;
//		//		}
//		//		PE_LOG(std::string((char*)errorBlob->GetBufferPointer()).c_str());
//		//		SafeRelease(vertexShader);
//		//		SafeRelease(errorBlob);
//		//		continue;
//		//	}
//		//	shaderInfo.Shader = vertexShader;
//
//		//	m_vertexShaders.insert(std::make_pair(shaderGuid, shaderInfo));
//		//	m_vertexShaderNames.insert(std::make_pair(shaderInfo.Name, shaderGuid));
//		//}
//
//		//Json pixelShaderMaJson = json[JSONPIXELSHADERMAP].get<Json>();
//		//for (const auto& [guid, shaderInfoJson] : pixelShaderMaJson.items())
//		//{
//		//	Guid shaderGuid = Guid(guid);
//		//	if (!shaderGuid.IsValid())
//		//	{
//		//		continue;
//		//	}
//
//		//	PShaderInfo shaderInfo = {};
//		//	shaderInfo.Name = shaderInfoJson[JSONSHADERINFONAME].get<std::string>();
//		//	shaderInfo.ShaderPath = shaderInfoJson[JSONSHADERINFOPATH].get<std::string>();
//
//		//	if (D3DCompileFromFile(shaderInfo.ShaderPath.wstring().c_str(), nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &pixelShader, &errorBlob) != S_OK)
//		//	{
//		//		if (!errorBlob)
//		//		{
//		//			continue;
//		//		}
//		//		PE_LOG(std::string((char*)errorBlob->GetBufferPointer()).c_str());
//		//		SafeRelease(pixelShader);
//		//		SafeRelease(errorBlob);
//		//		continue;
//		//	}
//		//	shaderInfo.Shader = pixelShader;
//
//		//	m_pixelShaders.insert(std::make_pair(shaderGuid, shaderInfo));
//		//	m_pixelShaderNames.insert(std::make_pair(shaderInfo.Name, shaderGuid));
//		//}
//
//		/*SafeRelease(errorBlob);*/
//	}
//
//	void ShaderLibrary::LoadShaders(const std::filesystem::path& _shadersDirPath)
//	{
//		std::filesystem::path vertexShaderDirPath = _shadersDirPath / "Vertex";
//		std::filesystem::path pixelShaderDirPath = _shadersDirPath / "Pixel";
//		std::filesystem::path combinedShaderDirPath = _shadersDirPath / "Combined";
//
//		if (!std::filesystem::exists(vertexShaderDirPath))
//		{
//			std::filesystem::create_directories(vertexShaderDirPath);
//		}
//		if (!std::filesystem::exists(pixelShaderDirPath))
//		{
//			std::filesystem::create_directories(pixelShaderDirPath);
//		}
//		if (!std::filesystem::exists(combinedShaderDirPath))
//		{
//			std::filesystem::create_directories(combinedShaderDirPath);
//		}
//
//		m_fileTree = Filesystem::GenerateFileTree<ShaderComplement>(_shadersDirPath, [this](const std::filesystem::path& _path, TreeNode<ShaderComplement>& _treeNode)->bool
//			{
//				std::string filename = _path.filename().string();
//
//				ShaderComplement complement = {};
//
//				if (Filesystem::HasParentDir(_path, "Vertex"))
//				{
//					if (m_vertexShaderNames.find(filename) != m_vertexShaderNames.end())
//					{
//						complement.VertexShaderInfo = m_vertexShaders[m_vertexShaderNames[filename]];
//						_treeNode.Asset = complement;
//						return true;
//					}
//
//					if (!LoadVertexShader(_path))
//					{
//						return false;
//					}
//
//					complement.VertexShaderInfo = m_vertexShaders[m_vertexShaderNames[filename]];
//					_treeNode.Asset = complement;
//					return true;
//				}
//
//				if (Filesystem::HasParentDir(_path, "Pixel"))
//				{
//					if (m_pixelShaderNames.find(filename) != m_pixelShaderNames.end())
//					{
//						complement.VertexShaderInfo = m_pixelShaders[m_pixelShaderNames[filename]];
//						_treeNode.Asset = complement;
//						return true;
//					}
//
//					if (!LoadPixelShader(_path))
//					{
//						return false;
//					}
//
//					complement.PixelShaderInfo = m_pixelShaders[m_pixelShaderNames[filename]];
//					_treeNode.Asset = complement;
//					return true;
//				}
//
//				if (Filesystem::HasParentDir(_path, "Combined"))
//				{
//					auto vertexShaderIterator = m_vertexShaderNames.find(filename);
//					auto pixelShaderIterator = m_pixelShaderNames.find(filename);
//
//					bool bVtxShaderExists = vertexShaderIterator != m_vertexShaderNames.end();
//					bool bPixelShaderExists = pixelShaderIterator != m_pixelShaderNames.end();
//
//					if (bVtxShaderExists && bPixelShaderExists)
//					{
//						PE_LOG_F("Shader with name %s already exists as vertex and pixel shader", filename.c_str());
//						return false;
//					}
//
//					Guid combinedShaderGuid = Guid::NewGuid();
//
//					if (bVtxShaderExists)
//					{
//						combinedShaderGuid = vertexShaderIterator->second;
//					}
//
//					if (bPixelShaderExists)
//					{
//						combinedShaderGuid = pixelShaderIterator->second;
//					}
//
//					bool bVtxShaderCompileFail = false;
//					bool bPixelShaderCompileFail = false;
//
//					if (!bVtxShaderExists)
//					{
//						if (LoadVertexShader(_path))
//						{
//							complement.VertexShaderInfo = m_vertexShaders[m_vertexShaderNames[filename]];
//
//						}
//						else
//						{
//							bVtxShaderCompileFail = true;
//						}
//					}
//
//					if (!bPixelShaderExists)
//					{
//						if (LoadPixelShader(_path))
//						{
//							complement.PixelShaderInfo = m_pixelShaders[m_pixelShaderNames[filename]];
//						}
//						else
//						{
//							bPixelShaderCompileFail = true;
//						}
//					}
//
//					if (bVtxShaderExists)
//					{
//						complement.VertexShaderInfo = m_vertexShaders[m_vertexShaderNames[filename]];
//					}
//					if (bPixelShaderExists)
//					{
//						complement.PixelShaderInfo = m_pixelShaders[m_pixelShaderNames[filename]];
//					}
//
//					_treeNode.Asset = complement;
//
//					if ((!bVtxShaderExists && bVtxShaderCompileFail) && (!bPixelShaderExists && bPixelShaderCompileFail))
//					{
//						return false;
//					}
//
//					return true;
//				}
//
//				PE_LOG_F("Path: %s is not in a content sub dir with Vertex/Pixel or Combined in it", _path.string().c_str());
//				return false;
//			}
//		, std::vector<std::filesystem::path>{".hlsl"});
//	}
//
//	bool ShaderLibrary::LoadVertexShader(const std::filesystem::path& _shaderPath)
//	{
//		std::string filename = _shaderPath.filename().string();
//
//		ID3DBlob* vertexShader = nullptr;
//		ID3DBlob* errorBlob = nullptr;
//
//		if (m_vertexShaderNames.find(filename) != m_vertexShaderNames.end())
//		{
//			return false;
//		}
//
//		if (D3DCompileFromFile(_shaderPath.wstring().c_str(), nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vertexShader, &errorBlob) == S_OK)
//		{
//			ShaderInfo shaderInfo = {};
//			shaderInfo.Name = filename;
//			shaderInfo.Shader = vertexShader;
//			shaderInfo.ShaderPath = _shaderPath;
//
//			Guid shaderGuid = Guid::NewGuid();
//
//			if (m_pixelShaderNames.find(filename) != m_pixelShaderNames.end())
//			{
//				shaderGuid = m_pixelShaderNames[filename];
//			}
//			m_vertexShaders.insert(std::make_pair(shaderGuid, shaderInfo));
//			m_vertexShaderNames.insert(std::make_pair(shaderInfo.Name, shaderGuid));
//			return true;
//		}
//		else
//		{
//			PE_LOG(std::string((char*)errorBlob->GetBufferPointer()).c_str());
//			SafeRelease(errorBlob);
//			SafeRelease(vertexShader);
//			return false;
//		}
//	}
//
//	bool ShaderLibrary::LoadPixelShader(const std::filesystem::path& _shaderPath)
//	{
//		std::string filename = _shaderPath.filename().string();
//
//		ID3DBlob* pixelShader = nullptr;
//		ID3DBlob* errorBlob = nullptr;
//
//		if (D3DCompileFromFile(_shaderPath.wstring().c_str(), nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &pixelShader, &errorBlob) == S_OK)
//		{
//			ShaderInfo shaderInfo = {};
//			shaderInfo.Name = filename;
//			shaderInfo.Shader = pixelShader;
//			shaderInfo.ShaderPath = _shaderPath;
//
//			Guid shaderGuid = Guid::NewGuid();
//
//			if (m_vertexShaderNames.find(shaderInfo.Name) != m_vertexShaderNames.end())
//			{
//				shaderGuid = m_vertexShaderNames[shaderInfo.Name];
//			}
//
//			m_pixelShaders.insert(std::make_pair(shaderGuid, shaderInfo));
//			m_pixelShaderNames.insert(std::make_pair(shaderInfo.Name, shaderGuid));
//			return true;
//		}
//		else
//		{
//			PE_LOG(std::string((char*)errorBlob->GetBufferPointer()).c_str());
//			SafeRelease(errorBlob);
//			SafeRelease(pixelShader);
//			return false;
//		}
//	}
//
//	bool ShaderLibrary::LoadCombinedShader(const std::filesystem::path& _shaderPath)
//	{
//		std::string filename = _shaderPath.filename().string();
//
//		if (m_vertexShaderNames.find(filename) != m_vertexShaderNames.end())
//		{
//			PE_LOG_F("Vertexshader with name %s already exists, can't load vertex shader with same name", filename.c_str());
//			return false;
//		}
//
//		if (m_pixelShaderNames.find(filename) != m_pixelShaderNames.end())
//		{
//			PE_LOG_F("Pixelshader with name %s already exists, can't load pixel shader with same name", filename.c_str());
//			return false;
//		}
//
//		ID3DBlob* vertexShader = nullptr;
//		ID3DBlob* pixelShader = nullptr;
//		ID3DBlob* errorBlob = nullptr;
//
//		if (D3DCompileFromFile(_shaderPath.wstring().c_str(), nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vertexShader, &errorBlob) != S_OK)
//		{
//			PE_LOG(std::string(static_cast<char*>(errorBlob->GetBufferPointer())));
//			SafeRelease(vertexShader);
//			SafeRelease(errorBlob);
//			return false;
//		}
//
//		if (D3DCompileFromFile(_shaderPath.wstring().c_str(), nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &pixelShader, &errorBlob) != S_OK)
//		{
//			PE_LOG(std::string(static_cast<char*>(errorBlob->GetBufferPointer())));
//			SafeRelease(pixelShader);
//			SafeRelease(errorBlob);
//			return false;
//		}
//
//		ShaderInfo shaderInfo = {};
//		shaderInfo.Name = filename;
//		shaderInfo.Shader = vertexShader;
//		shaderInfo.ShaderPath = _shaderPath;
//
//		m_vertexShaders.insert(std::make_pair(Guid::NewGuid(), shaderInfo));
//
//		shaderInfo = {};
//		shaderInfo.Name = filename;
//		shaderInfo.Shader = pixelShader;
//		shaderInfo.ShaderPath = _shaderPath;
//
//		m_pixelShaders.insert(std::make_pair(Guid::NewGuid(), shaderInfo));
//
//		SafeRelease(errorBlob);
//		return true;
//	}
//
//	void ShaderLibrary::UnloadVertexShader(const Guid& _guid)
//	{
//		ShaderInfo shaderInfo = m_vertexShaders.at(_guid);
//		SafeRelease(shaderInfo.Shader);
//		m_vertexShaders.erase(_guid);
//	}
//
//	void ShaderLibrary::UnloadPixelShader(const Guid& _guid)
//	{
//		ShaderInfo shaderInfo = m_pixelShaders.at(_guid);
//		SafeRelease(shaderInfo.Shader);
//		m_pixelShaders.erase(_guid);
//	}
//
//	void ShaderLibrary::UnloadShader(const Guid& _guid)
//	{
//		ShaderInfo shaderInfo = m_vertexShaders.at(_guid);
//		SafeRelease(shaderInfo.Shader);
//		m_vertexShaders.erase(_guid);
//
//		shaderInfo = m_pixelShaders.at(_guid);
//		SafeRelease(shaderInfo.Shader);
//		m_pixelShaders.erase(_guid);
//	}
//
//	bool ShaderLibrary::ContainsVertexShader(const Guid& _guid) const
//	{
//		return m_vertexShaders.find(_guid) != m_vertexShaders.end();
//	}
//
//	bool ShaderLibrary::ContainsPixelShader(const Guid& _guid) const
//	{
//		return m_pixelShaders.find(_guid) != m_pixelShaders.end();
//	}
//
//	bool ShaderLibrary::ContainsShader(const Guid& _guid) const
//	{
//		return (m_vertexShaders.find(_guid) != m_vertexShaders.end()) || (m_pixelShaders.find(_guid) != m_pixelShaders.end());
//	}
//
//	ShaderInfo ShaderLibrary::GetVertexShaderInfo(const Guid& _guid) const
//	{
//		return m_vertexShaders.at(_guid);
//	}
//
//	ShaderInfo ShaderLibrary::GetPixelShaderInfo(const Guid& _guid) const
//	{
//		return m_pixelShaders.at(_guid);
//	}
//
//	ShaderComplement ShaderLibrary::GetShaderInfos(const Guid& _guid) const
//	{
//		return ShaderComplement{ m_vertexShaders.at(_guid), m_pixelShaders.at(_guid) };
//	}
//
//	TreeObject<ShaderComplement> ShaderLibrary::GetShaderComplements() const
//	{
//		return m_fileTree;
//	}
//
//}