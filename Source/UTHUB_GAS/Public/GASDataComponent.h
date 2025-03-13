// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayAttributeEffector.h"
#include "Components/ActorComponent.h"
#include "GASDataComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTHUB_GAS_API UGASDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGASDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	


	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = true))
	UDataTable* DT_CoreStats;

	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = true))
	TArray<TSubclassOf<class UGameplayEffect>> AttributeInitializers;

	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = true))
	TMap<FGameplayAttribute, TSubclassOf<UGameplayAttributeEffector>> AttributeEffectors;

	UPROPERTY(EditAnywhere, Category = "Data", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UGameplayAbility> DefaultAbility;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
