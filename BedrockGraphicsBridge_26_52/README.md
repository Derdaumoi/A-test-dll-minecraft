# BedrockGraphicsBridge 26.52

DLL mod đồ họa cho Minecraft Bedrock Windows 26.52.

## Chức năng
- F8: bật/tắt menu.
- MBL Loader: quét toàn bộ `*.material.bin` trong resource_packs khi vào world/process.
- Load-once: mỗi file chỉ được đăng ký một lần trong phiên.
- Render bridge: có lớp quản lý trạng thái render và điểm móc D3D12 `Present`.
- Deferred Graphics: công tắc trong menu. Đây là lớp điều khiển/bridge; việc ép Bedrock dùng deferred renderer thật sự cần hook đúng renderer của build 26.52.

## Quan trọng
Đây không phải DLL đã biên dịch sẵn. Máy Linux/ChatGPT không có MSVC nên không thể tạo PE DLL đáng tin cậy cho Windows ở đây.

Project này cần:
- Visual Studio 2022 Build Tools / MSVC
- Windows 10/11 SDK
- CMake 3.25+
- Internet lần đầu để CMake tải Dear ImGui + MinHook.

## Build
Mở "x64 Native Tools Command Prompt for VS 2022":

    cd BedrockGraphicsBridge_26_52
    build.bat

DLL sẽ nằm ở:

    build\Release\BedrockGraphicsBridge.dll

## Inject
Chỉ dùng với bản Minecraft Bedrock 26.52 tương ứng. Địa chỉ nội bộ của Bedrock thay đổi theo bản cập nhật, vì vậy không dùng offset cố định từ bản khác.

## Material path
Mặc định scanner tìm:

    %APPDATA%\Minecraft Bedrock\users\shared\games\com.mojang\resource_packs

Có thể đổi bằng biến môi trường:

    set BGB_MATERIAL_ROOT=D:\MyPacks

## Giới hạn
`*.material.bin` là dữ liệu material của Bedrock; việc "nạp" file vào renderer không thể được thực hiện an toàn chỉ bằng cách đọc bytes. Renderer phải cung cấp API/internal object tương ứng. Source này có MaterialBridge để gắn implementation theo symbol/signature của 26.52.
