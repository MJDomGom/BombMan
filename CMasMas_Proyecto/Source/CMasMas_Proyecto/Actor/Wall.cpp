#include "Wall.h"
#include "Components/StaticMeshComponent.h"


AWall::AWall()
{
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh for wall"));
	RootComponent = Mesh;

	CurrentLife = MaxLife;
}

//Metodo que maneja el daño que se realiza a los muros
void AWall::GetDamage(int Damage)
{
	CurrentLife -= Damage;

	if (CurrentLife <= 0) {
		Destroy();
	}
}

int AWall::GetCurrentLife() const
{
	return CurrentLife;
}

int AWall::GetMaxLife() const
{
	return MaxLife;
}


