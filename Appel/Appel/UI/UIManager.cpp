#include "UIManager.h"
#include "windows.h"
#include "../Menu/menu.h"

#include "../interfaces.h"
#include "../Features/features.h"
#include <iostream>

namespace Appel::UI
{
	void UIManager::OnTick()
	{
		std::lock_guard lock(m_Mutex);

		m_DrawBaseY = m_PosY;
		DrawHeader();
		if (!m_SubmenuStack.empty())
		{
			auto sub = m_SubmenuStack.top();
			sub->Reset();
			sub->Execute();

			DrawSubmenuBar(sub);
			if (sub->GetNumOptions() != 0)
			{
				std::size_t startPoint = 0;
				std::size_t endPoint = sub->GetNumOptions() > m_OptionsPerPage ? m_OptionsPerPage : sub->GetNumOptions();
				if (sub->GetNumOptions() > m_OptionsPerPage && sub->GetSelectedOption() >= m_OptionsPerPage)
				{
					startPoint = sub->GetSelectedOption() - m_OptionsPerPage + 1;
					endPoint = sub->GetSelectedOption() + 1;
				}

				for (std::size_t i = startPoint, j = 0; i < endPoint; ++i, ++j)
				{
					DrawOption(sub->GetOption(i), i == sub->GetSelectedOption());
				}
			}
		}

		DrawFooter(m_SubmenuStack.top());
		//DrawDescription();
	}
	void UIManager::Up()
	{
		auto sub = m_SubmenuStack.top();
		sub->ScrollBackward();
		m_LastKey = 0;
	}
	void UIManager::Down()
	{
		auto sub = m_SubmenuStack.top();
		sub->ScrollForward();
		m_LastKey = 1;
	}
	void UIManager::Left()
	{
		auto sub = m_SubmenuStack.top();
		if (auto opt = sub->GetOption(sub->GetSelectedOption()))
		{
			opt->HandleAction(OptionAction::LeftPress);
		}
	}
	void UIManager::Right()
	{
		auto sub = m_SubmenuStack.top();
		if (auto opt = sub->GetOption(sub->GetSelectedOption()))
		{
			opt->HandleAction(OptionAction::RightPress);
		}
	}
	void UIManager::Enter()
	{
		auto sub = m_SubmenuStack.top();
		if (auto opt = sub->GetOption(sub->GetSelectedOption()))
		{
			opt->HandleAction(OptionAction::EnterPress);
		}
	}
	void UIManager::Back()
	{
		if (m_SubmenuStack.size() <= 1)
		{
			Menu::opened = false;
		}
		else
		{
			m_SubmenuStack.top()->SetSelectedOption(0);
			m_SubmenuStack.pop();
		}
	}
	void UIManager::DrawHeader()
	{
		switch (m_HeaderType)
		{
		case HeaderType::Mac:
			DrawRect(
				m_PosX,
				m_DrawBaseY,
				m_Width,
				m_HeaderHeight,
				m_HeaderBackgroundColor);
			if (m_HeaderText)
			{
				DrawCenteredText(
					"~gA~yp~op~re~pl",
					m_PosX + (m_Width / 2.0f),
					m_DrawBaseY + (m_HeaderHeight / 2.f),
					m_HeaderTextSize,
					m_HeaderFont,
					m_HeaderTextColor,
					true, false);
			}
			break;
		case HeaderType::Appel:
			DrawRect(
				m_PosX,
				m_DrawBaseY,
				m_Width,
				m_HeaderHeight,
				m_HeaderBackgroundColorA);
			if (m_HeaderText)
			{
				DrawCenteredText(
					"Appel",
					m_PosX + (m_Width / 2.0f),
					m_DrawBaseY + (m_HeaderHeight / 2.f),
					m_HeaderTextSize,
					m_HeaderFont,
					m_HeaderTextColor,
					true, false);
			}
			break;
		case HeaderType::Garry:
			DrawRect(
				m_PosX,
				m_DrawBaseY,
				m_Width,
				m_HeaderHeight,
				m_HeaderBackgroundColorG);
			if (m_HeaderText)
			{
				DrawCenteredText(
					"Appel",
					m_PosX + (m_Width / 2.0f),
					m_DrawBaseY + (m_HeaderHeight / 2.f),
					m_HeaderTextSize,
					m_HeaderFont,
					m_HeaderTextColor,
					true, false);
			}
			break;
		}


		m_DrawBaseY += m_HeaderHeight;
	}

