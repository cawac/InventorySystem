#include "ItemSlot.h"
#include "Components/InventoryComponentBase.h"
#include "ItemData.h"
#include "ItemAttribute.h"

FItemSlot::FItemSlot(UInventoryComponentBase* InventoryComponent, const FGuid& Guid,
    const UItemData* NewItemData, const int32 AmountOfItems)
{
    if (InventoryComponent == nullptr || !Guid.IsValid() || NewItemData == nullptr || AmountOfItems <= 0)
    {
        return;
    }
    ItemData = NewItemData;
    ItemGuid = Guid;
    AmountOf = AmountOfItems;
    for (FInstancedStruct& Attribute : ItemData->GetAttributes())
    {
        FItemAttribute* AttributeCopy = Attribute.GetMutablePtr<FItemAttribute>();
        if (AttributeCopy == nullptr)
        {
            continue;
        }
        AttributeCopy->SetItemInfo(InventoryComponent, Guid);
        ItemAttributes.Add(Attribute);
    }
}
