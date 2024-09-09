
#include "FP_player.h"
#include <Kismet/GameplayStatics.h>

AFP_player::AFP_player() //constructor //initialises during editor time
{
	PrimaryActorTick.bCanEverTick = true;   
    
    PlayerUpgradeComponent = CreateDefaultSubobject<UUpgradeComponent>(TEXT("Player Upgrades C++")); // add component to player if (!PlayerUpgradeComponent)
    if (!PlayerUpgradeComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpgradeComponent is not set!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UpgradeComponent is set!"));

        PlayerUpgradeComponent->UpgradeDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/UpgradeDataTable"));
        if (!PlayerUpgradeComponent->UpgradeDataTable)
        {
            UE_LOG(LogTemp, Warning, TEXT("UpgradeComponentTable is not set!"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("UpgradeComponentTable is set!"));
        }
    }
    Tags.AddUnique("Player");
}


void AFP_player::BeginPlay() //initialises during runtime
{
	Super::BeginPlay();

    PlayerUpgradeComponent = NewObject<UUpgradeComponent>(this, UUpgradeComponent::StaticClass(), TEXT("Player Upgrades C++"), RF_NoFlags);
    PlayerUpgradeComponent->RegisterComponent(); // add component to player
    if (!PlayerUpgradeComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("UpgradeComponent is not set!"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UpgradeComponent is set!"));

        PlayerUpgradeComponent->UpgradeDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/UpgradeDataTable"));
        if (!PlayerUpgradeComponent->UpgradeDataTable)
        {
            UE_LOG(LogTemp, Warning, TEXT("UpgradeComponentTable is not set!"));
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("UpgradeComponentTable is set!"));
        }
    }

    //starting player stats
    CurrentHealth = 100;
    MaxHealth = 100;
    XP = 0;
    XPToLevel = 3.0f;
    Level = 0;
    FireRate = 0.25f;
    CanFire = true;
    bIsFiring = false;
    MuzzleOffset = FVector(100.0f, 0.0f, 0.0f);
    ProjectileClass = AFPSProjectile::StaticClass();

    for (int32 i = 0; i < 3; ++i)
    {
        CurrentThreeUpgrades.Add(TEXT("Null Upgrade"));
    }
    if (PlayerUpgradeComponent)
    {
        PlayerUpgradeComponent->UnlockedUpgrades.Empty(); //wipe the map clean before starting;
    }
	else
	{
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("PLAYER UPGRADE COMPONENT NOT ATTACHED"));
	}



	check(GEngine != nullptr)
}

void AFP_player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    Seconds += DeltaTime;

    if (Seconds > 59.0f)
    {
        Minutes++;
        Seconds = 0;
    }
}

// Called to bind functionality to input
void AFP_player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up "movement" bindings.
	PlayerInputComponent->BindAxis("MoveForward", this, &AFP_player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFP_player::MoveRight);
	// Set up "look" bindings.
	PlayerInputComponent->BindAxis("Turn", this, &AFP_player::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &AFP_player::AddControllerPitchInput);
	// Set up "action" bindings.
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFP_player::StartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AFP_player::StopJump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFP_player::StartFiring);
    PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFP_player::StopFiring);
}

void AFP_player::MoveForward(float Value)
{
    // Find out which way is "forward" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::X);
    Direction.Z = 0.0f; // Remove the Z-component to prevent slowing down movement when looking up or down
    // Normalize the forward direction to maintain constant speed regardless of the camera angle
    Direction.Normalize();
    AddMovementInput(Direction, Value * MovementSpeedModifier);
}

void AFP_player::MoveRight(float Value)
{
    // Find out which way is "right" and record that the player wants to move that way.
    FVector Direction = FRotationMatrix(Controller->GetControlRotation()).GetScaledAxis(EAxis::Y);
    AddMovementInput(Direction, Value * MovementSpeedModifier);
}

void AFP_player::StartFiring()
{
    bIsFiring = true;
    GetWorldTimerManager().SetTimer(AutoFireRateTimerHandle, this, &AFP_player::Fire, FMath::Max(FireRate, 0.001f), true);
    Fire(); // Initial shot when button is pressed
}

void AFP_player::StopFiring()
{
    bIsFiring = false;
    GetWorldTimerManager().ClearTimer(AutoFireRateTimerHandle);
}

void AFP_player::StartJump()
{
	bPressedJump = true;
}

void AFP_player::StopJump()
{
	bPressedJump = false;
}

