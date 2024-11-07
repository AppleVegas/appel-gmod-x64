#pragma once
#include <functional>
#include <streambuf>
#include <iostream>

class MemoryStreamBuf : public std::streambuf
{
public:
	MemoryStreamBuf(char* array, std::size_t size)
	{
		std::fill_n(array, size, '\0');
		std::streambuf::setp(array, array + size - 1);
	}

	template <std::size_t size>
	MemoryStreamBuf(char(&array)[size]) :
		MemoryStreamBuf(&array[0], size)
	{
	}
};

/**
 * \brief An input/output stream that uses a char[] buffer
 */
class MemoryStringStream : virtual MemoryStreamBuf, public std::iostream
{
public:
	MemoryStringStream(char* array, std::size_t size) :
		MemoryStreamBuf(array, size),
		std::iostream(static_cast<MemoryStreamBuf*>(this))
	{
	}

	template <std::size_t size>
	MemoryStringStream(char(&array)[size]) :
		MemoryStreamBuf(array),
		std::iostream(static_cast<MemoryStreamBuf*>(this))
	{
	}
};

/**
 * \brief A MemoryStringStream with a built in buffer
 */
template <std::size_t N>
class MemoryStringStreamWithBuffer : public MemoryStringStream
{
public:
	MemoryStringStreamWithBuffer() :
		MemoryStringStream(m_Buffer)
	{}
private:
	char m_Buffer[N];
};

namespace Appel::UI
{
	enum class OptionAction
	{
		LeftPress,
		RightPress,
		EnterPress
	};

	enum class OptionFlag
	{
		Horizontal = (1 << 0),
		Enterable = (1 << 1)
	};

	class AbstractOption
	{
	public:
		virtual ~AbstractOption() noexcept = default;
	
		virtual const char* GetLeftText() = 0;
		virtual const char* GetRightText() = 0;
		virtual const char* GetCenterText() = 0;
		virtual bool IsText() = 0;
		virtual const char* GetDescription() = 0;
		virtual void HandleAction(OptionAction action) = 0;
		virtual bool GetFlag(OptionFlag flag) = 0;
	protected:
		explicit AbstractOption() = default;
		AbstractOption(AbstractOption const&) = default;
		AbstractOption& operator=(AbstractOption const&) = default;
		AbstractOption(AbstractOption&&) = default;
		AbstractOption& operator=(AbstractOption&&) = default;
	};
}
