#include "pch.h"
#include "GameEditor.h"
#include "../D3D_Graphics/D3D_Graphics.h"
#include "../Engine/ECS.h"
#include "../Engine/WorldManager.h"

#include "ImGuizmo.h"
// Component Headers
#include "../Engine/Transform.h"
#include "../Engine/BoxCollider.h"
#include "../Engine/Camera.h"
#include "../Engine/FreeCameraScript.h"
#include "../Engine/Light.h"
#include "../Engine/EntityIdentifier.h"
#include "../Engine/Movement.h"
#include "../Engine/StaticMesh.h"
#include "../Engine/Debug.h"
#include "../Engine/Sound.h"
#include "../Engine/RenderManager.h"
#include "../Engine/Sprite2D.h"
#include "../Engine/RigidBody.h"
#include "../Engine/UI.h"
#include "../Engine/Space.h"
#include "../Engine/DynamicText.h"
#include "../Engine/PlayerInformation.h"
#include "../Engine/Interactive.h"

// Script Headers
#include "../Engine/SampleScript.h"
#include "../Engine/FreeCameraScript.h"
#include "../Engine/PlayerScript.h"
#include "../Engine/POVCameraScript.h"
#include "../Engine/TestUIScript.h"
#include "../Engine/DynamicTextScript.h"
#include "../Engine/IntroCameraScript.h"
#include "../Engine/OutroScript.h"
#include "../Engine/DrawerScript.h"
#include "../Engine/IntroDoorScript.h"
#include "../Engine/DoorScript.h"

// system Headers
#include "../Engine/MovementSystem.h"
#include "../Engine/TransformSystem.h"
#include "../Engine/CameraSystem.h"
#include "../Engine/RenderSystem.h"
#include "../Engine/ScriptSystem.h"
#include "../Engine/TimeManager.h"
#include "../Engine/CollisionSystem.h"
#include "../Engine/SpriteSystem.h"
#include "../Engine/DebugSystem.h"
#include "../Engine/UISystem.h"
#include "../Engine/SpaceSystem.h"

#include "Prefab.h"
#include "NameManager.h"
#include "../D3D_Graphics/RenderTextureClass.h"

struct DoorScript;
using json = nlohmann::json;

GameEditor::GameEditor(HINSTANCE hInstance)
	:Engine(hInstance)
{
}

GameEditor::~GameEditor()
{
	m_EditorWorld->DestroyWorld();
	ShutDownImGui();
}

bool GameEditor::Initialize(UINT width, UINT height)
{
	__super::Initialize(width, height);

	m_Width = width;
	m_Height = height;

	NewScene();

	//std::string pngPath = "../Resource/UI/play button.png";
	//auto filePath = Renderer::Instance->ConvertToWchar(pngPath);
	//CreateTextureFromFile(Renderer::Instance->m_pDevice.Get(), filePath, &m_PlayButtonTexture);
	if (!InitImGui())
	{
		return false;
	}
	return true;
}

void GameEditor::Update()
{
	__super::Update();
}

void GameEditor::Render()
{
	RenderManager::GetInstance()->RenderBegin();
	RenderManager::GetInstance()->EditorRender();
	RenderImGui();
	RenderManager::GetInstance()->RenderEnd();
}


bool GameEditor::InitImGui()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	ImFontConfig imguiFontConfig;
	imguiFontConfig.MergeMode = false;
	io.FontDefault = io.Fonts->AddFontFromFileTTF("../Resource/font/Roboto-SemiMedium.ttf", 15.0f, &imguiFontConfig, io.Fonts->GetGlyphRangesKorean());
	
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	SetDarkThemeColors();	// 유니티 한스푼 첨가.

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX11_Init(RenderManager::GetInstance()->GetRender()->m_pDevice.Get(), RenderManager::GetInstance()->GetRender()->m_pDeviceContext.Get());

	return true;
}

void GameEditor::BeginRenderImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

}

void GameEditor::RenderImGui()
{
	BeginRenderImGui();
	// Note : Swtich this to true to enable dockspace
	static bool dockingEnabled = true;
	if (dockingEnabled)
	{

		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		static bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		//// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		//	// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		//if (m_show_another_window)
		//	ImGui::ShowDemoWindow(&m_show_another_window);
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
		{
			ImGui::PopStyleVar(2);
		}

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;								// 패널 좌우 간격 최소치를 정한다. -> 사용자가 패널을 줄여서 정보가 보이지 않는 것을 방지.
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Scene...", "Ctrl+O"))		// 단축키 설정은 아직 안함.
				{
					m_IsDialogOpen = true;
				}

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					NewScene();
				}

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					SaveWorld(m_SceneName);	// 현재 월드의 정보를 세이브
				}

				if (ImGui::MenuItem("Save Scene As..."))	// 다른 이름으로 월드를 저장
				{
					m_isScenePopup = true;
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					Close();
				}

				ImGui::EndMenu();
			}

			// Display Play & Pause Button 
			PlayButton();


			ImGui::EndMenuBar();
		}

		HandleShortcuts();

		m_SceneHierarchyPanel.RenderImGui();
		m_ContentsBrowserPanel.RenderImGui();

		ShowSceneDialog();
		ShowSaveSceneAsPopup();

