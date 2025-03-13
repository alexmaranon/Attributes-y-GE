// Copyright Epic Games, Inc. All Rights Reserved.

#include "UTHUB_GASCharacter.h"
#include "GameplayBaseStateTags.h"
#include "GameplayStatesManager.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "GASDataComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "CoreAttributeSet.h"
#include "Engine/World.h"
#include "ADC.h"

AUTHUB_GASCharacter::AUTHUB_GASCharacter()
{
	ASC = CreateDefaultSubobject<UADC>(TEXT("ASC"));

	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.f, 640.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	// Create a camera boom...
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CoreAttributeSet = CreateDefaultSubobject<UCoreAttributeSet>(TEXT("CoreAttr"));
	GASDataComponent = CreateDefaultSubobject<UGASDataComponent>(TEXT("GASData"));
}

void AUTHUB_GASCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}



void AUTHUB_GASCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayStates;
}

void AUTHUB_GASCharacter::AddTag(const FGameplayTag& InTag)
{
	GameplayStates.AddTag(InTag);
}

void AUTHUB_GASCharacter::RemoveTag(const FGameplayTag& InTag)
{
	GameplayStates.RemoveTag(InTag);
}

void AUTHUB_GASCharacter::ApplyZombieState(AActor* TargetPlayer, TSubclassOf<UGameplayEffect> EffectToApply)
{
	if(ASC)
	{
		AUTHUB_GASCharacter* TargetCharacter = Cast<AUTHUB_GASCharacter>(TargetPlayer);
		UADC* TargetASC = TargetPlayer->FindComponentByClass<UADC>();

		if(TargetASC)
		{
			//Si tiene tag (Si está infectado)
			if (TargetCharacter->ZombieTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Character.Zombie"))))
			{
					//Si nuestro player ya está infectado Borramos el efecto para volverlo a poner
				if(ZombieTag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Character.Zombie"))))
				{
					ASC->RemoveActiveGameplayEffectBySourceEffect(EffectToApply, ASC, 1);
					//Limpiamos el ya existente timer ya que al tener el tag significa que se está ejecutando
					GetWorld()->GetTimerManager().ClearTimer(InfectionTimer);
				}else
				{
					ZombieTag = TargetCharacter->ZombieTag;
				}
				

				ASC->ApplyGameplayEffectToSelf(EffectToApply.GetDefaultObject(), 1.0f, ASC->MakeEffectContext());


				//Añadimos un tiempo 

				GetWorld()->GetTimerManager().SetTimer(InfectionTimer,this, &AUTHUB_GASCharacter::RemoveTag	,10.0f, false);

				
			}
			//No está zombificado por lo que no pasaría nada
			else
			{
			}
			
		}

	}
}

void AUTHUB_GASCharacter::RemoveTag()
{
	//ASC->RemoveLooseGameplayTag(ZombieTag);
	ZombieTag = FGameplayTag::EmptyTag;
}

void AUTHUB_GASCharacter::ApplyGameplayEffect()
{
	if (ASC)
	{
		FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		const FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(SampleEffect, 1, EffectContext);

		ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}


UAbilitySystemComponent* AUTHUB_GASCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AUTHUB_GASCharacter::InitializeCharacter()
{
	if (CharacterData)
	{
		TArray<FCharacterAttributes*> OutData;
		CharacterData->GetAllRows(TEXT(""), OutData);

		if (!OutData.IsEmpty())
		{
			FCharacterAttributes** Attr = OutData.FindByPredicate([this](FCharacterAttributes* Row)
				{
					return Row->CharacterClassTag.MatchesTag(CharacterClassTag);
				});

			if (Attr) Attributes = *Attr;
		}
	}
}


void AUTHUB_GASCharacter::BeginPlay()
{
	
	Super::BeginPlay();

	SetAttributeCallbacks();

	

}

void AUTHUB_GASCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
}


void AUTHUB_GASCharacter::SetAttributeCallbacks()
{
	//for (auto AttributeEffectorsPais: GASDataComponent->AttributeEffectors)
	for (auto [Attribute, EffectorClass] : GASDataComponent->AttributeEffectors)
	{
		auto& Delegate = ASC->GetGameplayAttributeValueChangeDelegate(Attribute);

		UGameplayAttributeEffector* Effector = EffectorClass->GetDefaultObject<UGameplayAttributeEffector>();
		//UObject* Effector = EffectorClass->GetDefaultObject<UObject>();

		Delegate.AddUObject(Effector, &UGameplayAttributeEffector::ApplyAttributeEffector);
	}
}
