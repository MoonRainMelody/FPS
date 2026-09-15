// Copyright Druid Mechanics


#include "Data/WeaponData.h"

float UWeaponData::GetHeadShotDamageMultiplier(const FGameplayTag& WeaponType) const
{
	if (const float* ConfiguredMultiplier = HeadShotDamageMultipliers.Find(WeaponType))
	{
		return *ConfiguredMultiplier;
	}
	return DefaultHeadShotDamageMultiplier;
}
