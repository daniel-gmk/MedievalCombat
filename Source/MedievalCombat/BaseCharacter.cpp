// Fill out your copyright notice in the Description page of Project Settings.

#include "MedievalCombat.h"
#include "BaseCharacter.h"
#include "UnrealNetwork.h"
#include "DrawDebugHelpers.h"

#define COLLISION_ATTACK ECC_GameTraceChannel4

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->SetVisibility(false);
	GetCapsuleComponent()->InitCapsuleSize(35.0f, 70.0f);

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = false; // Character moves in the direction of input...	

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Create a capsule component to avoid people going through eachother
	PlayerCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("PlayerCollision"));
	PlayerCollision->SetVisibility(false);
	PlayerCollision->InitCapsuleSize(70.0f, 120.0f);
	PlayerCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PlayerCollision->bDynamicObstacle = true;
	PlayerCollision->bGenerateOverlapEvents = false;
	PlayerCollision->SetupAttachment(RootComponent);
	//PlayerCollision->OnComponentBeginOverlap.AddDynamic(this, &ABSNOneCharacter::OnLedgeGrabOverlapBegin);
	//PlayerCollision->OnComponentEndOverlap.AddDynamic(this, &ABSNOneCharacter::OnLedgeGrabOverlapEnd);

	// Create weapon
	Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	Weapon->SetupAttachment(RootComponent);
	Weapon->SetVisibility(false);
	Weapon->SetHiddenInGame(true);
	Weapon->bGenerateOverlapEvents = false;

	/***** Create weapon hurtboxes *****/
	// Weapon Hurtbox Base
	WeaponHurtboxBase = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponHurtboxBase"));
	WeaponHurtboxBase->SetVisibility(false);
	WeaponHurtboxBase->SetHiddenInGame(true);
	WeaponHurtboxBase->SetBoxExtent(FVector(0.6f, 0.22f, 32.0f));
	WeaponHurtboxBase->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponHurtboxBase->bDynamicObstacle = false;
	WeaponHurtboxBase->bGenerateOverlapEvents = true;
	WeaponHurtboxBase->SetupAttachment(Weapon);

	// Hurtbox1
	Hurtbox1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Hurtbox1"));
	Hurtbox1->SetVisibility(false);
	Hurtbox1->SetHiddenInGame(true);
	Hurtbox1->SetBoxExtent(FVector(0.5f, 0.5f, 0.5f));
	Hurtbox1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Hurtbox1->bDynamicObstacle = false;
	Hurtbox1->bGenerateOverlapEvents = false;
	Hurtbox1->SetupAttachment(WeaponHurtboxBase);

	// Hurtbox2
	Hurtbox2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Hurtbox2"));
	Hurtbox2->SetVisibility(false);
	Hurtbox2->SetHiddenInGame(true);
	Hurtbox2->SetBoxExtent(FVector(0.5f, 0.5f, 0.5f));
	Hurtbox2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Hurtbox2->bDynamicObstacle = false;
	Hurtbox2->bGenerateOverlapEvents = false;
	Hurtbox2->SetupAttachment(WeaponHurtboxBase);

	// Hurtbox3
	Hurtbox3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Hurtbox3"));
	Hurtbox3->SetVisibility(false);
	Hurtbox3->SetHiddenInGame(true);
	Hurtbox3->SetBoxExtent(FVector(0.5f, 0.5f, 0.5f));
	Hurtbox3->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Hurtbox3->bDynamicObstacle = false;
	Hurtbox3->bGenerateOverlapEvents = false;
	Hurtbox3->SetupAttachment(WeaponHurtboxBase);

	// Hurtbox4
	Hurtbox4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Hurtbox4"));
	Hurtbox4->SetVisibility(false);
	Hurtbox4->SetHiddenInGame(true);
	Hurtbox4->SetBoxExtent(FVector(0.5f, 0.5f, 0.5f));
	Hurtbox4->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Hurtbox4->bDynamicObstacle = false;
	Hurtbox4->bGenerateOverlapEvents = false;
	Hurtbox4->SetupAttachment(WeaponHurtboxBase);

	// Hurtbox5
	Hurtbox5 = CreateDefaultSubobject<UBoxComponent>(TEXT("Hurtbox5"));
	Hurtbox5->SetVisibility(false);
	Hurtbox5->SetHiddenInGame(true);
	Hurtbox5->SetBoxExtent(FVector(0.5f, 0.5f, 0.5f));
	Hurtbox5->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Hurtbox5->bDynamicObstacle = false;
	Hurtbox5->bGenerateOverlapEvents = false;
	Hurtbox5->SetupAttachment(WeaponHurtboxBase);

	// Store the hurtboxes inside the hitbox component array
	HitboxComponentArray[0] = WeaponHurtboxBase;
	HitboxComponentArray[1] = Hurtbox1;
	HitboxComponentArray[2] = Hurtbox2;
	HitboxComponentArray[3] = Hurtbox3;
	HitboxComponentArray[4] = Hurtbox4;
	HitboxComponentArray[5] = Hurtbox5;
}

