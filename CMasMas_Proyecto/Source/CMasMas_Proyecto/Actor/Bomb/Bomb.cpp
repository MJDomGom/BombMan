#include "Bomb.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include <CMasMas_Proyecto/CMasMas_ProyectoCharacter.h>
#include <CMasMas_Proyecto/Actor/Wall.h>

ABomb::ABomb()
{
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);

	Particles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particles"));
	Particles->SetupAttachment(Mesh);
	Particles->bAutoActivate = false;

	DamageCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Damage Collision"));
	DamageCollision->SetupAttachment(Mesh);
	DamageCollision->SetSphereRadius(0.f);
	DamageCollision->SetCollisionProfileName(TEXT("Overlap_All"));
}

void ABomb::BeginPlay() {
	Super::BeginPlay();

	DamageCollision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABomb::OnComponentBeginOverlap);

	GetWorldTimerManager().SetTimer(ExplodeTimerHandle,this, &ABomb::OnExplode, LifeTime,false);
	const FVector ImpulseDir = Mesh->GetForwardVector() * Impulse;
	Mesh->AddImpulse(ImpulseDir);
}

/*Metodo que maneja la explosion de la bomba.*/
void ABomb::OnExplode()
{
	Particles->Activate(true);
	GetWorld()->GetTimerManager().SetTimer(ExplodeTimerHandle, this, &ABomb::OnEndExplosion, ExplodingTime, false);
	DamageCollision->SetSphereRadius(BombRadius);

	if (Mesh) {
		Mesh->SetVisibility(false);
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	GetWorld()->GetTimerManager().SetTimer(ParticleTimerHandle, this, &ABomb::StartParticle, 0.1f, false);

}

void ABomb::StartParticle()
{
	if (!ExplosionParticle) {
		return;
	}
	Particles->SetTemplate(ExplosionParticle);
	Particles->Activate(true);


}

/*Metodo para finalizar la explosion*/
void ABomb::OnEndExplosion()
{
	ExplodeTimerHandle.Invalidate();
	this->Destroy();
}


/*Metodo que maneja las colisiones de la bomba en el momento de explotar.
Tanto si detecta a un Character o a un Wall, esta hara daño dependiento de
BombDamage.*/

void ABomb::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACMasMas_ProyectoCharacter* LocalChar = Cast<ACMasMas_ProyectoCharacter>(OtherActor)) {
		LocalChar->GetDamage(BombDamage);
	}

	if (AWall* Wall = Cast<AWall>(OtherActor)) {
		Wall->GetDamage(BombDamage);
	}
}