#ifdef _DEBUG
		ImGui::ShowDemoWindow();
#endif
		ImGui::End();

		/* Viewport ------------------------------------------------------------------------ */
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });	// 패딩 제거
		ImGui::Begin("Viewport");
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		ID3D11ShaderResourceView* myViewportTexture = RenderManager::GetInstance()->GetRender()->m_RenderTexture->GetShaderResourceView();

		// Calculate the aspect ratio of the image and the content region
		float imageAspectRatio = (float)m_Width / (float)m_Height;
		float contentRegionAspectRatio = viewportPanelSize.x / viewportPanelSize.y;

		// Scale the image horizontally if the content region is wider than the image
		if (contentRegionAspectRatio > imageAspectRatio)
		{
			float imageWidth = viewportPanelSize.y * imageAspectRatio;
			float xPadding = (viewportPanelSize.x - imageWidth) / 2;
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + xPadding);
			ImGui::Image((void*)myViewportTexture, ImVec2{ imageWidth, viewportPanelSize.y });
		}
		// Scale the image vertically if the content region is taller than the image
		else
		{
			float imageHeight = viewportPanelSize.x / imageAspectRatio;
			float yPadding = (viewportPanelSize.y - imageHeight) / 2;
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + yPadding);
			ImGui::Image((ImTextureID)(intptr_t)myViewportTexture, ImVec2(viewportPanelSize.x, imageHeight));
		}

		//ImGui::Image((void*)myViewportTexture, ImVec2{ viewportPanelSize.x, viewportPanelSize.y });
		/* Viewport end ------------------------------------------------------------------------ */


		//Entity* selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_Z))
		{
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;
			m_CurrentSnapMode = &m_TranslationSnapValue;
			TranslationSnapValue = 10.0f;
		}
		else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_X))
		{
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;
			m_CurrentSnapMode = &m_RotationSnapValue;
		}
		else if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_C))
		{
			m_GizmoType = ImGuizmo::OPERATION::SCALE;
			m_CurrentSnapMode = &m_ScaleSnapValue;
		}


		// Camera
		DirectX::XMMATRIX viewMatrix = RenderManager::GetInstance()->GetRender()->GetViewMatrix();
		auto floatViewMatrix = reinterpret_cast<const float*>(&viewMatrix);

		DirectX::XMMATRIX projectionMatrix = RenderManager::GetInstance()->GetRender()->GetProjectionMatrix();
		auto floatProjectionMatrix = reinterpret_cast<const float*>(&projectionMatrix);

		Entity* selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();

		// Gizmos
		if (selectedEntity)
		{
			ImGuizmo::SetOrthographic(false);
			ImGuizmo::SetDrawlist();
			float windowWidth = (float)ImGui::GetWindowWidth();
			float windowHeight = (float)ImGui::GetWindowHeight();
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);

			// Entity Transform
			auto& tc = selectedEntity->get<Transform>().get();
			auto myMatrix = tc.m_WorldMatrix;



			bool snap = InputManager::GetInstance()->GetKey(Key::CTRL);
			//std::cout << snap << std::endl;

			if (ImGuizmo::Manipulate(floatViewMatrix, floatProjectionMatrix, (ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, &myMatrix.m_11, nullptr, snap ? &m_CurrentSnapMode->m_X : nullptr))
			{
				if (ImGuizmo::IsUsing())
				{
					if (selectedEntity->get<EntityIdentifier>()->m_HasParent == true)
					{
						myMatrix = myMatrix.ConvertToMatrix() * DirectX::XMMatrixInverse(nullptr, selectedEntity->getParent()->get<Transform>()->m_WorldMatrix.ConvertToMatrix());
					}

					float fTranslation[3] = { 0.0f, 0.0f, 0.0f };
					float fRotation[3] = { 0.0f, 0.0f, 0.0f };
					float fScale[3] = { 0.0f, 0.0f, 0.0f };

					ImGuizmo::DecomposeMatrixToComponents(&myMatrix.m_11, fTranslation, fRotation, fScale);

					if (m_GizmoType == ImGuizmo::OPERATION::TRANSLATE)
					{
						tc.m_Position = DirectX::XMFLOAT3(fTranslation);
					}
					else if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
					{
						// Convert degrees to radians for quaternion creation
						float yawRadians = DirectX::XMConvertToRadians(fRotation[1]);  // Yaw
						float pitchRadians = DirectX::XMConvertToRadians(fRotation[0]); // Pitch
						float rollRadians = DirectX::XMConvertToRadians(fRotation[2]);  // Roll

						// Create a quaternion from Euler angles (in radians)
						DirectX::SimpleMath::Quaternion quaternion = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(yawRadians, pitchRadians, rollRadians);

						// Convert radians back to degrees for storing in tc.m_Rotation
						float yawDegrees = DirectX::XMConvertToDegrees(yawRadians);
						float pitchDegrees = DirectX::XMConvertToDegrees(pitchRadians);
						float rollDegrees = DirectX::XMConvertToDegrees(rollRadians);

						/*tc.m_Rotation.SetX(fRotation[0]);
						tc.m_Rotation.SetY(fRotation[1]);
						tc.m_Rotation.SetZ(fRotation[2]);*/
						tc.m_Rotation.SetX(pitchDegrees);
						tc.m_Rotation.SetY(yawDegrees);
						tc.m_Rotation.SetZ(rollDegrees);

					}
					else if (m_GizmoType == ImGuizmo::OPERATION::SCALE)
					{
						tc.m_Scale = DirectX::XMFLOAT3(fScale);
					}
			}

			
			}
		}

		ImGui::End();

		ImGui::PopStyleVar();

		ImGui::Begin("Settings");

		intTransSnapValue = static_cast<int>(m_TranslationSnapValue.m_X);
		ImGui::SliderInt("Translation SnapVale", &intTransSnapValue, 0.0f, 100.f);
		m_TranslationSnapValue.m_X = static_cast<float>(intTransSnapValue);
		m_TranslationSnapValue.m_Y = static_cast<float>(intTransSnapValue);
		m_TranslationSnapValue.m_Z = static_cast<float>(intTransSnapValue);

		intRotSnapValue = static_cast<int>(m_RotationSnapValue.m_X);
		ImGui::SliderInt("Rotation SnapVale", &intRotSnapValue, 0.0f, 100.f);
		m_RotationSnapValue.m_X = static_cast<float>(intRotSnapValue);
		m_RotationSnapValue.m_Y = static_cast<float>(intRotSnapValue);
		m_RotationSnapValue.m_Z = static_cast<float>(intRotSnapValue);

		intScaleSnapValue = static_cast<int>(m_ScaleSnapValue.m_X);
		ImGui::SliderInt("Scale SnapVale", &intScaleSnapValue, 0.0f, 100.f);
		m_ScaleSnapValue.m_X = static_cast<float>(intScaleSnapValue);
		m_ScaleSnapValue.m_Y = static_cast<float>(intScaleSnapValue);
		m_ScaleSnapValue.m_Z = static_cast<float>(intScaleSnapValue);

		ImGui::Text("Current Scene :");
		ImGui::SameLine();
		ImGui::Text(m_SceneName.c_str());		// 에디터에 현재 화면에 표시중인 씬 정보 표기하기

		ImGui::Text("Entity Count : ");
		ImGui::SameLine();

		if (m_IsPlaying)
		{
			ImGui::Text(+std::to_string(m_ActiveWorld->GetEntities().size()).c_str());	// 현재 씬에 있는 오브젝트 갯수
		}
		else
		{
			ImGui::Text(+std::to_string(m_EditorWorld->GetEntities().size()).c_str());	// 현재 씬에 있는 오브젝트 갯수
		}

		std::string videoMemoryInfo;
		RenderManager::GetInstance()->GetRender()->GetVideoMemoryInfo(videoMemoryInfo);
		ImGui::Text(videoMemoryInfo.c_str());

		std::string systemMemoryInfo;
		RenderManager::GetInstance()->GetRender()->GetSystemMemoryInfo(systemMemoryInfo);
		ImGui::Text(systemMemoryInfo.c_str());

		float FPS = TimeManager::GetInstance()->GetFPS();
		std::string framePerSecond = "Frame per Second : " + std::to_string(FPS);
		ImGui::Text(framePerSecond.c_str());

		std::string mousePos = "Mouse Position x : " + std::to_string(InputManager::GetInstance()->GetMousePos().x) + " y : " + std::to_string(InputManager::GetInstance()->GetMousePos().y);
		ImGui::Text(mousePos.c_str());

		ImGui::End();
	}
	else
	{
		static bool show = true;
		ImGui::ShowDemoWindow();
	}

	EndRenderImGui();
}

