#pragma once

#include "CoreMinimal.h"
#include "InventorySystem/Components/InventoryComponentBase.h"
#include "ItemAttribute.generated.h"

/**
 * Represent base item's attribute
 */
USTRUCT(BlueprintType)
struct INVENTORYSYSTEM_API FItemAttribute 
{
    GENERATED_BODY()
   
public:
    /**
     * Default constructor
     */
    FItemAttribute() {}

    /**
     * Sets attribute variables
     * @param InventoryComponent Pointer to owner inventory
     * @param Guid Item's guid
     */
    void SetItemInfo(UInventoryComponentBase* InventoryComponent, const FGuid& Guid);
   
protected:
    FGuid ItemGuid;
    
    UPROPERTY()
    UInventoryComponentBase* InventoryComp = nullptr;
};