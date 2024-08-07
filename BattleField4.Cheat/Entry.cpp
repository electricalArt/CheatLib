#include "BattleField4.Cheat.hpp"
#include <Windows.h>

//int CALLBACK wWinMain(_In_ HINSTANCE instance, _In_ HINSTANCE prevInstance, _In_ LPWSTR cmdLine, _In_ int cmdShow) {
int main(int argc, wchar_t cmdLine[])
{
	FCheatBF4 cheat = {};
	cheat.Run(cmdLine, L"Battlefield 4", 0);
	return 0;
}