// Allows Replication of variables for Client/Server Networking
void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ABaseCharacter, IsDead);
	DOREPLIFETIME(ABaseCharacter, Health);
	DOREPLIFETIME(ABaseCharacter, CanAttack);
	DOREPLIFETIME(ABaseCharacter, CurrentAttackHit);
	DOREPLIFETIME(ABaseCharacter, IsBlocking);
	DOREPLIFETIME(ABaseCharacter, AttackWasBlocked);
	DOREPLIFETIME(ABaseCharacter, SuccessfullyDefended);
	DOREPLIFETIME(ABaseCharacter, RecoilAnim);
	DOREPLIFETIME(ABaseCharacter, BlockingAnimation);
	DOREPLIFETIME(ABaseCharacter, HoldingBlock);
	DOREPLIFETIME(ABaseCharacter, FlinchDuration);
	DOREPLIFETIME(ABaseCharacter, FlinchTrigger);
	DOREPLIFETIME(ABaseCharacter, Flinched);
	DOREPLIFETIME(ABaseCharacter, RollAnim);
	DOREPLIFETIME(ABaseCharacter, RollSpeed);
	DOREPLIFETIME(ABaseCharacter, Resilience);
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	HitboxHandler();
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Function for handling DELAY equivalent from Blueprints
void ABaseCharacter::onTimerEnd()
{
}

void ABaseCharacter::HitboxHandler() {
	if (this->HasAuthority()) {
		if (CanDamage == true) {
			if (InitialHitbox == true) {
				InitialHitbox = false;
				FillHitboxArray();
			}
			const FName TraceTag("MyTraceTag"); // Trace for debug

			GetWorld()->DebugDrawTraceTag = TraceTag; // Trace for debug

			FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
			RV_TraceParams.TraceTag = TraceTag; // Trace for debug
			RV_TraceParams.bTraceComplex = true;
			RV_TraceParams.bTraceAsyncScene = true;
			RV_TraceParams.bReturnPhysicalMaterial = false;
			RV_TraceParams.bIgnoreBlocks = false;

			// Trace lines for each vector stored in the vector array
			for (int i = 0; i < 6; i++) {

				//Re-initialize hit info
				FHitResult Out_Hit(ForceInit);

				//call GetWorld() from within an actor extending class
				 if (GetWorld()->LineTraceSingleByChannel(Out_Hit, HitboxComponentArray[i]->GetSocketLocation(""), HitboxArray[i], COLLISION_ATTACK, RV_TraceParams) == true) {
					WeaponHitEvent(Out_Hit);
					break;
				}
			}
			FillHitboxArray();
		}
		else {
			if (InitialHitbox == false) {
				InitialHitbox = true;
			}
		}
	}
}

//Decrements cooldown by .1 every time called, if cd > 0
void ABaseCharacter::CooldownDecrement(UPARAM(ref) float cd, UPARAM(ref) FTimerHandle& Handle)
{
	if (cd > 0)
	{
		cd -= .1;
	}
	else
	{
		cd = 0;
		GetWorld()->GetTimerManager().ClearTimer(Handle);
	}
}

//Attempt at Roll Direction Handler
void ABaseCharacter::RollDirectionHandler()
{
	if (IsRolling == true)
	{
		// Left or Right input
		AddMovementInput(GetActorRightVector(), (CurrentLRLoc * RollSpeed));

		// Forward or Back input
		if (CurrentFBLoc != 0)
		{
			AddMovementInput(GetActorForwardVector(), (CurrentFBLoc * RollSpeed));
		}
		else if (CurrentFBLoc == 0 && CurrentLRLoc == 0)
		{
			AddMovementInput(GetActorForwardVector(), (.5 * RollSpeed));
		}

	}
}

void ABaseCharacter::WeaponHitEvent(FHitResult HitResult) {
	if (HitResult.GetActor() != this) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Some debug message!"));
	}
}

/* Please only put helper functions here (functions called by a main function) */
void ABaseCharacter::FillHitboxArray() {
	// Store the hurtboxes inside the hitbox component array
	HitboxArray[0] = WeaponHurtboxBase->GetSocketLocation("");
	HitboxArray[1] = Hurtbox1->GetSocketLocation("");
	HitboxArray[2] = Hurtbox2->GetSocketLocation("");
	HitboxArray[3] = Hurtbox3->GetSocketLocation("");
	HitboxArray[4] = Hurtbox4->GetSocketLocation("");
	HitboxArray[5] = Hurtbox5->GetSocketLocation("");
}