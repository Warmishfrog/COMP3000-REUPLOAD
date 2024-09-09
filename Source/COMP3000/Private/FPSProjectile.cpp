
#include "FPSProjectile.h"
#include <BasicEnemy.h>
#include "Engine/DamageEvents.h"

AFPSProjectile::AFPSProjectile()
{
	PrimaryActorTick.bCanEverTick = true;


    if (!RootComponent)
    {
        RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
    }

    if (!CollisionComponent)
    {
        CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
        CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
        CollisionComponent->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit); //hit
        CollisionComponent->InitSphereRadius(CollisionRadius); //collision radius
        RootComponent = CollisionComponent;
    }

    if (!ProjectileMovementComponent)
    {
        //movement manipulation
        ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
        ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
        ProjectileMovementComponent->InitialSpeed = InitialSpeed;
        ProjectileMovementComponent->MaxSpeed = MaxSpeed;
        ProjectileMovementComponent->bRotationFollowsVelocity = true;
        ProjectileMovementComponent->bShouldBounce = bShouldBounce;
        ProjectileMovementComponent->Bounciness = Bounciness;
        ProjectileMovementComponent->ProjectileGravityScale = 0.1f;
    }
    if (!ProjectileMeshComponent)
    {
        ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
        static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("/Script/Engine.StaticMesh'/Game/Sphere.Sphere'"));
        if (Mesh.Succeeded())
        {
            ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
        }
        static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("/Script/Engine.Material'/Game/SphereMaterial.SphereMaterial'"));
        if (Material.Succeeded())
        {
            ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
        }
        ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
        ProjectileMeshComponent->SetRelativeScale3D(FVector(ProjectileScale)); //visual size of projectile
        ProjectileMeshComponent->SetupAttachment(RootComponent);
    }
    
    Tags.AddUnique("Projectile");
}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Function that initializes the projectile's velocity in the shoot direction.
void AFPSProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

// Function that is called when the projectile hits something.
void AFPSProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor && OtherActor != this)
    {
        if (OtherActor->ActorHasTag("Player"))
        {
            return;
        }

        if (OtherActor->ActorHasTag("Projectile"))
        {

            return;
        }
        //if (OtherComponent->IsSimulatingPhysics())
        {
            //OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
           

            ABasicEnemy* HitBasicEnemy = Cast<ABasicEnemy>(OtherActor);
            if (HitBasicEnemy)
            {

                AController* ProjectileInstigatorController = GetInstigatorController(); // Get the controller of the actor that fired the projectile

                HitBasicEnemy->TakeDamage(Damage, FDamageEvent(), ProjectileInstigatorController, this);
                Destroy();

            }
        }
    }    
    else {
        Destroy();
    }
}

