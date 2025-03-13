// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CoreAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameplayTagContainer.h"
#include "ADC.h"
#include "GameFramework/Character.h"
#include "UTHUB_GASCharacter.generated.h"

class UGameplayBaseStateTags;

USTRUCT()
struct FCharacterAttributes : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)	FGameplayTag CharacterClassTag;
	UPROPERTY(EditAnywhere)	float Health;
	UPROPERTY(EditAnywhere) float AttackStrength;
	UPROPERTY(EditAnywhere)	float Speed;
	UPROPERTY(EditAnywhere)	UAnimMontage* AttackAnimation;
};

UCLASS(Blueprintable)
class AUTHUB_GASCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagCustomInterface
{
	GENERATED_BODY()




private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	FGameplayTag CharacterClassTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	FGameplayTag ZombieTag;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, meta =(AllowPrivateAccess))
	FCharacterAttributes* Attributes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess))
	UDataTable* CharacterData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<UGameplayEffect> SampleEffect;;


	UPROPERTY()
	UCoreAttributeSet* CoreAttributeSet;

public:
	AUTHUB_GASCharacter();

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;


	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual void AddTag(const FGameplayTag& InTag) override;
	virtual void RemoveTag(const FGameplayTag& InTag) override;

	UFUNCTION(BlueprintCallable, Category = "Zombie")
	void ApplyZombieState(AActor* TargetPlayer, TSubclassOf<UGameplayEffect> EffectToApply);
	void RemoveTag();

	UFUNCTION(CallInEditor, Category = "Test")
	void ApplyGameplayEffect();

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	FORCEINLINE virtual class UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag ActiveEventTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer GameplayStates;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGameplayBaseStateTags* CharacterStates;


protected:

	virtual void BeginPlay() override;

	virtual void PreInitializeComponents() override;

	virtual void SetAttributeCallbacks();


private:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UADC* ASC;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	class UGASDataComponent* GASDataComponent;

private:
	FTimerHandle InfectionTimer;
	void InitializeCharacter();
};

