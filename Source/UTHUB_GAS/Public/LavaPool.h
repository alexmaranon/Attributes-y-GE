// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LavaPool.generated.h"

UCLASS()
class UTHUB_GAS_API ALavaPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALavaPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION(BlueprintCallable, Category = "Lava")
	void AddBurnEffect(AActor* Actor, TSubclassOf<UGameplayEffect> EffectToApply);

	UFUNCTION(BlueprintCallable, Category = "Lava")
	void RemoveBurnEffect(AActor* Actor, TSubclassOf<UGameplayEffect> EffectToApply);
};
