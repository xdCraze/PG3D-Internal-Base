#pragma once

// Including
#include "IL2CPP-Helper.h"

// Pointers
// Example: void (*Set-XRay) (void*, bool);

void Pointers()
{
    // Example: Set-XRay = (void(*)(void*, bool))(RunBackend.baseAddr + 0xYourOffset);
}



// Weapon Sounds Hook Example
void(*oldWeaponSounds)(IL2CPP::Object* Instance);
void  WeaponSounds(IL2CPP::Object* Instance)
{

    if (RunBackend.KillAll)
    {
        Field<bool>("WeaponSounds", "isFrostSword", Instance) = true;
        Field<float>("WeaponSounds", "frostDamageMultiplier", Instance) = 5.0f;
        Field<float>("WeaponSounds", "frostRadius", Instance) = 99999.0f; // Adjust to your desired range
        Field<float>("WeaponSounds", "frostSwordnTime", Instance) = 0.5f;
    }

    return oldWeaponSounds(Instance);
}

// Hooking
void Hooks()
{
    printf("Initializing..\n");

    // Hooking non-obfuscated methods
    HookMethod("WeaponSounds", "Update", &WeaponSounds, &oldWeaponSounds);

    // Hooking obfuscated methods
    // Hook(0xYourOffset, &WeaponSounds, &oldWeaponSounds);

    // Pointer Usage
    Pointers();

    printf("Hooks initialized\n\n");
}