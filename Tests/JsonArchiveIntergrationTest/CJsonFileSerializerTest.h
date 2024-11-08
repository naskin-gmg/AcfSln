/********************************************************************************
**
**	Copyright (C) 2007-2017 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF Toolkit.
**
**	This file may be used under the terms of the GNU Lesser
**	General Public License version 2.1 as published by the Free Software
**	Foundation and appearing in the file LicenseLGPL.txt included in the
**	packaging of this file.  Please review the following information to
**	ensure the GNU Lesser General Public License version 2.1 requirements
**	will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
**	If you are unsure which license is appropriate for your use, please
**	contact us at info@imagingtools.de.
**
** 	See http://www.ilena.org or write info@imagingtools.de for further
** 	information about the ACF.
**
********************************************************************************/


#pragma once


// Qt includes
#include <QtCore/QObject>
#include <QtCore/QTemporaryFile>
#include <QtCore/QScopedPointer>

// ACF includes
#include <istd/TIFactory.h>
#include <istd/TDelPtr.h>
#include <ilog/CLoggerBase.h>


#include <GeneratedFiles/JsonArchiveIntergrationTest/CJsonArchiveIntergrationTest.h>

// forward declaration
namespace iprm
{
class IParamsSet;
} // namespace iprm

namespace iser
{
class ISerializable;
} // namespace iser

namespace ifile
{
class IFilePersistence;
} // namespace ifile



class CJsonFileSerializerTest: public QObject, public ilog::CLoggerBase
{
	Q_OBJECT
public:
	typedef QObject BaseClass;
	typedef ilog::CLoggerBase BaseClass2;
	CJsonFileSerializerTest();

private Q_SLOTS:
	void initTestCase();

	void JsonArchiveParamsSerializeTest();
	void JsonArchiveParamsDeserializeTest();
	void JsonArchiveParamsCompareTest();

	void cleanupTestCase();

private:
	// comfort methods
	static bool SetTextForParamsSet(iprm::IParamsSet& params, const QByteArray& paramId, const QString& text);
	static bool SetDigitValueForParamsSet(iprm::IParamsSet& params, const QByteArray& paramId, int value);
	static bool SetDigitValueForParamsSet(iprm::IParamsSet& params, const QByteArray& paramId, double value);
	static bool SelectOptionByIdForParamsSet(iprm::IParamsSet& params, const QByteArray& paramId, const QByteArray& selectionId);
	static bool SelectOptionByIdForParamsSet(iprm::IParamsSet& params, const QByteArray& paramId, const QString& selectionId);

private:
	istd::TDelPtr<CJsonArchiveIntergrationTest> m_testPartituraInstanceCompPtr;
	ifile::IFilePersistence* m_jsonFilePersistancePtr;
	iser::ISerializable* m_originalSerializableObjectPtr;
	iser::ISerializable* m_serializableObjectPtr;
	QScopedPointer<QTemporaryFile> m_jsonTempFilePtr;

};


