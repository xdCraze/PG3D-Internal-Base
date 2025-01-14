#include "BNM/BNM.hpp"
#include "../Backend/Backend.h"
#include "../Utils/Utils.h"
#include "../Utils/Logger.h"

// Define method to return a Field Offset
uint64_t GetField(const std::string& ClassName, const std::string& FieldName) {
    IL2CPP::Field TargetField = IL2CPP::Domain().Assembly("Assembly-CSharp")->Image()->Class(ClassName.c_str())->Field(FieldName.c_str());
    return TargetField.Offset();
}

// Define method to return an Offset
uint64_t GetOffset(const std::string& ClassName, const std::string& MethodName) {
    IL2CPP::Method TargetOffset = IL2CPP::Domain().Assembly("Assembly-CSharp")->Image()->Class(ClassName.c_str())->Method(MethodName.c_str());
    return TargetOffset.RVA();
}

// Define Field Patch
template <typename T>
T& Field(const std::string& ClassName, const std::string& FieldName, void* instance) {
    uint64_t Offset = GetField(ClassName, FieldName);
    return *(T*)((uint64_t)instance + Offset);
}

// Define Hook
void Hook(uint64_t offset, LPVOID detour, void* original)
{
    if (MH_CreateHook((LPVOID*)(RunBackend.baseAddr + offset), detour, (LPVOID*)original) == MH_OK)
        MH_EnableHook((LPVOID*)(RunBackend.baseAddr + offset));
    else
        Logger::Log("Failed to hook offset: 0x{:X}", Logger::ERRORR, offset);
}

// Define Hook that uses GetOffset
void HookMethod(const std::string& className, const std::string& methodName, LPVOID detour, void* original)
{
    uint64_t offset = GetOffset(className, methodName);
    Hook(offset, detour, original);
}