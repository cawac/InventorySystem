#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "ItemSlot.generated.h"

class UInventoryComponentBase;
class UItemData;

/**
 * Class which represents item in inventory
 */
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemSlot
{
    GENERATED_BODY()

public:
    /**
     * Default constructor
     */
    FItemSlot() {}

    /**
     * Construct which calls when need to create item
     */
    FItemSlot(UInventoryComponentBase* InventoryComponent, const FGuid& Guid,
               const UItemData* NewItemData, const int32 AmountOfItems);

public:
    /**
     * Function which finds dynamic attribute of item
     * @return Returns specified attribute if exists, otherwise return nullptr
     */
    template <typename T>
    T* FindProperty();

    /**
     * Validate item slot 
     * @return Returns result of validating
     */
    bool IsValid() const { return ItemData != nullptr && AmountOf != INDEX_NONE; }
    
    const FGuid& GetItemGuid() const { return ItemGuid; }

    const UItemData* GetItemData() const { return ItemData; }

    TArray<FInstancedStruct> GetAttributes() { return ItemAttributes; }

    int32 GetAmountOf() const { return AmountOf; }

    void SetAmountOf(const int32 NewAmountOf) { AmountOf = NewAmountOf; }

    
protected:
    /**
     * Amount of items in slot 
     */
    UPROPERTY(BlueprintReadOnly)
    int32 AmountOf = INDEX_NONE;

    /**
     * Pointer to static item data
     */
    UPROPERTY(BlueprintReadOnly)
    const UItemData* ItemData = nullptr;

    /**
     * Dynamic attributes of item 
     */
    UPROPERTY(BlueprintReadOnly, meta = (BaseStruct = "ItemAttribute"))
    TArray<FInstancedStruct> ItemAttributes;

    /**
     * Guid of item
     */
    FGuid ItemGuid;
};

template <typename T>
T* FItemSlot::FindProperty()
{
    for (FInstancedStruct& Property : ItemAttributes)
    {
        if (T* PropertyPtr = Property.GetMutablePtr<T>())
        {
            return PropertyPtr;
        }
    }
    return nullptr;
};
