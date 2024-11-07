#include "menu.h"
#include "windows.h"

#include "../Common/Imgui/imgui.h"
#include "../Common/Imgui/ImGuiFileDialog.h"
#include "../Common/Imgui/TextEditor.h"
#include "../Common/Imgui/imgui_impl_dx9.h"

TextEditor editor;

bool Menu::IG::initialised = false;
bool Menu::IG::opened = false;
bool linitialised = false;
bool savede = false;
bool loade = false;

void Menu::IG::DrawTextEditor()
{
	ImGui::GetIO().MouseDrawCursor = 0;
	if (!opened)
		return;

	if (!linitialised) {

		auto lang = TextEditor::LanguageDefinition::Lua();

		editor.SetLanguageDefinition(lang);
		//if (dirExists("C:/Appel"))
		//{
		//	if (fileExists("C:/Appel/appel_1.lua"))
		//		editor.SetText(openFile("C:/Appel/appel_1.lua"));
		//}
		linitialised = true;
	}

	auto cpos = editor.GetCursorPosition();
	if (ImGui::Begin("Appel Lua Editor", nullptr, ImVec2(500, 500), 0.8f, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoCollapse)) {

		ImGui::GetIO().MouseDrawCursor = 1;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Clear"))
				{
					editor.SetText("");
				}
				if (ImGui::MenuItem("Quit", "Alt-F4"))
				{
					opened = false;
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Save"))
			{
				if (ImGui::MenuItem("Save as"))
				{
					savede = true;
				}
				ImGui::Spacing();
				if (ImGui::MenuItem("Save in slot 1"))
				{
					auto textToSave = editor.GetText();
					//if (createDir("C:/Appel"))
					//	createFile("C:/Appel/appel_1.lua", textToSave);
				}
				if (ImGui::MenuItem("Save in slot 2"))
				{
					auto textToSave = editor.GetText();
					//if (createDir("C:/Appel"))
					//	createFile("C:/Appel/appel_2.lua", textToSave);
				}
				if (ImGui::MenuItem("Save in slot 3"))
				{
					auto textToSave = editor.GetText();
					//if (createDir("C:/Appel"))
					//	createFile("C:/Appel/appel_3.lua", textToSave);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Load"))
			{
				if (ImGui::MenuItem("Load"))
				{
					loade = true;
				}
				ImGui::Spacing();
				if (ImGui::MenuItem("Load slot 1"))
				{
					//if (dirExists("C:/Appel"))
					//{
					//	if (fileExists("C:/Appel/appel_1.lua"))
					//		editor.SetText(openFile("C:/Appel/appel_1.lua"));
					//}
				}
				if (ImGui::MenuItem("Load slot 2"))
				{
					//if (dirExists("C:/Appel"))
					//{
					//	if (fileExists("C:/Appel/appel_2.lua"))
					//		editor.SetText(openFile("C:/Appel/appel_2.lua"));
					//}
				}
				if (ImGui::MenuItem("Load slot 3"))
				{
					//if (dirExists("C:/Appel"))
					//{
					//	if (fileExists("C:/Appel/appel_3.lua"))
					//		editor.SetText(openFile("C:/Appel/appel_3.lua"));
					//}
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				bool ro = editor.IsReadOnly();
				if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
					editor.SetReadOnly(ro);
				ImGui::Separator();

				if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
					editor.Undo();
				if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
					editor.Redo();

				ImGui::Separator();

				if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
					editor.Copy();
				if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
					editor.Cut();
				if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
					editor.Delete();
				if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
					editor.Paste();

				ImGui::Separator();

				if (ImGui::MenuItem("Select all", nullptr, nullptr))
					editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

				ImGui::EndMenu();
			}

			//if (ImGui::BeginMenu("View"))
			//{
			//	if (ImGui::MenuItem("Dark palette"))
			//		editor.SetPalette(TextEditor::GetDarkPalette());
			//	if (ImGui::MenuItem("Light palette"))
			//		editor.SetPalette(TextEditor::GetLightPalette());
			//	if (ImGui::MenuItem("Retro blue palette"))
			//		editor.SetPalette(TextEditor::GetRetroBluePalette());
			//	ImGui::EndMenu();
			//}
			ImGui::EndMenuBar();
		}
		if (ImGui::Button("Run"))
		{
			const char* code = editor.GetText().c_str();

			//HackVars::luascript = code;
			//HackVars::runlua = true;
		}
		ImGui::SameLine();
		if (ImGui::Button("Run Menu"))
		{
			const char* code = editor.GetText().c_str();

			//AppelLua::RunLua(code, true);
		}
		ImGui::SameLine();
		if (ImGui::Button("Run Before Autorun"))
		{
			auto textToSave = editor.GetText();
			//RunBeforeAutorun(textToSave);
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear Before Autorun script"))
		{
			//ClearBeforeAutorunScript();
		}
		ImGui::SameLine();
		ImGui::Text("%6d lines  | %s", editor.GetTotalLines(), editor.CanUndo() ? "Edited" : " ");
		ImGui::Spacing();
		editor.Render("Lua Editor");
	}
	if (savede)
	{
		if (ImGuiFileDialog::Instance()->FileDialog("Save As", 0, "C:/Appel/", "appel"))
		{
			if (ImGuiFileDialog::Instance()->IsOk == true)
			{
				auto textToSave = editor.GetText();
				//if (createDir("C:/Appel"))
				//	createFile(std::string(ImGuiFileDialog::Instance()->GetFilepathName() + ".lua").c_str(), textToSave);
			}
			else
			{
			}
			savede = false;
		}
	}
	if (loade)
	{
		if (ImGuiFileDialog::Instance()->FileDialog("Load", 0, "C:/Appel/", "appel"))
		{
			if (ImGuiFileDialog::Instance()->IsOk == true)
			{
				//editor.SetText(openFile(ImGuiFileDialog::Instance()->GetFilepathName().c_str()));

			}
			else
			{
			}
			loade = false;
		}
	}
	ImGui::End();
}
bool Menu::IG::KeyboardHandler(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_INSERT:
			Menu::IG::opened = !Menu::IG::opened;
			return false;
		}
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_INSERT:
			return false;
		}
	}

	return true;
}