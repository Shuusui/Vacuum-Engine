#include "..\Header Files\Public\ShaderLibrary.h"

#include <d3dcompiler.h>
#include <unordered_set>
#include <fstream>

#define JSONVERTEXSHADERMAP "vertex_shader_map"
#define JSONPIXELSHADERMAP "pixel_shader_map"

#define JSONSHADERINFONAME "name"
#define JSONSHADERINFOPATH "path"

Protostar::CShaderLibrary* Protostar::CShaderLibrary::s_shaderLibrary = nullptr;

Protostar::CShaderLibrary::CShaderLibrary(const std::filesystem::path& _projectPath)
	:m_projectConfigPath(_projectPath / "Configs")
{
	m_shaderLibConfigPath = m_projectConfigPath / "shaderlib.ini";

	if (std::filesystem::exists(m_shaderLibConfigPath))
	{
		LoadShaderJson();
	}

	LoadShaders(_projectPath / "Content" / "Shaders");
}

Protostar::CShaderLibrary::~CShaderLibrary()
{
	if (m_vertexShaders.empty() && m_pixelShaders.empty())
	{
		return;
	}

	Save();
}

void Protostar::CShaderLibrary::Create(const std::filesystem::path& _projectPath)
{
	if (s_shaderLibrary)
	{
		return;
	}

	s_shaderLibrary = new CShaderLibrary(_projectPath);
}

void Protostar::CShaderLibrary::Destroy()
{
	if (s_shaderLibrary)
	{
		delete s_shaderLibrary; 
		s_shaderLibrary = nullptr;
	}
}

void Protostar::CShaderLibrary::Save()
{
	Json json = {};
	Json vertexShaderMapJson = {};
	Json pixelShaderMapJson = {};

	for (const auto& [guid, shaderInfo] : m_vertexShaders)
	{
		vertexShaderMapJson[guid.ToString()] = ShaderInfoAsJson(shaderInfo);
	}

	for (const auto& [guid, shaderInfo] : m_pixelShaders)
	{
		pixelShaderMapJson[guid.ToString()] = ShaderInfoAsJson(shaderInfo);
	}

	json[JSONVERTEXSHADERMAP] = vertexShaderMapJson;
	json[JSONPIXELSHADERMAP] = pixelShaderMapJson;

	std::ofstream configFile(m_shaderLibConfigPath, std::ios::trunc);
	configFile << json.dump();
}

Json Protostar::CShaderLibrary::ShaderInfoAsJson(const SShaderInfo& _shaderInfo) const
{
	return Json{
		{JSONSHADERINFONAME,_shaderInfo.Name},
		{JSONSHADERINFOPATH, _shaderInfo.ShaderPath.string()}, 
	};
}

void Protostar::CShaderLibrary::LoadShaderJson()
{
	std::ifstream configFile(m_shaderLibConfigPath);

	Json json = {};
	configFile >> json;

	ID3DBlob* vertexShader = nullptr;
	ID3DBlob* pixelShader = nullptr;
	ID3DBlob* errorBlob = nullptr;

	Json vertexShaderMapJson = json[JSONVERTEXSHADERMAP].get<Json>();
	for (const auto& [guid, shaderInfoJson] : vertexShaderMapJson.items())
	{
		SGuid shaderGuid = SGuid(guid);
		if (!shaderGuid.IsValid())
		{
			 continue;
		}

		SShaderInfo shaderInfo = {};
		shaderInfo.Name = shaderInfoJson[JSONSHADERINFONAME].get<std::string>();
		shaderInfo.ShaderPath = shaderInfoJson[JSONSHADERINFOPATH].get<std::string>();

		if (D3DCompileFromFile(shaderInfo.ShaderPath.wstring().c_str(), nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vertexShader, &errorBlob) != S_OK)
		{
			PE_LOG(std::string((char*)errorBlob->GetBufferPointer()));
			SafeRelease(vertexShader);
			SafeRelease(errorBlob);
			continue;
		}
		shaderInfo.Shader = vertexShader;

		m_vertexShaders.insert(std::make_pair(shaderGuid, shaderInfo));
	}

	Json pixelShaderMapJson = json[JSONPIXELSHADERMAP].get<Json>();
	for (const auto& [guid, shaderInfoJson] : pixelShaderMapJson.items())
	{
		SGuid shaderGuid = SGuid(guid);
		if (!shaderGuid.IsValid())
		{
			continue;
		}

		SShaderInfo shaderInfo = {};
		shaderInfo.Name = shaderInfoJson[JSONSHADERINFONAME].get<std::string>();
		shaderInfo.ShaderPath = shaderInfoJson[JSONSHADERINFOPATH].get<std::string>();

		if (D3DCompileFromFile(shaderInfo.ShaderPath.wstring().c_str(), nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &pixelShader, &errorBlob) != S_OK)
		{
			PE_LOG(std::string((char*)errorBlob->GetBufferPointer()));
			SafeRelease(vertexShader);
			SafeRelease(errorBlob);
			continue;
		}
		shaderInfo.Shader = pixelShader;

		m_pixelShaders.insert(std::make_pair(shaderGuid, shaderInfo));
	}

	SafeRelease(errorBlob);
}

