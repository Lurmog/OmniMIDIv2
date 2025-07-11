/*

	OmniMIDI v15+ (Rewrite) for Win32/Linux

	This file contains the required code to run the driver under both Windows and Linux

*/

#ifndef _PLUGINSYNTH_H
#define _PLUGINSYNTH_H

#pragma once

#include "SynthModule.hpp"

#define OMV2_ENTRY		"OMv2_PluginEntryPoint"

#define PLGMAJ			1
#define PLGMIN			0
#define PLGBLD			0
#define PLGREV			0

#define OMV2_PLGVER		MAKEVER(PLGMAJ, PLGMIN, PLGBLD, PLGREV)

using namespace OMShared;

struct PluginFuncs {
	uint32_t (WINAPI* SupportedAPIVer)() = nullptr;
	bool (WINAPI* InitPlugin)() = nullptr;
	bool (WINAPI* StopPlugin)() = nullptr;
	void (WINAPI* ShortData)(uint32_t ev) = nullptr;
	uint32_t (WINAPI* LongData)(uint8_t* data, uint32_t len) = nullptr;
	void (WINAPI* Reset)() = nullptr;
	uint64_t (WINAPI* ActiveVoices)() = nullptr;
	float (WINAPI* RenderingTime)() = nullptr;
};

typedef PluginFuncs* (*OMv2PEP)();

namespace OmniMIDI {
	class PluginSynth : public SynthModule {
	protected:
		bool Init = false;
		Lib* Plugin = nullptr;
		OMv2PEP _PluginEntryPoint = nullptr;
		PluginFuncs* _PluginFuncs = nullptr;
		std::jthread _PlgThread;

	public:
		PluginSynth(const char* pluginName, SettingsModule* sett, ErrorSystem::Logger* PErr);
		bool LoadSynthModule() override;
		bool UnloadSynthModule() override;
		bool StartSynthModule() override;
		bool StopSynthModule() override;
		bool IsSynthInitialized() override { return _PluginFuncs; }
		uint32_t SynthID() override { return 0x504C474E; }

		bool IsPluginSupported();
		void PluginThread();

		// Event handling system
		void PlayShortEvent(uint32_t ev) override {
			if (!_PluginFuncs)
				return;

			UPlayShortEvent(ev);
		}
		void PlayShortEvent(uint8_t status, uint8_t param1, uint8_t param2) override {
			if (!_PluginFuncs)
				return;

			UPlayShortEvent(status, param1, param2);
		}
		void UPlayShortEvent(uint32_t ev) override { _PluginFuncs->ShortData(ev); }
		void UPlayShortEvent(uint8_t status, uint8_t param1, uint8_t param2) override { _PluginFuncs->ShortData(status | (param1 << 8) | (param2 << 16)); }

		uint32_t PlayLongEvent(uint8_t* ev, uint32_t size) override {
			if (!_PluginFuncs)
				return 0; 

			return UPlayLongEvent(ev, size);
		}
		uint32_t UPlayLongEvent(uint8_t* ev, uint32_t size) override { return _PluginFuncs->LongData(ev, size); }

		SynthResult Reset(uint8_t type = 0) override { _PluginFuncs->Reset(); return Ok; }
	};
}

#endif