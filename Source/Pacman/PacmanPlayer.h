// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PacmanPlayer.generated.h"

UCLASS()
class PACMAN_API APacmanPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APacmanPlayer();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UStaticMeshComponent* PacmanDisplayMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* PacmanEye1;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* PacmanEye2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Lifes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float NormalSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float InvincibleTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void Injured();
	void Resetting();
	void SetMovement(bool bCanMove);

	void StartGame();
	void PauseGame();
	void RestartGame();

	void MoveX(float value);
	void MoveY(float value);

	UFUNCTION()
	void OnPacmanBeginOverlay(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	FVector Velocity;
	FVector StartLocation;
	class APacmanGameModeBase* GameModeRef;
	FTimerHandle ResettingTimerHandle;
};
