#include "pch.h"
#include "SceneHierarchyPanel.h"

#include "../Engine/Transform.h"
#include "../Engine/Camera.h"
#include "../Engine/Light.h"
// TODO: ������ �ִ� ��� ������Ʈ�� ��Ŭ��� �ؾ��Ѵ�. �� ���� �����?


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

			//ImGui::Text(std::to_string(entity->getEntityId()).c_str());		// ��� ���� �׳� ���
		}


		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = nullptr;

		// Right-click on blank space
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
	// �ش� ������Ʈ�� ���� ��� ������Ʈ ������ ����Ѵ�.
	if (m_SelectionContext)
	{
		DrawComponents(m_SelectionContext);
	}

	ImGui::End();	// Properties End

}

void SceneHierarchyPanel::DragDropEntity(ECS::Entity* entity)
{

}

void SceneHierarchyPanel::DrawEntityNode(ECS::Entity* entity)			// �����ͷ� ���� ������ �Լ� ����� ��ü�� �Ҹ��ڰ� ȣ��Ǿ ������ �� �� ����.
{
	std::string entID = std::to_string(entity->getEntityId());
	static std::string entityName = "entityName";
	std::string imguiID = "entity" + entID;



	ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;		// Ŭ���Ͽ� ������ ������ ���̶���Ʈ + ȭ��ǥ Ŭ���� ��� ��������
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, imguiID.c_str());

	if (ImGui::IsItemClicked())
	{
		m_SelectionContext = entity;
	}

	// Entity ���� �޴�
	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	// ��尡 �������ٸ� �ڽĵ� ���.
	if (opened)
	{
		// TODO: �ڽ��� �ִٸ� ��۽� �ڽ� ������Ʈ�鵵 ��Ÿ����

		// ���� �ڵ� 1    -> TODO: �̷� ������ �����ϱ�.. Entity �� id ���� ���� �ڽ� �ִ� �� Ȯ�� �� �ڽ� ��嵵 �� ��Ÿ���� �Ѵ�.
		//for (Transform* child : entity->get<Transform>().get().GetChildren())		
		//	DrawEntityNode(child->GetEntity());										// GetChildren �� GetEntity �� ���� ���� �Լ�.

		// ���� �ڵ� 2
		//ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		//bool opened = ImGui::TreeNodeEx((void*)9817239, flags, imguiID.c_str());
		//if (opened)
		//	ImGui::TreePop();

		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		m_Context->destroy(entity);

		if (m_SelectionContext == entity)
			m_SelectionContext = nullptr;
	}

}

void SceneHierarchyPanel::DrawComponents(ECS::Entity* entity)
{
	// Entity �̸��Է�ĭ
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

	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		// ������ �� �ִ� ��� ������Ʈ ����� ����Ѵ�.
		DisplayAddComponentEntry<Transform>("Transform");
		DisplayAddComponentEntry<Camera>("Camera");
		DisplayAddComponentEntry<Light>("Light");

		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();


}
