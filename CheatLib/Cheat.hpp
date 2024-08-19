#pragma once
/*+===================================================================
  File:      Cheat.hpp

  Summary:   Contains main `FCheat` class. You should inherit this
			 class within your cheat project.

  Classes:   `FCheat`

  Functions: -

  Origin:	 It is enhanced version of
			 https://github.com/gregtampa/cheat-lib repo.
===================================================================+*/

// Required.
#include "Process.hpp"
#include "Render.hpp"
#include "Timer.hpp"
#include "Window.hpp"

// Helpers.
#include "Core.hpp"
#include "Math.hpp"
#include "Matrix.hpp"
#include "Utility.hpp"
#include "Vector.hpp"

class FCheat {
public:
	//=========================================================================
	// Interface functions.
	//=========================================================================

	// Runs the cheat. Only a single instance is allowed at a time.
	void Run(
		WCHAR const* cmdLine,
		WCHAR const* wndName,
		WCHAR const* clsName,
		WCHAR const* targetModuleName
	);
	void RunConsole(
		WCHAR const* cmdLine,
		WCHAR const* wndName,
		WCHAR const* clsName,
		WCHAR const* targetModuleName
	);

protected:
	//=========================================================================
	// User functions.
	//=========================================================================

	virtual b32 Create() = 0;
	virtual b32 Ready() = 0;
	virtual void Update() = 0;

	//=========================================================================
	// Private interfaces.
	//=========================================================================

	FTimer Timer = {};
	FProcess Process = {};
	FWindow Window = {};
	FRender Render = {};
};
