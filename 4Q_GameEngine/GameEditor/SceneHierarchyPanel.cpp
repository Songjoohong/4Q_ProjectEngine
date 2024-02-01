#include "pch.h"
#include "SceneHierarchyPanel.h"

#include "../Engine/Transform.h"
#include "../Engine/Camera.h"
#include "../Engine/EntityIdentifer.h"
#include "../Engine/Light.h"
// TODO: 엔진에 있는 모든 컴포넌트를 인클루드 해야한다. 더 나은 방법이?


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

			//ImGui::Text(std::to_string(entity->getEntityId()).c_str());		// 토글 없이 그냥 출력
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = nullptr;

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				ECS::Entity* entity = m_Context->create();
				entity->Assign<EntityIdentifer>();	// 기본적으로 생성한다. (이름정보 때문)
				entity->Assign<Transform>();	// 에디터에서 오브젝트의 위치를 조정하기위해 Editor에서는 Transform을 기본적으로 생성해준다.
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();	/* Hierarchy End */

	ImGui::Begin("Properties");		
	// 선택된 오브젝트가 가진 모든 컴포넌트 정보를 출력한다. 
	if (m_SelectionContext)
	{
		DrawComponents(m_SelectionContext);
	}
	ImGui::End();	/* Properties End */
}

void SceneHierarchyPanel::DragDropEntity(ECS::Entity* entity)
{

}

void SceneHierarchyPanel::DrawEntityNode(ECS::Entity* entity)			// 포인터로 받지 않으면 함수 종료시 객체의 소멸자가 호출되어서 오류가 뜰 수 있음.
{
	std::string entID = std::to_string(entity->getEntityId());
	auto entityName = entity->get<EntityIdentifer>()->m_EntityName;
	std::string imguiID = "entt" + entID + " " + entityName;

	ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;		// 클릭하여 선택한 아이템 하이라이트 + 화살표 클릭시 노드 펼쳐지게
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, imguiID.c_str());
	if (ImGui::IsItemClicked())
	{
		m_SelectionContext = entity;
	}

	// Entity 삭제 메뉴
	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;

		ImGui::EndPopup();
	}

	// 노드가 펼쳐졌다면 자식도 출력.
	if (opened)
	{
		// TODO: 자식이 있다면 토글시 자식 오브젝트들도 나타내기

		// 예시 코드 1    -> TODO: 이런 식으로 구현하기.. Entity 의 id 값을 보고 자식 있는 지 확인 후 자식 노드도 다 나타내야 한다.
		//for (Transform* child : entity->get<Transform>().get().GetChildren())		
		//	DrawEntityNode(child->GetEntity());										// GetChildren 과 GetEntity 는 아직 없는 함수.

		// 예시 코드 2
		//ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		//bool opened = ImGui::TreeNodeEx((void*)9817239, flags, imguiID.c_str());
		//if (opened)
		//	ImGui::TreePop();

		ImGui::TreePop();
	}

	// Entity 삭제
	if (entityDeleted)
	{
		m_Context->destroy(entity);

		if (m_SelectionContext == entity)
			m_SelectionContext = nullptr;
	}

}

template <typename T, typename UIFunction>
static void DrawComponent(const std::string& name, ECS::Entity* entity, UIFunction uiFunction)
{

}

void SceneHierarchyPanel::DrawComponents(ECS::Entity* entity)
{
	// Entity 이름입력칸
	ImGui::SetNextItemWidth(100.f);
	auto& testentityID = entity->get<EntityIdentifer>()->m_EntityName;

	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	strncpy_s(buffer, sizeof(buffer), testentityID.c_str(), sizeof(buffer));
	if (ImGui::InputText("name", buffer, sizeof(buffer)))
	{
		testentityID = buffer;
	}

	ImGui::PushItemWidth(-1);

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		// 추가할 수 있는 모든 컴포넌트 목록을 출력한다.
		DisplayAddComponentEntry<Transform>("Transform");
		DisplayAddComponentEntry<Camera>("Camera");
		DisplayAddComponentEntry<Light>("Light");

		ImGui::EndPopup();
	}

	ImGui::PopItemWidth();

	DrawComponent<Transform>("Transform", entity, [](auto& component)
	{
		//component.m_Position;
	});
}

