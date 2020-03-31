
#pragma once
#include "CoreMinimal.h"
#include "IPropertyTypeCustomization.h"

class IPropertyHandle;
class IDetailChildrenBuilder;
class FDetailWidgetRow;
class IPropertyTypeCustomizationUtils;

class FAbnormalDetailsView : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();
 
	FAbnormalDetailsView()
	{ }
 
	virtual ~FAbnormalDetailsView() {}
 
	/** Begin IPropertyTypeCustomization interface */
	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> PropertyHandle, FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils) override {}
	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> StructPropertyHandle, IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils) override;
	/** End IPropertyTypeCustomization interface */
protected:
	EVisibility ArePropertiesVisible(const int32 VisibleType) const;
	TSharedPtr<IPropertyHandle> TargetTransformSourceProperty;
	TSharedPtr<IPropertyHandle> AbnormalActorTypeProperty;
};