// Copyright 2020 Dan Kestranek.


#include "Player/GDPlayerController.h"
#include "GameFramework/PlayerController.h"
#include "AbilitySystemComponent.h"
#include "Characters/Heroes/GDHeroCharacter.h"
#include "Player/GDPlayerState.h"
#include "UI/GDDamageTextWidgetComponent.h"

#include "Components/InputComponent.h"

#include "UI/GDHUDWidget.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GDGameplayTag.h"


void AGDPlayerController::CreateHUD()
{
	// Only create once
	if (UIHUDWidget)
	{
		return;
	}

	if (!UIHUDWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("%s() Missing UIHUDWidgetClass. Please fill in on the Blueprint of the PlayerController."), *FString(__FUNCTION__));
		return;
	}

	// Only create a HUD for local player
	if (!IsLocalPlayerController())
	{
		return;
	}

	// Need a valid PlayerState to get attributes from
	AGDPlayerState* PS = GetPlayerState<AGDPlayerState>();
	if (!PS)
	{
		return;
	}

	UIHUDWidget = CreateWidget<UGDHUDWidget>(this, UIHUDWidgetClass);
	UIHUDWidget->AddToViewport();

	// Set attributes
	UIHUDWidget->SetCurrentHealth(PS->GetHealth());
	UIHUDWidget->SetMaxHealth(PS->GetMaxHealth());
	UIHUDWidget->SetHealthPercentage(PS->GetHealth() / FMath::Max<float>(PS->GetMaxHealth(), 1.f));
	UIHUDWidget->SetCurrentMana(PS->GetMana());
	UIHUDWidget->SetMaxMana(PS->GetMaxMana());
	UIHUDWidget->SetManaPercentage(PS->GetMana() / FMath::Max<float>(PS->GetMaxMana(), 1.f));
	UIHUDWidget->SetHealthRegenRate(PS->GetHealthRegenRate());
	UIHUDWidget->SetManaRegenRate(PS->GetManaRegenRate());
	UIHUDWidget->SetCurrentStamina(PS->GetStamina());
	UIHUDWidget->SetMaxStamina(PS->GetMaxStamina());
	UIHUDWidget->SetStaminaPercentage(PS->GetStamina() / FMath::Max<float>(PS->GetMaxStamina(), 1.f));
	UIHUDWidget->SetStaminaRegenRate(PS->GetStaminaRegenRate());
	UIHUDWidget->SetExperience(PS->GetXP());
	UIHUDWidget->SetGold(PS->GetGold());
	UIHUDWidget->SetHeroLevel(PS->GetCharacterLevel());
}

UGDHUDWidget * AGDPlayerController::GetHUD()
{
	return UIHUDWidget;
}

void AGDPlayerController::ShowDamageNumber_Implementation(float DamageAmount, AGDCharacterBase* TargetCharacter)
{
	if (TargetCharacter && DamageNumberClass)
	{
		UGDDamageTextWidgetComponent* DamageText = NewObject<UGDDamageTextWidgetComponent>(TargetCharacter, DamageNumberClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->SetDamageText(DamageAmount);
	}
}

bool AGDPlayerController::ShowDamageNumber_Validate(float DamageAmount, AGDCharacterBase* TargetCharacter)
{
	return true;
}

void AGDPlayerController::SetRespawnCountdown_Implementation(float RespawnTimeRemaining)
{
	if (UIHUDWidget)
	{
		UIHUDWidget->SetRespawnCountdown(RespawnTimeRemaining);
	}
}

bool AGDPlayerController::SetRespawnCountdown_Validate(float RespawnTimeRemaining)
{
	return true;
}

// Server only
void AGDPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGDPlayerState* PS = GetPlayerState<AGDPlayerState>();
	if (PS)
	{
		// Init ASC with PS (Owner) and our new Pawn (AvatarActor)
		PS->GetAbilitySystemComponent()->InitAbilityActorInfo(PS, InPawn);
	}
}

void AGDPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// For edge cases where the PlayerState is repped before the Hero is possessed.
	CreateHUD();
}

void AGDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(MoveContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);

    Subsystem->AddMappingContext(MoveContext, 0);

	// const GDGameplayTags& GameplayTags = GDGameplayTags::Get();
	// GEngine->AddOnScreenDebugMessage(-1,
	// 	5.f,
	// 	FColor::Red,
	// 	FString::Printf(TEXT("LMBTag: %s, %s, %hs"), *GameplayTags.InputTag_LMB.ToString(), *GameplayTags.InputTag_RMB.ToString(), "haha"));
	//
}

void AGDPlayerController::BindAbilitiesInput(UEnhancedInputComponent* EnhancedInputComponent)
{
	check(InputConfig);
	
	for (const FGDInputAction& Action : InputConfig->InputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Started, this, &AGDPlayerController::OnInputPress, Action.InputTag);
			EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Triggered, this, &AGDPlayerController::OnInputHold, Action.InputTag);
			EnhancedInputComponent->BindAction(Action.InputAction, ETriggerEvent::Completed, this, &AGDPlayerController::OnInputRelease, Action.InputTag);
		}
	}
}

void AGDPlayerController::OnInputPress(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("InputTag Press: %s"), *InputTag.ToString()));

	AGDHeroCharacter* character = Cast<AGDHeroCharacter>(GetPawn());
	UAbilitySystemComponent* ASC = character->GetAbilitySystemComponent();
	check(ASC);
	TArray<FGameplayAbilitySpec>& allSkills = ASC->GetActivatableAbilities();
	for (FGameplayAbilitySpec& Spec : allSkills)
	{
		if (Spec.Ability && Spec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			Spec.InputPressed = true;
			if (Spec.IsActive())
			{
				if (Spec.Ability->bReplicateInputDirectly && ASC->IsOwnerActorAuthoritative() == false)
				{
					ASC->ServerSetInputPressed(Spec.Handle);
				}

				ASC->AbilitySpecInputPressed(Spec);

				// Invoke the InputPressed event. This is not replicated here. If someone is listening, they may replicate the InputPressed event to the server.
				ASC->InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());					
			}
			else
			{
				// Ability is not active, so try to activate it
				ASC->TryActivateAbility(Spec.Handle);
			}
		}
		
	}
	
	
}

void AGDPlayerController::OnInputHold(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("InputTag HOld: %s"), *InputTag.ToString()));
}

void AGDPlayerController::OnInputRelease(FGameplayTag InputTag)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("InputTag Release: %s"), *InputTag.ToString()));
}


void AGDPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGDPlayerController::Move);

	BindAbilitiesInput(EnhancedInputComponent);
}

void AGDPlayerController::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	
	auto cc = Cast<AGDHeroCharacter>(GetPawn());
	if (cc)
	{
		cc->MoveForward(MovementVector.Y);
		cc->MoveRight(MovementVector.X);
	}

	
}
