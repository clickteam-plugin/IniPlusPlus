#include "EditData.hpp"
#include "FusionAPI.hpp"
#include "RunData.hpp"

#ifdef FUSION_GET_COMPATIBILITY
auto FUSION_API GetInfos(std::int32_t const which) noexcept
-> std::uint32_t
{
	switch(which)
	{
	case KGI_VERSION:
		return FUSION_GET_COMPATIBILITY_VERSION;
	case KGI_PLUGIN:
		return FUSION_GET_COMPATIBILITY_PLUGIN;
	case KGI_PRODUCT:
		return PRODUCT_VERSION_STANDARD;
	case KGI_BUILD:
		return 294;
	case KGI_UNICODE:
		return FUSION_GET_COMPATIBILITY_UNICODE;
	}
	return 0;
}
#endif

#ifdef FUSION_GET_EXTENSION_INFO
auto FUSION_API GetRunObjectInfos(mv* const mV, kpxRunInfos* const run_info) noexcept
-> std::int16_t
{
	if(!run_info)
	{
		return FUSION_GET_EXTENSION_INFO_FAILURE;
	}

	run_info->identifier = 0x12FD53A0;
	run_info->version = SerializedEditData::CURRENT_VERSION;

	run_info->editDataSize = sizeof(SerializedEditData);

	run_info->windowProcPriority = 100;

	run_info->editFlags = FUSION_OEFLAGS;
	run_info->editPrefs = 0;

	run_info->    actions = const_cast<fusion::    action_func_array>(std::data(RunData::    ACTIONS));
	run_info-> conditions = const_cast<fusion:: condition_func_array>(std::data(RunData:: CONDITIONS));
	run_info->expressions = const_cast<fusion::expression_func_array>(std::data(RunData::EXPRESSIONS));

	run_info->numOfActions     = static_cast<short>(std::size(RunData::    ACTIONS));
	run_info->numOfConditions  = static_cast<short>(std::size(RunData:: CONDITIONS));
	run_info->numOfExpressions = static_cast<short>(std::size(RunData::EXPRESSIONS));

	return FUSION_GET_EXTENSION_INFO_SUCCESS;
}
#endif
