// Luis Correa


#include "Actor/AuraProjectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Aura/Aura.h"
#include "Components/AudioComponent.h"

// Sets default values
AAuraProjectile::AAuraProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(Sphere);

	Sphere->SetCollisionObjectType(ECC_PROJECTILE);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	Sphere->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Overlap);

	ProjectileMovement =  CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 550;
	ProjectileMovement->MaxSpeed = 550;
	ProjectileMovement->ProjectileGravityScale = 0;
	
}

void AAuraProjectile::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(ImpactSFX)
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX,this->GetActorLocation(), FRotator::ZeroRotator);

	if(NiagaraSystem)
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraSystem, this->GetActorLocation(), FRotator::ZeroRotator);

	if(HasAuthority())
	{
		if(UAbilitySystemComponent * TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

// Called when the game starts or when spawned
void AAuraProjectile::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AAuraProjectile::OnSphereOverlap);

	if(LoopingSFX)
	{
		UGameplayStatics::SpawnSoundAttached(LoopingSFX, RootComponent, NAME_None, FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::SnapToTargetIncludingScale, true, 1, 1, 0, nullptr, nullptr, true);
	}

	SetLifeSpan(Lifetime);
}

void AAuraProjectile::Destroyed()
{
	if(!bHit && !HasAuthority())
	{
		if(ImpactSFX)
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSFX,this->GetActorLocation(), FRotator::ZeroRotator);

		if(NiagaraSystem)
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, NiagaraSystem, this->GetActorLocation(), FRotator::ZeroRotator);
		
	}
	
	Super::Destroyed();
}

