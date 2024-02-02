#pragma once


class ContentsBrowserPanel
{
public:
	ContentsBrowserPanel();

	void RenderImGui();

private:
	std::filesystem::path m_CurrentDirectory;

};

