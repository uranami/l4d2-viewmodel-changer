#include <string_view>
#include <Windows.h>
#include "server_plguin_callbacks.h"
#include "cvar.hpp"
#include "Interfaces.h"
#include "hooks.h"

void* GetInterface(CreateInterfaceFn interfaceFactory, const char* pszInterfaceVersion)
{
    if (!interfaceFactory || !pszInterfaceVersion)
        return nullptr;

    const size_t length = strlen(pszInterfaceVersion);
    if (length < 3)
        return nullptr;

    char* szInterfaceVersion = new char[length + 1];
    strcpy_s(szInterfaceVersion, length + 1, pszInterfaceVersion);

    const size_t last_idx = length - 1;
    const size_t post_last_idx = length - 2;

    void* pInterface = nullptr;

    while (szInterfaceVersion[post_last_idx] != '0' || szInterfaceVersion[last_idx] != '0')
    {
        pInterface = interfaceFactory(szInterfaceVersion, nullptr);
        if (pInterface)
            break;

        if (szInterfaceVersion[last_idx] == '0')
        {
            szInterfaceVersion[last_idx] = '9';

            if (szInterfaceVersion[post_last_idx] != '0')
                --szInterfaceVersion[post_last_idx];
        }
        else
        {
            --szInterfaceVersion[last_idx];
        }
    }

    if (!pInterface)
        pInterface = interfaceFactory(szInterfaceVersion, nullptr);

    delete[] szInterfaceVersion;
    return pInterface;
}

ICvar* g_Cvar = nullptr;
class Hikari : public IServerPluginCallbacks {
private:

public:


    bool Load(CreateInterfaceFn interface_factory, CreateInterfaceFn gameserver_factory) override {

        g_Cvar = reinterpret_cast<ICvar *>(GetInterface(interface_factory, "VEngineCvar007"));
        hooks::InitHook();
        if (g_Cvar)
        {
            g_Cvar->PrintToConsole(Color(0,255,0), "viewmodel changer Loaded.\n");
        }
        return true;
    }

    virtual void Unload() override {
        hooks::ReleaseHook();
    }
    virtual void Pause() override { }
    virtual void UnPause() override { }
    virtual const char* GetPluginDescription() override { return "viewmodel changer plugin"; }
    virtual void LevelInit(const char* mapName) override { }
    virtual void ServerActivate(edict_t* pEdictList, int edictCount, int clientMax) override { }
    virtual void GameFrame(bool simulating) override { }
    virtual void LevelShutdown() override { }
    virtual void ClientActive(edict_t* pEntity) override { }
    virtual void ClientDisconnect(edict_t* pEntity) override { }
    virtual void ClientPutInServer(edict_t* pEntity, const char* playername) override { }
    virtual void SetCommandClient(int index) override { }
    virtual void ClientSettingsChanged(edict_t* pEdict) override { }
    virtual PLUGIN_RESULT ClientConnect(bool *bAllowConnect, edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen) override { return PLUGIN_CONTINUE; }
    virtual PLUGIN_RESULT ClientCommand(edict_t* pEntity, const CCommand& args) override { return PLUGIN_CONTINUE; }
    virtual PLUGIN_RESULT NetworkIDValidated(const char* pszUserName, const char* pszNetworkID) override { return PLUGIN_CONTINUE; }
    virtual void OnQueryCvarValueFinished(int iCookie, edict_t* pPlayerEntity, EQueryCvarValueStatus eStatus, const char* cvarName, const char* cvarValue) override { }
    virtual void OnEdictAllocated(edict_t* edict) override { }
    virtual void OnEdictFreed(const edict_t* edict) override { }
};

extern "C" __declspec(dllexport) void* CreateInterface(const char* name, int*) {
    static Hikari plugin {};
    if (strcmp(name, "ISERVERPLUGINCALLBACKS003") == 0)
        return &plugin;
    return nullptr;
}