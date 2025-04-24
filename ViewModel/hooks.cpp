#include "hooks.h"
#include "../Utils.hpp"
#include "cvar.hpp"
#include "Interfaces.h"

namespace detail
{

    template <typename T>
    void Hook(T& hk, void* addr, std::string_view name, void* target)
    {
        static_assert(std::is_same_v<T, safetyhook::InlineHook> || std::is_same_v<T, safetyhook::MidHook>, "Only support safetyhook::InlineHook or safetyhook::MidHook");

        auto address = addr;

        if (!address)
        {
            g_Cvar->PrintToConsole(Color(255,0,0), std::format("[!][Hook] Failed to find address for {}\n", name).c_str());
            return;
        }

        if constexpr (std::is_same_v<T, safetyhook::InlineHook>)
            hk = safetyhook::create_inline(address, target);
        else
            hk = create_mid(address, reinterpret_cast<safetyhook::MidHookFn>(target));

        if (!hk)
        {
            g_Cvar->PrintToConsole(Color(255,0,0), std::format("[!][Hook] Failed to hook {}\n", name).c_str());
            return;
        }

        g_Cvar->PrintToConsole(Color(0,255,0), std::format("[+][Hook] Create Hook {} original->{}\n", name, address).c_str());
    }

    template <std::size_t idx, typename T>
    void Hook(T& hk, std::string_view name, void* vtable, void* target)
    {
        static_assert(std::is_same_v<T, safetyhook::InlineHook> || std::is_same_v<T, safetyhook::MidHook>, "Only support safetyhook::InlineHook or safetyhook::MidHook");

        auto address = reinterpret_cast<void*>(VirtualFunction<idx>(vtable));
        if (!address)
        {
            g_Cvar->PrintToConsole(Color(255,0,0), std::format("[!][Hook] Failed to find address for {}\n", name).c_str());
            return;
        }

        if constexpr (std::is_same_v<T, safetyhook::InlineHook>)
            hk = safetyhook::create_inline(address, target);
        else
            hk = safetyhook::create_mid(address, reinterpret_cast<safetyhook::MidHookFn>(target));

        if (!hk)
        {
            g_Cvar->PrintToConsole(Color(255,0,0), std::format("[!][Hook] Failed to hook {}\n", name).c_str());
            return;
        }


    }
}

void hooks::InitHook()
{
	static auto CalcViewModelView_sig = FindPattern("client.dll", "55 8B EC 83 EC 48 A1 ? ? ? ? 33 C5 89 45 FC 8B 45 10 8B 10");
	detail::Hook(CalcViewModelView::hk, reinterpret_cast<void*>(CalcViewModelView_sig), "C_TerrorViewModel::CalcViewModelView", CalcViewModelView::Fn);

}
inline float Deg2Rad(float deg) {

	return (deg * (3.141f / 180.f));
}

inline void AngleVector(const Vector3D& angles, Vector3D* forward, Vector3D* right, Vector3D* up)
{

	float cos_y = std::cos(Deg2Rad(angles.y)), sin_y = std::sin(Deg2Rad(angles.y));
	float cos_x = std::cos(Deg2Rad(angles.x)), sin_x = std::sin(Deg2Rad(angles.x));
	float cos_z = std::cos(Deg2Rad(angles.z)), sin_z = std::sin(Deg2Rad(angles.z));

	if (forward) {

		forward->x = cos_x * cos_y;
		forward->y = cos_x * sin_y;
		forward->z = -sin_x;

	}

	if (right) {

		right->x = -1.f * sin_z * sin_x * cos_y + -1.f * cos_z * -sin_y;
		right->y = -1.f * sin_z * sin_x * sin_y + -1.f * cos_z * cos_y;
		right->z = -1.f * sin_z * cos_x;

	}

	if (up) {

		up->x = cos_z * sin_x * cos_y + -sin_z * -sin_y;
		up->y = cos_z * sin_x * sin_y + -sin_z * cos_y;
		up->z = cos_z * cos_x;

	}

}

void __fastcall hooks::CalcViewModelView::Fn(void* ecx, void* edx, int a2, Vector3D& eyePosition, const Vector3D& eyeAngles)
{
	if (ecx)
    {
        Vector3D Forward = {}, Right = {}, Up = {};
		AngleVector(eyeAngles, &Forward, &Right, &Up);

        Vector3D ViewModelPos = eyePosition + (
            (Forward * g_Cvar->FindVar("viewmodel_offset_x")->GetFloat()/*x*/) +
            (Right * g_Cvar->FindVar("viewmodel_offset_y")->GetFloat()/*y*/) +
            (Up * g_Cvar->FindVar("viewmodel_offset_z")->GetFloat()/*z*/)
            );

        hk.fastcall(ecx, edx, a2, std::ref(ViewModelPos),  std::ref(eyeAngles));
    }
    else hk.fastcall(ecx, edx, a2, std::ref(eyePosition), std::ref(eyeAngles));
}

void hooks::ReleaseHook()
{
	CalcViewModelView::hk.reset();
}
