#pragma once
#include "BaseOption.h"

namespace Appel::UI
{
	template <typename DataType, typename PositionType>
	class ChooseOption : public BaseOption<ChooseOption<DataType, PositionType>>
	{
	public:
		explicit ChooseOption() = default;

		template <PositionType N>
		explicit ChooseOption(const char* text, const char* description, DataType(*array)[N], PositionType* position, bool actionOnHorizontal = false, std::function<void()> action = [] {}):
			m_Data(&(*array)[0]),
			m_DataSize(N),
			m_Position(position),
			m_ActionOnHorizontal(actionOnHorizontal)
		{
			Base::SetLeftText(text);
			if (description)
				Base::SetDescription(description);
			Base::SetAction(std::move(action));
		}

		template <typename VectorType>
		explicit ChooseOption(const char* text, const char* description, const VectorType* v, PositionType* position, bool actionOnHorizontal = false, std::function<void()> action = [] {}) :
			m_Data(v->data()),
			m_DataSize(v->size()),
			m_Position(position),
			m_ActionOnHorizontal(actionOnHorizontal)
		{
			Base::SetLeftText(text);
			if (description)
				Base::SetDescription(description);
			Base::SetAction(std::move(action));
		}

		const char* GetRightText() override
		{
			if (m_Data)
			{
				snprintf(Base::m_RightText, sizeof(Base::m_RightText), "%s ~c[~q%d~c / ~q%d~c]", m_Data[*m_Position], *m_Position + 1, m_DataSize);
			}
			else
			{
				snprintf(Base::m_RightText, sizeof(Base::m_RightText), "%s", "Unknown ~c[~q0~c /~q 0~c]");
			}
			return Base::GetRightText();
		}

		void HandleAction(OptionAction action) override
		{
			if (action == OptionAction::LeftPress)
			{
				if (m_Data)
				{
					if (*m_Position > 0)
						--(*m_Position);
					else
						*m_Position = static_cast<PositionType>(m_DataSize - 1);

					if (m_ActionOnHorizontal && Base::m_Action)
						std::invoke(Base::m_Action);
				}
			}
			else if (action == OptionAction::RightPress)
			{
				if (m_Data)
				{
					if (*m_Position < m_DataSize - 1)
						++(*m_Position);
					else
						*m_Position = 0;

					if (m_ActionOnHorizontal && Base::m_Action)
						std::invoke(Base::m_Action);
				}
			}

			if (m_Data)
				Base::HandleAction(action);
		}

		~ChooseOption() noexcept = default;
		ChooseOption(ChooseOption const&) = default;
		ChooseOption& operator=(ChooseOption const&) = default;
		ChooseOption(ChooseOption&&) = default;
		ChooseOption& operator=(ChooseOption&&) = default;
	private:
		const DataType* m_Data{};
		PositionType m_DataSize{};
		PositionType* m_Position{};
		bool m_ActionOnHorizontal{};

		using Base = BaseOption<ChooseOption<DataType, PositionType>>;
	};
}
