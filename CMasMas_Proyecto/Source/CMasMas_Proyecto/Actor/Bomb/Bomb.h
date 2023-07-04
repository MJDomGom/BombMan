#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/PrimitiveComponent.h"
#include "Bomb.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;



UCLASS()
class CMASMAS_PROYECTO_API ABomb : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Components", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* Mesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Components", meta = (AllowPrivateAccess = "true"))
		UParticleSystemComponent* Particles = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Components", meta = (AllowPrivateAccess = "true"))
		USphereComponent* DamageCollision = nullptr;

	ABomb();




protected:
	/*Atributos modificables en el editor para las bombas*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Parameters")
		float ExplodingTime = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Parameters")
		float LifeTime = 1.2f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Parameters")
		float BombRadius = 300.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Parameters")
		int BombDamage = 2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Parameters")
		float Impulse = 10000.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bomb Parameters")
		UParticleSystem* ExplosionParticle = nullptr;


	UFUNCTION(BlueprintCallable)
		void OnExplode();

	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle ExplodeTimerHandle;
	FTimerHandle ParticleTimerHandle;

	void StartParticle();
	void OnEndExplosion();
	

};