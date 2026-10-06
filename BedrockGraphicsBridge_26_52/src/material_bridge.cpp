#include "material_bridge.h"
#include "state.h"
#include <windows.h>
#include <cstdlib>

namespace fs = std::filesystem;

static fs::path DefaultRoot() {
    char* appdata = nullptr;
    size_t n = 0;
    if (_dupenv_s(&appdata, &n, "APPDATA") == 0 && appdata) {
        fs::path p = fs::path(appdata) /
            "Minecraft Bedrock/users/shared/games/com.mojang/resource_packs";
        free(appdata);
        return p;
    }
    return {};
}

static fs::path Root() {
    char* custom = nullptr;
    size_t n = 0;
    if (_dupenv_s(&custom, &n, "BGB_MATERIAL_ROOT") == 0 && custom && *custom) {
        fs::path p(custom);
        free(custom);
        return p;
    }
    if (custom) free(custom);
    return DefaultRoot();
}

MaterialBridge& MaterialBridge::Instance() {
    static MaterialBridge x;
    return x;
}

void MaterialBridge::LoadAllOnce() {
    if (loaded_) return;
    loaded_ = true;

    materials_.clear();
    Scan(Root());

    State().materialCount = static_cast<uint32_t>(materials_.size());
    State().loadedForSession = true;

    SetStatus("MBL scan complete: " + std::to_string(materials_.size()) + " material files");
}

void MaterialBridge::Scan(const fs::path& root) {
    std::error_code ec;
    if (root.empty() || !fs::exists(root, ec)) {
        SetStatus("Material root not found");
        return;
    }

    for (fs::recursive_directory_iterator it(root,
             fs::directory_options::skip_permission_denied, ec), end;
         it != end && !ec; it.increment(ec)) {

        if (!it->is_regular_file(ec)) continue;
        const auto& p = it->path();

        if (_stricmp(p.extension().string().c_str(), ".bin") == 0 &&
            _stricmp(p.filename().string().c_str(), "") != 0) {

            // Chỉ đăng ký metadata ở lớp này.
            // Renderer bridge phải chuyển metadata/bytes vào Bedrock material API.
            MaterialRecord r;
            r.path = p;
            r.size = it->file_size(ec);
            materials_.push_back(std::move(r));
        }
    }
}
