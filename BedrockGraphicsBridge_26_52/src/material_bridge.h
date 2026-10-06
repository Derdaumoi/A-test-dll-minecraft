#pragma once
#include <filesystem>
#include <vector>
#include <string>

struct MaterialRecord {
    std::filesystem::path path;
    uint64_t size{};
};

class MaterialBridge {
public:
    static MaterialBridge& Instance();

    // Quét một lần trong phiên.
    void LoadAllOnce();

    const std::vector<MaterialRecord>& Materials() const { return materials_; }

private:
    void Scan(const std::filesystem::path& root);
    std::vector<MaterialRecord> materials_;
    bool loaded_ = false;
};