void AFP_player::Fire()
{
    
    // Attempt to fire a projectile.
    if (ProjectileClass)
    {
        // Get the camera transform.
        FVector CameraLocation;
        FRotator CameraRotation;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);

        // Set MuzzleOffset to spawn projectiles slightly in front of the camera.
        MuzzleOffset.Set(50.0f, 0.0f, 0.0f);

        // Transform MuzzleOffset from camera space to world space.
        FVector MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

        FRotator MuzzleRotation = CameraRotation;
        MuzzleRotation.Pitch += 5.0f;

        UWorld* World = GetWorld();
        if (World)
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.Owner = this;
            SpawnParams.Instigator = GetInstigator();

            if (CanFire)
            {
                // spawn from muzzle

                Projectile = World->SpawnActor<AFPSProjectile>(
                    ProjectileClass,
                    MuzzleLocation,
                    MuzzleRotation,
                    SpawnParams
                );
                Projectile->SetLifeSpan(FMath::Max(val_lifespan, 0.2f));                
                Projectile->ProjectileMovementComponent->bShouldBounce = val_bShouldBounce;
                Projectile->ProjectileMovementComponent->Bounciness = val_Bounciness;
                Projectile->ProjectileMovementComponent->InitialSpeed = FMath::Max(val_InitialSpeed, 10.0f);
                Projectile->ProjectileMovementComponent->MaxSpeed = FMath::Max(val_MaxSpeed, 10.0f);
                Projectile->ProjectileMovementComponent->ProjectileGravityScale = val_ProjectileGravityScale;
                Projectile->Damage = FMath::Max(val_Damage, 0.5f);
                Projectile->CollisionComponent->InitSphereRadius(val_CollisionSphere);
                Projectile->ProjectileMeshComponent->SetRelativeScale3D(FVector(FMath::Max(val_ProjectileScale,0.01)));
                /**/
                
                UGameplayStatics::PlaySoundAtLocation(GetWorld(), ShootingSFX, MuzzleLocation);


                if (Projectile)
                {
                    //initial trajectory.
                    FVector LaunchDirection = MuzzleRotation.Vector();
                    Projectile->FireInDirection(LaunchDirection);

                    // Check for the "Extra Projectile" upgrade
                    int32 ExtraProjectiles = PlayerUpgradeComponent->QueryUpgrade("extra_projectile");
                    if (ExtraProjectiles > 0)
                    {
                        for (int32 i = 0; i < ExtraProjectiles; ++i)
                        {
                            // Offset the position of the extra projectiles slightly
                            //FVector Offset = FVector::RightVector * (i + 1) * 50.0f; // Adjust the offset as needed
                            //math random for y offset
                            float RandomYOffset = FMath::RandRange(-30.0f, 30.0f);
                            float RandomZOffset = FMath::RandRange(-30.0f, 30.0f);
                            MuzzleOffset.Set(125.0f, RandomYOffset, RandomZOffset); // x,y,z
                            MuzzleLocation = CameraLocation + FTransform(CameraRotation).TransformVector(MuzzleOffset);

                            AFPSProjectile* ExtraProjectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

                            if (ExtraProjectile)
                            {
                                ExtraProjectile->SetLifeSpan(FMath::Max(val_lifespan, 0.2f));
                                ExtraProjectile->ProjectileMovementComponent->bShouldBounce = val_bShouldBounce;
                                ExtraProjectile->ProjectileMovementComponent->Bounciness = val_Bounciness;
                                ExtraProjectile->ProjectileMovementComponent->InitialSpeed = FMath::Max(val_InitialSpeed, 10.0f);
                                ExtraProjectile->ProjectileMovementComponent->MaxSpeed = FMath::Max(val_MaxSpeed, 10.0f);
                                ExtraProjectile->ProjectileMovementComponent->ProjectileGravityScale = val_ProjectileGravityScale;
                                ExtraProjectile->Damage = FMath::Max(val_Damage, 0.5f);
                                ExtraProjectile->CollisionComponent->InitSphereRadius(val_CollisionSphere);
                                ExtraProjectile->ProjectileMeshComponent->SetRelativeScale3D(FVector(FMath::Max(val_ProjectileScale, 0.01)));
                                ExtraProjectile->FireInDirection(LaunchDirection);
                            }
                        }
                    }
                }

                CanFire = false; // start of cooldown period
                GetWorldTimerManager().SetTimer(FireRateTimerHandle, this, &AFP_player::ResetCanFire, FMath::Max(FireRate, 0.001f) , false);
            }
        }
    }
}

void AFP_player::ResetCanFire()
{
    CanFire = true; // end of cooldown period
}

