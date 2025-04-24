#pragma once
#include "../safetyhook/safetyhook.hpp"
#include "Vector3D.hpp"

namespace hooks
{
#define CREATE_HOOK_DEFINE_INLINE(name, type)                                                                                                                                                  \
    namespace name                                                                                                                                                                             \
    {                                                                                                                                                                                          \
        using fn = std::remove_pointer_t<type>;                                                                                                                                                \
        fn Fn;                                                                                                                                                                                 \
        inline safetyhook::InlineHook hk {};                                                                                                                                                   \
    };

#define CREATE_HOOK_DEFINE_MIDFUNC(name)                                                                                                                                                                         \
    namespace name                                                                                                                                                                                                  \
    {                                                                                                                                                                                                            \
        static std::remove_pointer_t<safetyhook::MidHookFn> Hook;                                                                                                                                                \
        inline safetyhook::MidHook hk {};                                                                                                                                                          \
    };
	void InitHook();
	void ReleaseHook();


	CREATE_HOOK_DEFINE_INLINE(CalcViewModelView, void(__fastcall*) (void* ecx, void* edx, int a2, Vector3D& eyePosition, const Vector3D& eyeAngles ));

};