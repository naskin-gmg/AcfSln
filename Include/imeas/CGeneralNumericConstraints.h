// SPDX-License-Identifier: LGPL-2.1-or-later OR GPL-2.0-or-later OR GPL-3.0-or-later OR LicenseRef-ACF-Commercial
#ifndef imeas_CGeneralNumericConstraints_included
#define imeas_CGeneralNumericConstraints_included


// ACF includes
#include <imath/CGeneralUnitInfo.h>

// ACF-Solutions includes
#include <imeas/INumericValue.h>
#include <imeas/INumericConstraints.h>


namespace imeas
{


/**
	Simple implementation of imeas::INumericConstraints interface.
*/
class CGeneralNumericConstraints:
			virtual public INumericConstraints,
			virtual protected iprm::IOptionsList
{
public:
	/**
		Removes all value informations.
	*/
	void Reset();
	/**
		Add new value information.
	*/
	virtual bool InsertValueInfo(
				const QString& name,
				const QString& description,
				const QByteArray& valueId,
				const imath::CGeneralUnitInfo& unitInfo,
				int index = -1);

	/**
		Remove single value information.
	*/
	void RemoveValueInfo(int index);

	/**
		Get unit info stored alway in this structure.
	*/
	const imath::CGeneralUnitInfo& GetGeneralUnitInfo(int index) const;

	// reimplemented (imeas::INumericConstraints)
	virtual const iprm::IOptionsList& GetValueListInfo() const override;
	virtual const imath::IUnitInfo* GetNumericValueUnitInfo(int index) const override;

	// reimplemented (iprm::IOptionsList)
	virtual int GetOptionsFlags() const override;
	virtual int GetOptionsCount() const override;
	virtual QString GetOptionName(int index) const override;
	virtual QString GetOptionDescription(int index) const override;
	virtual QByteArray GetOptionId(int index) const override;
	virtual bool IsOptionEnabled(int index) const override;

private:
	struct Info
	{
		QString name;
		QString description;
		QByteArray id;
		imath::CGeneralUnitInfo unitInfo;
	};

	QVector<Info> m_valueInfos;
};


} // namespace imeas


#endif // !imeas_CGeneralNumericConstraints_included


