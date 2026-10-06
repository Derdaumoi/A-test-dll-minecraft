#include "render_hook.h"
#include "state.h"
#include <windows.h>
#include <dxgi.h>
#include <d3d12.h>
#include "MinHook.h"

// NOTE:
// This file intentionally does NOT hard-code Minecraft Bedrock addresses.
// The safe way is to resolve the 26.52 renderer path by signature/symbol
// and then connect the bridge to the renderer's material/deferred pipeline.

static bool g_installed = false;

bool InstallRenderHook() {
    // MinHook initialization is ready for a version-specific hook.
    // A generic IDXGISwapChain Present hook is not enough to turn Bedrock
    // "Fancy" into Deferred; it only gives a frame boundary.
    if (MH_Initialize() != MH_OK && MH_StatusToString(MH_ERROR_ALREADY_INITIALIZED)) {
        // already initialized is harmless
    }

    g_installed = true;
    State().renderHook = true;
    SetStatus("Render bridge initialized; Bedrock 26.52 renderer resolver required");
    return true;
}

void RemoveRenderHook() {
    if (!g_installed) return;
    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    g_installed = false;
    State().renderHook = false;
}

void ApplyDeferredRenderState() {
    if (!State().deferred) return;

    // Version-specific implementation point:
    // 1. Resolve Bedrock render pipeline.
    // 2. Locate the Fancy/Deferred quality selector.
    // 3. Switch pipeline to deferred.
    // 4. Rebind material resources from MaterialBridge.
    //
    // Do not patch arbitrary D3D12 command lists here; doing so without
    // knowing Bedrock's resource lifetime causes crashes/device removal.
}
