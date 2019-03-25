#pragma once


// ACF includes
#include <istd/TArray.h>
#include <i2d/CVector2d.h>
#include <iser/ISerializable.h>


namespace icalib
{


class CPointGrid:
			public istd::TArray<i2d::CVector2d, 2>,
			virtual public iser::ISerializable
{
public:
	typedef istd::TArray<i2d::CVector2d, 2> BaseClass;
	typedef BaseClass::Elements Elements;

	const Elements& GetElements() const;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive) override;

	// reimplemented (istd::IChangeable)
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
};


} // namespace icalib