void GameEditor::EndRenderImGui()
{
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(m_ClientWidth, m_ClientHeight);

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void GameEditor::ShutDownImGui()
{
	// Cleanup
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void GameEditor::SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Frame BG
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}


// jsonFile 이름 넘기기
void GameEditor::SaveWorld(const std::string& _filename)
{
	std::string fullPath = basePath + "/scene/" + _filename + ".scene";

	std::ofstream outputFile(fullPath);

	json worldData;
	for (const auto& entity : m_EditorWorld->GetEntities())
	{
		SaveComponents<EntityIdentifier>(entity, worldData);
		SaveComponents<Transform>(entity, worldData);
		SaveComponents<StaticMesh>(entity, worldData);
		SaveComponents<BoxCollider>(entity, worldData);
		SaveComponents<Camera>(entity, worldData);
		SaveComponents<Light>(entity, worldData);
		SaveComponents<Movement>(entity, worldData);
		SaveComponents<Debug>(entity, worldData);
		SaveComponents<Sound>(entity, worldData);
		SaveComponents<Sprite2D>(entity, worldData);
		SaveComponents<Script>(entity, worldData);
		SaveComponents<RigidBody>(entity, worldData);
		SaveComponents<UI>(entity, worldData);
		SaveComponents<Space>(entity, worldData);
		SaveComponents<DynamicText>(entity, worldData);
		SaveComponents<PlayerInformation>(entity, worldData);
	}

	outputFile << std::setw(4) << worldData << std::endl;

	outputFile.close();

}

void GameEditor::LoadWorld(const std::string& fileName)
{
	// 월드 생성
	m_EditorWorld = ECS::World::CreateWorld(fileName);
	WorldManager::GetInstance()->ChangeWorld(m_EditorWorld);
	m_NameManager->ClearContainer();

	// 시스템 등록
	m_EditorWorld->registerSystem(new RenderSystem);
	m_EditorWorld->registerSystem(new TransformSystem);
	m_EditorWorld->registerSystem(new MovementSystem);
	m_EditorWorld->registerSystem(new CameraSystem);
	m_EditorWorld->registerSystem(new ScriptSystem);
	m_EditorWorld->registerSystem(new CollisionSystem);
	m_EditorWorld->registerSystem(new SpriteSystem);

	m_EditorWorld->registerSystem(new DebugSystem);
	m_EditorWorld->registerSystem(new class UISystem);
	m_EditorWorld->registerSystem(new SpaceSystem);

	Deserialize(m_EditorWorld, fileName);
}

void GameEditor::ShowSceneDialog()
{
	std::string fileName;

	if (m_IsDialogOpen)
	{
		IGFD::FileDialogConfig config; config.path = "../Resource/scene";
		ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".scene", config);
	}

	// display
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
		if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
			fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
			// action

			// 현재 에디터가 화면에 띄우고 있는 월드의 이름을 변경
			LoadWorld("scene/" + fileName);	// TODO : ??

			// ".scene" 문자열을 찾습니다.
			size_t found = fileName.find(".scene");

			// 만약 ".scene" 문자열이 발견되었다면 해당 부분을 제거합니다.
			if (found != std::string::npos) {
				fileName.erase(found, 6); // ".scene" 문자열은 6개의 문자로 이루어져 있습니다.
			}

			m_SceneName = fileName;
		}

		// close
		ImGuiFileDialog::Instance()->Close();
		m_IsDialogOpen = false;
	}
}

