#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InstancedStruct.h"
#include "ItemData.generated.h"

/**
 * Data Asset for set static info about item 
 */
UCLASS(BlueprintType)
class INVENTORYSYSTEM_API UItemData: public UDataAsset
{
    GENERATED_BODY()

public:
    /**
     * Default constructor
     */
    UItemData() {}

public:
    int32 GetStackMaxSize() const { return StackMaxSize; } 

    FText GetItemName() { return Name; }

    UTexture2D* GetThumbnail() const { return Thumbnail; }

    FText GetDescription() const { return Description; }

    UStaticMesh* GetMesh() const { return Mesh; }

    TArray<FInstancedStruct> GetAttributes() const { return Attributes; }

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "1"))
    int32 StackMaxSize = 1;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UTexture2D* Thumbnail = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FText Description;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UStaticMesh* Mesh = nullptr;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (BaseStruct = "ItemAttribute"))
    TArray<FInstancedStruct> Attributes;
};