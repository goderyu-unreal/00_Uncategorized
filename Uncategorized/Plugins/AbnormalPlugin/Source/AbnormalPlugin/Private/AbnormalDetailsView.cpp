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

        IDetailPropertyRow &Property = Group.AddPropertyRow(ChildHandle);

        if (ChildHandle->GetProperty()->GetFName() == "AbnormalActorType")
        {
            AbnormalActorTypeProperty = ChildHandle;
            continue;
        }

        if (ChildHandle->GetProperty()->GetFName() == "TargetTransformSource")
        {
            TargetTransformSourceProperty = ChildHandle;
        }


        // 获取该属性在代码中设置的AbnormalActorType的枚举值
        static const FName AbnormalActorTypeName = "AbnormalActorType";
        int32 AbnormalActorType = ChildHandle->GetIntMetaData(AbnormalActorTypeName);
        // 获取该属性在代码中设置的TargetTransformSource的枚举值
        static const FName TargetTransformName = "TargetTransformSource";
        int32 TargetTransform = ChildHandle->GetIntMetaData(TargetTransformName);
        int32 Flag = AbnormalActorType | TargetTransform;
        // 单个属性的可见性只能绑定一个函数，如果绑定多个，会以最后绑定的那个为主
        Property.Visibility(TAttribute<EVisibility>::Create(TAttribute<EVisibility>::FGetter::CreateSP(this, &FAbnormalDetailsView::ArePropertiesVisible, Flag)));
    }
}

EVisibility FAbnormalDetailsView::ArePropertiesVisible(const int32 VisibleType) const
{
    // 获取该属性在细节面板设置的AbnormalActorType的枚举值
    uint8 AbnormalActorTypeValue = 0;
    AbnormalActorTypeProperty->GetValue(OUT AbnormalActorTypeValue);
    // 获取该属性在细节面板设置的TargetTransformSource的枚举值
    uint8 TargetTransformSourceValue = 0;
    TargetTransformSourceProperty->GetValue(OUT TargetTransformSourceValue);

    // 采用位运算，提取该属性设置的TargetTransformSource的枚举值
    // 如果为0，说明没有设置该枚举值，那么该值不能控制属性的可见性
    if ((VisibleType & (uint8)62) == 0)
    {
        // 如果属性中没有填写显隐限制，则无论何时都不应该隐藏
        return EVisibility::Visible;
    }
    else if ((VisibleType & ~(uint8)48) == 0)
    {
        // 如果属性中没有填写TargetTransformSource的显隐限制，则显隐只受AbnormalActorType控制
        return (((VisibleType & (uint8)48) & AbnormalActorTypeValue) != 0) ? EVisibility::Visible : EVisibility::Collapsed;
    }
    else if ((VisibleType & ~(uint8)14) == 0)
    {
        // 如果属性中没有填写AbnormalActorType的显隐限制，则显隐只受TargetTransformSource控制
        return (((VisibleType & (uint8)14) & TargetTransformSourceValue) != 0) ? EVisibility::Visible : EVisibility::Collapsed;
    }
    else
    {
        return (((VisibleType & (uint8)48) & AbnormalActorTypeValue) != 0) && (((VisibleType & (uint8)14) & TargetTransformSourceValue) != 0) ? EVisibility::Visible : EVisibility::Collapsed;
    }
}

