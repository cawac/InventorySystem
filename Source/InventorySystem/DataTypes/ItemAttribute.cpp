#include "ItemAttribute.h"
#include "Components/InventoryComponentBase.h"

void FItemAttribute::SetItemInfo(UInventoryComponentBase* InventoryComponent, const FGuid& Guid)
{
    if (InventoryComponent == nullptr || !Guid.IsValid())
    {
        return;
    }
    InventoryComp = InventoryComponent;
    ItemGuid = Guid;
}
