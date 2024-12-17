// Undead Domain By Rahmat

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UD_BaseCharacter.generated.h"

// Forward Declaration
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UCameraComponent;
class AUD_HUD;

UCLASS()
class UNDEADDOMAIN_API AUD_BaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AUD_BaseCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetHUDCrosshairs(float DeltaTime);
protected:
	virtual void BeginPlay() override;

private:
	/// Components
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UCameraComponent> ActorCamera;

	/// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> BaseCharacterIMC;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> IA_Shoot;

	/// Input Functions
	void CharacterShoot(const FInputActionValue& InputActionValue);

	/// Effects
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UAnimMontage> FireMontage;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UParticleSystem> ImpactParticle;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<USoundBase> ImpactSound;

	/// Weapon
	UPROPERTY(EditAnywhere, Category = "Weapon")
	float ProjectileTaragetHit = 4000.f;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UTexture2D> CrosshairsCenter;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UTexture2D> CrosshairsLeft;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UTexture2D> CrosshairsRight;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UTexture2D> CrosshairsTop;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TObjectPtr<UTexture2D> CrosshairsBottom;

	UPROPERTY()
	AUD_HUD* HUD;

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;

public:	
	

};
