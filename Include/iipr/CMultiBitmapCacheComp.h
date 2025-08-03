#ifndef iipr_CMultiBitmapCacheComp_included
#define iipr_CMultiBitmapCacheComp_included


// Qt includes
#include <QtCore/QStringList>

// ACF includes
#include <istd/TPointerVector.h>
#include <icomp/CComponentBase.h>
#include <i2d/ICalibration2d.h>
#include <i2d/IMultiCalibrationProvider.h>

// ACF-Solutions includes
#include <iimg/IMultiBitmapProvider.h>


namespace iipr
{


/**
	Multi bitmap provider returning always stored bitmaps.
	Stored bitmaps can be changed only copying their from another multi bitmap provider using CopyFrom method.
	This object will be used to manage threading barrier for object supplier chain.
*/
class CMultiBitmapCacheComp:
			public icomp::CComponentBase,
			virtual public iimg::IMultiBitmapProvider,
			virtual public i2d::IMultiCalibrationProvider,
			virtual public iser::ISerializable
{
public:
	typedef icomp::CComponentBase BaseClass;

	I_BEGIN_COMPONENT(CMultiBitmapCacheComp);
		I_REGISTER_INTERFACE(iimg::IMultiBitmapProvider);
		I_REGISTER_INTERFACE(iser::ISerializable);
		I_ASSIGN(m_copyConstraintsAttrPtr, "CopyConstraints", "Copy bitmap constraints from provider", true, false);
	I_END_COMPONENT;

	CMultiBitmapCacheComp();

	// reimplemented (iimg::IMultiBitmapProvider)
	virtual const iprm::IOptionsList* GetBitmapListInfo() const;
	virtual int GetBitmapsCount() const;
	virtual const iimg::IBitmap* GetBitmap(int bitmapIndex) const;

	// reimplemented (i2d::IMultiCalibrationProvider)
	virtual const iprm::IOptionsList* GetCalibrationSelectionContraints() const;
	virtual int GetCalibrationsCount() const;
	virtual const i2d::ICalibration2d* GetCalibration(int calibrationIndex) const;

	// reimplemented (istd::IChangeable)
	virtual bool CopyFrom(const IChangeable& object, CompatibilityMode mode = CM_WITHOUT_REFS);

	// reimplemented (iser::ISerializable)
	virtual bool Serialize(iser::IArchive& archive);

protected:
	// reimplemented (icomp::CComponentBase)
	virtual void OnComponentCreated() override;

private:
	class CBitmapConstraints: virtual public iprm::IOptionsList
	{
		friend class CMultiBitmapCacheComp;

	public:
		CBitmapConstraints()
		{
			Reset();
		}

		void Reset()
		{
			m_flags = 0;
			m_count = 0;
			m_names.clear();
			m_descriptions.clear();
			m_ids.clear();
			m_enabled.clear();
		}

		virtual int GetOptionsFlags() const override
		{
			return m_flags;
		}

		virtual int GetOptionsCount() const override
		{
			return m_count;
		}

		virtual QString GetOptionName(int index) const override
		{
			return m_names.at(index);
		}

		virtual QString GetOptionDescription(int index) const override
		{
			return m_descriptions.at(index);
		}

		virtual QByteArray GetOptionId(int index) const override
		{
			return m_ids.at(index);
		}

		virtual bool IsOptionEnabled(int index) const override
		{
			return m_enabled.at(index);
		}

	private:
		int m_flags;
		int m_count;
		QStringList m_names;
		QStringList m_descriptions;
		QList<QByteArray> m_ids;
		QList<bool> m_enabled;
	};

	CBitmapConstraints m_bitmapConstraints;
	bool m_copyConstraints;

	istd::TPointerVector<iimg::IBitmap> m_bitmapsPtr;
	istd::TPointerVector<i2d::ICalibration2d> m_transformsPtr;

	I_ATTR(bool, m_copyConstraintsAttrPtr);
};


} // namespace iipr


#endif // !iipr_CMultiBitmapCacheComp_included