float AFP_player::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float DamageCaused = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    DamageCaused =  FMath::Min(CurrentHealth, DamageCaused);
    CurrentHealth -= DamageCaused;
    //UE_LOG(LogTemp, Log, TEXT("Damage amount: %f"), DamageAmount);
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, TEXT("Player is taking damage"));
    if (CurrentHealth <= 0)
    {
        // GEngine->AddOnScreenDebugMessage(-1, 99.f, FColor::Red, TEXT("Player is DEAD"));
        //UGameplayStatics::OpenLevel(GetWorld(), FName("Menu")); //temp fix //change this later to be more smooth
        DEATH(); //event trigger for blueprint
	}
    return 0.0f;
}

void AFP_player::GainXP(float XPAmount)
{
    XP += XPAmount * XPModifier;
        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Player gained %f XP"), XPAmount));
        //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Player has %f XP"), XP));
	if (XP >= XPToLevel)
	{
		LevelUp();
	}
}

void AFP_player::LevelUp()
{
    Level++;
    XP -= XPToLevel;
    XPToLevel *= 1.5f;
    StopFiring();
    if (PlayerUpgradeComponent)
    {
        CurrentThreeUpgrades = PlayerUpgradeComponent->FindThreeUpgrades();

        LevelUpTRIGGER(); //event trigger for blueprint
        //ApplyUpgradeEffect();
    }
    else
    {
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("PLAYER UPGRADE COMPONENT NOT ATTACHED"));
	}
    //GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Player leveled up to level %d"), Level));

}

void AFP_player::ApplyUpgradeEffect() 
{    
    switch (TempRow.UpgradeID)
    {
    case 1://  Faster Fire Rate
        FireRate -= 0.025f;
        break;

    case 2://  Larger Projectile
        val_CollisionSphere += 3.0f; //collision size of projectile
        val_ProjectileScale += 0.027f; //visual size of projectile
        break;

    case 3://  Faster Projectile
        val_InitialSpeed += 300.0f;
        val_MaxSpeed += 300.0f;
        break;

    case 4://  Bouncing Projectile
        val_bShouldBounce = true;
        val_ProjectileGravityScale = 0.8f;
        val_Bounciness += 0.4f;
        val_lifespan += 0.5f;
        break;

    case 5://  Exploding Projectile
        break;

    case 6://  Extra Projectile
        val_ProjectileScale -= 0.02f; //visual size of projectile
        val_Damage -= 1.0f;

        //code is in the Fire function
        break;

    case 7://  More Damage
        val_Damage *= 2.5f;
        break;

    case 8://  Damage Over Time
        break;

    case 9://  Faster XP Gain
        XPModifier += 3.0f;
        break;

    case 10://  Super Rare
        XPModifier += 5.0f;
        MovementSpeedModifier += 0.5f;
        CurrentHealth = MaxHealth;
        MaxHealth *= 2.0f;
        CurrentHealth += MaxHealth;
        break;

    case 11://  Speed Boost
        MovementSpeedModifier += 0.2f;
		break;

    case 12: //  Restore Health
        CurrentHealth = MaxHealth;
		break;

    case 13: // Longer Range
        val_lifespan += 0.5f;
        break;

    case 14: // Increase max health
        MaxHealth += 20.0f;
        CurrentHealth = MaxHealth;
		break;
    case 15: //Overhealth
        CurrentHealth += MaxHealth;
        break;

    case 16: //  Spreadshot
        for (int32 i = 0; i < 3; ++i) 
        {
            PlayerUpgradeComponent->AddUpgrade("extra_projectile", 1);
        }
        val_ProjectileScale -= 0.04f; //visual size of projectile
        val_CollisionSphere -= 6.0f; //collision size of projectile
        val_Damage -= 4.5f;
        val_lifespan -= 0.5f;
        break;

    case 17: //longshot
        val_lifespan += 1.0f;
        val_InitialSpeed += 900.0f;
        val_MaxSpeed += 900.0f;
        val_ProjectileScale -= 0.02f; //visual size of projectile
        val_CollisionSphere -= 3.0f; //collision size of projectile
		break;

    case 18: //minigun
        FireRate -= 0.15f;
        val_Damage -= 3.0f;
		break;

    case 19:// bulkshot
        val_CollisionSphere += 5.0f; //collision size of projectile
        val_ProjectileScale += 0.07f; //visual size of projectile
        val_Damage += 7.0f;
        val_lifespan -= 0.4f;
        FireRate += 0.25f;
        break;

    case 20: //whacky
        val_bShouldBounce = true;
        val_ProjectileGravityScale = 0.9f;
        val_Bounciness += 1.0f;
        val_lifespan += 10.0f;
        val_Damage += 5.0f;
        break;

    case 21: // reroll
        XPToLevel /= 1.5f;
        XP = XPToLevel;
        LevelUp();
        break;

    default:
        // Handle unknown upgrade ID
        break;
    }
}
