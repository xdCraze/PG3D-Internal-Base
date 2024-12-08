#pragma once

// Including
#include "../Utils.h"
#include "../Backend/Backend.h"
#include "../BNM-IL2CPP/Structures/IL2CPP/Array.hpp"
#include "../BNM-IL2CPP/Structures/IL2CPP/Assembly.hpp"
#include "../BNM-IL2CPP/Structures/IL2CPP/Class.hpp"
#include "../BNM-IL2CPP/Structures/IL2CPP/Domain.hpp"
#include "../BNM-IL2CPP/Structures/IL2CPP/Field.hpp"
#include "../BNM-IL2CPP/Structures/IL2CPP/Image.hpp"
#include "../BNM-IL2CPP/Structures/IL2CPP/String.hpp"

// Define method to return a Field Offset
uint64_t GetField(const std::string& ClassName, const std::string& FieldName) {
    IL2CPP::Field* TargetField = IL2CPP::Domain().Assembly("Assembly-CSharp")->Image()->Class(ClassName.c_str())->Field(FieldName.c_str());
    return TargetField->Offset();
}

// Define method to return an Offset
uint64_t GetOffset(const std::string& ClassName, const std::string& MethodName) {
    IL2CPP::Method* TargetOffset = IL2CPP::Domain().Assembly("Assembly-CSharp")->Image()->Class(ClassName.c_str())->Method(MethodName.c_str());
    return TargetOffset->RVA();
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
    {
        MH_EnableHook((LPVOID*)(RunBackend.baseAddr + offset));
    }
}

// Define Hook that uses GetOffset
void HookUpdate(const std::string& className, const std::string& methodName, LPVOID detour, void* original)
{
    uint64_t offset = GetOffset(className, methodName);
    Hook(offset, detour, original);
}

// Pointers
// Example: void (*Set-XRay) (void*, bool);

void Pointers()
{

}



// Weapon Sounds Hook Example
void(*oldWeaponSounds)(void* instance);
void  WeaponSounds(void* instance)
{

    if (RunBackend.KillAll)
    {
        Field<bool>("WeaponSounds", "isFrostSword", instance) = true;
        Field<float>("WeaponSounds", "frostDamageMultiplier", instance) = 5.0f;
        Field<float>("WeaponSounds", "frostRadius", instance) = 99999.0f; // Adjust to your desired range
        Field<float>("WeaponSounds", "frostSwordnTime", instance) = 0.5f;
    }

    return oldWeaponSounds(instance);
}

// Hooking
void Hooks()
{
    printf("Initializing..\n");

    HookUpdate("WeaponSounds", "Update", &WeaponSounds, &oldWeaponSounds);

    // Pointer Usage
    Pointers();

    printf("Hooks initialized\n\n");
}