// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef imeas_CGeneralNumericValue_included
#define imeas_CGeneralNumericValue_included


// Qt includes
#include "QtCore/QMap"

// ACF includes
#include <imath/CVarVector.h>

// ACF-Solutions includes
#include <imeas/INumericValue.h>

namespace imeas
{


class CGeneralNumericValue: virtual public imeas::INumericValue
{
public:
	void SetComponentValue(imeas::INumericValue::ValueTypeId valueTypeId, imath::CVarVector value);

	// reimplemented (imeas::INumericValue)
	virtual bool IsValueTypeSupported(imeas::INumericValue::ValueTypeId valueTypeId) const override;
	virtual const imeas::INumericConstraints* GetNumericConstraints() const override;
	virtual imath::CVarVector GetComponentValue(imeas::INumericValue::ValueTypeId valueTypeId) const override;
	virtual imath::CVarVector GetValues() const override;
	virtual bool SetValues(const imath::CVarVector& values) override;

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive &archive) override;

	// reimplemented (istd::IChangable)
	virtual bool CopyFrom(const istd::IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;
	virtual istd::IChangeableUniquePtr CloneMe(CompatibilityMode mode = CM_WITHOUT_REFS) const override;

private:
	typedef QMap<imeas::INumericValue::ValueTypeId, imath::CVarVector> SupportMap;
	SupportMap m_supportMap;
};


} // namespace imeas


#endif // !imeas_CGeneralNumericValue_included


