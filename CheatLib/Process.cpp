#include "Process.hpp"
#include <easylogging++.h>
#include <assert.h>

b32 FProcess::Find(WCHAR const* wndName, WCHAR const* wndClass) {
	Window = FindWindowW(wndClass, wndName);

	if (Window == 0) {
		ProcessID = 0;
		return false;
	}

	GetWindowThreadProcessId(Window, &ProcessID);

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
		LOG(DEBUG) << "[pBuff + 0x" << std::hex << offsets[i] << "]: " << pBuff;
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
	b32 result = WriteProcessMemory(Process, (void*)base, src, srcBytes, 0);
	if (result == 0) {
		LOG(ERROR) << "WriteProcessMemory() failed.";
		throw std::runtime_error("Faild to write to the process memory.");
	}
	return result;
}
