#include <string>

#define COLOR_DARKBLUE	0x1
#define COLOR_DARKGREEN	0x2
#define COLOR_TEAL		0x3
#define COLOR_DARKRED	0x4
#define COLOR_MAGENTA	0x5
#define COLOR_DYELLOW	0x6
#define COLOR_DEFAULT	0x7
#define COLOR_GRAY		0x8
#define COLOR_BLUE		0x9
#define COLOR_GREEN		0xA
#define COLOR_CYAN		0xB
#define COLOR_RED		0xC
#define COLOR_PURPLE	0xD
#define COLOR_YELLOW	0xE
#define COLOR_WHITE		0xF

void CreateConsole(const char* Name);

void print(std::string Message, ...);

void printColor(int color, std::string Message, ...);

void printWarning(std::string Message, ...);

void printError(std::string Message, ...);

void printColorS(int color, std::string Message, ...);

void printWarningS(std::string Message, ...);

void printInfo(std::string Message, ...);

void printErrorS(std::string Message, ...);