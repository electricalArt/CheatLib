#pragma once
#include "Types.hpp"
#include <Windows.h>

class FProcess {
	friend class FWindow;
public:
	// Finds the process.
	b32 Find(WCHAR const* wndName, WCHAR const* wndClass);

	// Returns true if the process is currently open.
	b32 IsOpen();

	/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
	  Summary:  Opens the process for memory access.

	  Args:     b32 writeAccess
					`true` to PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION.
					`false` to  : PROCESS_VM_READ.

	  Returns:  b32
					`true` if success.
					`false` if fail.
	F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
	b32 Open(b32 writeAccess);

	// Closes the process for read/write access.
	void Close();

	/*F+F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F+++F
	  Summary:  Reads memory from a process.

	  Args:     u64 base
				u32 dstBytes

	  Returns:  b32
					If the function succeeds, the return value is nonzero.
					(the same as `ReadProcessMemory()`).
	F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F---F-F*/
	b32 ReadBytes(u64 base, void* dst, u32 dstBytes);
	b32 ReadBytes(u64* offsets, u32 offsetsCount, void* dst, u32 dstBytes);

	// Writes memory into the process.
	b32 WriteBytes(u64 base, void const* src, u32 srcBytes);

	// Type generic memory read.
	template <typename Type>
	Type Read(u64 base) {
		Type x{};
		if (ReadBytes(base, &x, sizeof(x))) {
			return x;
		} else {
			return Type{};
		}
	}
	template <typename Type>
	Type Read(u64* offsets, u32 offsetsCount) {
		Type x{};
		if (ReadBytes(offsets, offsetsCount, &x, sizeof(x))) {
			return x;
		} else {
			return Type{};
		}
	}

	// Type generic memory write.
	template <typename Type>
	b32 Write(u64 base, Type const& x) {
		return WriteBytes(base, &x, sizeof(x));
	}

private:
	HANDLE Process;
	HWND Window;
	DWORD ProcessID;
};
