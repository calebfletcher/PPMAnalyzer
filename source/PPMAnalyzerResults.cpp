#include "PPMAnalyzerResults.h"
#include <AnalyzerHelpers.h>
#include "PPMAnalyzer.h"
#include "PPMAnalyzerSettings.h"
#include <iostream>
#include <fstream>

PPMAnalyzerResults::PPMAnalyzerResults(PPMAnalyzer* analyzer, PPMAnalyzerSettings* settings)
	: AnalyzerResults(),
	mSettings(settings),
	mAnalyzer(analyzer)
{
}

PPMAnalyzerResults::~PPMAnalyzerResults()
{
}

void PPMAnalyzerResults::GenerateBubbleText(U64 frame_index, Channel& channel, DisplayBase display_base)
{
	ClearResultStrings();
	Frame frame = GetFrame(frame_index);
	FrameType frame_type = (FrameType)frame.mType;

	U32 sample_rate = mAnalyzer->GetSampleRate();

	char number_str_1[128];
	char number_str_2[128];
	double pulse_duration;
	double value;
	//AnalyzerHelpers::GetNumberString( frame.mData1, display_base, 8, number_str, 128 );

	switch (frame_type)
	{
	case FrameType::Sync:
		sprintf(number_str_1, "Sync");
		AddResultString(number_str_1);
		AddResultString("S");
		break;
	case FrameType::Channel:
		pulse_duration = static_cast<double>(frame.mData1) / static_cast<double>(mAnalyzer->GetSampleRate());
		value = (pulse_duration - 0.0005) * 100000.0;

		sprintf(number_str_1, "Channel %llu: %.1f%%", frame.mData2, value);
		AddResultString(number_str_1);
		sprintf(number_str_2, "%.1f%%", value);
		AddResultString(number_str_2);
		AddResultString("C");
		break;
	default:
		strcpy(number_str_1, "Unknown");
		AddResultString(number_str_1);
		AddResultString("U");
		break;
	}
}

void PPMAnalyzerResults::GenerateExportFile(const char* file, DisplayBase display_base, U32 export_type_user_id)
{
	std::ofstream file_stream(file, std::ios::out);

	U64 trigger_sample = mAnalyzer->GetTriggerSample();
	U32 sample_rate = mAnalyzer->GetSampleRate();

	file_stream << "Time [s],Value" << std::endl;

	U64 num_frames = GetNumFrames();
	for (U32 i = 0; i < num_frames; i++)
	{
		Frame frame = GetFrame(i);

		char time_str[128];
		AnalyzerHelpers::GetTimeString(frame.mStartingSampleInclusive, trigger_sample, sample_rate, time_str, 128);

		char number_str[128];
		AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, 128);

		file_stream << time_str << "," << number_str << std::endl;

		if (UpdateExportProgressAndCheckForCancel(i, num_frames) == true)
		{
			file_stream.close();
			return;
		}
	}

	file_stream.close();
}

void PPMAnalyzerResults::GenerateFrameTabularText(U64 frame_index, DisplayBase display_base)
{
#ifdef SUPPORTS_PROTOCOL_SEARCH
	Frame frame = GetFrame(frame_index);
	ClearTabularText();

	char number_str[128];
	AnalyzerHelpers::GetNumberString(frame.mData1, display_base, 8, number_str, 128);
	AddTabularText(number_str);
#endif
}

void PPMAnalyzerResults::GeneratePacketTabularText(U64 packet_id, DisplayBase display_base)
{
	//not supported

}

void PPMAnalyzerResults::GenerateTransactionTabularText(U64 transaction_id, DisplayBase display_base)
{
	//not supported
}