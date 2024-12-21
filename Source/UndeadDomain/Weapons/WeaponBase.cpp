// Undead Domain By Rahmat


#include "WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UndeadDomain/Character/UD_BaseCharacter.h"

AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(SphereComponent);

}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	if (SphereComponent)
	{
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetGenerateOverlapEvents(true);
		SphereComponent->UpdateOverlaps();
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnSphereBeginOverlap);
		SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnSphereEndOverlap);

	}
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::Shoot()
{
	UAnimInstance* AnimInstance = WeaponMesh->GetAnimInstance();
	if (FireMontage && AnimInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shoot"))
		AnimInstance->Montage_Play(FireMontage);
	}
}

void AWeaponBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AUD_BaseCharacter* Character = Cast<AUD_BaseCharacter>(OtherActor);
	if (Character)
	{
		Character->SetCanPick(true);
		Character->SetWeapon(this);
	}
}

void AWeaponBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AUD_BaseCharacter* Character = Cast<AUD_BaseCharacter>(OtherActor);
	if (Character)
	{
		Character->SetCanPick(false);
	}
}