/********************************************************************************
**
**	Copyright (c) 2007-2014 Witold Gantzke & Kirill Lepskiy
**
**	This file is part of the ACF-Solutions Toolkit.
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
** 	See http://www.ilena.org, write info@imagingtools.de or contact
**	by Skype to ACF_infoline for further information about the ACF-Solutions.
**
********************************************************************************/


#include "ifileproc/CFileInfoCopyComp.h"


// Qt includes
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QFile>

// ACF includes
#include "iser/IVersionInfo.h"
#include "istd/CSystem.h"


namespace ifileproc
{


// reimplemented (ifileproc::IFileConversion)

int CFileInfoCopyComp::ConvertFiles(
			const QString& inputPath,
			QString& outputPath,
			const iprm::IParamsSet* /*paramsSetPtr*/,
			ibase::IProgressManager* /*progressManagerPtr*/) const
{
	QString inputFileName = inputPath;
	QString outputFileName = outputPath;

	SendInfoMessage(MI_FILE_INFO, QObject::tr("Processing file %1 to %2").arg(inputFileName).arg(outputFileName));

	if (!*m_useSubstitutionAttrPtr && !m_licensePathAttrPtr.IsValid()){
		QFile::remove(outputFileName);

		return QFile::copy(inputFileName, outputFileName) ? iproc::IProcessor::TS_OK : iproc::IProcessor::TS_INVALID;
	}

	QFile inputFile(inputFileName);
	QFile outputFile(outputFileName);
	if (!inputFile.open(QIODevice::ReadOnly | QIODevice::Text)){
		SendWarningMessage(MI_INPUT_OPEN, QObject::tr("Opening input file failed (%1)").arg(inputFileName));

		return iproc::IProcessor::TS_INVALID;
	}

	if (!outputFile.open(QIODevice::WriteOnly  | QIODevice::Text)){
		SendWarningMessage(MI_OUTPUT_OPEN, QObject::tr("Opening output file failed (%1)").arg(outputFileName));

		return iproc::IProcessor::TS_INVALID;
	}

	QTextStream inputStream(&inputFile);
	QTextStream outputStream(&outputFile);

	if (m_licensePathAttrPtr.IsValid()){
		QString licenseFileName = istd::CSystem::GetEnrolledPath(*m_licensePathAttrPtr);

		QFile licenseFile(licenseFileName);
		if (!licenseFile.open(QIODevice::ReadOnly | QIODevice::Text)){
			SendWarningMessage(MI_LICENSE_OPEN, QObject::tr("Opening license file failed (%1)").arg(licenseFileName));

			return iproc::IProcessor::TS_INVALID;
		}

		QTextStream licenseStream(&licenseFile);

		while (!licenseStream.atEnd()){
			QString line = licenseStream.readLine();
			outputStream << line << endl;
		}
	}

	for (int lineCounter = 1; !inputStream.atEnd(); ++lineCounter) {
		QString line = inputStream.readLine();

		int endIndex = -1;
		if (*m_useSubstitutionAttrPtr){
			for (int beginIndex; (beginIndex = line.indexOf("$", endIndex + 1)) >= 0;){
				endIndex = line.indexOf("$", beginIndex + 1);
				if (endIndex < 0){
					SendWarningMessage(MI_BAD_TAG, QObject::tr("%1(%2) : Substitution tag is incomplete").arg(inputFileName).arg(lineCounter));

					break;
				}
				QString substitutionTag = line.mid(beginIndex + 1, endIndex - beginIndex - 1);
				QString substituted;

				if (ProcessSubstitutionTag(substitutionTag, substituted)){
					line.replace(beginIndex, endIndex - beginIndex + 1, substituted);

					endIndex += substituted.length() - (endIndex - beginIndex + 1);
				}
				else{
					SendWarningMessage(MI_BAD_TAG, QObject::tr("%1(%2) : Cannot process tag '%3'").arg(inputFileName).arg(lineCounter).arg(substitutionTag));

					break;
				}
			}
		}

		outputStream << line << endl;
	}

	return iproc::IProcessor::TS_OK;
}


// protected methods

bool CFileInfoCopyComp::ProcessSubstitutionTag(const QString& tag, QString& result) const
{
	static const QString acfCompanyNameTag("AcfCompanyName");
	static const QString acfProductNameTag("AcfProductName");
	static const QString acfApplicationNameTag("AcfApplicationName");
	static const QString acfApplicationSubnameTag("AcfApplicationSubname");
	static const QString acfApplicationTypeTag("AcfApplicationType");
	static const QString acfLegalCopyrightTag("AcfLegalCopyright");
	static const QString acfVersionTag("AcfVersion");
	static const QString acfRawVersionTag("AcfRawVersion");
	static const QString acfRcVersionTag("AcfRcVersion");
	static const QString acfTimestampTag("AcfTimestamp");

	if (tag.isEmpty()){
		result = "$";

		return true;
	}

	int separatorIndex = tag.indexOf(":");

	if (separatorIndex >= 0){
		QString paramTag = tag.left(separatorIndex);
		QString parameter = tag.right(tag.length() - separatorIndex - 1);

		if ((paramTag == acfVersionTag) || (paramTag == acfRawVersionTag) || (paramTag == acfRcVersionTag)){
			if (m_applicationInfoCompPtr.IsValid()){
				bool isNumOk;
				int versionId = parameter.toInt(&isNumOk);

				quint32 versionNumber;
				const iser::IVersionInfo& versionInfo = m_applicationInfoCompPtr->GetVersionInfo();
				if (isNumOk && (versionId >= 0) && versionInfo.GetVersionNumber(versionId, versionNumber)){
					if (paramTag == acfRawVersionTag){
						result = QString::number(versionNumber);
						return true;
					}
					else{
						result = versionInfo.GetEncodedVersionName(versionId, versionNumber);
						if (paramTag == acfVersionTag){
							return true;
						}
						else if (paramTag == acfRcVersionTag){
							result.replace(".", ", ");
							result.replace("'", "");

							return true;
						}
					}
				}
			}
		}
	}
	else{
		if (m_applicationInfoCompPtr.IsValid()){
			if (tag == acfCompanyNameTag){
				result = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_COMPANY_NAME);

				return true;
			}
			else if (tag == acfProductNameTag){
				result = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_PRODUCT_NAME);

				return true;
			}
			else if (tag == acfApplicationNameTag){
				result = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_NAME);

				return true;
			}
			else if (tag == acfApplicationSubnameTag){
				result = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_SUBNAME);

