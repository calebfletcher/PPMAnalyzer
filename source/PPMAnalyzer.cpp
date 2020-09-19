#include "PPMAnalyzer.h"
#include "PPMAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

PPMAnalyzer::PPMAnalyzer()
	: Analyzer2(),
	mSettings(new PPMAnalyzerSettings()),
	mSimulationInitilized(false)
{
	SetAnalyzerSettings(mSettings.get());
}

PPMAnalyzer::~PPMAnalyzer()
{
	KillThread();
}

void PPMAnalyzer::SetupResults()
{
	mResults.reset(new PPMAnalyzerResults(this, mSettings.get()));
	SetAnalyzerResults(mResults.get());
	mResults->AddChannelBubblesWillAppearOn(mSettings->mInputChannel);
}

void PPMAnalyzer::WorkerThread()
{
	mSampleRateHz = GetSampleRate();

	mPPM = GetAnalyzerChannelData(mSettings->mInputChannel);

	// Start on high
	if (mPPM->GetBitState() == BIT_LOW)
		mPPM->AdvanceToNextEdge();

	U32 sync_threshold_samples = mSampleRateHz * (static_cast<double>(mSettings->mSyncTimePercentage) / (100.0 * static_cast<double>(mSettings->mFrequency)));

	U32 channel_number = 0;

	// Go to start of the frame
	mPPM->AdvanceToNextEdge(); // Falling edge

	for (; ; )
	{
		U64 start = mPPM->GetSampleNumber();
		mPPM->AdvanceToNextEdge(); // Rising edge
		U64 middle = mPPM->GetSampleNumber();
		mPPM->AdvanceToNextEdge(); // Falling edge
		U64 end = mPPM->GetSampleNumber();

		U64 Tf = middle - start;
		U64 Tv = end - middle;

		Frame frame;

		// Check if we we're in channel time
		if (Tv < sync_threshold_samples) {
			U64 dotLoation = middle + Tv / 2;
			mResults->AddMarker(dotLoation, AnalyzerResults::Dot, mSettings->mInputChannel);

			Frame frame;
			frame.mType = static_cast<U8>(FrameType::Channel);
			frame.mData1 = Tv;
			frame.mData2 = channel_number;
			frame.mFlags = 0;
			frame.mStartingSampleInclusive = start;
			frame.mEndingSampleInclusive = end-1;

			mResults->AddFrame(frame);
			mResults->CommitResults();
			ReportProgress(end);

			channel_number++;
		}
		else {
			frame.mType = static_cast<U8>(FrameType::Sync);
			frame.mFlags = 0;
			frame.mStartingSampleInclusive = start;
			frame.mEndingSampleInclusive = end-1;

			mResults->AddFrame(frame);
			mResults->CommitResults();
			ReportProgress(end);

			channel_number = 0;

			//mResults->CommitPacketAndStartNewPacket();
		}
	}
}

bool PPMAnalyzer::NeedsRerun()
{
	return false;
}

U32 PPMAnalyzer::GenerateSimulationData(U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels)
{
	if (mSimulationInitilized == false)
	{
		mSimulationDataGenerator.Initialize(GetSimulationSampleRate(), mSettings.get());
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData(minimum_sample_index, device_sample_rate, simulation_channels);
}

U32 PPMAnalyzer::GetMinimumSampleRateHz()
{
	return 25000;	// Smallest sample rate
}

const char* PPMAnalyzer::GetAnalyzerName() const
{
	return "PPM RC";
}

const char* GetAnalyzerName()
{
	return "PPM RC";
}

Analyzer* CreateAnalyzer()
{
	return new PPMAnalyzer();
}

void DestroyAnalyzer(Analyzer* analyzer)
{
	delete analyzer;
}