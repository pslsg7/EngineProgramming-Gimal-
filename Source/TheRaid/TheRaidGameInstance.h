// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TheRaid.h"
#include "Engine/DataTable.h"
#include "Engine/GameInstance.h"
#include "TheRaidGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FTheRaidCharacterData : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FTheRaidCharacterData() : Level(1), MaxHP(100.0f), Attack(10.0f), DropExp(10), NextExp(30) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float MaxHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 DropExp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int32 NextExp;
};

/**
 * 
 */
UCLASS()
class THERAID_API UTheRaidGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UTheRaidGameInstance();

	virtual void Init() override;
	FTheRaidCharacterData* GetTheRaidCharacterData(int32 Level);

private:
	UPROPERTY()
	class UDataTable* TheRaidCharacterTable;
	
};