	void UIManager::DrawSubmenuBar(AbstractSubmenu* sub)
	{
		char leftText[64] = {};
		std::strncpy(&leftText[0], sub->GetName(), sizeof(leftText) - 1);
		//std::transform(std::begin(leftText), std::end(leftText), std::begin(leftText), [](char c) { return static_cast<char>(toupper(c)); });


		switch (m_HeaderType)
		{
		case HeaderType::Mac:
			DrawRect(
				m_PosX,
				m_DrawBaseY,
				m_Width, m_SubmenuBarHeight,
				m_SubmenuBarBackgroundColor);
			DrawCenteredText(
				&leftText[0],
				m_PosX + (m_Width / 2.0f),
				m_DrawBaseY + (m_SubmenuBarHeight / 2.0f),
				m_SubmenuBarTextSize, m_SubmenuBarFont,
				m_SubmenuBarTextColor,
				false, false);
			break;
		case HeaderType::Appel:
			DrawRect(
				m_PosX,
				m_DrawBaseY,
				m_Width, m_SubmenuBarHeight,
				m_SubmenuBarBackgroundColorA);
			DrawCenteredText(
				&leftText[0],
				m_PosX + (m_Width / 2.0f),
				m_DrawBaseY + (m_SubmenuBarHeight / 2.0f),
				m_SubmenuBarTextSize, m_SubmenuBarFont,
				m_SubmenuBarTextColorA,
				false, false);
			break;
		case HeaderType::Garry:
			DrawRect(
				m_PosX,
				m_DrawBaseY,
				m_Width, m_SubmenuBarHeight,
				m_SubmenuBarBackgroundColorG);
			DrawCenteredText(
				&leftText[0],
				m_PosX + (m_Width / 2.0f),
				m_DrawBaseY + (m_SubmenuBarHeight / 2.0f),
				m_SubmenuBarTextSize, m_SubmenuBarFont,
				m_SubmenuBarTextColorA,
				false, false);
			break;
		}
		m_DrawBaseY += m_SubmenuBarHeight;
	}

	void UIManager::DrawOption(AbstractOption* opt, bool selected)
	{
		if (opt->IsText() && selected)
		{
			auto sub = m_SubmenuStack.top();
			if (m_LastKey == 0)
				sub->ScrollBackward();
			else
				sub->ScrollForward();
		}
		DrawRect(
			m_PosX,
			m_DrawBaseY,
			m_Width,
			m_OptionHeight,
			selected ? m_OptionSelectedBackgroundColor : m_OptionUnselectedBackgroundColor);
		DrawLeftText(
			opt->GetLeftText(),
			m_PosX + (m_Width * m_OptionPadding),
			m_DrawBaseY + (m_OptionHeight / 2.0f),
			m_OptionTextSize,
			m_OptionFont,
			selected ? m_OptionSelectedTextColor : m_OptionUnselectedTextColor,
			selected ? false : true, false);
		DrawRightText(
			opt->GetRightText(),
			m_PosX + (m_Width - (m_Width * m_OptionPadding)),
			m_DrawBaseY + (m_OptionHeight / 2.0f),
			m_OptionRightTextSize,
			m_OptionFont,
			selected ? m_OptionSelectedTextColor : m_OptionUnselectedTextColor,
			selected ? false : true, false);
		DrawCenteredText(
			opt->GetCenterText(),
			m_PosX + (m_Width / 2.f),
			m_DrawBaseY + (m_OptionHeight / 2.0f),
			m_TextOptionTextSize,
			Font::Pricedown,
			selected ? m_OptionSelectedTextColor : m_OptionUnselectedTextColor,
			selected ? false : true, false);

		if (opt->GetFlag(OptionFlag::Enterable))
		{
			DrawRightText(
				">",
				m_PosX + (m_Width - (m_Width * m_OptionPadding)),
				m_DrawBaseY + (m_OptionHeight / 2.0f),
				m_OptionTextSize,
				m_OptionFont,
				selected ? m_OptionSelectedTextColor : m_OptionUnselectedTextColor,
				false, false);
		}

		m_DrawBaseY += m_OptionHeight;
	}

