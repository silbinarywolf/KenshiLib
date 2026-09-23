#pragma once

#include <Defines.h>
#include <stdint.h>

// NOTE: Only used for static_assert
#include <string>
#include <vector>
#include <map>

// Validate we are using MSVC 2010 by checking the size of common C++ data structures.
//
// This is to ensure developers new to C/C++ aren't going to compile mods with the wrong compiler tools and accidentally
// create mods that randomly corrupt memory.
//
// We do this check in Functions.h as every mod needs to at least call GetRealAddress() in one file.
//
// WARNING: If you got an error here, you're using an invalid C++ compiler and will likely cause memory bugs / crashes if you remove these checks and push forward.
static_assert(sizeof(std::string) == 40, "std::string should be 40 bytes, must use a MSVC 2010 compatible compiler or memory bugs/crashes are likely to occur"); // Can be 24 bytes in Clang, 32 bytes in other MSVC versions, incompatible.
static_assert(sizeof(std::vector<std::string>) == 32, "std::vector<...> must be 32 bytes, must use a MSVC 2010 compatible compiler or memory bugs/crashes are likely to occur"); // Can be 24 bytes in Clang, 24 bytes in other MSVC versions, incompatible.
static_assert(sizeof(std::map<std::string, std::string>) == 32, "std::map<..., ...> must be 32 bytes, must use a MSVC 2010 compatible compiler or memory bugs/crashes are likely to occur"); // Can be 24 bytes in Clang, 16 bytes in other MSVC versions, incompatible.

namespace KenshiLib
{
	KLIB_EXPORT intptr_t GetRealAddress(void* fun);

	//       convenience functions

	// usage: GetRealAddress(&Class::function)
	// NOTE: doesn't work with virtual functions
	template<typename T>
	inline intptr_t GetRealAddress(T fun)
	{
		return GetRealAddress((void*&)fun);
	}

	// NOTE: doesn't work with virtual functions
	template<typename T>
	inline T GetRealFunction(T fun)
	{
		return (T)GetRealAddress(fun);
	}

	enum HookStatus
	{
		SUCCESS,
		FAIL
	};

	// immediately add hook (high overhead)
	KLIB_EXPORT HookStatus AddHook(void* target, void* detour, void** original);
	// queue hook to be added (note: each module/DLL has it's own global queue)
	HookStatus QueueHook(void* target, void* detour, void** original);
	// flushes the hook queue for our module
	HookStatus ApplyQueuedHooks();

	// convenience functions
	template<typename T>
	inline HookStatus AddHook(intptr_t target, void* detour, T** original)
	{
		return AddHook((void*)target, detour, (void**)original);
	}
	template<typename T1, typename T2>
	inline HookStatus AddHook(T1* target, void* detour, T2** original)
	{
		return AddHook((void*)target, detour, (void**)original);
	}
	template<typename T>
	inline HookStatus QueueHook(intptr_t target, void* detour, T** original)
	{
		return QueueHook((void*)target, detour, (void**)original);
	}
	template<typename T1, typename T2>
	inline HookStatus QueueHook(T1* target, void* detour, T2** original)
	{
		return QueueHook((void*)target, detour, (void**)original);
	}

	// *********** DO NOT USE ***********
	// BACKWARDS COMPATIBILITY WILL NOT BE MAINTAINED FOR INTERNAL FUNCTIONS
#ifdef KENSHILIB_INTERNAL
	bool InitRVAs();
#endif
}