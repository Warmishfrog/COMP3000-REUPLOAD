// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradeComponent.generated.h"

USTRUCT(BlueprintType)
struct FUpgradeData: public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int UpgradeID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Rarity;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Thumbnail;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Tiers;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FColor Colour;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class COMP3000_API UUpgradeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UUpgradeComponent();
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FUpgradeData UpgradeInfo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    class UDataTable* UpgradeDataTable;
    

    // Unlocked Upgrades
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TMap<FString, int32> UnlockedUpgrades; //string would be the name of the upgrade and integer is the tier (key),

    // Function to add an upgrade
    UFUNCTION(BlueprintCallable)
    void AddUpgrade(const FString& UpgradeName, int32 UpgradeKey);

    //Function to Query if an upgrade is unlocked
    UFUNCTION(BlueprintCallable)
    int32 QueryUpgrade(const FString& UpgradeName);

    //Function to find three random upgrades
    UFUNCTION(BlueprintCallable)
    TArray<FName> FindThreeUpgrades();

protected:
	virtual void BeginPlay() override;

};
