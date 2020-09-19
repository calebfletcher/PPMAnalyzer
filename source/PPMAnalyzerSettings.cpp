#include "PPMAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


PPMAnalyzerSettings::PPMAnalyzerSettings()
	: mInputChannel(UNDEFINED_CHANNEL),
	  mFrequency(50),
	  mSyncTimePercentage(20)
{
	mInputChannelInterface.reset(new AnalyzerSettingInterfaceChannel());
	mInputChannelInterface->SetTitleAndTooltip("Serial", "Standard PPM RC");
	mInputChannelInterface->SetChannel(mInputChannel);

	mFrequencyInterface.reset(new AnalyzerSettingInterfaceInteger());
	mFrequencyInterface->SetTitleAndTooltip("Frequency (Hz)", "Approximate frequency of the signal.");
	mFrequencyInterface->SetMax(10000000);
	mFrequencyInterface->SetMin(1);
	mFrequencyInterface->SetInteger(mFrequency);

	mSyncTimePercentageInterface.reset(new AnalyzerSettingInterfaceInteger());
	mSyncTimePercentageInterface->SetTitleAndTooltip("Sync Time Percentage (%)", "Percentage of the frequency to use as a threshold for detecting the sync time.");
	mSyncTimePercentageInterface->SetMax(99);
	mSyncTimePercentageInterface->SetMin(1);
	mSyncTimePercentageInterface->SetInteger(mSyncTimePercentage);

	AddInterface(mInputChannelInterface.get());
	AddInterface(mFrequencyInterface.get());
	AddInterface(mSyncTimePercentageInterface.get());

	AddExportOption(0, "Export as text/csv file");
	AddExportExtension(0, "text", "txt");
	AddExportExtension(0, "csv", "csv");

	ClearChannels();
	AddChannel(mInputChannel, "PPM RC", false);
}

PPMAnalyzerSettings::~PPMAnalyzerSettings()
{
}

bool PPMAnalyzerSettings::SetSettingsFromInterfaces()
{
	mInputChannel = mInputChannelInterface->GetChannel();
	mFrequency = mFrequencyInterface->GetInteger();
	mSyncTimePercentage = mSyncTimePercentageInterface->GetInteger();

	ClearChannels();
	AddChannel(mInputChannel, "PPM RC", true);

	return true;
}

void PPMAnalyzerSettings::UpdateInterfacesFromSettings()
{
	mInputChannelInterface->SetChannel(mInputChannel);
	mFrequencyInterface->SetInteger(mFrequency);
	mSyncTimePercentageInterface->SetInteger(mSyncTimePercentage);
}

void PPMAnalyzerSettings::LoadSettings(const char* settings)
{
	SimpleArchive text_archive;
	text_archive.SetString(settings);

	text_archive >> mInputChannel;
	text_archive >> mFrequency;
	text_archive >> mSyncTimePercentage;

	ClearChannels();
	AddChannel(mInputChannel, "PPM RC", true);

	UpdateInterfacesFromSettings();
}

const char* PPMAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;

	text_archive << mInputChannel;
	text_archive << mFrequency;
	text_archive << mSyncTimePercentage;

	return SetReturnString(text_archive.GetString());
}
