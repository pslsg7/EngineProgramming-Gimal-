// Fill out your copyright notice in the Description page of Project Settings.

#include "TheRaidGameInstance.h"


UTheRaidGameInstance::UTheRaidGameInstance()
{
	FString CharacterDataPath = TEXT("/Game/TheRaid/GameData/TheRaidCharacterData.TheRaidCharacterData");
	static ConstructorHelpers::FObjectFinder<UDataTable> DT_COUNTESS(*CharacterDataPath);
	ABCHECK(DT_COUNTESS.Succeeded());
	TheRaidCharacterTable = DT_COUNTESS.Object;
	//ABCHECK(TheRaidCharacterTable->RowMap.Num() > 0);
	ABCHECK(TheRaidCharacterTable->GetRowMap().Num() > 0);
}

void UTheRaidGameInstance::Init()
{
	Super::Init();
	ABLOG(Warning, TEXT("DropExp of Level 20 Countess : %d"), GetTheRaidCharacterData(20)->DropExp);
}

FTheRaidCharacterData * UTheRaidGameInstance::GetTheRaidCharacterData(int32 Level)
{
	return TheRaidCharacterTable->FindRow<FTheRaidCharacterData>(*FString::FromInt(Level), TEXT(""));
}