	void UIManager::DrawFooter(AbstractSubmenu* sub)
	{
		switch (m_HeaderType)
		{
		case HeaderType::Mac:
			DrawRect(
				m_PosX,
				m_DrawBaseY,
				m_Width,
				m_FooterHeight,
				m_FooterBackgroundColor);
			break;
		case HeaderType::Appel:
			DrawRect(
				m_PosX,
				m_DrawBaseY,
				m_Width,
				m_FooterHeight,
				m_FooterBackgroundColorA);
			break;
		case HeaderType::Garry:
			DrawRect(
				m_PosX,
				m_DrawBaseY,
				m_Width,
				m_FooterHeight,
				m_FooterBackgroundColorG);
			break;
		}
		char numText[32] = {};
		std::snprintf(numText, sizeof(numText) - 1, "%zu / %zu", sub->GetSelectedOption() + 1, sub->GetNumOptions());
		DrawLeftText(
			&numText[0],
			m_PosX + (m_Width * m_SubmenuBarPadding),
			m_DrawBaseY + (m_FooterHeight / 2.0f),
			m_FooterBarTextSize, Font::ChaletComprimeCologneSmol,
			m_SubmenuBarTextColor,
			true, false);

		m_DrawBaseY += m_FooterHeight;
	}

	void UIManager::DrawDescription()
	{
		const char* description{};

		if (!m_SubmenuStack.empty())
		{
			auto sub = m_SubmenuStack.top();
			if (sub->GetNumOptions())
			{
				if (auto opt = sub->GetOption(sub->GetSelectedOption()))
				{
					description = opt->GetDescription();
				}
			}
		}

		if (!description || !*description)
			return;

		m_DrawBaseY += m_DescriptionHeightPadding;

		DrawRect(
			m_PosX,
			m_DrawBaseY,
			m_Width,
			m_DescriptionHeight,
			m_DescriptionBackgroundColor);

		DrawLeftText(
			description,
			m_PosX - (m_Width / m_DescriptionPadding),
			m_DrawBaseY,
			m_DescriptionTextSize,
			m_DescriptionFont,
			m_DescriptionTextColor,
			false, false
		);

		m_DrawBaseY += m_DescriptionHeight;
	}

