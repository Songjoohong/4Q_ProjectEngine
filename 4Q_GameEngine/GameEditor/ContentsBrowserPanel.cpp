#include "pch.h"
#include "ContentsBrowserPanel.h"
#include "Prefab.h"
#include "d3d11.h"
#include "../D3D_Graphics/D3D_Graphics.h"
static const std::filesystem::path s_AssetsPath = "../Resource/prefab";

ContentsBrowserPanel::ContentsBrowserPanel()
	:m_CurrentDirectory(s_AssetsPath)
{
}

void ContentsBrowserPanel::Initialize()
{
	std::string pngPath = "../Resource/UI/box.png";
	auto filePath = Renderer::Instance->ConvertToWchar(pngPath);
	CreateTextureFromFile(Renderer::Instance->m_pDevice.Get(), filePath, &texture);
}

void ContentsBrowserPanel::RenderImGui()
{

	ImGui::Begin("Contents Browser");

	if (m_CurrentDirectory != std::filesystem::path(s_AssetsPath))
	{
		if (ImGui::Button("<-"))
		{
			m_CurrentDirectory = m_CurrentDirectory.parent_path();
		}
	}

	static float padding = 16.0f;
	static float thumbnailSize = 128.0f;
	float cellSize = thumbnailSize + padding;

	float panelWidth = ImGui::GetContentRegionAvail().x;
	int columnCount = (int)(panelWidth / cellSize);
	if (columnCount < 1)
		columnCount = 1;

	ImGui::Columns(columnCount, 0, false);

	for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentDirectory))
	{
		const auto& path = directoryEntry.path();
		auto relativePath = std::filesystem::relative(path, s_AssetsPath);
		std::string filenameString = relativePath.string();

		
		//나중에 ImageButton으로 수정
		//ImGui::Button(filenameString.c_str(), { thumbnailSize, thumbnailSize });

		ImGui::ImageButton((void*)texture, { thumbnailSize, thumbnailSize });
		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (directoryEntry.is_directory())
			{
				m_CurrentDirectory /= path.filename();
				continue;
			}
			
			m_PrefabManager->LoadPrefab("../Resource/prefab/" + filenameString);
			m_PrefabManager->m_prefabContainer.clear();
		}
		ImGui::Text(filenameString.c_str());

		ImGui::NextColumn();
	}

	ImGui::Columns(1);

	ImGui::SliderFloat("Thumbnail Size", &thumbnailSize, 16, 512);
	ImGui::SliderFloat("Padding", &padding, 0, 32);

	ImGui::End();
}

void ContentsBrowserPanel::SetContext(ECS::World* world, std::shared_ptr<PrefabManager> prefabManager)
{
	m_World = world;
	m_PrefabManager = prefabManager;
	m_PrefabManager->SetContext(world);
}

