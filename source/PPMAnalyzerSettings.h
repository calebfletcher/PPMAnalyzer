#ifndef PPM_ANALYZER_SETTINGS
#define PPM_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class PPMAnalyzerSettings : public AnalyzerSettings
{
public:
	PPMAnalyzerSettings();
	virtual ~PPMAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	
	Channel mInputChannel;
	U32 mFrequency;
	U32 mSyncTimePercentage;

protected:
	std::auto_ptr< AnalyzerSettingInterfaceChannel >	mInputChannelInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mFrequencyInterface;
	std::auto_ptr< AnalyzerSettingInterfaceInteger >	mSyncTimePercentageInterface;
};

#endif //PPM_ANALYZER_SETTINGS