	void UIManager::DrawRect(float x, float y, float width, float height, Color color)
	{
		Draw.BoxFilled(x * Draw.Screen.Width, y * Draw.Screen.Height, width * Draw.Screen.Width, height * Draw.Screen.Height, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b));
	}

	void UIManager::DrawTextH(const char* text, float x, float y, text_alignment horizontal, text_alignment_v vertical, Font font, Color color, bool outline, bool shadow)
	{
		int len = strlen(text);

		bool hascolors = false;
		const char* newtext[16] = {};
		DWORD colors[16] = { D3DCOLOR_ARGB(color.a, color.r, color.g, color.b) };

		char cursub[128] = {};
		int pos = 0;
		int spaces = 0;
		for (int i = 0; i <= len; i++)
		{
			char l = text[i];
			switch (l)
			{
			case '~':
				if (i + 1 <= len)
				{
					hascolors = true;
					DWORD color = MENU_PUREWHITE(255);
					switch (text[i + 1])
					{
					case 'r':
						color = MENU_RED(255);
						break;
					case 'g':
						color = MENU_GREEN(255);
						break;
					case 'b':
						color = MENU_BLUE(255);
						break;
					case 'q':
						color = MENU_PINK(255);
						break;
					case 'o':
						color = MENU_ORANGE(255);
						break;
					case 'y':
						color = MENU_YELLOW(255);
						break;
					case 'c':
						color = MENU_GREY(255);
						break;
					case 'p':
						color = MENU_PURPLE(255);
						break;
					case 'w':
						color = MENU_WHITE(255);
						break;
					}
					char* newsub = new char[256];
					strcpy(newsub, cursub);
					newtext[pos] = newsub;
					pos++;
					colors[pos] = color;
					memset(cursub, 0, sizeof(cursub));

					memset(cursub, ' ', spaces);
					spaces = 0;

					i = i + 1;
				}
				break;
			case ' ':
				spaces++;
				break;
			default:
				spaces = 0;
				strcat(cursub, &l);
				break;
			}
		}
		newtext[pos] = cursub;

		float outlinewidth = 1;
		if (outline)
		{
			outlinewidth = outlinewidth * (GetFontSize(font) / 450.0f);
			if (outlinewidth < 1)
				outlinewidth = 1;
		}
		if (hascolors)
		{
			Draw.ColoredText(pos, newtext, x * Draw.Screen.Width, y * Draw.Screen.Height, horizontal, vertical, (int)font, outline, colors, D3DCOLOR_ARGB(255, 0, 0, 0), outlinewidth);
			memset(newtext, 0, sizeof(newtext));
		}
		else
			Draw.Text((char*)text, x * Draw.Screen.Width, y * Draw.Screen.Height, horizontal, vertical, (int)font, outline, D3DCOLOR_ARGB(color.a, color.r, color.g, color.b), D3DCOLOR_ARGB(color.a, 0, 0, 0), outlinewidth);
	}
	void UIManager::DrawLeftText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow)
	{
		DrawTextH(text, x, y, lefted, center, font, color, outline, shadow);
	}

	void UIManager::DrawCenteredText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow)
	{
		DrawTextH(text, x, y, centered, center, font, color, outline, shadow);
	}

	void UIManager::DrawCenteredTextC(const char* text[], float x, float y, float size, Font font, D3DCOLOR color[], bool outline, bool shadow)
	{
		float outlinewidth = 1;
		if (outline)
		{
			outlinewidth = outlinewidth * (GetFontSize(font) / 450.0f);
			if (outlinewidth < 1)
				outlinewidth = 1;
		}
		//Draw.ColoredText(text, x * Draw.Screen.Width, y * Draw.Screen.Height, centered, center, (int)font, outline, color, D3DCOLOR_ARGB(ExtractAlpha(color[0]), 0, 0, 0), outlinewidth);
	}

	void UIManager::DrawRightText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow)
	{
		DrawTextH(text, x, y, righted, center, font, color, outline, shadow);
	}

	float UIManager::GetFontSize(Font font)
	{
		float Scale = Draw.Screen.Height;//0.05f;
		switch (font)
		{
		case Font::Tahoma:
			return m_OptionRightTextSize * Scale;
		case Font::Pricedown:
			return m_TextOptionTextSize * Scale;
		case Font::PricedownHeader:
			return m_HeaderTextSize * Scale;
		case Font::ChaletComprimeCologne:
			return m_TextOptionTextSize * Scale;
		case Font::ChaletComprimeCologneSmol:
			return m_FooterBarTextSize * Scale;
		}
		return 0.0f;
	}

	void UIManager::InitFonts()
	{
		//int W = Draw.Screen.Width;
		float Scaling = Draw.Screen.Height * 0.065f;

		Draw.AddFont((char*)"Tahoma", m_OptionRightTextSize * Scaling, false, false);
		Draw.AddPricedown(m_TextOptionTextSize * Scaling, false, false);
		Draw.AddPricedown(m_HeaderTextSize * Scaling, false, false); 
		Draw.AddChalet(m_TextOptionTextSize * Scaling, false, false);
		Draw.AddChalet(m_FooterBarTextSize * Scaling, false, false);
		finit = true;
	}
	bool UIManager::FontsInitialized()
	{
		return finit;
	}
	void UIManager::UnInitFonts()
	{
		Draw.FontReset();
		finit = false;
	}
}
