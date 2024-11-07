#pragma once
#include "TextBox.h"
#include "AbstractSubmenu.h"
#include <iomanip>    
#include "../Drawing/dx_drawing.h"

#define MENU_RED(alpha)         D3DCOLOR_ARGB(alpha, 224, 50, 50)
#define MENU_BLUE(alpha)         D3DCOLOR_ARGB(alpha, 93, 182, 229)
#define MENU_GREEN(alpha)         D3DCOLOR_ARGB(alpha, 114, 204, 114)
#define MENU_YELLOW(alpha)         D3DCOLOR_ARGB(alpha, 240, 200, 80)
#define MENU_ORANGE(alpha)         D3DCOLOR_ARGB(alpha, 255, 133, 85)
#define MENU_PURPLE(alpha)         D3DCOLOR_ARGB(alpha, 132, 102, 226)
#define MENU_PINK(alpha)         D3DCOLOR_ARGB(alpha, 203, 54, 148)
#define MENU_GREY(alpha)         D3DCOLOR_ARGB(alpha, 155, 155, 155)
#define MENU_WHITE(alpha)         D3DCOLOR_ARGB(alpha, 240, 240, 240)
#define MENU_PUREWHITE(alpha)         D3DCOLOR_ARGB(alpha, 255, 255, 255)

#pragma pack(push, 1)
struct Vector2
{
	float x{};
	float y{};
};
#pragma pack(pop)

#pragma pack(push, 1)
struct Color
{
	int r{ 255 };
	int g{ 255 };
	int b{ 255 };
	int a{ 255 };
};
#pragma pack(pop)

namespace Appel::UI
{
	enum class Font : int
	{
		Tahoma = 1,
		Pricedown = 2,
		PricedownHeader = 3,
		ChaletComprimeCologne = 4,
		ChaletComprimeCologneSmol = 5
	};

	enum class HeaderType : std::int32_t
	{
		Mac = 0,
		Appel = 1,
		Garry = 2
	};

	struct Rectangle
	{
		Vector2 m_Center;
		Vector2 m_Size;

		bool IsInBounds(Vector2 point)
		{
			auto left = m_Center.x - (m_Size.x / 2.f);
			auto right = m_Center.x + (m_Size.x / 2.f);
			auto top = m_Center.y + (m_Size.y / 2.f);
			auto bottom = m_Center.y - (m_Size.y / 2.f);
			
			if (point.x > right || point.x < left)
				return false;
			if (point.y > top || point.y < bottom)
				return false;

			return true;
		}
	};

	class UIManager
	{
	public:
		explicit UIManager() = default;
		~UIManager() noexcept = default;
		UIManager(UIManager const&) = delete;
		UIManager(UIManager&&) = delete;
		UIManager& operator=(UIManager const&) = delete;
		UIManager& operator=(UIManager&&) = delete;
	
		template <typename SubmenuType, typename ...TArgs>
		void AddSubmenu(TArgs&&... args)
		{
			auto sub = std::make_unique<SubmenuType>(std::forward<TArgs>(args)...);
			if (m_SubmenuStack.empty())
			{
				m_SubmenuStack.push(sub.get());
			}

			m_AllSubmenus.push_back(std::move(sub));
		}

		void SwitchToSubmenu(std::uint32_t id)
		{
			for (auto&& sub : m_AllSubmenus)
			{
				if (sub->GetId() == id)
				{
					m_SubmenuStack.push(sub.get());
					return;
				}
			}
		}

		void OnTick();
		void Up();
		void Down();
		void Left();
		void Right();
		void Enter();
		void Back();
		void InitFonts();
		bool FontsInitialized();
		void UnInitFonts();
	public:
		std::mutex m_Mutex;

		bool m_MouseLocked = false;
		float m_PosX = 0.77f;
		float m_PosY = 0.055f;
		float m_Width = 0.21f;
		std::size_t m_OptionsPerPage = 16;
		bool m_Sounds = false;
		
		// Input
		std::int32_t m_OpenDelay = 100;
		std::int32_t m_BackDelay = 100;
		std::int32_t m_EnterDelay = 100;
		std::int32_t m_VerticalDelay = 100;
		std::int32_t m_HorizontalDelay = 100;
		std::int32_t m_ChangeDelay = 10;

		int m_LastKey = 0;