void Protostar::CShaderLibrary::LoadShaders(const std::filesystem::path& _shadersDirPath)
{
	std::filesystem::path vertexShaderDirPath = _shadersDirPath / "Vertex";
	std::filesystem::path pixelShaderDirPath = _shadersDirPath / "Pixel";
	std::filesystem::path combinedShaderDirPath = _shadersDirPath / "Combined";

	if (!std::filesystem::exists(vertexShaderDirPath))
	{
		std::filesystem::create_directories(vertexShaderDirPath);
	}
	if (!std::filesystem::exists(pixelShaderDirPath))
	{
		std::filesystem::create_directories(pixelShaderDirPath);
	}
	if (!std::filesystem::exists(combinedShaderDirPath))
	{
		std::filesystem::create_directories(combinedShaderDirPath);
	}

	ID3DBlob* vertexShader = nullptr;
	ID3DBlob* pixelShader = nullptr;
	ID3DBlob* errorBlob = nullptr;

	std::unordered_map<std::string, SGuid> vertexShaderNames = {};
	std::unordered_map<std::string, SGuid> pixelShaderNames = {};

	for (const std::filesystem::path& vertexShaderPath : std::filesystem::recursive_directory_iterator(vertexShaderDirPath))
	{
		if (D3DCompileFromFile(vertexShaderPath.wstring().c_str(), nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vertexShader, &errorBlob) == S_OK)
		{
			SShaderInfo shaderInfo = {};
			shaderInfo.Name = vertexShaderPath.filename().string();
			shaderInfo.Shader = vertexShader;
			shaderInfo.ShaderPath = vertexShaderPath;

			if (vertexShaderNames.find(shaderInfo.Name) != vertexShaderNames.end())
			{
				continue;
			}

			SGuid shaderGuid = SGuid::NewGuid();

			m_vertexShaders.insert(std::make_pair(shaderGuid, shaderInfo));
			vertexShaderNames.insert(std::make_pair(shaderInfo.Name, shaderGuid));
		}
		else
		{
			PE_LOG(std::string((char*)errorBlob->GetBufferPointer()));
			SafeRelease(errorBlob);
			SafeRelease(vertexShader);
		}
	}
	for (const std::filesystem::path& pixelShaderPath : std::filesystem::recursive_directory_iterator(pixelShaderDirPath))
	{
		if (D3DCompileFromFile(pixelShaderPath.wstring().c_str(), nullptr, nullptr, "ps_main", "vs_5_0", 0, 0, &pixelShader, &errorBlob) == S_OK)
		{
			SShaderInfo shaderInfo = {};
			shaderInfo.Name = pixelShaderPath.filename().string();
			shaderInfo.Shader = pixelShader;
			shaderInfo.ShaderPath = pixelShaderPath;

			SGuid shaderGuid = SGuid::NewGuid();

			if (pixelShaderNames.find(shaderInfo.Name) != pixelShaderNames.end())
			{
				continue;
			}

			if (vertexShaderNames.find(shaderInfo.Name) != vertexShaderNames.end())
			{
				shaderGuid = vertexShaderNames[shaderInfo.Name];
			}

			m_pixelShaders.insert(std::make_pair(shaderGuid, shaderInfo));
			pixelShaderNames.insert(std::make_pair(shaderInfo.Name, shaderGuid));
		}
		else
		{
			PE_LOG(std::string((char*)errorBlob->GetBufferPointer()));
			SafeRelease(errorBlob);
			SafeRelease(pixelShader);
		}
	}
	for (const std::filesystem::path& combinedShaderPath : std::filesystem::recursive_directory_iterator(combinedShaderDirPath))
	{
		auto vertexShaderIterator = vertexShaderNames.find(combinedShaderPath.filename().string());
		auto pixelShaderIterator = pixelShaderNames.find(combinedShaderPath.filename().string());

		if (vertexShaderIterator != vertexShaderNames.end() && pixelShaderIterator != pixelShaderNames.end())
		{
			continue;
		}

		SGuid combinedShaderGuid = SGuid::NewGuid();

		if (vertexShaderIterator != vertexShaderNames.end())
		{
			combinedShaderGuid = vertexShaderIterator->second;
		}

		if (pixelShaderIterator != pixelShaderNames.end())
		{
			combinedShaderGuid = pixelShaderIterator->second;
		}

		if (D3DCompileFromFile(combinedShaderPath.wstring().c_str(), nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vertexShader, &errorBlob) == S_OK)
		{
			SShaderInfo shaderInfo = {};
			shaderInfo.Name = combinedShaderPath.filename().string();
			shaderInfo.Shader = vertexShader;
			shaderInfo.ShaderPath = combinedShaderPath;


			m_vertexShaders.insert(std::make_pair(combinedShaderGuid, shaderInfo));
			if (vertexShaderIterator == vertexShaderNames.end())
			{
				vertexShaderNames.insert(std::make_pair(shaderInfo.Name, combinedShaderGuid));
			}
		}
		else
		{
			PE_LOG(std::string((char*)errorBlob->GetBufferPointer()));
			SafeRelease(errorBlob);
			SafeRelease(vertexShader);
		}

		if (D3DCompileFromFile(combinedShaderPath.wstring().c_str(), nullptr, nullptr, "ps_main", "vs_5_0", 0, 0, &pixelShader, &errorBlob) == S_OK)
		{
			SShaderInfo shaderInfo = {};
			shaderInfo.Name = combinedShaderPath.filename().string(); 
			shaderInfo.Shader = pixelShader;
			shaderInfo.ShaderPath = combinedShaderPath;

			m_pixelShaders.insert(std::make_pair(combinedShaderGuid, shaderInfo));
			if (pixelShaderIterator == pixelShaderNames.end())
			{
				pixelShaderNames.insert(std::make_pair(shaderInfo.Name, combinedShaderGuid));
			}
		}
		else
		{
			PE_LOG(std::string((char*)errorBlob->GetBufferPointer()));
			SafeRelease(errorBlob);
			SafeRelease(pixelShader);
		}
	}
	SafeRelease(errorBlob);
}

