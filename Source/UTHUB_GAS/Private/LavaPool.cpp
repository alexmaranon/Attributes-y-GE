// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaPool.h"
#include "UTHUB_GAS/UTHUB_GASCharacter.h"


// Sets default values
ALavaPool::ALavaPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ALavaPool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALavaPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALavaPool::AddBurnEffect(AActor* Actor, TSubclassOf<UGameplayEffect> EffectToApply)
{

	if(Actor)
	{
		AUTHUB_GASCharacter* TargetCharacter = Cast<AUTHUB_GASCharacter>(Actor);

		if(TargetCharacter)
		{
			UADC* TargetASC = TargetCharacter->FindComponentByClass<UADC>();

			if(TargetASC)
			{
				TargetASC->ApplyGameplayEffectToSelf(EffectToApply.GetDefaultObject(), 1.0f, TargetASC->MakeEffectContext());
			}

		}
	}

}

void ALavaPool::RemoveBurnEffect(AActor* Actor, TSubclassOf<UGameplayEffect> EffectToApply)
{

	if (Actor)
	{
		AUTHUB_GASCharacter* TargetCharacter = Cast<AUTHUB_GASCharacter>(Actor);

		if (TargetCharacter)
		{
			UADC* TargetASC = TargetCharacter->FindComponentByClass<UADC>();

			if (TargetASC)
			{
				TargetASC->RemoveActiveGameplayEffectBySourceEffect(EffectToApply, TargetASC, 1);
			}

		}
	}

}

