
#include "UpgradeComponent.h"

// Sets default values for this component's properties
UUpgradeComponent::UUpgradeComponent()
{	
	//pee pee poo poo    
}

void UUpgradeComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!UpgradeDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpgradeDataTable is not set!"));
	}

}


void UUpgradeComponent::AddUpgrade(const FString& UpgradeName, int32 UpgradeKey)
{
	if (UnlockedUpgrades.Contains(UpgradeName))
	{
		// If it does, increment the key value by 1
		int32* TierPtr = UnlockedUpgrades.Find(UpgradeName);
		if (TierPtr)
		{
			*TierPtr += UpgradeKey;
            //ue log for upgrade name and tier
            UE_LOG(LogTemp, Warning, TEXT("Upgrade %s is now at tier %d"), *UpgradeName, *TierPtr);
		}
	}
	else
	{
		//add to the map with a key value of 1
		UnlockedUpgrades.Add(UpgradeName, 1);
	}
	
}

int32 UUpgradeComponent::QueryUpgrade(const FString& UpgradeName)
{
	if (UnlockedUpgrades.Contains(UpgradeName))
	{
	//return the tier value
		int32* TierPtr = UnlockedUpgrades.Find(UpgradeName);
		if (TierPtr)
		{
            UE_LOG(LogTemp, Warning, TEXT("Upgrade %s is currently at tier %d"), *UpgradeName, *TierPtr);
			return *TierPtr;
		}
	}
	return 0;
}

TArray<FName> UUpgradeComponent::FindThreeUpgrades()
{
    TArray<FName> SelectedUpgradeRowName;

    if (UpgradeDataTable)
    {
        TArray<FName> RowNames;
        RowNames = UpgradeDataTable->GetRowNames();
        int32 NumRows = RowNames.Num();
        if (NumRows > 0)
        {
            // Create a copy of RowNames to track available upgrades
            TArray<FName> AvailableRowNames = RowNames;
            // Calculate total rarity of all upgrades
            float TotalRarity = 0.0f;
            for (const FName& RowName : RowNames)
            {
                FUpgradeData* RowData = UpgradeDataTable->FindRow<FUpgradeData>(RowName, TEXT(""));
                if (RowData)
                {
                    TotalRarity += RowData->Rarity;
                }
            }
            // three upgrades
            for (int32 i = 0; i < 3; i++)
            {
                // Generate a random value between 0 and total rarity
                float RandomRarity = FMath::FRandRange(0.0f, TotalRarity);

                // Iterate through the upgrades to find the selected upgrade
                float CumulativeRarity = 0.0f;
                for (int32 j = 0; j < AvailableRowNames.Num(); j++)
                {
                    FName RowName = AvailableRowNames[j];
                    FUpgradeData* RowData = UpgradeDataTable->FindRow<FUpgradeData>(RowName, TEXT(""));
                    if (RowData)
                    {
                        CumulativeRarity += RowData->Rarity;
                        if (RandomRarity <= CumulativeRarity)
                        {
                            //print percentage chance of upgrade
                            UE_LOG(LogTemp, Warning, TEXT("Chance of %s: %f%%"), *RowName.ToString(), (RowData->Rarity / TotalRarity) * 100);

                            // Found the selected upgrade
                            SelectedUpgradeRowName.Add(RowName);

                            // Remove the selected upgrade from the available upgrades
                            AvailableRowNames.RemoveAt(j);

                            // Update total rarity after removing the selected upgrade
                            TotalRarity -= RowData->Rarity;
                            break;
                        }
                    }
                }
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("UpgradeDataTable has no rows"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UpgradeDataTable is not set"));
    }

    return SelectedUpgradeRowName;
}


