#include <string>
#include <filesystem>
#include <fstream>

#include "../Structures/IL2CPP.hpp"
#include "../../ImGui/ImGuiNotify.hpp"

namespace BNM
{
    namespace Utils
    {
        // Runtime dumper
        void Dump(std::string filename = "Dump", bool show_offsets = true, bool show_patterns = true, int patterns_length = 9999)
        {
            std::filesystem::path path(std::filesystem::current_path().string() + "\\" + filename + ".cs");
            std::ofstream stream(path);
            for (auto it : IL2CPP::Domain().Assemblies())
            {
                stream << it->Image()->ToString(show_offsets, show_patterns, patterns_length);
            }
            ImGui::InsertNotification({ ImGuiToastType::Success, 5000, "Successfully dumped!\nPath to dump: %s", path.string().c_str()});
            stream.close();
        }
    }
}