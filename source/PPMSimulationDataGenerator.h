#ifndef PPM_SIMULATION_DATA_GENERATOR
#define PPM_SIMULATION_DATA_GENERATOR

#include <SimulationChannelDescriptor.h>
#include <string>
class PPMAnalyzerSettings;

class PPMSimulationDataGenerator
{
public:
	PPMSimulationDataGenerator();
	~PPMSimulationDataGenerator();

	void Initialize( U32 simulation_sample_rate, PPMAnalyzerSettings* settings );
	U32 GenerateSimulationData( U64 newest_sample_requested, U32 sample_rate, SimulationChannelDescriptor** simulation_channel );

protected:
	PPMAnalyzerSettings* mSettings;
	U32 mSimulationSampleRateHz;

protected:
	void CreateSerialByte();
	std::string mSerialText;
	U32 mStringIndex;

	SimulationChannelDescriptor mSerialSimulationData;

};
#endif //PPM_SIMULATION_DATA_GENERATOR