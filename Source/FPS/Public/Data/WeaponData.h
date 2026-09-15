// Copyright Druid Mechanics

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

class UAnimMontage;
class UBlendSpace;
class UAnimSequence;

USTRUCT(BlueprintType)
struct FPlayerAnims
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> IdleAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> AimIdleAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> CrouchIdleAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    TObjectPtr<UAnimSequence> SprintAnim = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBlendSpace> AimOffset_Hip;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBlendSpace> AimOffset_Aim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBlendSpace> Strafe_Standing;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBlendSpace> Strafe_Crouching;
};

USTRUCT(BlueprintType)
struct FMontageData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> EquipMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ReloadMontage = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> FireMontage = nullptr;
};

UCLASS()
class FPS_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, Category = "FPS|WeaponData|Weapons")
	TMap<FGameplayTag, FName> GripPoints;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Weapons")
	TMap<FGameplayTag, FMontageData> WeaponMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|FirstPerson")
	TMap<FGameplayTag, FPlayerAnims> FirstPersonAnims;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|ThirdPerson")
	TMap<FGameplayTag, FPlayerAnims> ThirdPersonAnims;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|FirstPerson")
	TMap<FGameplayTag, FMontageData> FirstPersonMontages;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|ThirdPerson")
	TMap<FGameplayTag, FMontageData> ThirdPersonMontages;
	
	/** 各武器类型的爆头伤害倍率，按 WeaponType 配置（例如 Weapon.Type.Pistol -> 2.0） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Damage")
	TMap<FGameplayTag, float> HeadShotDamageMultipliers;
	
	/** 未在 HeadShotDamageMultipliers 中单独配置的武器类型所使用的默认爆头倍率（1.0 表示不放大） */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FPS|WeaponData|Damage")
	float DefaultHeadShotDamageMultiplier = 1.0f;
	
	/** 查询指定武器类型的爆头伤害倍率，未配置时回退到 DefaultHeadShotDamageMultiplier */
	UFUNCTION(BlueprintPure, Category = "FPS|WeaponData|Damage")
	float GetHeadShotDamageMultiplier(const FGameplayTag& WeaponType) const;
	
};
