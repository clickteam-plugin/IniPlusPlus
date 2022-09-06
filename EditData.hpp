#pragma once

#include "FusionAPI.hpp"

#include "Settings.hpp"

struct SerializedEditData final
{
	static constexpr std::int16_t const INIPP_V1_ANSI{1}; //Jax's original ANSI version
	static constexpr std::int16_t const INIPP_V2_UNICODE{2}; //Unicode rewrite
	static constexpr std::int16_t const CURRENT_VERSION{INIPP_V2_UNICODE};
	extHeader eHeader;

	std::byte raw[sizeof(std::uint32_t)];

	#ifndef FUSION_RUNTIME_ONLY
	static void serialize(mv* const, SerializedEditData*&, EdittimeSettings const&);
	#endif
	auto deserialize(std::optional<std::uint32_t> codepage) const -> EdittimeSettings;

	SerializedEditData() = delete;
	SerializedEditData(SerializedEditData const&) = delete;
	SerializedEditData(SerializedEditData&&) = delete;
	SerializedEditData &operator=(SerializedEditData&&) = delete;
	SerializedEditData &operator=(SerializedEditData const&) = delete;
	~SerializedEditData() = delete;
};

static_assert(fusion::is_valid_editdata_structure());
