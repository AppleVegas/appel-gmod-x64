#include "BaseOption.h"

namespace Appel::UI
{
	class TextOption : public BaseOption<TextOption>
	{
	public:
		explicit TextOption() = default;
		explicit TextOption(const char* left = nullptr, const char* right = nullptr, const char* center = nullptr)
		{
			SetIsText(true);
			if (left)
				SetLeftText(left);
			if (right)
				SetRightText(right);
			if (center)
				SetCenterText(center);
		}
	
		~TextOption() noexcept = default;
		TextOption(TextOption const&) = default;
		TextOption& operator=(TextOption const&) = default;
		TextOption(TextOption&&) = default;
		TextOption& operator=(TextOption&&) = default;
	};
}