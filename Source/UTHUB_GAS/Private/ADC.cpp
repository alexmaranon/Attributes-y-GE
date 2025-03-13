// Fill out your copyright notice in the Description page of Project Settings.


#include "ADC.h"

#include "CoreAttributeSet.h"
#include "GASDataComponent.h"
#include "NativeGameplayTags.h"

UADC::UADC()
{
	PrimaryComponentTick.bCanEverTick = true;
}
void UADC::BeginPlay()
{
	Super::BeginPlay();
	//InitializeAttribute(GetOwner());

	InitAbilityActorInfo(GetOwner(), GetOwner());
}

void UADC::InitializeAttributesFromEffect()
{
	//Init Atributes
	if (UGASDataComponent* DataComponent = GetOwner()->FindComponentByClass<UGASDataComponent>())
	{
		for (const auto& EffectClass : DataComponent->AttributeInitializers)
		{
			ApplyEffectFromClass(EffectClass);
		}
	}
}

void UADC::AddAbilityFromClass(TSubclassOf<UGameplayAbility>& InGameplayAnilityClass)
{
	FGameplayAbilitySpec Spec(InGameplayAnilityClass, 1, 0);

	GiveAbility(Spec);
}

void UADC::ApplyEffectFromClass(const TSubclassOf<UGameplayEffect>& EffectClass)
{
	FGameplayEffectContextHandle EffectContext = MakeEffectContext();
	EffectContext.AddSourceObject(this);

	const FGameplayEffectSpecHandle Spec = MakeOutgoingSpec(EffectClass, 1, EffectContext);

	ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void UADC::RemoveEffect(FActiveGameplayEffectHandle EffectHandle)
{
	if (EffectHandle.IsValid())
	{
		RemoveActiveGameplayEffect(EffectHandle);
	}
}

void UADC::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	InitializeAttributesFromEffect();

	UGASDataComponent* DataComponent = GetOwner()->FindComponentByClass<UGASDataComponent>();
	if (DataComponent) { AddAbilityFromClass(DataComponent->DefaultAbility); }
	// INIT EFFECT
	// INI ATTRIBS
	// INIT SKILLS
}

void UADC::InitializeAttribute(AActor* InOwnerActor)
{
	if (UCoreAttributeSet* CoreAttributes = GetAttributeSetFrowmOwner<UCoreAttributeSet>())
	{
		if (UGASDataComponent* DataComponent = InOwnerActor->FindComponentByClass<UGASDataComponent>())
		{
			if (DataComponent->DT_CoreStats)
			{
				/*UCoreAttributeSet* CoreSet = const_cast<UCoreAttributeSet*>(GetSet<UCoreAttributeSet>());*/
				//CTrol-K-C


				auto InitializeAttrribute = [this, CoreAttributes](const FName& RowName, const FGameplayCoreAttribute& Row)
					{


						float NewValue = Row.AttributeBaeValue;

						Row.Attribute.SetNumericValueChecked(NewValue, CoreAttributes);

					};
				DataComponent->DT_CoreStats->ForeachRow<FGameplayCoreAttribute>(TEXT(""), MoveTemp(InitializeAttrribute));
			}
		}
	}
}

// Called every frame
void UADC::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
