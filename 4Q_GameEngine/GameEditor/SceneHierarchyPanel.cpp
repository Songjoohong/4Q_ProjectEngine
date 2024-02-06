#include "pch.h"
#include "SceneHierarchyPanel.h"

#include "../Engine/Transform.h"
#include "../Engine/Camera.h"
#include "../Engine/EntityIdentifier.h"
#include "../Engine/Light.h"

#include "../Engine/BoxCollider.h"
#include "../Engine/StaticMesh.h"

#include "Prefab.h"


SceneHierarchyPanel::SceneHierarchyPanel(ECS::World* context)
{
	
}

void SceneHierarchyPanel::SetContext(ECS::World* context, std::shared_ptr<PrefabManager> prefab)
{
	m_Context = context;
	m_SelectionContext = nullptr;	// 현재 World에서 Entity 를 초기화.

	m_PrefabManager = prefab;
}

void SceneHierarchyPanel::RenderImGui()
{
	ImGui::Begin("Scene Hierarchy");

	if (m_Context)
	{
		//현재 씬이 가지고 있는 모든 Entity 의 정보를 그린다.
		for (auto entity : m_Context->GetEntities())
		{
			// 최상위 부모로 등록된 애들만 먼저 그림
			/*if (entity->get<EntityIdentifier>().get().m_HasParent == false)
			{
				DrawEntityNode(entity);
			}*/

			if (entity->m_parent == nullptr)
				DrawEntityNode(entity);

			DragDropEntityHierarchy(entity);
			int test = 1;		//TESTSTETESTTEST!!
		}

		// Unselect object when left-clicking on blank space.
		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			m_SelectionContext = nullptr;

		// Right-click on blank space
		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
			{
				ECS::Entity* entity = m_Context->create();
				entity->Assign<EntityIdentifier>();	// 기본적으로 생성한다. (이름정보 때문)
				entity->Assign<Transform>();	// 에디터에서 오브젝트의 위치를 조정하기위해 Transform은 기본적으로 생성해준다.
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

void SceneHierarchyPanel::DragDropEntityHierarchy(ECS::Entity* entity)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None))
	{
		ImGui::Text(entity->get<EntityIdentifier>()->m_EntityName.c_str());

		ImGui::SetDragDropPayload("EntityID", entity, sizeof(ECS::Entity));
		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("EntityID");

		if (payLoad)
		{
			ECS::Entity* picked = static_cast<ECS::Entity*>(payLoad->Data);
			ECS::Entity* target = entity;

			// 현재 자신의 부모 위에 올려두면 부모자식 관계를 해제한다.
			if (picked == target)
				target->RemoveChild(picked);
			else
			{
				picked->get<EntityIdentifier>().get().m_ParentEntityId = target->getEntityId();
				target->addChild(picked);
			}

			int wait = 1;
		}

		ImGui::EndDragDropTarget();
	}
}

