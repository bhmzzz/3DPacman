// Fill out your copyright notice in the Description page of Project Settings.


#include "Pacdot.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APacdot::APacdot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PacdotCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PacdotCollision"));
	PacdotDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacdotDisplayMesh"));
	PacdotDisplayMesh->SetupAttachment(PacdotCollision);
	RootComponent = PacdotCollision;

	bIsSuperPacdot = false;
}

// Called when the game starts or when spawned
void APacdot::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APacdot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

