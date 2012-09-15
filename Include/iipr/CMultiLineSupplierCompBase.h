#ifndef iipr_CMultiLineSupplier_included
#define iipr_CMultiLineSupplier_included


// ACF includes
#include "i2d/CPolyline.h"
#include "iprm/IParamsSet.h"
#include "iproc/TSupplierCompWrap.h"

// ACF-Solutions includes
#include "imeas/INumericValueProvider.h"


namespace iipr
{


class CMultiLineSupplierCompBase:
			public iproc::TSupplierCompWrap< QVector<i2d::CLine2d> >,
			virtual public imeas::INumericValueProvider
{
public:
	typedef iproc::TSupplierCompWrap< QVector<i2d::CLine2d> > BaseClass;

	I_BEGIN_BASE_COMPONENT(CMultiLineSupplierCompBase);
		I_REGISTER_INTERFACE(imeas::INumericValueProvider);
	I_END_COMPONENT;

	// reimplemented (imeas::INumericValueProvider)
	virtual int GetValuesCount() const;
	virtual const imeas::INumericValue& GetNumericValue(int index) const;

private:
	class Line: virtual public imeas::INumericValue
	{
	public:
		Line();
		Line(const i2d::CLine2d& line);

		// reimplemented (imeas::INumericValue)
		virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const;
		virtual const imeas::INumericConstraints* GetNumericConstraints() const;
		virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const;
		virtual imath::CVarVector GetValues() const;
		virtual bool SetValues(const imath::CVarVector& values);

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive);

		// reimplemented (istd::IChangeable)
		virtual bool CopyFrom(const IChangeable& object);

	protected:
		imath::CVarVector m_values;
	};

	typedef QMap<int, Line> LinesCache;

	mutable LinesCache m_linesCache;
};


} // namespace iipr


#endif // !iipr_CMultiLineSupplier_included


