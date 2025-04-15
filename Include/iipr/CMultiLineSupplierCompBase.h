#ifndef iipr_CMultiLineSupplier_included
#define iipr_CMultiLineSupplier_included


// ACF includes
#include <i2d/CPolyline.h>
#include <iprm/IParamsSet.h>
#include <iinsp/TSupplierCompWrap.h>

// ACF-Solutions includes
#include <iipr/IFeaturesProvider.h>


namespace iipr
{


class CMultiLineSupplierCompBase:
			public iinsp::TSupplierCompWrap< QVector<i2d::CLine2d> >,
			virtual public iipr::IFeaturesProvider
{
public:
	typedef iinsp::TSupplierCompWrap< QVector<i2d::CLine2d> > BaseClass;

	I_BEGIN_BASE_COMPONENT(CMultiLineSupplierCompBase);
		I_REGISTER_INTERFACE(iipr::IFeaturesProvider);
	I_END_COMPONENT;

	// reimplemented (iipr::IFeaturesProvider)
	virtual int GetFeaturesCount() const override;
	virtual const imeas::INumericValue& GetFeature(int index) const override;

private:
	class Line: virtual public imeas::INumericValue
	{
	public:
		Line();
		Line(const i2d::CLine2d& line);

		// reimplemented (imeas::INumericValue)
		virtual bool IsValueTypeSupported(ValueTypeId valueTypeId) const override;
		virtual const imeas::INumericConstraints* GetNumericConstraints() const override;
		virtual imath::CVarVector GetComponentValue(ValueTypeId valueTypeId) const override;
		virtual imath::CVarVector GetValues() const override;
		virtual bool SetValues(const imath::CVarVector& values) override;

		// reimplemented (iser::ISerializable)
		virtual bool Serialize(iser::IArchive& archive) override;

		// reimplemented (istd::IChangeable)
		virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS) override;

	protected:
		imath::CVarVector m_values;
	};

	typedef QMap<int, Line> LinesCache;

	mutable LinesCache m_linesCache;
};


} // namespace iipr


#endif // !iipr_CMultiLineSupplier_included


