// Fill out your copyright notice in the Description page of Project Settings.


#include "PacmanEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PacmanPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Public/TimerManager.h"

// Sets default values
APacmanEnemy::APacmanEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PacmanEnemyDisplayMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanEnemyDisplayMesh"));
	PacmanEnemyDisplayMesh->SetupAttachment(GetCapsuleComponent());
	PacmanEnemyEye1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanEnemyEye1"));
	PacmanEnemyEye2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PacmanEnemyEye2"));
	PacmanEnemyEye1->SetupAttachment(PacmanEnemyDisplayMesh);
	PacmanEnemyEye2->SetupAttachment(PacmanEnemyDisplayMesh);

	NormalSpeed = 300.0f;
	VulnerableSpeed = 100.0f;
	bIsDead = false;
	bIsVulnerable = false;
	DeadTime = 5.0f;
	VulnerableTime = 5.0f;
}

// Called when the game starts or when spawned
void APacmanEnemy::BeginPlay()
{
	Super::BeginPlay();
	StartLocation = GetActorLocation();

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APacmanEnemy::OnEnemyBeginOverlay);
	SetMovement(true);
}

// Called every frame
void APacmanEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APacmanEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APacmanEnemy::SetDead(bool bRestoreTimer, float timer)
{
	if (bRestoreTimer && bIsDead)
	{
		GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &APacmanEnemy::SetNormal, timer, false);
		return;
	}

	if (bIsDead)
	{
		return;
	}
	bIsDead = true;
	SetMovement(true);
	SetActorLocation(StartLocation);

	GetWorldTimerManager().ClearTimer(VulnerableTimerHandle);
	GetWorldTimerManager().SetTimer(DeadTimerHandle, this, &APacmanEnemy::SetNormal, DeadTime, false);
}

void APacmanEnemy::SetVulnerable(bool bRestoreTimer, float timer)
{
	if (bRestoreTimer)
	{
		GetWorldTimerManager().SetTimer(VulnerableTimerHandle, this, &APacmanEnemy::SetNormal, timer, false);
	}
	else
	{
		GetWorldTimerManager().SetTimer(VulnerableTimerHandle, this, &APacmanEnemy::SetNormal, VulnerableTime, false);
	}
	if (bIsVulnerable)
	{
		return;
	}
	bIsVulnerable = true;
	SetMovement(true);
}

void APacmanEnemy::SetNormal()
{
	GetWorldTimerManager().ClearTimer(DeadTimerHandle);
	GetWorldTimerManager().ClearTimer(VulnerableTimerHandle);

	bIsDead = false;
	bIsVulnerable = false;

	SetMovement(true);
}

void APacmanEnemy::SetMovement(bool bCanMove)
{
	if (bCanMove)
	{
		if (bIsVulnerable == false && bIsDead == false)
		{
			GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		}
		if (bIsVulnerable == true && bIsDead == false)
		{
			GetCharacterMovement()->MaxWalkSpeed = VulnerableSpeed;
		}
		if (bIsDead == true)
		{
			GetCharacterMovement()->MaxWalkSpeed = 0;
		}
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = 0;
	}
}

void APacmanEnemy::OnEnemyBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APacmanPlayer::StaticClass()))
	{
		if (bIsDead)
		{
			return;
		}
		if (bIsVulnerable)
		{
			SetDead();
		}
		else
		{
			Cast<APacmanPlayer>(OtherActor)->Injured();
		}
	}
}

