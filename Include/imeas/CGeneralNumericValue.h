#ifndef imeas_CGeneralNumericValue_included
#define imeas_CGeneralNumericValue_included


// Qt includes
#include "QtCore/QMap"

// ACF includes
#include "imath/CVarVector.h"

// ACF-Solutions includes
#include "imeas/INumericValue.h"

namespace imeas
{


class CGeneralNumericValue: virtual public imeas::INumericValue
{
public:
	void SetComponentValue(imeas::INumericValue::ValueTypeId valueTypeId, imath::CVarVector value);

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(imeas::INumericValue::ValueTypeId valueTypeId) const;
	virtual const imeas::INumericConstraints* GetNumericConstraints() const;
	virtual imath::CVarVector GetComponentValue(imeas::INumericValue::ValueTypeId valueTypeId) const;
	virtual imath::CVarVector GetValues() const;
	virtual bool SetValues(const imath::CVarVector& values);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive &archive);

	// reimplemented (istd::IChangable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);
	virtual istd::IChangeable* CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const;

private:
	typedef QMap<imeas::INumericValue::ValueTypeId, imath::CVarVector> SupportMap;
	SupportMap m_supportMap;
};


} // namespace imeas


#endif // !imeas_CGeneralNumericValue_included


