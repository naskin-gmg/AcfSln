#include "iqtsig/CScriptDataProcessorComp.h"


// ACF includes
#include "istd/TChangeNotifier.h"
#include "ibase/ITextDocument.h"
#include "iproc/IProgressManager.h"

// ACF-Solution includes
#include "imeas/IDataSequence.h"


namespace iqtsig
{


CScriptDataProcessorComp::CScriptDataProcessorComp()
:	m_frameNumber(0)
{
}


// reimplemented (iproc::IProcessor)

int CScriptDataProcessorComp::DoProcessing(
			const iprm::IParamsSet* paramsPtr,
			const istd::IPolymorphic* /*inputPtr*/,
			istd::IChangeable* outputPtr,
			iproc::IProgressManager* progressManagerPtr)
{
	if (outputPtr == NULL){
		return TS_OK;
	}

	QScriptValue frameValue(&m_scriptEngine, ++m_frameNumber);
	m_scriptEngine.globalObject().setProperty("frame", frameValue);

	QString functionScript;
	
	if (m_scriptParamIdAttrPtr.IsValid()){
		const iser::ISerializable* parameterPtr = paramsPtr->GetParameter((*m_scriptParamIdAttrPtr).ToString());
		const ibase::ITextDocument* textPtr = dynamic_cast<const ibase::ITextDocument*>(parameterPtr);
		if (textPtr != NULL){
			functionScript = iqt::GetQString(textPtr->GetText());
		}
		else if (parameterPtr != NULL){
			SendCriticalMessage(MI_BAD_PARAMS, "Unknown script parameter object type");

			return TS_INVALID;
		}
	}
	else if (m_defaultScriptAttrPtr.IsValid()){
		functionScript = iqt::GetQString(*m_defaultScriptAttrPtr);
	}

	if (functionScript.isEmpty()){
		SendCriticalMessage(MI_BAD_PARAMS, "No script");

		return TS_INVALID;
	}

	m_scriptEngine.evaluate(functionScript);

	istd::TChangeNotifier<imeas::IDataSequence> containerPtr(dynamic_cast<imeas::IDataSequence*>(outputPtr));
	if (!containerPtr.IsValid()){
		return TS_INVALID;
	}

	int samplesCount = m_samplesCountAttrPtr.IsValid()?
				*m_samplesCountAttrPtr:
				containerPtr->GetSamplesCount();

	int channelsCount = m_channelsCountAttrPtr.IsValid()?
				*m_channelsCountAttrPtr:
				containerPtr->GetChannelsCount();

	if ((samplesCount != containerPtr->GetSamplesCount()) || (channelsCount != containerPtr->GetChannelsCount())){
		if (!containerPtr->CreateSequence(samplesCount, channelsCount)){
			return TS_INVALID;
		}
	}

	QScriptValue calcCtor = m_scriptEngine.evaluate("Calc");

	int progressSessionId = -1;
	if (progressManagerPtr != NULL){
		progressSessionId = progressManagerPtr->BeginProgressSession("SampleCalc", "Calculate sampling", true);
	}

	int retVal = TS_OK;

	for (int sampleIndex = 0; sampleIndex < samplesCount; ++sampleIndex){
		for (int channelIndex = 0; channelIndex < channelsCount; ++channelIndex){
			QScriptValueList arguments;
			QScriptValue xValue(&m_scriptEngine, sampleIndex);
			arguments << xValue;
			QScriptValue yValue(&m_scriptEngine, channelIndex);
			arguments << yValue;

			double sample = calcCtor.call(m_scriptEngine.nullValue(), arguments).toNumber();

			if (m_scriptEngine.hasUncaughtException()) {
				retVal = TS_INVALID;

				SendCriticalMessage(MI_PROCESSING_ERROR, iqt::GetCString(QObject::tr("Script error in line %1").arg(m_scriptEngine.uncaughtExceptionLineNumber())));

				goto end_iteration;
			}

			containerPtr->SetSample(sampleIndex, channelIndex, sample);
		}

		if ((progressSessionId >= 0) && ((sampleIndex % 100) == 50)){
			I_ASSERT(progressManagerPtr != NULL);

			if (progressManagerPtr->IsCanceled(progressSessionId)){
				retVal = TS_CANCELED;

				break;
			}

			progressManagerPtr->OnProgress(progressSessionId, double(sampleIndex + 1) / samplesCount);
		}
	}

end_iteration:
	if (progressSessionId >= 0){
		I_ASSERT(progressManagerPtr != NULL);

		progressManagerPtr->EndProgressSession(progressSessionId);
	}

	return TS_OK;
}


// reimplemented (isig::ISamplingConstraints)

istd::CRange CScriptDataProcessorComp::GetIntervalRange() const
{
	return istd::CRange(0.001, 1);
}


bool CScriptDataProcessorComp::IsSamplingModeSupported(int /*mode*/) const
{
	return true;
}


istd::CRange CScriptDataProcessorComp::GetValueRange(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return istd::CRange(*m_minValueAttrPtr, *m_maxValueAttrPtr);
}


int CScriptDataProcessorComp::GetMaximalSamplesCount(bool /*forInput*/, bool /*forOutput*/, const iprm::IParamsSet* /*paramsSetPtr*/) const
{
	return -1;
}


// protected methods

// reimplemented (icomp::CComponentBase)

void CScriptDataProcessorComp::OnComponentCreated()
{
	BaseClass::OnComponentCreated();

	QScriptValue minValue(&m_scriptEngine, *m_minValueAttrPtr);
	m_scriptEngine.globalObject().setProperty("minValue", minValue);
	QScriptValue maxValue(&m_scriptEngine, *m_maxValueAttrPtr);
	m_scriptEngine.globalObject().setProperty("maxValue", maxValue);
}


} // namespace iqtsig


