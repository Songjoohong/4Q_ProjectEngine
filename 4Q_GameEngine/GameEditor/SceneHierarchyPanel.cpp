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
	m_SelectionContext = nullptr;	// ���� World���� Entity �� �ʱ�ȭ.
}

void SceneHierarchyPanel::RenderImGui()
{
	ImGui::Begin("Scene Hierarchy");

	if (m_Context)
	{
		//���� ���� ������ �ִ� ��� Entity �� ������ �׸���.
		for (auto entity : m_Context->GetEntities())
		{
			DrawEntityNode(entity);
		}

		if (ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				ECS::Entity* entity = m_Context->create();
				entity->Assign<Transform>();	// Transform�� �⺻������ �������ش�.
			}

			ImGui::EndPopup();
		}
	}

	ImGui::End();	// Hierarchy End


	ImGui::Begin("Properties");
	// ���̾��Ű â���� ���õ� ������Ʈ�� �ִٸ�
	//if (m_SelectionContext)
	//{
	//	DrawComponents(m_SelectionContext);
	//}

	ImGui::End();	// Properties End

}

void SceneHierarchyPanel::DrawEntityNode(ECS::Entity* entity)			// �����ͷ� ���� ������ �Լ� ����� ��ü�� �Ҹ��ڰ� ȣ��Ǿ ������ �� �� ����.
{
	std::string entID = std::to_string(entity->getEntityId());
	static std::string entityName = "entityName";
	std::string imguiID = "entity" + entID;

	if (ImGui::TreeNodeEx(imguiID.c_str()))
		ImGui::TreePop();
}

void SceneHierarchyPanel::DrawComponents(ECS::Entity* entity)
{

	// Entity �̸��Է¹�ư
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
