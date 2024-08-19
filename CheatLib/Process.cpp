#include "Process.hpp"
#include <easylogging++.h>
#include <assert.h>
#include <Windows.h>
#include <psapi.h>

b32 FProcess::Find(WCHAR const* wndName, WCHAR const* wndClass) {
	Window = FindWindowW(wndClass, wndName);

	if (Window == 0) {
		ProcessID = 0;
		throw std::runtime_error("Failed to find the game window. Is the game opened?");
	}

	GetWindowThreadProcessId(Window, &ProcessID);

	return true;
}
b32 FProcess::FindTargetModule(const WCHAR* wszcTargetModuleName)
{
	LOG_IF(Process == NULL, FATAL) << "`Process` is NULL";
	if (wszcTargetModuleName == NULL) {
		return false;
	}
	HMODULE hModules[1024];
	DWORD cbNeeded = 0;
	if (EnumProcessModules(
			Process,
			hModules,
			sizeof(hModules),
			&cbNeeded ) == 0) {
		LOG(ERROR) << "EnumProcessModules() failed";
        throw std::runtime_error("Failed to enumarate game process modules.");
        return NULL;
    }

    unsigned int cuFoundModules = cbNeeded / sizeof(HMODULE);
    HMODULE hTargetModule = NULL;
    for (int i = 0; i < cuFoundModules; i++)
    {
        WCHAR wszModuleName[1024];
        GetModuleBaseNameW(
            Process,
            hModules[i],
            wszModuleName,
            1024
        );
        LOG_N_TIMES(3, TRACE) << "Found module: " << wszModuleName;
        if (wcscmp(wszModuleName, wszcTargetModuleName) == 0)
        {
            hTargetModule = hModules[i];
            break;
        }
    }
    LOG(INFO) << "hTargetModule: " << hTargetModule;
	if (hTargetModule == NULL) {
		throw std::runtime_error("Failed to find target module within game process.");
	}
	TargetModule = hTargetModule;

    return true;
}

b32 FProcess::IsOpen() {
	if (IsWindow(Window)) {
		return true;
	}

	Window = 0;
	ProcessID = 0;

	return false;
}

b32 FProcess::Open(b32 writeAccess) {
	if (Process) {
		Close();
	}

	Process = OpenProcess(writeAccess ? PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION : PROCESS_VM_READ, FALSE, ProcessID);
	if (Process == NULL) {
		throw std::runtime_error("Failed to open the game process. Is the game running?");
	}
	LOG(INFO) << "Process: " << Process;

	return (Process != NULL);
}

void FProcess::Close() {
	if (Process) {
		CloseHandle(Process);
		Process = NULL;
	}
}


b32 FProcess::ReadBytes(u64 base, void * dst, u32 dstBytes) {
	return ReadProcessMemory(Process, (void*)base, dst, dstBytes, 0);
}

b32 FProcess::ReadBytes(u64* offsets, u32 offsetsCount, void* dst, u32 dstBytes) {
	u64 pBuff = 0;
	int i = 0;
	b32 result = 0;

	for (; i < offsetsCount - 1; i++) {
		pBuff += offsets[i];
		assert(pBuff);
		ReadProcessMemory(
			Process,
			(void*)pBuff,
			&pBuff,
			sizeof(u64),
			0);
		LOG(DEBUG) << "[pBuff + 0x" << std::hex << offsets[i] << "]: 0x" << std::hex << pBuff;
	}
	pBuff += offsets[i];
	assert(pBuff);
	result = ReadProcessMemory(
		Process, 
		(void*)pBuff,
		dst,
		dstBytes,
		0);
	LOG(DEBUG) << "[pBuff + 0x" << std::hex << offsets[i] << "]: " << pBuff;

	return result;
}

b32 FProcess::WriteBytes(u64 base, void const * src, u32 srcBytes) {
	LOG_IF(Process == NULL, FATAL) << "`Process` is NULL";
	b32 result = WriteProcessMemory(Process, (void*)base, src, srcBytes, 0);
	if (result == 0) {
		LOG(ERROR) << "WriteProcessMemory() failed with address: 0x" << std::hex << base;
		throw std::runtime_error("Failed to write to the game process memory.");
	}
	return result;
}