				return true;
			}
			else if (tag == acfApplicationTypeTag){
				result = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_APPLICATION_TYPE);

				return true;
			}
			else if (tag == acfLegalCopyrightTag){
				result = m_applicationInfoCompPtr->GetApplicationAttribute(ibase::IApplicationInfo::AA_LEGAL_COPYRIGHT);

				return true;
			}
			else if ((tag == acfVersionTag) || (tag == acfRawVersionTag) || (tag == acfRcVersionTag)){
				quint32 versionNumber;

				const iser::IVersionInfo& versionInfo = m_applicationInfoCompPtr->GetVersionInfo();
				int versionId = m_applicationInfoCompPtr->GetMainVersionId();
				if ((versionId >= 0) && versionInfo.GetVersionNumber(versionId, versionNumber)){
					if (tag == acfRawVersionTag){
						result = QString::number(versionNumber);
						return true;
					}
					else{
						result = versionInfo.GetEncodedVersionName(versionId, versionNumber);
						if (tag == acfVersionTag){
							return true;
						}
						else if (tag == acfRcVersionTag){
							result.replace(".", ", ");
							result.replace("'", "");

							return true;
						}
					}
				}
			}
		}

		if (tag == acfTimestampTag){
			QDateTime currentTime = QDateTime::currentDateTime();
			result = currentTime.toString();

			return true;
		}

		int userTagsCount = qMin(m_userSubstitutionTagsAttrPtr.GetCount(), m_userSubstitutionValuesAttrPtr.GetCount());
		for (int userTagIndex = 0; userTagIndex < userTagsCount; ++userTagIndex){
			if (tag == m_userSubstitutionTagsAttrPtr[userTagIndex]){
				result = m_userSubstitutionValuesAttrPtr[userTagIndex];

				return true;
			}
		}

		QString variable = istd::CSystem::FindVariableValue(tag);
		if (!variable.isEmpty()){
			result = variable;

			return true;
		}
	}

	return false;
}


} // namespace ifileproc


