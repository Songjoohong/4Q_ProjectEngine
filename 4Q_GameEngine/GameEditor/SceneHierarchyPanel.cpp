#include "pch.h"
#include "SceneHierarchyPanel.h"

// Component Headers 
#include "../Engine/Transform.h"
#include "../Engine/Camera.h"
#include "../Engine/EntityIdentifier.h"
#include "../Engine/Light.h"
#include "../Engine/Script.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/StaticMesh.h"
#include "../Engine/Debug.h"
#include "../Engine/Movement.h"
#include "../Engine/RigidBody.h"
#include "../Engine/Sound.h"
#include "../Engine/Sprite2D.h"
#include "../Engine/UI.h"

// Script Headers
#include "../Engine/SampleScript.h"
#include "../Engine/FreeCameraScript.h"
#include "../Engine/PlayerScript.h"
#include "../Engine/POVCameraScript.h"
#include "../Engine/TestUIScript.h"

#include "Prefab.h"
#include "NameManager.h"
#include "ImGuizmo.h"

#include <cassert>
SceneHierarchyPanel::SceneHierarchyPanel(ECS::World* context)
{
	SetContext(context, m_PrefabManager, m_NameManager);
}

SceneHierarchyPanel::~SceneHierarchyPanel()
{
	std::remove("../Test/CopiedEntity.json");
}

void SceneHierarchyPanel::SetContext(ECS::World* context, std::shared_ptr<PrefabManager> prefab, std::shared_ptr<NameManager> nameManager)
{
	m_Context = context;
	m_SelectionContext = nullptr;	// ���� World���� Entity �� �ʱ�ȭ.

	m_PrefabManager = prefab;
	m_NameManager = nameManager;
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

				entity->Assign<EntityIdentifier>(entity->getEntityId());	// �⺻������ �����Ѵ�. (�̸����� ����)
				m_NameManager->AddEntityName(entity);
				entity->Assign<Transform>();	// �����Ϳ��� ������Ʈ�� ��ġ�� �����ϱ����� Transform�� �⺻������ �������ش�.
			}

			ImGui::EndPopup();
		}
	}

	ImGuiIO& io = ImGui::GetIO();
	if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_C))
	{
		m_PrefabManager->SavePrefab(m_SelectionContext, "CopiedEntity.json");
	}

	bool isFileExists = FileExists("../Test/CopiedEntity.json");
	if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_V) && isFileExists)
	{
		m_PrefabManager->LoadPrefab("CopiedEntity.json");
		m_PrefabManager->m_prefabContainer.clear();
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
				m_OpenTextPopup = false;
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				std::string prefabFile = prefabName;
				prefabFile += ".prefab";
				m_PrefabManager.get()->SavePrefab(entity, prefabFile);
				ImGui::CloseCurrentPopup();
				m_SelectionContext = nullptr;
				m_OpenTextPopup = false;
			}

			ImGui::Spacing();

			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup(); 
				m_SelectionContext = nullptr;
				m_OpenTextPopup = false;
			}
			else if (ImGui::IsKeyPressed(ImGuiKey_Escape))
			{
				ImGui::CloseCurrentPopup();
				m_SelectionContext = nullptr;
				m_OpenTextPopup = false;
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

			ECS::Entity* picked = m_Context->getById(pickedID);
			ECS::Entity* target = entity;

			// �ڱ� �ڽ��� �ڽ����� ����Ϸ��� ��� �ƹ��� ó���� ���� �ʴ´�. (�̰� ����ϸ� ��ƼƼ�� ������.)
			if (picked->isDescendant(target))
			{
				return;
			}

			//picked->get<EntityIdentifier>().get().m_ParentEntityId = target->getEntityId();
			//picked->get<EntityIdentifier>().get().m_HasParent = true;
			//target->addChild(picked);

			SetParent(picked, target);

			m_SelectionContext = nullptr;
			
		}

		ImGui::EndDragDropTarget();
	}
}

