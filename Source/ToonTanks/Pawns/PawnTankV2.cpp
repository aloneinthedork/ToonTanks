// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTankV2.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnTankV2::APawnTankV2()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTankV2::BeginPlay()
{
    Super::BeginPlay();
    PlContRef = Cast<APlayerController>(GetController());
}

// Called every frame
void APawnTankV2::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    Rotate();
    Move();

    if (PlContRef) {
        FHitResult TraceHitResult;
        PlContRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
        FVector HitLocation = TraceHitResult.ImpactPoint;

        RotateTurretFunction(HitLocation);
    }
}

// Called to bind functionality to input
void APawnTankV2::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &APawnTankV2::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn", this, &APawnTankV2::CalculateRotateInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTankV2::Fire);
}

void APawnTankV2::CalculateMoveInput(float Value) 
{
    MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}
void APawnTankV2::CalculateRotateInput(float Value) 
{
    float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
    FRotator Rotation = FRotator(0, RotateAmount, 0);
    RotationDirection = FQuat(Rotation);
}

void APawnTankV2::Move() 
{
    AddActorLocalOffset(MoveDirection, true);
}

void APawnTankV2::Rotate() 
{
    AddActorLocalRotation(RotationDirection, true);
}

void APawnTankV2::HandleDestruction()
{
    Super::HandleDestruction();
    bIsPlayerAlive = false;

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);

}