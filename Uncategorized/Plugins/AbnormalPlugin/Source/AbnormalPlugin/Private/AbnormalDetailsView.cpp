#include "AbnormalDetailsView.h"
#include "IDetailGroup.h"
#include "IDetailPropertyRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyTypeCustomization.h"
#include "PropertyHandle.h"

TSharedRef<IPropertyTypeCustomization> FAbnormalDetailsView::MakeInstance()
{
    return MakeShareable(new FAbnormalDetailsView());
}

void FAbnormalDetailsView::CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder &StructBuilder, IPropertyTypeCustomizationUtils &StructCustomizationUtils)
{
    uint32 NumChildren;
    StructPropertyHandle->GetNumChildren(NumChildren);
    FString Name = StructPropertyHandle->GetPropertyDisplayName().ToString();
    IDetailGroup &Group = StructBuilder.AddGroup(FName(*Name), FText::FromString(Name));

    for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
    {
        TSharedRef<IPropertyHandle> ChildHandle = StructPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();
        //IDetailPropertyRow& Property = StructBuilder.AddProperty(ChildHandle);

        IDetailPropertyRow &Property = Group.AddPropertyRow(ChildHandle);
        if (ChildHandle->GetProperty()->GetName() == "AbnormalActor"
        || ChildHandle->GetProperty()->GetName() == "AbnormalClass"
        || ChildHandle->GetProperty()->GetName() == "AdditiveTransform"
        || ChildHandle->GetProperty()->GetName() == "PlayCount")
        {
            continue;
        }


        if (ChildHandle->GetProperty()->GetFName() == "TargetTransformSource")
        {
            EnumProperty = ChildHandle;
        }
        else
        {
            static const FName EditConditionName = "EnumCondition";
            int32 EnumCondition = ChildHandle->GetIntMetaData(EditConditionName);
            Property.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAbnormalDetailsView::ArePropertiesVisible, EnumCondition)));
        }
    }
}

EVisibility FAbnormalDetailsView::ArePropertiesVisible(const int32 VisibleType) const
{
    uint8 CurrentEnumValue = 0;
    EnumProperty->GetValue(CurrentEnumValue);
    return (CurrentEnumValue == VisibleType) ? EVisibility::Visible : EVisibility::Collapsed;
}