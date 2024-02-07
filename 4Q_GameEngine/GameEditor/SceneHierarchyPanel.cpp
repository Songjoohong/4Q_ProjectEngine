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
	SetContext(context, m_PrefabManager);
}

void SceneHierarchyPanel::SetContext(ECS::World* context, std::shared_ptr<PrefabManager> prefab)
{
	m_Context = context;
	m_SelectionContext = nullptr;	// ���� World���� Entity �� �ʱ�ȭ.

	m_PrefabManager = prefab;
}

void SceneHierarchyPanel::RenderImGui()
{
	ImGui::Begin("Scene Hierarchy");

	if (m_Context)
	{
		//���� ���� ������ �ִ� ��� Entity �� ������ �׸���.
		for (auto entity : m_Context->GetEntities())
		{
			// �ֻ��� �θ�� ��ϵ� �ֵ鸸 ���� �׸�
			/*if (entity->get<EntityIdentifier>().get().m_HasParent == false)
			{
				DrawEntityNode(entity);
			}*/

			if (entity->m_parent == nullptr)
				DrawEntityNode(entity);


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
				entity->Assign<EntityIdentifier>();	// �⺻������ �����Ѵ�. (�̸����� ����)
				entity->Assign<Transform>();	// �����Ϳ��� ������Ʈ�� ��ġ�� �����ϱ����� Transform�� �⺻������ �������ش�.
			}

			ImGui::EndPopup();
		}
	}
	ImGui::End();	/* Hierarchy End */

	ImGui::Begin("Properties");		
	// ���õ� ������Ʈ�� ���� ��� ������Ʈ ������ ����Ѵ�. 
	if (m_SelectionContext)
	{
		DrawComponents(m_SelectionContext);
		SetPrefabFileName(m_SelectionContext);
	}
	ImGui::End();	/* Properties End */
}

void SceneHierarchyPanel::SetPrefabFileName(ECS::Entity* entity)
{
	if (m_OpenTextPopup)
	{
		ImGui::SetNextWindowSize(ImVec2(320, 120));
		ImGui::OpenPopup("Prefab Name");
		if (ImGui::BeginPopupModal("Prefab Name"))
		{
			static char prefabName[256] = ""; // Fixed-size buffer for input

			ImGui::InputText("Prefab Name", prefabName, sizeof(prefabName));
			//ImGui::EndGroup();
			ImGui::Spacing();
			//ImGui::SetCursorPosX(ImGui::GetWindowSize().x - ImGui::GetStyle().ItemSpacing.x - ImGui::CalcTextSize("Submit").x - 250.f);
			if (ImGui::Button("Submit"))
			{
				std::string prefabFile = prefabName;
				prefabFile += ".prefab";
				m_PrefabManager.get()->SavePrefab(entity, prefabFile);
				ImGui::CloseCurrentPopup();
				m_SelectionContext = nullptr;
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				std::string prefabFile = prefabName;
				prefabFile += ".prefab";
				m_PrefabManager.get()->SavePrefab(entity, prefabFile);
				ImGui::CloseCurrentPopup();
				m_SelectionContext = nullptr;
			}

			//ImGui::EndGroup();
			ImGui::Spacing();
			//ImGui::SetCursorPosX(ImGui::GetWindowSize().x - ImGui::GetStyle().ItemSpacing.x - ImGui::CalcTextSize("Close").x -270.f);

			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup(); // Close the current popup window
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_Escape))
			{
				ImGui::CloseCurrentPopup(); // Close the current popup window
			}

			ImGui::EndPopup();
		}
	}
}

void SceneHierarchyPanel::DragDropEntityHierarchy(ECS::Entity* entity)
{
	if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
	{
		//const auto& selectedEntityID = m_SelectionContext->getEntityId();
		size_t entityID = entity->getEntityId();

		//if (m_SelectionContext != entity)
		//{
			ImGui::Text(entity->get<EntityIdentifier>()->m_EntityName.c_str());
			ImGui::SetDragDropPayload("EntityID", &entityID, 1 * sizeof(size_t));
		//}
		//else
		//{
		//	ECS::Entity* entt = m_Context->getByIndex(selectedEntityID);
		//	const char* name = entt->get<EntityIdentifier>()->m_EntityName.c_str();
		//	ImGui::Text(name);

		//	ImGui::SetDragDropPayload("EntityID", &selectedEntityID, 1 * sizeof(size_t));
		//}

		ImGui::EndDragDropSource();
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payLoad = ImGui::AcceptDragDropPayload("EntityID", ImGuiDragDropFlags_AcceptNoDrawDefaultRect);

		if (payLoad)
		{
			size_t pickedID = *(static_cast<size_t*>(payLoad->Data));

			ECS::Entity* picked = m_Context->getByIndex(pickedID - 1);
			ECS::Entity* target = entity;

			// �ڱ� �ڽ��� �ڽ����� ����Ϸ��� ��� �ƹ��� ó���� ���� �ʴ´�. (�̰� ����ϸ� ��ƼƼ�� ������.)
			if (picked->isDescendant(target))
			{
				return;
			}

			picked->get<EntityIdentifier>().get().m_ParentEntityId = target->getEntityId();
			target->addChild(picked);
			m_SelectionContext = nullptr;

		}

		ImGui::EndDragDropTarget();
	}
}

void SceneHierarchyPanel::DrawEntityNode(ECS::Entity* entity)			// �����ͷ� ���� ������ �Լ� ����� ��ü�� �Ҹ��ڰ� ȣ��Ǿ ������ �� �� ����.
{
	bool temp = entity->has<EntityIdentifier>();
	if (!temp)
	{
		return;
	}
	std::string entID = std::to_string(entity->getEntityId());
	auto entityName = entity->get<EntityIdentifier>()->m_EntityName;
	std::string imguiID = "entt" + entID + " " + entityName;

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
		if (ImGui::MenuItem("Make Prefab"))
		{
			m_OpenTextPopup = true;
			m_SelectionContext = entity;
		}
		ImGui::EndPopup();
	}

	DragDropEntityHierarchy(entity);


	// ��尡 �������ٸ� �ڽĵ� ���.
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

	// Entity ����
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
		// �߰��� �� �ִ� ��� ������Ʈ ����� ����ϰ� �����ϸ� �߰��Ѵ�.
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
		// �̰͵� ������..
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

		// TODO: ���� 1. PointLight �� ��

		// TODO: ���� 2. Directional Light �� ��

		/// -> ����Ʈ Ÿ�Ժ� ��Ÿ���� �ϴ� ������ �ٸ���.     �ٸ���? ����..
	});
}