		// Header
		float m_HeaderHeight = 0.081f;
		bool m_HeaderText = true;
		float m_HeaderTextSize = 1.f;
		Font m_HeaderFont = Font::PricedownHeader;
		Color m_HeaderBackgroundColor{ 226, 210, 167, 255 };
		Color m_HeaderBackgroundColorA{ 225, 29, 86, 255 };
		Color m_HeaderBackgroundColorG{ 18, 148, 239, 255 };
		Color m_HeaderTextColor{ 255, 255, 255, 255 };
		HeaderType m_HeaderType = HeaderType::Appel;

		bool m_HeaderGradientTransparent = false;
		bool m_HeaderGradientFlip = false;
		float m_HeaderGradientStretch = 0.f;
		float m_HeaderGradientFiller = 0.f;
		Color m_HeaderGradientColorLeft{ 0, 186, 255, 255 };
		Color m_HeaderGradientColorRight{ 252, 0, 255, 255 };

		// Submenu bar
		float m_SubmenuBarHeight = 0.025f;
		float m_SubmenuBarTextSize = 0.34f;
		Font m_SubmenuBarFont = Font::ChaletComprimeCologne;
		float m_SubmenuBarPadding = 0.021f;
		Color m_SubmenuBarBackgroundColor{ 176,160, 117, 220 };
		Color m_SubmenuBarTextColor{ 255, 255, 255, 255 };
		Color m_SubmenuBarBackgroundColorA{ 205, 9, 66, 220 };
		Color m_SubmenuBarBackgroundColorG{ 12, 100, 162, 220 };
		Color m_SubmenuBarTextColorA{ 255, 255, 255, 255 };
		// Options
		float m_OptionHeight = 0.0335f;
		float m_OptionTextSize = 0.37f;
		float m_OptionRightTextSize = 0.33f;
		float m_TextOptionTextSize = 0.33f;
		Font m_OptionFont = Font::ChaletComprimeCologne;
		float m_OptionPadding = 0.021f;
		Color m_OptionSelectedTextColor{ 10, 10, 10, 255 };
		Color m_OptionUnselectedTextColor{ 255, 255, 255, 255 };
		Color m_OptionSelectedBackgroundColor{ 255, 255, 255, 255 };
		Color m_OptionUnselectedBackgroundColor{ 0, 0, 0, 160 };

		// Footer
		float m_FooterHeight = 0.025f;
		float m_FooterSpriteSize = 0.025f;
		float m_FooterBarTextSize = 0.30f;
		Color m_FooterBackgroundColor{ 226, 210, 167, 200 };
		Color m_FooterBackgroundColorA{ 225, 29, 86, 255 };
		Color m_FooterBackgroundColorG{ 18, 148, 239, 255 };
		Color m_FooterSpriteColor{ 255, 255, 255, 255 };

		// Description
		float m_DescriptionHeightPadding = 0.01f;
		float m_DescriptionHeight = 0.033f;
		float m_DescriptionTextSize = 0.28f;
		Font m_DescriptionFont = Font::ChaletComprimeCologne;
		Color m_DescriptionBackgroundColor{ 0, 0, 0, 120 };
		Color m_DescriptionTextColor{ 255, 255, 255, 255 };
		float m_DescriptionSpriteSize = 0.025f;
		float m_DescriptionPadding = 2.1f;
		Color m_DescriptionSpriteColor{ 255, 255, 255, 255 };
	private:
		bool finit = false;

		float m_DrawBaseY{};
		void DrawHeader();
		void DrawSubmenuBar(AbstractSubmenu* sub);
		void DrawOption(AbstractOption* opt, bool selected);
		void DrawFooter(AbstractSubmenu* sub);
		void DrawDescription();

		void DrawRect(float x, float y, float width, float height, Color color);
		void DrawTextH(const char* text, float x, float y, text_alignment horizontal, text_alignment_v vertical, Font font, Color color, bool outline, bool shadow);
		void DrawLeftText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow);
		void DrawCenteredText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow);
		void DrawCenteredTextC(const char* text[], float x, float y, float size, Font font, D3DCOLOR color[], bool outline, bool shadow);
		void DrawRightText(const char* text, float x, float y, float size, Font font, Color color, bool outline, bool shadow);
		float GetFontSize(Font font);

		std::vector<std::unique_ptr<AbstractSubmenu>> m_AllSubmenus;
		std::stack<AbstractSubmenu*, std::vector<AbstractSubmenu*>> m_SubmenuStack;
	};
	inline std::unique_ptr<UIManager> g_UiManager;
}