void Protostar::CShaderLibrary::LoadVertexShader(const std::filesystem::path& _shaderPath)
{
	std::string filename = _shaderPath.filename().string();

	for (const auto& [guid, shaderInfo] : m_vertexShaders)
	{
		if (shaderInfo.Name == filename)
		{
			PE_LOG("Vertexshader with name %s already exists, can't load vertex shader with same name");
			return;
		}
	}
	
	ID3DBlob* vertexShader = nullptr;
	ID3DBlob* errorBlob = nullptr;
	if (D3DCompileFromFile(_shaderPath.wstring().c_str(), nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vertexShader, &errorBlob) != S_OK)
	{
		PE_LOG(std::string((char*)errorBlob->GetBufferPointer()));
		SafeRelease(errorBlob);
		SafeRelease(vertexShader);
		return;
	}

	SShaderInfo shaderInfo = {};
	shaderInfo.Name = filename;
	shaderInfo.Shader = vertexShader;
	shaderInfo.ShaderPath = _shaderPath;

	SafeRelease(errorBlob);

	m_vertexShaders.insert(std::make_pair(SGuid::NewGuid(), shaderInfo));
}

void Protostar::CShaderLibrary::LoadPixelShader(const std::filesystem::path& _shaderPath)
{
	std::string filename = _shaderPath.filename().string();

	for (const auto& [guid, shaderInfo] : m_pixelShaders)
	{
		if (shaderInfo.Name == filename)
		{
			PE_LOG_F("Pixelshader with name %s already exists, can't load pixel shader with same name", filename.c_str());
			return;
		}
	}

	ID3DBlob* pixelShader = nullptr;
	ID3DBlob* errorBlob = nullptr;
	if (D3DCompileFromFile(_shaderPath.wstring().c_str(), nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &pixelShader, &errorBlob) != S_OK)
	{
		PE_LOG(std::string((char*)errorBlob->GetBufferPointer()));
		SafeRelease(errorBlob);
		SafeRelease(pixelShader);
		return;
	}

	SShaderInfo shaderInfo = {};
	shaderInfo.Name = filename;
	shaderInfo.Shader = pixelShader;
	shaderInfo.ShaderPath = _shaderPath;

	SafeRelease(errorBlob);

	m_pixelShaders.insert(std::make_pair(SGuid::NewGuid(), shaderInfo));
}