void GameEditor::ShowSaveSceneAsPopup()
{
	if (m_isScenePopup)
	{
		ImGui::SetNextWindowSize(ImVec2(320, 120));
		ImGui::OpenPopup("Scene Name");
		if (ImGui::BeginPopupModal("Scene Name"))
		{
			static char sceneName[256] = ""; // Fixed-size buffer for input

			ImGui::InputText("Scene Name", sceneName, sizeof(sceneName));
			ImGui::Spacing();

			if (ImGui::Button("Submit") || ImGui::IsKeyPressed(ImGuiKey_Enter))
			{
				// 씬 저장.
				SaveWorld(sceneName);

				m_isScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::Spacing();

			if (ImGui::Button("Close") || ImGui::IsKeyPressed(ImGuiKey_Escape))
			{
				m_isScenePopup = false;
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}

void GameEditor::PlayDeserialize(ECS::World* currentWorld, const std::string& _filename)
{
	std::string fullPath = basePath + _filename;

	// Deserialize
	std::ifstream inputFile(fullPath);
	json jsonObject;
	inputFile >> jsonObject;
	inputFile.close();

	for (const auto& entity : jsonObject["WorldEntities"])
	{
		for (auto it = entity.begin(); it != entity.end(); ++it)
		{
			Entity* playEntity = currentWorld->create();
			int oldID = 0;
			for (const auto& component : it.value())
			{
				std::string componentName = component.begin().key();
				if (componentName == "EntityIdentifier")
				{
					playEntity->Assign<EntityIdentifier>();
					oldID = component["EntityIdentifier"][0]["m_EntityId"];
					playEntity->get<EntityIdentifier>()->m_ComponentName = component["EntityIdentifier"][0]["m_ComponentName"];
					playEntity->get<EntityIdentifier>()->m_EntityName = component["EntityIdentifier"][0]["m_EntityName"];
					playEntity->get<EntityIdentifier>()->m_HasParent = component["EntityIdentifier"][0]["m_HasParent"];
					playEntity->get<EntityIdentifier>()->m_ParentEntityId = component["EntityIdentifier"][0]["m_ParentEntityId"];
					playEntity->get<EntityIdentifier>()->m_EntityId = playEntity->getEntityId();
				}
				else if (componentName == "Transform")
				{
					m_PrefabManager->AssignComponents<Transform>(playEntity, component["Transform"][0]);
				}
				else if (componentName == "BoxCollider")
				{
					m_PrefabManager->AssignComponents<BoxCollider>(playEntity, component["BoxCollider"][0]);
				}

				else if (componentName == "Camera")
				{
					m_PrefabManager->AssignComponents<Camera>(playEntity, component["Camera"][0]);
				}

				else if (componentName == "Light")
				{
					m_PrefabManager->AssignComponents<Light>(playEntity, component["Light"][0]);
				}

				else if (componentName == "Movement")
				{
					m_PrefabManager->AssignComponents<Movement>(playEntity, component["Movement"][0]);
				}

				else if (componentName == "StaticMesh")
				{
					std::string fileName = component["StaticMesh"][0]["m_FileName"];
					playEntity->Assign<StaticMesh>(fileName);
					playEntity->get<StaticMesh>().get().m_ComponentName = component["StaticMesh"][0]["m_ComponentName"];
					playEntity->get<StaticMesh>().get().m_FileName = component["StaticMesh"][0]["m_FileName"];
					playEntity->get<StaticMesh>().get().m_IsModelCreated = component["StaticMesh"][0]["m_IsModelCreated"];
				}
				else if (componentName == "Debug")
				{
					m_PrefabManager->AssignComponents<Debug>(playEntity, component["Debug"][0]);
				}
				else if (componentName == "Sound")
				{
					m_PrefabManager->AssignComponents<Sound>(playEntity, component["Sound"][0]);
				}
				else if (componentName == "RigidBody")
				{
					m_PrefabManager->AssignComponents<RigidBody>(playEntity, component["RigidBody"][0]);
				}
				else if (componentName == "UI")
				{
					m_PrefabManager->AssignComponents<UI>(playEntity, component["UI"][0]);
				}
				else if (componentName == "Space")
				{
					m_PrefabManager->AssignComponents<Space>(playEntity, component["Space"][0]);
				}
				else if (componentName == "DynamicText")
				{
					m_PrefabManager->AssignComponents<DynamicText>(playEntity, component["DynamicText"][0]);
				}
				else if (componentName == "Sprite2D")
				{
					m_PrefabManager->AssignComponents<Sprite2D>(playEntity, component["Sprite2D"][0]);
				}
				else if (componentName == "PlayerInformation")
				{
					m_PrefabManager->AssignComponents<PlayerInformation>(playEntity, component["PlayerInformation"][0]);
				}
				else if (componentName == "Interactive")
				{
					m_PrefabManager->AssignComponents<Interactive>(playEntity, component["Interactive"][0]);
				}
				else if (componentName == "Script")
				{
					if (component["Script"][0]["m_ComponentName"].get<std::string>() == "FreeCameraScript")
					{
						m_PrefabManager->AssignComponents<FreeCameraScript>(playEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "SampleScript")
					{
						m_PrefabManager->AssignComponents<SampleScript>(playEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "PlayerScript")
					{
						m_PrefabManager->AssignComponents<PlayerScript>(playEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "POVCameraScript")
					{
						m_PrefabManager->AssignComponents<POVCameraScript>(playEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "TestUIScript")
					{
						m_PrefabManager->AssignComponents<TestUIScript>(playEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "DynamicTextScript")
					{
						m_PrefabManager->AssignComponents<DynamicTextScript>(playEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "IntroCameraScript")
					{
						m_PrefabManager->AssignComponents<IntroCameraScript>(playEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "OutroScript")
					{
						m_PrefabManager->AssignComponents<OutroScript>(playEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "DrawerScript")
					{
						m_PrefabManager->AssignComponents<DrawerScript>(playEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "IntroDoorScript")
					{
						m_PrefabManager->AssignComponents<IntroDoorScript>(playEntity, component["Script"][0]);
					}
					else if (component["Script"][0]["m_ComponentName"].get<std::string>() == "DoorScript")
					{
						m_PrefabManager->AssignComponents<DoorScript>(playEntity, component["Script"][0]);
					}
					//요기
				}
			}
			m_PrefabManager->m_prefabContainer.push_back({ playEntity, oldID });
		}
	}


	for (const auto& playChild : m_PrefabManager->m_prefabContainer)
	{
		for (const auto& playParent : m_PrefabManager->m_prefabContainer)
		{
			if (playChild.first->get<EntityIdentifier>().get().m_HasParent == true)
			{
				if (playChild.first->get<EntityIdentifier>().get().m_ParentEntityId == playParent.second)
				{
					SetParent(playChild.first, playParent.first);
				}
			}
		}
	}

	for (const auto& prefab : m_PrefabManager->m_prefabContainer)
	{
		m_NameManager->AddEntityName(prefab.first);
	}

	m_PrefabManager->m_prefabContainer.clear();
}

void GameEditor::Deserialize(ECS::World* currentWorld, const std::string& fileName)
{
	std::string fullPath = basePath + fileName;

	// Deserialize
	std::ifstream inputFile(fullPath);
	json jsonObject;
	inputFile >> jsonObject;
	inputFile.close();

	for (const auto& entity : jsonObject["WorldEntities"])
	{
		for (auto it = entity.begin(); it != entity.end(); ++it)
		{
			Entity* loadEntity = currentWorld->create();
			int oldID = 0;
			for (const auto& component : it.value())
			{
				std::string componentName = component.begin().key();
				if (componentName == "EntityIdentifier")
				{
					loadEntity->Assign<EntityIdentifier>();
					oldID = component["EntityIdentifier"][0]["m_EntityId"];
					loadEntity->get<EntityIdentifier>()->m_ComponentName = component["EntityIdentifier"][0]["m_ComponentName"];
					loadEntity->get<EntityIdentifier>()->m_EntityName = component["EntityIdentifier"][0]["m_EntityName"];
					loadEntity->get<EntityIdentifier>()->m_HasParent = component["EntityIdentifier"][0]["m_HasParent"];
					loadEntity->get<EntityIdentifier>()->m_ParentEntityId = component["EntityIdentifier"][0]["m_ParentEntityId"];
					loadEntity->get<EntityIdentifier>()->m_EntityId = loadEntity->getEntityId();
				}
				else if (componentName == "Transform")
				{
					AssignComponents<Transform>(loadEntity, component["Transform"][0]);
				}
				else if (componentName == "BoxCollider")
				{
					AssignComponents<BoxCollider>(loadEntity, component["BoxCollider"][0]);
				}

				else if (componentName == "Camera")
				{
					AssignComponents<Camera>(loadEntity, component["Camera"][0]);
				}

				else if (componentName == "Light")
				{
					AssignComponents<Light>(loadEntity, component["Light"][0]);
				}

				else if (componentName == "Movement")
				{
					AssignComponents<Movement>(loadEntity, component["Movement"][0]);
				}

				else if (componentName == "StaticMesh")
				{
					std::string fileName = component["StaticMesh"][0]["m_FileName"];
					loadEntity->Assign<StaticMesh>(fileName);
					loadEntity->get<StaticMesh>().get().m_ComponentName = component["StaticMesh"][0]["m_ComponentName"];
					loadEntity->get<StaticMesh>().get().m_FileName = component["StaticMesh"][0]["m_FileName"];
					loadEntity->get<StaticMesh>().get().m_IsModelCreated = component["StaticMesh"][0]["m_IsModelCreated"];
				}
				else if (componentName == "Debug")
				{
					AssignComponents<Debug>(loadEntity, component["Debug"][0]);
				}
				else if (componentName == "Sound")
				{
					AssignComponents<Sound>(loadEntity, component["Sound"][0]);
				}
				else if (componentName == "RigidBody")
				{
					AssignComponents<RigidBody>(loadEntity, component["RigidBody"][0]);
				}
				else if (componentName == "UI")
				{
					AssignComponents<UI>(loadEntity, component["UI"][0]);
				}
				else if (componentName == "Space")
				{
					AssignComponents<Space>(loadEntity, component["Space"][0]);
				}
				else if (componentName == "DynamicText")
				{
					AssignComponents<DynamicText>(loadEntity, component["DynamicText"][0]);
				}
				else if (componentName == "Sprite2D")
				{
					AssignComponents<Sprite2D>(loadEntity, component["Sprite2D"][0]);
				}
				else if (componentName == "PlayerInformation")
				{
					AssignComponents<PlayerInformation>(loadEntity, component["PlayerInformation"][0]);
				}
				else if (componentName == "Interactive")
				{
					AssignComponents<Interactive>(loadEntity, component["Interactive"][0]);
				}
				else if (componentName == "Script")
				{
					AssignComponents<Script>(loadEntity, component["Script"][0]);
				}
			}
			m_LoadEntityContainer.push_back({ loadEntity, oldID });
		}
	}


	for (const auto& LoadEntityChild : m_LoadEntityContainer)
	{
		for (const auto& LoadEntityParent : m_LoadEntityContainer)
		{
			if (LoadEntityChild.first->get<EntityIdentifier>().get().m_HasParent == true)
			{
				if (LoadEntityChild.first->get<EntityIdentifier>().get().m_ParentEntityId == LoadEntityParent.second)
				{
					SetParent(LoadEntityChild.first, LoadEntityParent.first);
				}
			}
		}
	}

	for (const auto& prefab : m_LoadEntityContainer)
	{
		m_NameManager->AddEntityName(prefab.first);
	}

	m_LoadEntityContainer.clear();
	// HierarchyPanel에 등록
	m_SceneHierarchyPanel.SetContext(currentWorld, m_PrefabManager, m_NameManager);
	m_ContentsBrowserPanel.SetContext(currentWorld, m_PrefabManager, m_NameManager);
}

void GameEditor::PlayButton()
{
	// 중간에 버튼 하나를 배치할것이다.
	ImGui::SameLine(ImGui::GetWindowWidth() / 2 - 63);
	if (m_IsPlaying)
	{
		if (ImGui::Button("Stop", ImVec2(60.0f, 0.0f)))
		{
			m_IsPlaying = false;

			WorldManager::GetInstance()->ChangeWorld(m_EditorWorld);

			m_SceneHierarchyPanel.SetContext(m_EditorWorld, m_PrefabManager, m_NameManager);
			m_ContentsBrowserPanel.SetContext(m_EditorWorld, m_PrefabManager, m_NameManager);

			m_ActiveWorld->DestroyWorld();
		}
	}
	else
	{
		if (ImGui::Button("Play", ImVec2(60.0f, 0.0f)))
		{
			m_IsPlaying = true;
			PlayScene();
		}
	}
}

void GameEditor::HandleShortcuts()
{
	ImGuiIO& io = ImGui::GetIO();

	// Save Scene, Ctrl + S
	if (io.KeyCtrl && ImGui::IsKeyDown(ImGuiKey_S))		
		SaveWorld(m_SceneName);

	// New Scene, Ctrl + N
	if (io.KeyCtrl && ImGui::IsKeyDown(ImGuiKey_N))
		NewScene();

	// Open Scene, Ctrl + O
	if (io.KeyCtrl && ImGui::IsKeyDown(ImGuiKey_O))
		m_IsDialogOpen = true;

	// Delete Entity, Delete
	if (ImGui::IsKeyDown(ImGuiKey_Delete) && m_SceneHierarchyPanel.GetSelectedEntity() != nullptr)
		m_SceneHierarchyPanel.DeleteSelectedEntity();
}

void GameEditor::NewScene()
{
	m_SceneName = "NewScene";	// 씬 이름 기본 설정

	m_EditorWorld = World::CreateWorld("../Resource/scene/" + m_SceneName + ".scene");
	WorldManager::GetInstance()->ChangeWorld(m_EditorWorld);

	// 시스템 등록
	m_EditorWorld->registerSystem(new RenderSystem);
	m_EditorWorld->registerSystem(new TransformSystem);
	m_EditorWorld->registerSystem(new MovementSystem);
	m_EditorWorld->registerSystem(new CameraSystem);
	m_EditorWorld->registerSystem(new ScriptSystem);
	m_EditorWorld->registerSystem(new CollisionSystem);
	m_EditorWorld->registerSystem(new SpriteSystem);
	//m_EditorWorld->registerSystem(new DebugSystem);
	m_EditorWorld->registerSystem(new class UISystem);
	m_EditorWorld->registerSystem(new SpaceSystem);

	// Scene 새로 불러올 때 원래 이름값들 초기화

	m_NameManager = std::make_shared<NameManager>();
	m_PrefabManager = std::make_shared<PrefabManager>(m_EditorWorld, m_NameManager);

	m_NameManager->ClearContainer();

	// Panel들 등록
	m_SceneHierarchyPanel.SetContext(m_EditorWorld, m_PrefabManager, m_NameManager);
	m_ContentsBrowserPanel.SetContext(m_EditorWorld, m_PrefabManager, m_NameManager);
	m_ContentsBrowserPanel.Initialize();

	Vector3D pos1 = { 1.0f, 3.0f, 5.0f };
	Vector3D rot = { 0.0f, 0.0f, 0.0f };
	Vector3D scale = { 1000.0f, 1000.0f, 1000.0f };
	Vector3D pos2 = { 10.0f, 30.0f, 50.0f };
	Vector3D pos3 = { 100.0f, 300.0f, 500.0f };
	Vector3D posPlayer = { 1.0f, 1.0f, 1.0f };

	// Player
	Entity* player = m_EditorWorld->create();
	player->Assign<EntityIdentifier>(player->getEntityId(), "Player");
	player->Assign<Transform>(posPlayer);
	player->Assign<Movement>();
	player->Assign<RigidBody>();
	player->Assign<BoxCollider>();

	// PlayerCamera
	Entity* playerCamera = m_EditorWorld->create();
	playerCamera->Assign<EntityIdentifier>(playerCamera->getEntityId(), "PlayerCamera");
	playerCamera->Assign<Transform>(posPlayer);
	playerCamera->Assign<Camera>();
	playerCamera->Assign<Movement>();

	SetParent(playerCamera, player);
	SetParentTransform(playerCamera, player);

	//Free Camera
	Entity* ent = m_EditorWorld->create();
	ent->Assign<EntityIdentifier>(ent->getEntityId(), "Main Camera");
	ent->Assign<Transform>(Vector3D(0.f, 10.f, 0.f), Vector3D{ 0.f,0.f,0.f });
	//ent->Assign<Debug>();
	ent->Assign<Camera>();
	ent->Assign<FreeCameraScript>(ent);
	ent->get<Script>()->m_ComponentName = "FreeCameraScript";
	ent->get<Script>()->m_IsFreeCamera = true;
	ent->Assign<Movement>();

	// for test
#ifdef _DEBUG
	{
		Entity* ent2 = WorldManager::GetInstance()->GetCurrentWorld()->create();
		ent2->Assign<EntityIdentifier>(ent2->getEntityId(), "Test UI");
		ent2->Assign<Transform>(Vector3D(0.f, 10.f, 0.f), Vector3D{ 0.f,0.f,0.f });
		ent2->Assign<UI>(100, 100);
		ent2->Assign<Sprite2D>("../Resource/UI/image.jpg", 0, 100, 100);
		ent2->Assign<TestUIScript>(ent2);
	}
#endif
	// for test 2
	//Entity* ent2 = WorldManager::GetInstance()->GetCurrentWorld()->create();
	//ent2->Assign<EntityIdentifier>(ent2->getEntityId(), "Test Outro");
	//ent2->Assign<Transform>(Vector3D(0.f, 10.f, 0.f), Vector3D{ 0.f,0.f,0.f });
	//ent2->Assign<Sprite2D>("../Resource/UI/cutscene_long.jpg", 0, 100, 100);
	//ent2->Assign<OutroScript>(ent2);
	//ent->get<Script>()->m_ComponentName = "OutroScript";


	for (const auto& entity : m_EditorWorld->GetEntities())
	{
		m_NameManager->AddEntityName(entity);
	}
}

void GameEditor::PlayScene()
{

	m_ActiveWorld = ECS::World::CreateWorld("scene/" + m_SceneName + ".scene");
	WorldManager::GetInstance()->ChangeWorld(m_ActiveWorld);

	// 시스템 등록
	m_ActiveWorld->registerSystem(new RenderSystem);
	m_ActiveWorld->registerSystem(new TransformSystem);
	m_ActiveWorld->registerSystem(new MovementSystem);
	m_ActiveWorld->registerSystem(new CameraSystem);
	m_ActiveWorld->registerSystem(new ScriptSystem);
	m_ActiveWorld->registerSystem(new CollisionSystem);
	m_ActiveWorld->registerSystem(new SpriteSystem);
	m_ActiveWorld->registerSystem(new DebugSystem);
	m_ActiveWorld->registerSystem(new class UISystem);
	m_ActiveWorld->registerSystem(new SpaceSystem);

	if (m_IsPlaying)
	{
		SaveWorld(m_SceneName);
	}

	m_SceneHierarchyPanel.SetContext(m_ActiveWorld, m_PrefabManager, m_NameManager);
	m_ContentsBrowserPanel.SetContext(m_ActiveWorld, m_PrefabManager, m_NameManager);

	m_NameManager->ClearContainer();


	m_ActiveWorld->ResetLastEntityId();

	PlayDeserialize(m_ActiveWorld, "scene/" + m_SceneName + ".scene");

	for (const auto& entity : m_ActiveWorld->GetEntities())
	{
		if (entity->get<EntityIdentifier>()->m_EntityName == "Main Camera")
		{
			m_ActiveWorld->destroy(entity);
		}
	}
}

void GameEditor::SetParent(ECS::Entity* child, ECS::Entity* parent)
{
	child->get<EntityIdentifier>().get().m_ParentEntityId = parent->getEntityId();
	child->get<EntityIdentifier>().get().m_HasParent = true;

	parent->addChild(child);
}

void GameEditor::SetParentTransform(ECS::Entity* child, ECS::Entity* parent)
{
	auto matrix = child->get<Transform>().get().m_RelativeMatrix.ConvertToMatrix() * DirectX::XMMatrixInverse(nullptr, parent->get<Transform>()->m_WorldMatrix.ConvertToMatrix());

	float fTranslation[3] = { 0.0f, 0.0f, 0.0f };
	float fRotation[3] = { 0.0f, 0.0f, 0.0f };
	float fScale[3] = { 0.0f, 0.0f, 0.0f };

	ImGuizmo::DecomposeMatrixToComponents(*matrix.m, fTranslation, fRotation, fScale);

	child->get<Transform>()->m_Position = { fTranslation[0],fTranslation[1],fTranslation[2] };
	child->get<Transform>()->m_Rotation = { fRotation[0],fRotation[1],fRotation[2] };
	child->get<Transform>()->m_Scale = { fScale[0],fScale[1],fScale[2] };
}
