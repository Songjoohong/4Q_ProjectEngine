#include "pch.h"
#include "SceneHierarchyPanel.h"

#include "../Engine/Transform.h"


SceneHierarchyPanel::SceneHierarchyPanel(ECS::World* context)
{
	SetContext(context);
}

void SceneHierarchyPanel::SetContext(ECS::World* context)
{
	m_Context = context;
	m_SelectionContext = nullptr;	// 현재 World에서 Entity 를 초기화.
}

void SceneHierarchyPanel::RenderImGui()
{
	ImGui::Begin("Scene Hierarchy");

	if (m_Context)
	{
		//현재 씬이 가지고 있는 모든 Entity 의 정보를 그린다.
		for (auto entity : m_Context->GetEntities())
		{
			DrawEntityNode(entity);
		}

		if (ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				ECS::Entity* entity = m_Context->create();
				entity->Assign<Transform>();	// Transform은 기본적으로 생성해준다.
			}

			ImGui::EndPopup();
		}
	}

	ImGui::End();	// Hierarchy End


	ImGui::Begin("Properties");
	// 하이어라키 창에서 선택된 오브젝트가 있다면
	//if (m_SelectionContext)
	//{
	//	DrawComponents(m_SelectionContext);
	//}

	ImGui::End();	// Properties End

}

void SceneHierarchyPanel::DrawEntityNode(ECS::Entity* entity)			// 포인터로 받지 않으면 함수 종료시 객체의 소멸자가 호출되어서 오류가 뜰 수 있음.
{
	std::string entID = std::to_string(entity->getEntityId());
	static std::string entityName = "entityName";
	std::string imguiID = "entity" + entID;

	if (ImGui::TreeNodeEx(imguiID.c_str()))
		ImGui::TreePop();
}

void SceneHierarchyPanel::DrawComponents(ECS::Entity* entity)
{

	// Entity 이름입력버튼
	ImGui::SetNextItemWidth(100.f);

	std::string entID = std::to_string(entity->getEntityId());
	static std::string entityName = "object name";
	char buffer[256];

	memset(buffer, 0, sizeof(buffer));
	strncpy_s(buffer, sizeof(buffer), entityName.c_str(), sizeof(buffer));
	if (ImGui::InputText(("name" + entID).c_str(), buffer, sizeof(buffer)))
	{
		entityName = buffer;
	}

	//ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		DisplayAddComponentEntry<Transform>("Transform");

		ImGui::EndPopup();
	}

	//ImGui::PopItemWidth();


}