void Protostar::CShaderLibrary::LoadCombinedShader(const std::filesystem::path& _shaderPath)
{
	std::string filename = _shaderPath.filename().string();

	for (const auto& [guid, shaderInfo] : m_vertexShaders)
	{
		if (shaderInfo.Name == filename)
		{
			PE_LOG_F("Vertexshader with name %s already exists, can't load vertex shader with same name", filename.c_str());
			return;
		}
	}

	for (const auto& [guid, shaderInfo] : m_pixelShaders)
	{
		if (shaderInfo.Name == filename)
		{
			PE_LOG_F("Pixelshader with name %s already exists, can't load pixel shader with same name", filename.c_str());
			return;
		}
	}

	ID3DBlob* vertexShader = nullptr;
	ID3DBlob* pixelShader = nullptr;
	ID3DBlob* errorBlob = nullptr;

	if (D3DCompileFromFile(_shaderPath.wstring().c_str(), nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &vertexShader, &errorBlob) != S_OK)
	{
		PE_LOG(std::string((char*)errorBlob->GetBufferPointer()));
		SafeRelease(vertexShader);
		SafeRelease(errorBlob);
		return;
	}

	if (D3DCompileFromFile(_shaderPath.wstring().c_str(), nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &pixelShader, &errorBlob) != S_OK)
	{
		PE_LOG(std::string((char*)errorBlob->GetBufferPointer()));
		SafeRelease(pixelShader);
		SafeRelease(errorBlob);
		return;
	}

	SShaderInfo shaderInfo = {};
	shaderInfo.Name = filename;
	shaderInfo.Shader = vertexShader;
	shaderInfo.ShaderPath = _shaderPath;

	m_vertexShaders.insert(std::make_pair(SGuid::NewGuid(), shaderInfo));

	shaderInfo = {};
	shaderInfo.Name = filename;
	shaderInfo.Shader = pixelShader;
	shaderInfo.ShaderPath = _shaderPath;

	m_pixelShaders.insert(std::make_pair(SGuid::NewGuid(), shaderInfo));

	SafeRelease(errorBlob);
}

void Protostar::CShaderLibrary::UnloadVertexShader(const SGuid& _guid)
{
	SShaderInfo shaderInfo =  m_vertexShaders.at(_guid);
	SafeRelease(shaderInfo.Shader);
	m_vertexShaders.erase(_guid);
}

void Protostar::CShaderLibrary::UnloadPixelShader(const SGuid& _guid)
{
	SShaderInfo shaderInfo = m_pixelShaders.at(_guid);
	SafeRelease(shaderInfo.Shader);
	m_pixelShaders.erase(_guid);
}

void Protostar::CShaderLibrary::UnloadShader(const SGuid& _guid)
{
	SShaderInfo shaderInfo = m_vertexShaders.at(_guid);
	SafeRelease(shaderInfo.Shader);
	m_vertexShaders.erase(_guid);

	shaderInfo = m_pixelShaders.at(_guid);
	SafeRelease(shaderInfo.Shader);
	m_pixelShaders.erase(_guid);
}

bool Protostar::CShaderLibrary::ContainsVertexShader(const SGuid& _guid) const
{
	return m_vertexShaders.find(_guid) != m_vertexShaders.end();
}

bool Protostar::CShaderLibrary::ContainsPixelShader(const SGuid& _guid) const
{
	return m_pixelShaders.find(_guid) != m_pixelShaders.end();
}

bool Protostar::CShaderLibrary::ContainsShader(const SGuid& _guid) const
{
	return (m_vertexShaders.find(_guid) != m_vertexShaders.end()) || (m_pixelShaders.find(_guid) != m_pixelShaders.end());
}

Protostar::SShaderInfo Protostar::CShaderLibrary::GetVertexShaderInfo(const SGuid& _guid) const
{
	return m_vertexShaders.at(_guid);
}

Protostar::SShaderInfo Protostar::CShaderLibrary::GetPixelShaderInfo(const SGuid& _guid) const
{
	return m_pixelShaders.at(_guid);
}

Protostar::SShaderComplement Protostar::CShaderLibrary::GetShaderInfos(const SGuid& _guid) const
{
	return SShaderComplement{m_vertexShaders.at(_guid), m_pixelShaders.at(_guid)};
}

std::unordered_map<Protostar::SGuid, Protostar::SShaderComplement> Protostar::CShaderLibrary::GetShaderComplements() const
{
	std::unordered_map<SGuid, SShaderComplement> shaderComplements = {};

	for (const auto& [guid, shaderInfo] : m_vertexShaders)
	{
		shaderComplements.insert(std::make_pair(guid, SShaderComplement{shaderInfo}));
	}

	for (const auto& [guid, shaderInfo] : m_pixelShaders)
	{
		if (shaderComplements.find(guid) == shaderComplements.end())
		{
			SShaderComplement& shaderComplement = shaderComplements[guid];
			shaderComplement.PixelShaderInfo = shaderInfo;
			continue;
		}

		shaderComplements.insert(std::make_pair(guid, SShaderComplement{shaderInfo}));
	}

	return shaderComplements;
}


#undef JSONVERTEXSHADERMAP
#undef JSONPIXELSHADERMAP

#undef JSONSHADERINFONAME
#undef JSONSHADERINFOPATH