void SceneHierarchyPanel::DrawEntityNode(ECS::Entity* entity)			// 포인터로 받지 않으면 함수 종료시 객체의 소멸자가 호출되어서 오류가 뜰 수 있음.
{
	bool temp = entity->has<EntityIdentifier>();
	if (!temp)
	{
		return;
	}
	std::string entID = std::to_string(entity->getEntityId());
	auto entityName = entity->get<EntityIdentifier>()->m_EntityName;
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
		if (ImGui::MenuItem("Make Prefab"))
		{
			m_PrefabManager->SavePrefab(entity, "Prefab.json");
		}
		ImGui::EndPopup();
	}
	// 노드가 펼쳐졌다면 자식도 출력.
	if (opened)
	{
		if (entID == "1")					// TEST!!!!!!!!!!
			int a = 1324;

		for (const auto& child : entity->m_children)
		{
			DrawEntityNode(child);
		}

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

static void DrawVec3Control(const std::string& label, Vector3D& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
	ImGui::PushFont(boldFont);

	if (ImGui::Button("X", buttonSize))
		values.SetX(resetValue); //
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();

	float tempX = values.GetX();//

	ImGui::DragFloat("##X", &tempX, 0.1f, 0.0f, 0.0f, "%.2f"); //
	values.SetX(tempX);	//

	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.SetY(resetValue);
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	float tempY = values.GetY();	//
	ImGui::DragFloat("##Y", &tempY, 0.1f, 0.0f, 0.0f, "%.2f");	//
	values.SetY(tempY);	//
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.SetZ(resetValue);
	ImGui::PopFont();
	ImGui::PopStyleColor(3);

	ImGui::SameLine();
	float tempZ = values.GetZ();	//
	ImGui::DragFloat("##Z", &tempZ, 0.1f, 0.0f, 0.0f, "%.2f");	//
	values.SetZ(tempZ);	//
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}

template <typename T, typename UIFunction>
static void DrawComponent(const std::string& name, ECS::Entity* entity, UIFunction uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (entity->has<T>())
	{
		auto component = entity->get<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar(
		);
		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove component"))
				removeComponent = true;

			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity->remove<T>();
	}
}

void SceneHierarchyPanel::DrawComponents(ECS::Entity* entity)
{
	// Type Entity's Name
	if (entity->has<EntityIdentifier>())
	{
		auto& testentityID = entity->get<EntityIdentifier>()->m_EntityName;
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strncpy_s(buffer, sizeof(buffer), testentityID.c_str(), sizeof(buffer));
		if (ImGui::InputText("##Name", buffer, sizeof(buffer)))
		{
			testentityID = buffer;
		}
	}

	ImGui::PushItemWidth(-1);
	ImGui::SameLine();

	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("AddComponent");

	if (ImGui::BeginPopup("AddComponent"))
	{
		// 추가할 수 있는 모든 컴포넌트 목록을 출력하고 선택하면 추가한다.
		DisplayAddComponentEntry<Transform>("Transform");
		DisplayAddComponentEntry<StaticMesh>("StaticMesh");
		DisplayAddComponentEntry<BoxCollider>("BoxCollider");
		DisplayAddComponentEntry<Camera>("Camera");
		DisplayAddComponentEntry<Light>("Light");

		ImGui::EndPopup();
	}
	ImGui::PopItemWidth();

	DrawComponent<Transform>("Transform", entity, [](auto component)
	{
		DrawVec3Control("Position", component->m_Position);
		DrawVec3Control("Rotation", component->m_Rotation);
		DrawVec3Control("Scale", component->m_Scale, 1.0f);
	});

	DrawComponent<StaticMesh>("StaticMesh", entity, [](auto component)
	{
		std::string temp = component->m_FileName;

		ImGui::Text(temp.c_str());
	});

	DrawComponent<BoxCollider>("BoxCollider", entity, [](auto component)
	{
		// 이것도 사용법을..
	});

	DrawComponent<Camera>("Camera", entity, [](auto component)
	{
	});

	DrawComponent<Light>("Light", entity, [](auto component)
	{
		const char* lightTypeStrings[] = { "Point Light", "Directional Light" };
		const char* currentLightTypeString = lightTypeStrings[(int)component->m_Type];
		ImGui::SetNextItemWidth(150.f);

		if (ImGui::BeginCombo("Light Type", currentLightTypeString))
		{
			for (int i = 0; i < 2; i++)
			{
				bool isSelected = currentLightTypeString == lightTypeStrings[i];
				if (ImGui::Selectable(lightTypeStrings[i], isSelected))
				{
					currentLightTypeString = lightTypeStrings[i];
					component->m_Type = static_cast<LightType>(i);
				}

				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}

			ImGui::EndCombo();
		}

		// TODO: 조건 1. PointLight 일 때

		// TODO: 조건 2. Directional Light 일 때

		/// -> 라이트 타입별 나타내야 하는 정보가 다르다.     다른가? 흐음..
	});
}