bool SceneHierarchyPanel::FileExists(const std::string& filename)
{
	std::ifstream file(filename);
	return file.good();
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
	std::string imguiID = entityName;

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
		{
			entityDeleted = true;
		}

		if (ImGui::MenuItem("Make Prefab"))
		{
			m_OpenTextPopup = true;
			m_SelectionContext = entity;
		}

		if (ImGui::MenuItem("Set Top Level Parent"))
		{
			if (entity->m_parent != nullptr)
			{
				entity->m_parent->RemoveChild(entity);
			}
		}

		ImGui::EndPopup();
	}

	DragDropEntityHierarchy(entity);


	// ��尡 �������ٸ� �ڽĵ� ���.
	if (opened)
	{
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
		DisplayAddComponentEntry<Script>("Script");
		ImGui::EndPopup();
	}

	ShowStaticModelDialog();	// TODO: ����..?

	ImGui::PopItemWidth();

	DrawComponent<EntityIdentifier>("EntityIdentifier", entity, [](auto component)
	{
			std::string entityName = "EntityName : " + component->m_EntityName;
			ImGui::Text(entityName.c_str());
			std::string entityID = "EntityID : " + std::to_string(component->m_EntityId);
			ImGui::Text(entityID.c_str());
			std::string trueOrFalse = component->m_HasParent ? "true" : "false";
			std::string HasParent = "HasParent : " + trueOrFalse;
			ImGui::Text(HasParent.c_str());
			std::string parentID = "ParentID : " + std::to_string(component->m_ParentEntityId);
			ImGui::Text(parentID.c_str());
	});

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
		switch (component->m_CollisionType)
		{
		case(0):
			ImGui::Text("CollisionType : Dynamic");
			break;
		case(1):
			ImGui::Text("CollisionType : Static");
			break;
		case(2):
			ImGui::Text("CollisionType : Plane");
			break;
		}

		switch (component->m_CollisionMask)
		{
		case(0):
			ImGui::Text("CollisionMask : Player");
			break;
		case(1):
			ImGui::Text("CollisionMask : Wall");
			break;
		case(2):
			ImGui::Text("CollisionMask : Ground");
			break;
		case(3):
			ImGui::Text("CollisionMask : Slope");
			break;
		case(4):
			ImGui::Text("CollisionMask : Object");
			break;
		case(5):
			ImGui::Text("CollisionMask : Block");
			break;
		}

		DrawVec3Control("Center", component->m_Center);
		DrawVec3Control("Size", component->m_Size);
		DrawVec3Control("Rotation", component->m_Rotation);

		std::string trueOrFalse = component->m_IsTrigger ? "true" : "false";
		std::string Trigger = "IsTrigger : " + trueOrFalse;
		ImGui::Text(Trigger.c_str());

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

	DrawComponent<Script>("Script", entity, [](auto component)
	{
		const char* scripts[] = { 
			"FreeCameraScript"
			, "SampleScript"
			, "PlayerScript"
			, "POVCameraScript"
			, "TestUIScript" };

		static int item_current = 1;
		ImGui::ListBox("ScriptList", &item_current, scripts, IM_ARRAYSIZE(scripts), 4);

		if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			if (item_current >= 0 && item_current < IM_ARRAYSIZE(scripts)) // Check if the index is valid
			{
				component->m_ComponentName = scripts[item_current]; // Assign the selected script name
			}
		}
		std::string s = "SelectedScript : " + component->m_ComponentName;

		ImGui::Text(s.c_str());
	});

	DrawComponent<Debug>("Debug", entity, [](auto component)
	{

	});

	DrawComponent<Debug>("MoveMent", entity, [](auto component)
	{

	});
}

void SceneHierarchyPanel::ShowStaticModelDialog()
{
	std::string fileName;
	std::string filePathName;
	std::string filePath;

	if (m_IsDialogOpen)
	{
		IGFD::FileDialogConfig config; config.path = "../Resource/FBXLoad_Test/fbx";
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".fbx", config);
	}

	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
		if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
			fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
			filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
			// action

			m_SelectionContext->Assign<StaticMesh>("FBXLoad_Test/fbx/" + fileName);
		}

		// close
		ImGuiFileDialog::Instance()->Close();
		m_IsDialogOpen = false;
	}
}

void SceneHierarchyPanel::SetParent(ECS::Entity* child, ECS::Entity* parent)
{
	child->get<EntityIdentifier>().get().m_ParentEntityId = parent->getEntityId();
	child->get<EntityIdentifier>().get().m_HasParent = true;


	auto matrix = child->get<Transform>().get().m_RelativeMatrix.ConvertToMatrix() * DirectX::XMMatrixInverse(nullptr, parent->get<Transform>()->m_WorldMatrix.ConvertToMatrix());

	float fTranslation[3] = { 0.0f, 0.0f, 0.0f };
	float fRotation[3] = { 0.0f, 0.0f, 0.0f };
	float fScale[3] = { 0.0f, 0.0f, 0.0f };

	ImGuizmo::DecomposeMatrixToComponents(*matrix.m, fTranslation, fRotation, fScale);

	child->get<Transform>()->m_Position = { fTranslation[0],fTranslation[1],fTranslation[2] };
	child->get<Transform>()->m_Rotation = { fRotation[0],fRotation[1],fRotation[2] };
	child->get<Transform>()->m_Scale = { fScale[0],fScale[1],fScale[2] };

	parent->addChild(child);
}

