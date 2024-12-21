// Undead Domain By Rahmat


#include "UD_BaseCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "Kismet/GameplayStatics.h"
#include "UndeadDomain/Weapon/Projectile.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "UndeadDomain/HUD/UD_HUD.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UndeadDomain/Weapons/WeaponBase.h"
#include "Components/SphereComponent.h"


AUD_BaseCharacter::AUD_BaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ActorCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActorCamera"));
	ActorCamera->SetupAttachment(GetRootComponent());

	GetMesh()->SetupAttachment(ActorCamera);

}

void AUD_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	check(BaseCharacterIMC);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		check(Subsystem);
		Subsystem->AddMappingContext(BaseCharacterIMC, 0);
	}

}

void AUD_BaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetHUDCrosshairs(DeltaTime);
}

void AUD_BaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInputComponent->BindAction(IA_Shoot, ETriggerEvent::Started, this, &AUD_BaseCharacter::CharacterShoot);
	EnhancedInputComponent->BindAction(IA_Pickup, ETriggerEvent::Started, this, &AUD_BaseCharacter::Pickup);

}

void AUD_BaseCharacter::Pickup(const FInputActionValue& InputActionValue)
{
	if (!bCanPick || !Weapon)
		return;
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Socket_Pistol"));
	Weapon->SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bHasWeapon = true;
}

void AUD_BaseCharacter::CharacterShoot(const FInputActionValue& InputActionValue)
{
	if (!bHasWeapon || !Weapon)
		return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!AnimInstance || !FireMontage || !PlayerController)
		return;

	FVector CameraLocation;
	FRotator CameraRotation;

	PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
	FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * ProjectileTaragetHit);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECollisionChannel::ECC_Visibility, CollisionParams);

	if (bHit)
	{
		FVector HitLocation = HitResult.ImpactPoint;
		FRotator HitRotation = HitResult.ImpactNormal.Rotation();
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticle, HitLocation, HitRotation);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitLocation);
	}
	AnimInstance->Montage_Play(FireMontage);
	Weapon->Shoot();
}

void AUD_BaseCharacter::SetHUDCrosshairs(float DeltaTime)
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
		return;
	HUD = HUD == nullptr ? Cast<AUD_HUD>(PlayerController->GetHUD()) : HUD;

	if (HUD)
	{
		FHUDPackage HUDPackage;
		
		HUDPackage.CrosshairsCenter = CrosshairsCenter;
		HUDPackage.CrosshairsLeft = CrosshairsLeft;
		HUDPackage.CrosshairsRight = CrosshairsRight;
		HUDPackage.CrosshairsBottom = CrosshairsBottom;
		HUDPackage.CrosshairsTop = CrosshairsTop;

		FVector2D WalkSpeedRange(0.f, GetCharacterMovement()->MaxWalkSpeed);
		FVector2D VelocityMultiplierRange(0.f, 1.f);
		FVector Velocity = GetVelocity();
		Velocity.Z = 0.f;

		CrosshairVelocityFactor = FMath::GetMappedRangeValueClamped(WalkSpeedRange, VelocityMultiplierRange, Velocity.Size());

		if (GetCharacterMovement()->IsFalling())
		{
			CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 2.25f, DeltaTime, 2.25f);
		}
		else
		{
			CrosshairInAirFactor = FMath::FInterpTo(CrosshairInAirFactor, 0.f, DeltaTime, 30.f);
		}
		HUDPackage.CrosshairSpread = CrosshairVelocityFactor + CrosshairInAirFactor;
		// Update HUD
		HUD->SetHUDPackage(HUDPackage);
	}
}