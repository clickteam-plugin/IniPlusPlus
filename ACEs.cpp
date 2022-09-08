#include "FusionAPI.hpp"
#include "lSDK.hpp"
#include "lSDK/WindowsUtilities.hpp"
#include "RunData.hpp"

#include <algorithm>
#include <array>
#include <cassert>
#include <charconv>
#include <concepts>
#include <limits>
#include <memory>
#include <span>
#include <type_traits>
#include <variant>

using namespace std::string_literals;
using namespace std::string_view_literals;

using fusion::action_return_t;
using fusion::condition_return_t;
using fusion::expression_return_t;
using fusion::ac_param_t;
using fusion::e_params_t;

using lSDK::char_t;
using lSDK::string_t;
using lSDK::string_view_t;

[[nodiscard]] static constexpr auto toLower(TCHAR const c) noexcept
-> TCHAR
{
	if(c >= 'A' && c <= 'Z')
	{
		return (c - 'A') + 'a';
	}
	return c;
}
[[nodiscard]] static constexpr auto toUpper(TCHAR const c) noexcept
-> TCHAR
{
	if(c >= 'a' && c <= 'z')
	{
		return (c - 'a') + 'A';
	}
	return c;
}
[[nodiscard]] static auto matchesPattern(string_view_t pattern, string_view_t s, bool const case_sensitive) noexcept
-> bool
{
	if(empty(pattern))
	{
		//empty pattern only matches empty string
		return empty(s);
	}
	bool const inverted = (pattern.front() == _T('!'));
	if(inverted)
	{
		pattern.remove_prefix(1);
		if(empty(pattern))
		{
			//empty inverted pattern only matches non-empty strings
			return !empty(s);
		}
	}

	for(std::size_t i = 0; i < size(pattern); ++i)
	{
		TCHAR const thisC = pattern[i];
		if(thisC == _T('*'))
		{
			if(i + 1 >= size(pattern))
			{
				//pattern ending with * matches entire rest of string, successful match
				return !inverted;
			}
			TCHAR const nextC = pattern[i + 1];
			//TODO won't work for patterns like *. but why would someone do that anyway?
			if(case_sensitive)
			{
				std::size_t const next = s.find(nextC);
				if(next == decltype(s)::npos)
				{
					//next match can't be found, failed to match
					return inverted;
				}
				s.remove_prefix(next);
				assert(s.front() == nextC);
			}
			else
			{
				TCHAR const both[2]{toLower(nextC), toUpper(nextC)};
				std::size_t const next = s.find_first_of(both);
				if(next == decltype(s)::npos)
				{
					//next match can't be found, failed to match
					return inverted;
				}
				s.remove_prefix(next);
				assert(s.front() == toLower(nextC) || s.front() == toUpper(nextC));
			}
			++i;
			s.remove_prefix(1);
		}
		else if(empty(s))
		{
			//empty string but more pattern to match and not a *, failed to match
			return inverted;
		}
		else if(thisC == _T('.'))
		{
			s.remove_prefix(1);
			continue;
		}
		else if(case_sensitive)
		{
			if(thisC != s.front())
			{
				//obvious failure to match
				return inverted;
			}
			s.remove_prefix(1);
			continue;
		}
		else
		{
			if(toLower(thisC) != toLower(s.front()))
			{
				//obvious failure to match
				return inverted;
			}
			s.remove_prefix(1);
			continue;
		}
	}

	//exhausted pattern
	if(empty(s))
	{
		//exhausted string too, successful match
		return !inverted;
	}
	//extra string leftover not accounted for in the pattern, failed to match
	return inverted;
}

static auto alphanum_less(string_view_t a, string_view_t b) noexcept
-> bool
{
	static constexpr string_view_t const numbers{TSL("0123456789")};
	while(!empty(a) && !empty(b))
	{
		std::size_t const ani{a.find_first_of(numbers)};
		std::size_t const aai{a.find_first_not_of(numbers)};
		std::size_t const bni{b.find_first_of(numbers)};
		std::size_t const bai{b.find_first_not_of(numbers)};
		if(ani == 0 && bni == 0)
		{
			auto const na{std::stoull(string_t{a.substr(0, aai)})};
			auto const nb{std::stoull(string_t{b.substr(0, bai)})};
			if(na != nb)
			{
				return na < nb;
			}
			a.remove_prefix(std::min(aai, size(a)));
			b.remove_prefix(std::min(bai, size(b)));
		}
		else if(aai == 0 && bai == 0)
		{
			string_view_t const sa{a.substr(0, ani)};
			string_view_t const sb{b.substr(0, bni)};
			if(sa != sb)
			{
				return sa < sb;
			}
			a.remove_prefix(std::min(ani, size(a)));
			b.remove_prefix(std::min(bni, size(b)));
		}
		else
		{
			break;
		}
	}
	return a < b;
}

static string_t string_from_float(double const v)
{
	std::array<char, std::numeric_limits<double>::max_digits10 + 3> buf{};
	auto const [ptr, ec]{std::to_chars(std::data(buf), std::data(buf) + std::size(buf), v)};
	assert(ec == std::errc{});
	assert(ptr > std::data(buf));
	string_t ret;
	ret.reserve(ptr - std::data(buf));
	for(char const* p{std::data(buf)}; p < ptr; ++p)
	{
		ret += static_cast<char_t>(*p);
	}
	return ret;
}
template<std::floating_point T>
static T float_from_string(string_view_t const s)
{
	std::string buf;
	buf.reserve(std::size(s));
	for(auto const c : s)
	{
		buf += static_cast<char>(c);
	}
	T v{};
	std::ignore = std::from_chars(std::data(buf), std::data(buf) + std::size(buf), v);
	return v;

}

struct ValueDoer final
: Doer
{
	Data& data;
	string_t group, item, newv;
	bool gexisted, iexisted;
	string_t oldv;
	ValueDoer(Data& data_, string_t group_, string_t item_, string_t newv_)
	: data{data_}
	, group{move(group_)}
	, item{move(item_)}
	, newv{move(newv_)}
	, gexisted{data.has_group(group)}
	, iexisted{data.has_item(group, item)}
	, oldv{iexisted? data.get_value_or_create(group, item) : string_t{}}
	{
	}

	void redo() const override
	{
		if(!gexisted)
		{
			std::visit([this]<typename IniT>(IniT& ini)
			{
				typename IniT::mapped_type g{};
				g.emplace(item, newv);
				ini.emplace(group, std::move(g));
			}, data.ini);
		}
		else if(!iexisted)
		{
			std::visit([this](auto* g)
			{
				g->emplace(item, newv);
			}, data.get_group_or_create(group));
		}
		else
		{
			data.get_value_or_create(group, item) = newv;
		}
	}
	void undo() const override
	{
		if(!gexisted)
		{
			std::visit([this](auto& ini){ ini.erase(group); }, data.ini);
		}
		else if(!iexisted)
		{
			std::visit([this](auto* g){ g->erase(item); }, data.get_group_or_create(group));
		}
		else
		{
			data.get_value_or_create(group, item) = oldv;
		}
	}
};
struct DeleteItemDoer final
: Doer
{
	Data& data;
	string_t group, item;
	bool iexisted;
	string_t oldv;
	DeleteItemDoer(Data& data_, string_t group_, string_t item_)
	: data(data_)
	, group{move(group_)}
	, item{move(item_)}
	, iexisted{data.has_item(group, item)}
	, oldv{iexisted? data.get_value_or_create(group, item) : string_t{}}
	{
	}

	void redo() const override
	{
		if(iexisted)
		{
			bool is_empty{};
			std::visit([this, &is_empty](auto* g)
			{
				std::ignore = g->erase(item);
				is_empty = std::empty(*g);
			}, data.get_group_or_create(group));
			if(is_empty && !data.settings.empty_groups)
			{
				data.erase_group(group);
			}
		}
	}
	void undo() const override
	{
		if(iexisted)
		{
			std::visit([this](auto* g)
			{
				std::ignore = g->emplace(item, oldv);
			}, data.get_group_or_create(group));
		}
	}
};
struct DeleteGroupDoer final
: Doer
{
	Data& data;
	string_t group;
	bool gexisted;
	std::variant<OrderedGroup_t, UnorderedGroup_t> oldg;
	DeleteGroupDoer(Data& data_, string_t group_)
	: data{data_}
	, group{std::move(group_)}
	, gexisted{data.has_group(group)}
	{
		std::visit([this]<typename GroupT>(GroupT const* const g)
		{
			if(g != nullptr)
			{
				oldg.emplace<GroupT>(*g);
			}
		}, data.get_group_if_exists(group));
	}

	void redo() const override
	{
		if(gexisted)
		{
			data.erase_group(group);
		}
	}
	void undo() const override
	{
		if(gexisted)
		{
			std::visit([this]<typename IniT>(IniT& ini)
			{
				ini.emplace(group, std::get<typename IniT::mapped_type>(oldg));
			}, data.ini);
		}
	}
};
struct CopyGroupDoer final
: Doer
{
	Data& data;
	string_t group_src, group_dst;
	bool merge, dst_existed;
	std::variant<OrderedGroup_t, UnorderedGroup_t> dst_backup;
	CopyGroupDoer(Data& data_, string_t group_src_, string_t group_dst_, bool const merge_)
	: data{data_}
	, group_src{std::move(group_src_)}
	, group_dst{std::move(group_dst_)}
	, merge{merge_}
	, dst_existed{data.has_group(group_dst)}
	{
		assert(group_src != group_dst);
		std::visit([this]<typename GroupT>(GroupT const* const g)
		{
			if(g != nullptr)
			{
				dst_backup.emplace<GroupT>(*g);
			}
		}, data.get_group_if_exists(group_dst));
	}

	void redo() const override
	{
		std::visit([this]<typename GroupT>(GroupT const* const src)
		{
			if(src)
			{
				auto& dst{*std::get<GroupT*>(data.get_group_or_create(group_dst))};
				if(merge)
				{
					for(auto const& [item_name, item_value] : *src)
					{
						dst.insert_or_assign(item_name, item_value);
					}
				}
				else
				{
					dst = *src;
				}
			}
		}, data.get_group_if_exists(group_src));
	}
	void undo() const override
	{
		if(dst_existed)
		{
			std::visit([this]<typename GroupT>(GroupT* const dst)
			{
				*dst = std::get<GroupT>(dst_backup);
			}, data.get_group_or_create(group_dst));
		}
		else
		{
			data.erase_group(group_dst);
		}
	}
};
struct DeletePatternDoer final
: Doer
{
	Data& data;
	UnorderedIni_t deleted;
	DeletePatternDoer(Data& data_, string_view_t const gpattern, string_view_t const ipattern, string_view_t const vpattern, bool const case_sensitive)
	: data{data_}
	{
		bool const actual_case_sensitive{data.settings.case_sensitive && case_sensitive};
		std::visit([this, gpattern, ipattern, vpattern, actual_case_sensitive](auto const& ini)
		{
			for(auto const& [group_name, group] : ini)
			{
				if(!matchesPattern(gpattern, group_name, actual_case_sensitive))
				{
					continue;
				}
				for(auto const& [item_name, value] : group)
				{
					if(!matchesPattern(ipattern, item_name, actual_case_sensitive))
					{
						continue;
					}
					if(!matchesPattern(vpattern, value, actual_case_sensitive))
					{
						continue;
					}
					deleted.try_emplace(group_name).first->second.try_emplace(item_name, value);
				}
			}
		}, data.ini);
	}

	void redo() const override
	{
		std::visit([this](auto& ini)
		{
			for(auto const& [group_name, group] : deleted)
			{
				auto const it{ini.find(group_name)};
				assert(it != std::end(ini));
				if(it == std::end(ini))
				{
					continue;
				}
				for(auto const& [item_name, _] : group)
				{
					it->second.erase(item_name);
				}
			}
		}, data.ini);
	}
	void undo() const override
	{
		std::visit([this](auto& ini)
		{
			for(auto const& [group_name, group] : deleted)
			{
				for(auto const& [item_name, value] : group)
				{
					ini.try_emplace(group_name).first->second.try_emplace(item_name, value);
				}
			}
		}, data.ini);
	}
};
struct ClearAllDoer final
: Doer
{
	Data& data;
	Ini_t ini;
	ClearAllDoer(Data& data)
	: data{data}
	, ini{data.ini}
	{
	}

	void redo() const override
	{
		std::visit([](auto& ini){ ini.clear(); }, data.ini);
	}
	void undo() const override
	{
		data.ini = ini;
	}
};
struct SaveObjectDoer final
: Doer
{
	Data& data;
	string_t group;
	bool gexisted;
	struct NotRelevant final {};
	struct Nonexistant final {};
	using Value_v = std::variant<NotRelevant, Nonexistant, string_t>;
	struct SavedObject final
	{
		Value_v posx, posy, dir, spd, ani, frame, flags;
		std::optional<std::vector<Value_v>> values, strings;
	};
	SavedObject o, n;
	void make_relevant(Value_v& ov, Value_v& nv, string_view_t const name, string_t new_value)
	{
		if(data.has_item(group, name))
		{
			ov.emplace<string_t>(data.get_value_or_create(group, name));
		}
		else
		{
			ov.emplace<Nonexistant>();
		}
		nv.emplace<string_t>(std::move(new_value));
	}
	static auto padded_index_name(string_t prefix, std::size_t const i)
	{
		if(i < 10)
		{
			prefix += TSL('0');
		}
		prefix += std::to_wstring(i);
		return prefix;
	}
	template<bool Const, typename T>
	using mc = std::conditional_t<Const, T const, T>;
	template<bool Const>
	static auto get_alterables(std::conditional_t<Const, RunObject const, RunObject>& obj)
	{
		struct
		{
			std::span<mc<Const, CValue>> values;
			std::span<std::conditional_t<Const, void const* const, void*>> strings;
			std::span<mc<Const, std::int32_t>> flags;
		} ret;
		if((obj.roHo.hoOEFlags & OEFLAG_VALUES) != 0)
		{
			mv* const mV{obj.roHo.hoAdRunHeader->rh4.rh4Mv};
			auto const fusion_version{mV->mvGetVersion()};
			mc<Const, void>* const rov_ptr{reinterpret_cast<mc<Const, std::byte>*>(&obj.roHo) + obj.roHo.hoOffsetValue};
			if((fusion_version & MMFBUILD_MASK) < 243)
			{
				auto& rov{*static_cast<mc<Const, rVal20a>*>(rov_ptr)};
				assert(false); //not easy to support values in this version like this
				ret.strings = {reinterpret_cast<decltype(ret.strings)::pointer>(rov.rvStrings), STRINGS_NUMBEROF_ALTERABLE};
				ret.flags = {reinterpret_cast<decltype(ret.flags)::pointer>(&rov.rvValueFlags), 1};
			}
			else if((fusion_version & MMFVERSION_MASK) < MMFVERSION_25)
			{
				auto& rov{*static_cast<mc<Const, rVal20b>*>(rov_ptr)};
				ret.values = {rov.rvpValues, VALUES_NUMBEROF_ALTERABLE};
				ret.strings = {reinterpret_cast<decltype(ret.strings)::pointer>(rov.rvStrings), STRINGS_NUMBEROF_ALTERABLE};
				ret.flags = {reinterpret_cast<decltype(ret.flags)::pointer>(&rov.rvValueFlags), 1};
			}
			else if((fusion_version & MMFBUILD_MASK) < 292)
			{
				auto& rov{*static_cast<mc<Const, rVal25>*>(rov_ptr)};
				if(rov.rvNumberOfValues >= 0)
				{
					ret.values = {rov.rvpValues, static_cast<std::size_t>(rov.rvNumberOfValues)};
				}
				ret.strings = {reinterpret_cast<decltype(ret.strings)::pointer>(rov.rvStrings), STRINGS_NUMBEROF_ALTERABLE};
				ret.flags = {reinterpret_cast<decltype(ret.flags)::pointer>(&rov.rvValueFlags), 1};
			}
			else
			{
				auto& rov{*static_cast<mc<Const, rVal25P>*>(rov_ptr)};
				if(rov.rvNumberOfValues >= 0)
				{
					ret.values = {rov.rvpValues, static_cast<std::size_t>(rov.rvNumberOfValues)};
				}
				if(rov.rvNumberOfStrings >= 0)
				{
					ret.strings = {reinterpret_cast<decltype(ret.strings)::pointer>(rov.rvpStrings), static_cast<std::size_t>(rov.rvNumberOfStrings)};
				}
				ret.flags = {reinterpret_cast<decltype(ret.flags)::pointer>(&rov.rvValueFlags), 1};
			}
		}
		return ret;
	}
	static auto resize_values(RunObject& obj, std::size_t new_size) noexcept
	{
		static constexpr std::size_t const max_size{std::numeric_limits<std::int32_t>::max()/sizeof(CValue)};
		if(new_size > max_size)
		{
			assert(false);
			new_size = 100;
		}
		std::span<CValue> values;
		if((obj.roHo.hoOEFlags & OEFLAG_VALUES) != 0)
		{
			mv* const mV{obj.roHo.hoAdRunHeader->rh4.rh4Mv};
			auto const fusion_version{mV->mvGetVersion()};
			void* const rov_ptr{reinterpret_cast<std::byte*>(&obj.roHo) + obj.roHo.hoOffsetValue};
			if((fusion_version & MMFBUILD_MASK) < 243)
			{
				auto& rov{*static_cast<rVal20a*>(rov_ptr)};
				std::ignore = new_size;//can't resize
				assert(false); //not easy to support values in this version like this
			}
			else if((fusion_version & MMFVERSION_MASK) < MMFVERSION_25)
			{
				auto& rov{*static_cast<rVal20b*>(rov_ptr)};
				std::ignore = new_size;//can't resize
				values = {rov.rvpValues, VALUES_NUMBEROF_ALTERABLE};
			}
			else
			{
				CValue** rvpValues{};
				std::int32_t* rvNumberOfValues{};
				if((fusion_version & MMFBUILD_MASK) < 292)
				{
					auto& rov{*static_cast<rVal25*>(rov_ptr)};
					rvpValues = &(rov.rvpValues);
					rvNumberOfValues = reinterpret_cast<std::int32_t*>(&(rov.rvNumberOfValues));
				}
				else
				{
					auto& rov{*static_cast<rVal25P*>(rov_ptr)};
					rvpValues = &(rov.rvpValues);
					rvNumberOfValues = reinterpret_cast<std::int32_t*>(&(rov.rvNumberOfValues));
				}
				if((*rvNumberOfValues >= 0 && static_cast<std::size_t>(*rvNumberOfValues) >= new_size))
				{
					values = {*rvpValues, static_cast<std::size_t>(*rvNumberOfValues)};
				}
				else if(auto const new_values{mvReAlloc(mV, *rvpValues, new_size*sizeof(CValue))})
				{
					*rvpValues = static_cast<CValue*>(new_values);
					auto const old_size{*rvNumberOfValues};
					*rvNumberOfValues = new_size;
					values = {*rvpValues, new_size};
					for(auto i{old_size}; i < *rvNumberOfValues; ++i)
					{
						values[i].m_type = TYPE_INT;
						values[i].m_long = {};
					}
				}
				else if(*rvNumberOfValues >= 0)
				{
					values = {*rvpValues, static_cast<std::size_t>(*rvNumberOfValues)};
				}
			}
		}
		return values;
	}
	static auto resize_strings(RunObject& obj, std::size_t new_size) noexcept
	{
		static constexpr std::size_t const max_size{std::numeric_limits<std::int32_t>::max()/sizeof(void*)};
		if(new_size > max_size)
		{
			assert(false);
			new_size = 100;
		}
		std::span<void*> strings;
		if((obj.roHo.hoOEFlags & OEFLAG_VALUES) != 0)
		{
			mv* const mV{obj.roHo.hoAdRunHeader->rh4.rh4Mv};
			auto const fusion_version{mV->mvGetVersion()};
			void* const rov_ptr{reinterpret_cast<std::byte*>(&obj.roHo) + obj.roHo.hoOffsetValue};
			if((fusion_version & MMFBUILD_MASK) < 243)
			{
				auto& rov{*static_cast<rVal20a*>(rov_ptr)};
				std::ignore = new_size; //can't resize
				strings = {reinterpret_cast<decltype(strings)::pointer>(rov.rvStrings), STRINGS_NUMBEROF_ALTERABLE};
			}
			else if((fusion_version & MMFVERSION_MASK) < MMFVERSION_25)
			{
				auto& rov{*static_cast<rVal20b*>(rov_ptr)};
				std::ignore = new_size; //can't resize
				strings = {reinterpret_cast<decltype(strings)::pointer>(rov.rvStrings), STRINGS_NUMBEROF_ALTERABLE};
			}
			else if((fusion_version & MMFBUILD_MASK) < 292)
			{
				auto& rov{*static_cast<rVal25*>(rov_ptr)};
				std::ignore = new_size; //can't resize
				strings = {reinterpret_cast<decltype(strings)::pointer>(rov.rvStrings), STRINGS_NUMBEROF_ALTERABLE};
			}
			else
			{
				auto& rov{*static_cast<rVal25P*>(rov_ptr)};
				if(rov.rvNumberOfStrings >= 0 && static_cast<std::size_t>(rov.rvNumberOfStrings) >= new_size)
				{
					strings = {reinterpret_cast<decltype(strings)::pointer>(rov.rvpStrings), static_cast<std::size_t>(rov.rvNumberOfStrings)};
				}
				else if(auto const new_strings{mvReAlloc(mV, rov.rvpStrings, new_size*sizeof(void*))})
				{
					rov.rvpStrings = static_cast<decltype(rov.rvpStrings)>(new_strings);
					auto const old_size{rov.rvNumberOfStrings};
					rov.rvNumberOfStrings = new_size;
					strings = {reinterpret_cast<decltype(strings)::pointer>(rov.rvpStrings), new_size};
					for(auto i{old_size}; i < rov.rvNumberOfStrings; ++i)
					{
						strings[i] = nullptr;
					}
				}
				else if(rov.rvNumberOfStrings >= 0)
				{
					strings = {reinterpret_cast<decltype(strings)::pointer>(rov.rvpStrings), static_cast<std::size_t>(rov.rvNumberOfStrings)};
				}
			}
		}
		return strings;
	}
	SaveObjectDoer(Data& data_, RunObject const& obj, string_t group_, bool const save_pos_, bool const save_vals_)
	: data{data_}
	, group{std::move(group_)}
	, gexisted{data.has_group(group)}
	{
		if(save_pos_)
		{
			make_relevant(o.posx, n.posx, TSL("posx"sv), std::to_wstring(obj.roHo.hoX));
			make_relevant(o.posy, n.posy, TSL("posy"sv), std::to_wstring(obj.roHo.hoY));
			if((obj.roHo.hoOEFlags & OEFLAG_MOVEMENTS) != 0)
			{
				make_relevant(o.dir, n.dir, TSL("dir"sv), std::to_wstring(obj.roc.rcDir));
				make_relevant(o.spd, n.spd, TSL("spd"sv), std::to_wstring(obj.roc.rcSpeed));
				make_relevant(o.ani, n.ani, TSL("ani"sv), std::to_wstring(obj.roc.rcAnim));
				make_relevant(o.frame, n.frame, TSL("frame"sv), std::to_wstring(obj.roc.rcImage));
			}
		}
		if(save_vals_)
		{
			auto const [values, strings, flags]{get_alterables<true>(obj)};
			if(!std::empty(flags))
			{
				make_relevant(o.flags, n.flags, TSL("flags"sv), std::to_wstring(flags.front()));
			}
			if(!std::empty(values))
			{
				o.values.emplace();
				n.values.emplace();
				for(std::size_t i{0}; i < VALUES_NUMBEROF_ALTERABLE || (i < std::size(values) && i < 100); ++i)
				{
					auto const name{padded_index_name(TSL("val"s), i)};
					Value_v ov, nv;
					if(i >= std::size(values))
					{
						make_relevant(ov, nv, name, TSL("0"s));
					}
					else if(values[i].m_type == TYPE_INT)
					{
						make_relevant(ov, nv, name, std::to_wstring(values[i].m_long));
					}
					else if(values[i].m_type == TYPE_FLOAT)
					{
						make_relevant(ov, nv, name, string_from_float(values[i].m_double));
					}
					else assert(false);
					o.values->emplace_back(std::move(ov));
					n.values->emplace_back(std::move(nv));
				}
			}
			if(!std::empty(strings))
			{
				o.strings.emplace();
				n.strings.emplace();
				mv* const mV{obj.roHo.hoAdRunHeader->rh4.rh4Mv};
				bool const is_unicode{mvIsUnicodeVersion(mV) != FALSE};
				auto const codepage  {mvGetAppCodePage  (mV, obj.roHo.hoAdRunHeader->rhApp)};
				for(std::size_t i{0}; i < STRINGS_NUMBEROF_ALTERABLE || (i < std::size(strings) && i < 100); ++i)
				{
					auto const name{padded_index_name(TSL("str"s), i)};
					Value_v ov, nv;
					if(i >= std::size(strings) || strings[i] == nullptr)
					{
						make_relevant(ov, nv, name, {});
					}
					else if(is_unicode)
					{
						make_relevant(ov, nv, name, static_cast<LPCWSTR>(strings[i]));
					}
					else
					{
						make_relevant(ov, nv, name, lSDK::wide_from_narrow(static_cast<LPCSTR>(strings[i]), codepage));
					}
					o.strings->emplace_back(std::move(ov));
					n.strings->emplace_back(std::move(nv));
				}
			}
		}
	}
	static void load(Data const& data, string_view_t const group, RunObject& obj) noexcept
	{
		if(data.has_group(group))
		{
			{
				auto newX{obj.roHo.hoX};
				auto newY{obj.roHo.hoY};
				if(auto const v{data.get_value_if_exists(group, TSL("posx"sv))})
				{
					newX = std::stoi(*v);
				}
				if(auto const v{data.get_value_if_exists(group, TSL("posy"sv))})
				{
					newY = std::stoi(*v);
				}
				std::ignore = obj.roHo.hoAdRunHeader->rh4.rh4KpxFunctions[RFUNCTION_SETPOSITION].routine(&(obj.roHo), newX, newY);
			}
			if((obj.roHo.hoOEFlags & OEFLAG_MOVEMENTS) != 0)
			{
				if(auto const v{data.get_value_if_exists(group, TSL("dir"sv))})
				{
					obj.roc.rcDir = std::stoi(*v);
					obj.roc.rcChanged = TRUE;
				}
				if(auto const v{data.get_value_if_exists(group, TSL("spd"sv))})
				{
					obj.roc.rcSpeed = std::stoi(*v);
					obj.roc.rcChanged = TRUE;
				}
				if(auto const v{data.get_value_if_exists(group, TSL("ani"sv))})
				{
					obj.roc.rcAnim = std::stoi(*v);
					obj.roc.rcChanged = TRUE;
				}
				if(auto const v{data.get_value_if_exists(group, TSL("frame"sv))})
				{
					obj.roc.rcImage = std::stoi(*v);
					obj.roc.rcChanged = TRUE;
				}
			}
			auto [values, strings, flags]{get_alterables<false>(obj)};
			if(!std::empty(flags))
			if(auto const v{data.get_value_if_exists(group, TSL("flags"sv))})
			{
				flags.front() = std::stoi(*v);
			}
			if(!std::empty(values))
			{
				//count number of values in ini ahead of time
				std::size_t num_values{0};
				for(; num_values < 100; ++num_values)
				{
					if(!data.has_item(group, padded_index_name(TSL("val"s), num_values)))
					{
						break;
					}
				}
				values = resize_values(obj, num_values);
				if(!std::empty(values))
				{
					for(std::size_t i{0}; i < std::size(values); ++i)
					{
						if(auto const v{data.get_value_if_exists(group, padded_index_name(TSL("val"s), i))})
						{
							if(v->find(TSL('.')) != Value_t::npos)
							{
								values[i].m_type = TYPE_FLOAT;
								values[i].m_double = float_from_string<double>(*v);
							}
							else
							{
								values[i].m_type = TYPE_INT;
								values[i].m_long = std::stoi(*v);
							}
						}
					}
				}
			}
			if(!std::empty(strings))
			{
				//count number of strings in ini ahead of time
				std::size_t num_strings{0};
				for(; num_strings < 100; ++num_strings)
				{
					if(!data.has_item(group, padded_index_name(TSL("str"s), num_strings)))
					{
						break;
					}
				}
				strings = resize_strings(obj, num_strings);
				if(!std::empty(strings))
				{
					mv* const mV{obj.roHo.hoAdRunHeader->rh4.rh4Mv};
					bool const is_unicode{mvIsUnicodeVersion(mV) != FALSE};
					auto const codepage  {mvGetAppCodePage  (mV, obj.roHo.hoAdRunHeader->rhApp)};
					for(std::size_t i{0}; i < std::size(strings); ++i)
					{
						if(auto const v{data.get_value_if_exists(group, padded_index_name(TSL("str"s), i))})
						{
							if(is_unicode)
							{
								auto const buf_size{(std::size(*v) + 1)*sizeof(char_t)};
								if(auto const str_ptr{mvMalloc(mV, buf_size)})
								{
									std::ignore = std::memcpy(str_ptr, v->c_str(), buf_size);
									if(strings[i] != nullptr)
									{
										mvFree(mV, strings[i]);
									}
									strings[i] = str_ptr;
								}
							}
							else try
							{
								auto const narrow_v{lSDK::narrow_from_wide(*v, codepage)};
								auto const buf_size{std::size(narrow_v) + 1};
								if(auto const str_ptr{mvMalloc(mV, buf_size)})
								{
									std::ignore = std::memcpy(str_ptr, narrow_v.c_str(), buf_size);
									if(strings[i] != nullptr)
									{
										mvFree(mV, strings[i]);
									}
									strings[i] = str_ptr;
								}
							}
							catch(...)
							{
							}
						}
					}
				}
			}
		}
	}

	void use_relevant(Value_v const& v, string_t name) const
	{
		if(std::holds_alternative<string_t>(v))
		{
			data.get_value_or_create(group, std::move(name)) = std::get<string_t>(v);
		}
		else
		{
			return use_relevant(v, string_view_t{name});
		}
	}
	void use_relevant(Value_v const& v, string_view_t const name) const
	{
		if(std::holds_alternative<Nonexistant>(v))
		{
			std::visit([this, name](auto* g)
			{
				if(auto const it{g->find(name)}; it != std::end(*g))
				{
					g->erase(it);
				}
			}, data.get_group_or_create(group));
		}
		else if(std::holds_alternative<string_t>(v))
		{
			use_relevant(v, string_t{name});
		}
	}
	void use_relevant(SavedObject const& obj) const
	{
		use_relevant(obj.posx, TSL("posx"sv));
		use_relevant(obj.posy, TSL("posy"sv));
		use_relevant(obj.dir, TSL("dir"sv));
		use_relevant(obj.spd, TSL("spd"sv));
		use_relevant(obj.ani, TSL("ani"sv));
		use_relevant(obj.frame, TSL("frame"sv));
		use_relevant(obj.flags, TSL("flags"sv));
		if(obj.values)
		{
			for(std::size_t i{0}; i < std::size(*obj.values); ++i)
			{
				use_relevant(obj.values->at(i), padded_index_name(TSL("val"s), i));
			}
		}
		if(obj.strings)
		{
			for(std::size_t i{0}; i < std::size(*obj.strings); ++i)
			{
				use_relevant(obj.strings->at(i), padded_index_name(TSL("str"s), i));
			}
		}
	}
	void redo() const override
	{
		use_relevant(n);
	}
	void undo() const override
	{
		if(!gexisted)
		{
			data.erase_group(group);
		}
		else
		{
			use_relevant(o);
		}
	}
};

#if defined(FUSION_ACTIONS) || defined(FUSION_CONDITIONS)
static auto get_ac_float(RunData* const run_data) noexcept
-> float
{
	std::int32_t const v{CNC_GetFloatParameter(run_data)};
	return *reinterpret_cast<float const*>(&v);
}
#endif

#ifdef FUSION_ACTIONS
auto FUSION_API actionSetCurrentGroup(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	if(param1 == 0 || !data.has_group(group))
	{
		run_data->settings->current_group = group;
	}
	else
	{
		run_data->settings->current_group = group;
		run_data->settings->current_group.reserve(std::size(group) + 2);
		run_data->settings->current_group += TSL('.');
		std::size_t n{1};
		while(data.has_group((run_data->settings->current_group += std::to_wstring(n))))
		{
			++n;
			run_data->settings->current_group.resize(std::size(group) + 1);
		}
	}

	return action_return_t();
}
auto FUSION_API actionSetValue(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};
	string_view_t group;
	if(run_data->rHo.hoEventNumber == 1)
	{
		group = run_data->settings->current_group;
	}
	else
	{
		group = reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data));
	}
	string_view_t const item{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	string_t value;
	if(flag)
	{
		value = string_from_float(get_ac_float(run_data));
	}
	else
	{
		value = std::to_wstring(CNC_GetIntParameter(run_data));
	}

	data.do_doer<ValueDoer>(data, string_t{group}, string_t{item}, move(value));

	return action_return_t();
}
auto FUSION_API actionSetValueG(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	return actionSetValue(run_data, 0, 0);
}
auto FUSION_API actionSetStringG(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const item {reinterpret_cast<TCHAR const*>(param0)};
	string_view_t const value{reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	data.do_doer<ValueDoer>(data, std::cref(run_data->settings->current_group), string_t{item}, string_t{value});

	return action_return_t();
}
auto FUSION_API actionSetString(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const value{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	Data& data{*(run_data->data)};

	data.do_doer<ValueDoer>(data, string_t{group}, string_t{item}, string_t{value});

	return action_return_t();
}
auto FUSION_API actionSetStringMD5G(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const item {reinterpret_cast<TCHAR const*>(param0)};
	string_view_t const value{reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSetStringMD5(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const value{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSaveObjectG(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	auto const o{reinterpret_cast<RunObject const*>(CNC_GetParameter(run_data))};
	ac_param_t const flag_pos{CNC_GetIntParameter(run_data)};
	ac_param_t const flag_alt{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	data.do_doer<SaveObjectDoer>(data, *o, run_data->settings->current_group, flag_pos != 0, flag_alt != 0);

	return action_return_t();
}
auto FUSION_API actionSaveObject(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	auto const o{reinterpret_cast<RunObject const*>(CNC_GetParameter(run_data))};
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag_pos{CNC_GetIntParameter(run_data)};
	ac_param_t const flag_alt{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	data.do_doer<SaveObjectDoer>(data, *o, string_t{group}, flag_pos != 0, flag_alt != 0);

	return action_return_t();
}
auto FUSION_API actionLoadObjectG(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	auto const o{reinterpret_cast<RunObject*>(param0)};
	Data const& data{*(run_data->data)};

	SaveObjectDoer::load(data, run_data->settings->current_group, *o);

	return action_return_t();
}
auto FUSION_API actionLoadObject(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	auto const o{reinterpret_cast<RunObject*>(param0)};
	string_view_t const group{reinterpret_cast<TCHAR const*>(param1)};
	Data const& data{*(run_data->data)};

	SaveObjectDoer::load(data, group, *o);

	return action_return_t();
}
auto FUSION_API actionSavePositionG(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const item{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	auto const x{static_cast<std::int16_t>(HIWORD(param1))};
	auto const y{static_cast<std::int16_t>(LOWORD(param1))};
	data.do_doer<ValueDoer>(data, run_data->settings->current_group, string_t{item}, std::to_wstring(x) + TSL(","s) + std::to_wstring(y));

	return action_return_t();
}
auto FUSION_API actionSavePosition(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const position{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	auto const x{static_cast<std::int16_t>(HIWORD(position))};
	auto const y{static_cast<std::int16_t>(LOWORD(position))};
	data.do_doer<ValueDoer>(data, string_t{group}, string_t{item}, std::to_wstring(x) + TSL(","s) + std::to_wstring(y));

	return action_return_t();
}
auto FUSION_API actionSaveGlobalValuesG(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSaveAltValues(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionLoadGlobalValuesG(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionLoadAltValues(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionDeleteGroupG(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	data.do_doer<DeleteGroupDoer>(data, run_data->settings->current_group);

	return action_return_t();
}
auto FUSION_API actionDeleteGroup(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	data.do_doer<DeleteGroupDoer>(data, string_t{group});

	return action_return_t();
}
auto FUSION_API actionDeleteItemG(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const item{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	data.do_doer<DeleteItemDoer>(data, std::cref(run_data->settings->current_group), string_t{item});

	return action_return_t();
}
auto FUSION_API actionDeleteItem(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(param0)};
	string_view_t const item {reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	data.do_doer<DeleteItemDoer>(data, string_t{group}, string_t{item});

	return action_return_t();
}
auto FUSION_API actionRenameGroupG(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const new_group{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionRenameGroup(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const     group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const new_group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const mode{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionRenameItemG(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const     item{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const new_item{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionRenameItem(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const    group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const     item{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const new_item{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionMoveItemToGroupG(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const item{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const new_group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionMoveItemToGroup(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const     group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const     item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const new_group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionMoveItem(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const     group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const     item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const new_group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const new_item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionCopyGroup(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const     group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const new_group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const mode{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	data.do_doer<CopyGroupDoer>(data, string_t{group}, string_t{new_group}, mode != 0);

	return action_return_t();
}
auto FUSION_API actionCopyItem(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const     group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const     item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const new_group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const new_item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionDeleteItemInAllGroups(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const item {reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionDeleteFromPattern(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const gpattern{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const ipattern{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const vpattern{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	data.do_doer<DeletePatternDoer>(data, gpattern, ipattern, vpattern, (flag != 0));

	return action_return_t();
}
auto FUSION_API actionClearAll(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	data.do_doer<ClearAllDoer>(data);

	return action_return_t();
}
auto FUSION_API actionPerformSearch(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const gcond{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const icond{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const vcond{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	paramExt const* settings{reinterpret_cast<paramExt const*>(CNC_GetParameter(run_data))};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionPerformMultiplesSearch(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(param0)};
	string_view_t const item {reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionClearResults(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionCreateSubINI(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const file{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const gcond{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const icond{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const vcond{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionCreateSubINIObject(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	paramExt const* object_name{reinterpret_cast<paramExt const*>( CNC_GetParameter(run_data))};
	string_view_t const file {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const gcond{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const icond{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const vcond{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionMergeFile(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const file{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionMergeGroupFile(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const file{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const file_group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionMerge(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	paramExt const* object_name{reinterpret_cast<paramExt const*>(param0)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionMergeGroupObject(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	paramExt const* object_name  {reinterpret_cast<paramExt const*>(      CNC_GetParameter(run_data))};
	string_view_t const object_group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const        group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionNew(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const file{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	if(param1 == 0)
	{
		data.close(run_data);
	}
	std::filesystem::path file_path{file};
	if(!file_path.has_root_directory())
	{
		file_path = run_data->settings->default_dir/file_path;
	}
	file_path.make_preferred();
	data.settings.ini_path = std::move(file_path);
	if(param1 != 0)
	{
		data.autosave(run_data);
	}

	return action_return_t();
}
auto FUSION_API actionLoad(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const file{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	data.close(run_data);
	data.settings.read_only = (param1? true : false);
	std::filesystem::path file_path{file};
	if(!file_path.has_root_directory())
	{
		file_path = run_data->settings->default_dir/file_path;
	}
	file_path.make_preferred();
	data.settings.ini_path = file_path;
	data.load(run_data);

	return action_return_t();
}
auto FUSION_API actionSave(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	data.save(run_data);

	return action_return_t();
}
auto FUSION_API actionSaveAs(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const file{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	std::filesystem::path file_path{file};
	if(!file_path.has_root_directory())
	{
		file_path = run_data->settings->default_dir/file_path;
	}
	file_path.make_preferred();
	data.save(run_data, file_path);

	return action_return_t();
}
auto FUSION_API actionBackupTo(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const file{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag{CNC_GetIntParameter(run_data)};
	string_view_t const key{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionClose(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	data.close(run_data);

	return action_return_t();
}
auto FUSION_API actionLoadFromString(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const ini_str{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	if(param1 == 0)
	{
		data.close(run_data);
	}
	data.load(ini_str);

	return action_return_t();
}
auto FUSION_API actionSetAutoSave(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	data.settings.auto_save = (param0? true : false);
	run_data->settings->auto_load = (param1? true : false);
	//yes, auto save and auto load are stored in different places... ask Jax why

	return action_return_t();
}
auto FUSION_API actionSetCompression(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSetEncryption(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const new_encrypt_key{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	if(std::empty(new_encrypt_key))
	{
		data.settings.encrypt_key.reset();
	}
	else
	{
		data.settings.encrypt_key.emplace(lSDK::narrow_from_wide(new_encrypt_key));
	}

	return action_return_t();
}
auto FUSION_API actionSetReadOnly(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSetCaseSensitive(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSetEscapeChars(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	ac_param_t const flag_groups{CNC_GetIntParameter(run_data)};
	ac_param_t const flag_items {CNC_GetIntParameter(run_data)};
	ac_param_t const flag_values{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionNeverQuoteStrings(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSetRepeatModes(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	ac_param_t const flag_groups {CNC_GetIntParameter(run_data)};
	ac_param_t const flag_items  {CNC_GetIntParameter(run_data)};
	ac_param_t const flag_repeats{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSetNewLineChar(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const str{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSetDefaultDirectory(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const directory{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionCompressFile(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const file_source{reinterpret_cast<TCHAR const*>(param0)};
	string_view_t const file_dest  {reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionDeCompressFile(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const file_source{reinterpret_cast<TCHAR const*>(param0)};
	string_view_t const file_dest  {reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionOpenDialog(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const title{reinterpret_cast<TCHAR const*>(param0)};
	paramExt const* settings{reinterpret_cast<paramExt const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionAddRepeatedItem(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const str  {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const mode{CNC_GetIntParameter(run_data)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionCloseDialog(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionRefreshDialog(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionExportCVS(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	ac_param_t const io{CNC_GetIntParameter(run_data)};
	string_view_t const file{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const separator{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionImportCVS(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	ac_param_t const io{CNC_GetIntParameter(run_data)};
	string_view_t const file{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const separator{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionToChart(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	paramExt const* object_name{reinterpret_cast<paramExt const*>(param0)};
	string_view_t const group{reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionFindSubGroups(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionEnableSubGroups(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSwitchGlobalObject(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const data_slot{reinterpret_cast<TCHAR const*>(param0)};

	if(auto const it{Data::global.find(data_slot)}; it != std::end(Data::global))
	{
		//give up our data and use the other
		run_data->data = &(it->second);
		delete run_data->local_data;
		run_data->local_data = nullptr;
	}
	else if(run_data->local_data)
	{
		//move our data into global
		run_data->data = &(Data::global.emplace(data_slot, std::move(*(run_data->local_data))).first->second);
		delete run_data->local_data;
		run_data->local_data = nullptr;
	}
	else
	{
		//copy our data into global
		run_data->data = &(Data::global.emplace(data_slot, *(run_data->data)).first->second);
	}

	return action_return_t();
}
auto FUSION_API actionSSS(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	paramExt const* settings{reinterpret_cast<paramExt const*>(param0)};
	string_view_t const group{reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSetItemArray(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const part{CNC_GetIntParameter(run_data)};
	string_view_t const v{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionLoadFromArray(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	paramExt const* object_name{reinterpret_cast<paramExt const*>(param0)};
	string_view_t const group{reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSaveToArray(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	paramExt const* object_name{reinterpret_cast<paramExt const*>(param0)};
	string_view_t const group{reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionFromChart(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	paramExt const* object_name{reinterpret_cast<paramExt const*>(param0)};
	string_view_t const group{reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSaveChartSettings(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	paramExt const* object_name{reinterpret_cast<paramExt const*>(param0)};
	string_view_t const group{reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionLoadChartSettings(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	paramExt const* object_name{reinterpret_cast<paramExt const*>(param0)};
	string_view_t const group{reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionLoadChangeFile(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> action_return_t
{
	string_view_t const file{reinterpret_cast<TCHAR const*>(param0)};
	paramExt const* settings{reinterpret_cast<paramExt const*>(param1)};
	Data& data{*(run_data->data)};

	enum struct LoadType
	{
		Load,
		Nothing
	};
	enum struct FNameType
	{
		Change,
		Nothing,
		ChangeIfOk
	};
	enum struct DeleteType
	{
		Clear,
		Nothing,
		ClearIfOk
	};
	LoadType   const lt{LoadType  (settings->pextData[0])};
	FNameType  const ft{FNameType (settings->pextData[1])};
	DeleteType const dt{DeleteType(settings->pextData[2])};
	bool save_now {(((settings->pextData[3]) & 1)? true : false)};
	bool read_only{(((settings->pextData[3]) & 2)? true : false)};

	std::filesystem::path file_path{file};
	if(!file_path.has_root_directory())
	{
		file_path = run_data->settings->default_dir/file_path;
	}
	file_path.make_preferred();

	bool const file_exists{exists(file_path) && is_regular_file(file_path)};

	if(dt == DeleteType::Clear || (dt == DeleteType::ClearIfOk && file_exists))
	{
		data.do_doer<ClearAllDoer>(data);
	}

	if(lt == LoadType::Load)
	{
		data.settings.read_only = read_only;
		data.load(run_data, file_path);
	}
	if(ft == FNameType::Change || (ft == FNameType::ChangeIfOk && file_exists))
	{
		data.settings.ini_path = file_path;
	}
	if(save_now)
	{
		data.save(run_data);
	}

	return action_return_t();
}
auto FUSION_API actionUndo(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	data.undo();

	return action_return_t();
}
auto FUSION_API actionRedo(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	data.redo();

	return action_return_t();
}
auto FUSION_API actionClearUndoStack(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionAddNewUndoBlock(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
auto FUSION_API actionSetManualMode(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> action_return_t
{
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return action_return_t();
}
#endif

#ifdef FUSION_CONDITIONS
auto FUSION_API conditionEvent(RunData*, ac_param_t, ac_param_t) noexcept
-> condition_return_t
{
	return FUSION_CONDITIONS_TRUE;
}
auto FUSION_API conditionHasGroupG(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> condition_return_t
{
	Data& data{*(run_data->data)};

	return data.has_group(run_data->settings->current_group)? FUSION_CONDITIONS_TRUE : FUSION_CONDITIONS_FALSE;
}
auto FUSION_API conditionHasGroup(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> condition_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	return data.has_group(group)? FUSION_CONDITIONS_TRUE : FUSION_CONDITIONS_FALSE;
}
auto FUSION_API conditionHasItemG(RunData* const run_data, ac_param_t const param0, ac_param_t) noexcept
-> condition_return_t
{
	string_view_t const item{reinterpret_cast<TCHAR const*>(param0)};
	Data& data{*(run_data->data)};

	return data.has_item(run_data->settings->current_group, item)? FUSION_CONDITIONS_TRUE : FUSION_CONDITIONS_FALSE;
}
auto FUSION_API conditionHasItem(RunData* const run_data, ac_param_t const param0, ac_param_t const param1) noexcept
-> condition_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(param0)};
	string_view_t const item {reinterpret_cast<TCHAR const*>(param1)};
	Data& data{*(run_data->data)};

	return data.has_item(group, item)? FUSION_CONDITIONS_TRUE : FUSION_CONDITIONS_FALSE;
}
auto FUSION_API conditionCompareMD5G(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> condition_return_t
{
	string_view_t const item{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const def {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const comp{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return FUSION_CONDITIONS_FALSE;
}
auto FUSION_API conditionCompareMD5(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> condition_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const item {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const def  {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const comp {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return FUSION_CONDITIONS_FALSE;
}
auto FUSION_API conditionWildcatMatch(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> condition_return_t
{
	string_view_t const pattern{reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	string_view_t const str    {reinterpret_cast<TCHAR const*>(CNC_GetStringParameter(run_data))};
	ac_param_t const flag = CNC_GetIntParameter(run_data);
	Data& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return FUSION_CONDITIONS_FALSE;
}
auto FUSION_API conditionHasUndo(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> condition_return_t
{
	Data& data{*(run_data->data)};

	return std::empty(data.undos)? FUSION_CONDITIONS_FALSE : FUSION_CONDITIONS_TRUE;
}
auto FUSION_API conditionHasRedo(RunData* const run_data, ac_param_t, ac_param_t) noexcept
-> condition_return_t
{
	Data& data{*(run_data->data)};

	return std::empty(data.undos)? FUSION_CONDITIONS_FALSE : FUSION_CONDITIONS_TRUE;
}
#endif

#ifdef FUSION_EXPRESSIONS
template<bool First = false>
static auto get_e_float(RunData* const run_data, e_params_t const params_handle) noexcept
-> float
{
	if constexpr(First)
	{
		std::int32_t const v{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_FLOAT)};
		return *reinterpret_cast<float const*>(&v);
	}
	else if constexpr(!First)
	{
		std::int32_t const v{CNC_GetNextExpressionParameter(run_data, params_handle, TYPE_FLOAT)};
		return *reinterpret_cast<float const*>(&v);
	}
}
static auto temp_string(RunData* const run_data, string_view_t const str) noexcept
-> expression_return_t
{
	if(!str.empty())
	{
		LPARAM const buffer_size{static_cast<LPARAM>((str.size()+1)*sizeof(TCHAR))};
		if(char_t* const p{reinterpret_cast<char_t*>(callRunTimeFunction(run_data, RFUNCTION_GETSTRINGSPACE_EX, 0, buffer_size))})
		{
			*std::copy(cbegin(str), cend(str), p) = TSL('\0');
			return reinterpret_cast<expression_return_t>(p);
		}
	}
	return reinterpret_cast<expression_return_t>(TSL(""));
}

auto FUSION_API expressionGetItemValue(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	Data const& data{*(run_data->data)};
	string_view_t group, item;
	if(run_data->rHo.hoEventNumber == 0)
	{
		group = run_data->settings->current_group;
		item  = reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING));
	}
	else
	{
		group = reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING));
		item  = reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING));
	}
	auto const def{CNC_GetNextExpressionParameter(run_data, params_handle, TYPE_FLOAT)};

	if(auto const v{data.get_value_if_exists(group, item)})
	{
		if(v->find(_T('.')) != Value_t::npos)
		{
			run_data->rHo.hoFlags |= HOF_FLOAT;
			float const ret{float_from_string<float>(*v)};
			return *reinterpret_cast<std::int32_t const*>(&ret);
		}
		else
		{
			return (expression_return_t)std::stoll(*v);
		}
	}
	else
	{
		run_data->rHo.hoFlags |= HOF_FLOAT;
		return def;
	}
}
auto FUSION_API expressionGetItemValueG(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	return expressionGetItemValue(run_data, params_handle);
}
auto FUSION_API expressionGetItemStringG(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const item{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	auto          const def {                               CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING) };
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	if(auto const v{data.get_value_if_exists(run_data->settings->current_group, item)})
	{
		return temp_string(run_data, *v);
	}
	else
	{
		return def; //no need to make a copy
	}
}
auto FUSION_API expressionGetItemString(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	string_view_t const item {reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING))};
	auto          const def  {                               CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING) };
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	if(auto const v{data.get_value_if_exists(group, item)})
	{
		return temp_string(run_data, *v);
	}
	else
	{
		return def; //no need to make a copy
	}
}
auto FUSION_API expressionGetXG(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const item{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	Data const& data{*(run_data->data)};

	if(auto const v{data.get_value_if_exists(run_data->settings->current_group, item)})
	{
		return std::stoi(v->substr(0, v->find(TSL(','))));
	}

	return 0;
}
auto FUSION_API expressionGetYG(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const item{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	Data const& data{*(run_data->data)};

	if(auto const v{data.get_value_if_exists(run_data->settings->current_group, item)})
	if(auto const comma_pos{v->find(TSL(','))}; comma_pos != Value_t::npos)
	{
		return std::stoi(v->substr(comma_pos + 1));
	}

	return 0;
}
auto FUSION_API expressionGetX(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	string_view_t const item {reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING))};
	Data const& data{*(run_data->data)};

	if(auto const v{data.get_value_if_exists(group, item)})
	{
		return std::stoi(v->substr(0, v->find(TSL(','))));
	}

	return 0;
}
auto FUSION_API expressionGetY(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	string_view_t const item {reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING))};
	Data const& data{*(run_data->data)};

	if(auto const v{data.get_value_if_exists(group, item)})
	if(auto const comma_pos{v->find(TSL(','))}; comma_pos != Value_t::npos)
	{
		return std::stoi(v->substr(comma_pos + 1));
	}

	return 0;
}
auto FUSION_API expressionNthGroup(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	auto const n{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_INT)};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	return std::visit([run_data, n](auto const& ini) noexcept
	-> expression_return_t
	{
		if(n >= 0 && static_cast<std::size_t>(n) < ini.size())
		{
			using std::cbegin;
			auto it{cbegin(ini)};
			using std::advance;
			advance(it, n);
			return temp_string(run_data, it->first);
		}

		return (expression_return_t)_T("");
	}, data.ini);
}
auto FUSION_API expressionNthItemG(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	auto const n{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_INT)};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	return std::visit([run_data, n](auto const* const g) noexcept
	-> expression_return_t
	{
		if(g != nullptr && n >= 0 && static_cast<std::size_t>(n) < g->size())
		{
			using std::cbegin;
			auto it{cbegin(*g)};
			using std::advance;
			advance(it, n);
			return temp_string(run_data, it->first);
		}

		return (expression_return_t)_T("");
	}, data.get_group_if_exists(run_data->settings->current_group));
}
auto FUSION_API expressionNthItem(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	auto const n{CNC_GetNextExpressionParameter(run_data, params_handle, TYPE_INT)};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	return std::visit([run_data, n](auto const* const g) noexcept
	-> expression_return_t
	{
		if(g != nullptr && n >= 0 && static_cast<std::size_t>(n) < g->size())
		{
			using std::cbegin;
			auto it{cbegin(*g)};
			using std::advance;
			advance(it, n);
			return temp_string(run_data, it->first);
		}

		return (expression_return_t)_T("");
	}, data.get_group_if_exists(group));
}
auto FUSION_API expressionNthItemValueG(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	auto const n{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_INT)};
	Data const& data{*(run_data->data)};

	return std::visit([run_data, n](auto const* const g) noexcept
	-> expression_return_t
	{
		if(g != nullptr && n >= 0 && static_cast<std::size_t>(n) < g->size())
		{
			using std::cbegin;
			auto it{cbegin(*g)};
			using std::advance;
			advance(it, n);
			if(it->second.find(_T('.')) != Value_t::npos)
			{
				run_data->rHo.hoFlags |= HOF_FLOAT;
				float const ret{std::stof(it->second)};
				return *reinterpret_cast<std::int32_t const*>(&ret);
			}
			else
			{
				return (expression_return_t)std::stoll(it->second);
			}
		}

		return 0;
	}, data.get_group_if_exists(run_data->settings->current_group));
}
auto FUSION_API expressionNthItemValue(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	auto const n{CNC_GetNextExpressionParameter(run_data, params_handle, TYPE_INT)};
	Data const& data{*(run_data->data)};

	return std::visit([run_data, n](auto const* const g) noexcept
	-> expression_return_t
	{
		if(g != nullptr && n >= 0 && static_cast<std::size_t>(n) < g->size())
		{
			using std::cbegin;
			auto it{cbegin(*g)};
			using std::advance;
			advance(it, n);
			if(it->second.find(_T('.')) != Value_t::npos)
			{
				run_data->rHo.hoFlags |= HOF_FLOAT;
				float const ret{std::stof(it->second)};
				return *reinterpret_cast<std::int32_t const*>(&ret);
			}
			else
			{
				return (expression_return_t)std::stoll(it->second);
			}
		}

		return 0;
	}, data.get_group_if_exists(group));
}
auto FUSION_API expressionNthItemStringG(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	auto const n{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_INT)};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	return std::visit([run_data, n](auto const* const g) noexcept
	-> expression_return_t
	{
		if(g != nullptr && n >= 0 && static_cast<std::size_t>(n) < g->size())
		{
			using std::cbegin;
			auto it{cbegin(*g)};
			using std::advance;
			advance(it, n);
			return temp_string(run_data, it->second);
		}

		return (expression_return_t)_T("");
	}, data.get_group_if_exists(run_data->settings->current_group));
}
auto FUSION_API expressionNthItemString(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	auto const n{CNC_GetNextExpressionParameter(run_data, params_handle, TYPE_INT)};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	return std::visit([run_data, n](auto const* const g) noexcept
	-> expression_return_t
	{
		if(g != nullptr && n >= 0 && static_cast<std::size_t>(n) < g->size())
		{
			using std::cbegin;
			auto it{cbegin(*g)};
			using std::advance;
			advance(it, n);
			return temp_string(run_data, it->second);
		}

		return (expression_return_t)_T("");
	}, data.get_group_if_exists(group));
}
auto FUSION_API expressionGroupCount(RunData* const run_data, e_params_t) noexcept
-> expression_return_t
{
	Data const& data{*(run_data->data)};

	return std::visit([](auto const& ini) noexcept -> expression_return_t { return std::size(ini); }, data.ini);
}
auto FUSION_API expressionItemCountG(RunData* const run_data, e_params_t) noexcept
-> expression_return_t
{
	Data const& data{*(run_data->data)};

	return std::visit([](auto const* const g) noexcept -> expression_return_t
	{
		if(g != nullptr)
		{
			return std::size(*g);
		}
		return 0;
	}, data.get_group_if_exists(run_data->settings->current_group));
}
auto FUSION_API expressionItemCount(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	Data const& data{*(run_data->data)};

	return std::visit([](auto const* const g) noexcept -> expression_return_t
	{
		if(g != nullptr)
		{
			return std::size(*g);
		}
		return 0;
	}, data.get_group_if_exists(group));
}
auto FUSION_API expressionTotalItems(RunData* const run_data, e_params_t) noexcept
-> expression_return_t
{
	Data const& data{*(run_data->data)};

	expression_return_t n{0};
	std::visit([&n](auto const& ini) noexcept
	{
		for(auto const& group : ini)
		{
			n += group.second.size();
		}
	}, data.ini);

	return n;
}
auto FUSION_API expressionSearchResultCounts(RunData* const run_data, e_params_t) noexcept
-> expression_return_t
{
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return 0;
}
auto FUSION_API expressionGetNthSearchResultGroup(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	auto const n{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_INT)};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionGetNthSearchResultItem(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	auto const n{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_INT)};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionGetNthSearchResultItemValue(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	auto const n{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_INT)};
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return 0;
}
auto FUSION_API expressionGetNthSearchResultItemString(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	auto const n{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_INT)};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionGetNthSearchResultPath(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	auto const n{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_INT)};
	string_view_t const separator{reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter(run_data, params_handle, TYPE_STRING))};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionWriteStream(RunData* const run_data, e_params_t) noexcept
-> expression_return_t
{
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	return temp_string(run_data, data.stringify());
}
auto FUSION_API expressionHashString(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const str{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionEncryptString(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const p1{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	string_view_t const p2{reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING))};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionEscapeString(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const str{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionUnEscapeString(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const str{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionInnerProduct(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const p1{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	string_view_t const p2{reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING))};
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return 0;
}
auto FUSION_API expressionInnerProductStr(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const p1{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	string_view_t const p2{reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING))};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionNthSortedName(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	auto const n{CNC_GetNextExpressionParameter(run_data, params_handle, TYPE_INT)};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionNthSortedValue(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	auto const n{CNC_GetNextExpressionParameter(run_data, params_handle, TYPE_INT)};
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return 0;
}
auto FUSION_API expressionGetCSV(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const p1{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	string_view_t const p2{reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING))};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionNthOverallItem(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	auto const n{CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_INT)};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionUniqueItemCount(RunData* const run_data, e_params_t) noexcept
-> expression_return_t
{
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return 0;
}
auto FUSION_API expressionGetItemArray(RunData* const run_data, e_params_t const params_handle) noexcept
-> expression_return_t
{
	string_view_t const group{reinterpret_cast<TCHAR const*>(CNC_GetFirstExpressionParameter(run_data, params_handle, TYPE_STRING))};
	string_view_t const item {reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING))};
	auto const index{CNC_GetNextExpressionParameter(run_data, params_handle, TYPE_INT)};
	string_view_t const def  {reinterpret_cast<TCHAR const*>(CNC_GetNextExpressionParameter (run_data, params_handle, TYPE_STRING))};
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionCurGroup(RunData* const run_data, e_params_t) noexcept
-> expression_return_t
{
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	return temp_string(run_data, run_data->settings->current_group);
}
auto FUSION_API expressionCurGroupString(RunData* const run_data, e_params_t) noexcept
-> expression_return_t
{
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
auto FUSION_API expressionFname(RunData* const run_data, e_params_t) noexcept
-> expression_return_t
{
	run_data->rHo.hoFlags |= HOF_STRING;
	Data const& data{*(run_data->data)};

	NOT_YET_IMPLEMENTED
	//

	return (expression_return_t)_T("");
}
#endif

#ifdef FUSION_ACTIONS
std::vector<fusion::action_func_pointer> const RunData::ACTIONS
{
	/*  0 */ actionSetCurrentGroup,
	/*  1 */ actionSetValueG,
	/*  2 */ actionSetStringG,
	/*  3 */ actionSetStringMD5G,
	/*  4 */ actionSaveObjectG,
	/*  5 */ actionLoadObjectG,
	/*  6 */ actionSavePositionG,
	/*  7 */ actionSaveGlobalValuesG,
	/*  8 */ actionLoadGlobalValuesG,
	/*  9 */ actionDeleteGroupG,
	/* 10 */ actionDeleteItemG,
	/* 11 */ actionRenameGroupG,
	/* 12 */ actionRenameItemG,
	/* 13 */ actionMoveItemToGroupG,
	/* 14 */ actionSetValue,
	/* 15 */ actionSetString,
	/* 16 */ actionSetStringMD5,
	/* 17 */ actionSaveObject,
	/* 18 */ actionLoadObject,
	/* 19 */ actionSavePosition,
	/* 20 */ actionSaveAltValues,
	/* 21 */ actionLoadAltValues,
	/* 22 */ actionRenameGroup,
	/* 23 */ actionRenameItem,
	/* 24 */ actionMoveItem,
	/* 25 */ actionMoveItemToGroup,
	/* 26 */ actionCopyGroup,
	/* 27 */ actionCopyItem,
	/* 28 */ actionDeleteGroup,
	/* 29 */ actionDeleteItem,
	/* 30 */ actionDeleteItemInAllGroups,
	/* 31 */ actionDeleteFromPattern,
	/* 32 */ actionClearAll,
	/* 33 */ actionPerformSearch,
	/* 34 */ actionPerformMultiplesSearch,
	/* 35 */ actionClearResults,
	/* 36 */ actionCreateSubINI,
	/* 37 */ actionCreateSubINIObject,
	/* 38 */ actionMergeFile,
	/* 39 */ actionMergeGroupFile,
	/* 40 */ actionMerge,
	/* 41 */ actionMergeGroupObject,
	/* 42 */ actionNew,
	/* 43 */ actionLoad,
	/* 44 */ actionSave,
	/* 45 */ actionSaveAs,
	/* 46 */ actionBackupTo,
	/* 47 */ actionClose,
	/* 48 */ actionLoadFromString,
	/* 49 */ actionSetAutoSave,
	/* 50 */ actionSetCompression,
	/* 51 */ actionSetEncryption,
	/* 52 */ actionSetReadOnly,
	/* 53 */ actionSetCaseSensitive,
	/* 54 */ actionSetEscapeChars,
	/* 55 */ actionNeverQuoteStrings,
	/* 56 */ actionSetRepeatModes,
	/* 57 */ actionSetNewLineChar,
	/* 58 */ actionSetDefaultDirectory,
	/* 59 */ actionCompressFile,
	/* 60 */ actionDeCompressFile,
	/* 61 */ actionOpenDialog,
	/* 62 */ actionAddRepeatedItem,
	/* 63 */ actionCloseDialog,
	/* 64 */ actionRefreshDialog,
	/* 65 */ actionExportCVS,
	/* 66 */ actionImportCVS,
	/* 67 */ actionToChart,
	/* 68 */ actionFindSubGroups,
	/* 69 */ actionEnableSubGroups,
	/* 70 */ actionSwitchGlobalObject,
	/* 71 */ actionSSS,
	/* 72 */ actionSetItemArray,
	/* 73 */ actionLoadFromArray,
	/* 74 */ actionSaveToArray,
	/* 75 */ actionFromChart,
	/* 76 */ actionSaveChartSettings,
	/* 77 */ actionLoadChartSettings,
	/* 78 */ actionLoadChangeFile,
	/* 79 */ actionUndo,
	/* 80 */ actionRedo,
	/* 81 */ actionClearUndoStack,
	/* 82 */ actionAddNewUndoBlock,
	/* 83 */ actionSetManualMode,
};
#endif
#ifdef FUSION_CONDITIONS
std::vector<fusion::condition_func_pointer> const RunData::CONDITIONS
{
	/*  0 */ conditionHasGroupG,
	/*  1 */ conditionHasItemG,
	/*  2 */ conditionCompareMD5G,
	/*  3 */ conditionHasGroup,
	/*  4 */ conditionHasItem,
	/*  5 */ conditionCompareMD5,
	/*  6 */ conditionWildcatMatch,
	/*  7 */ conditionEvent,
	/*  8 */ conditionEvent,
	/*  9 */ conditionEvent,
	/* 10 */ conditionEvent,
	/* 11 */ conditionEvent,
	/* 12 */ conditionEvent,
	/* 13 */ conditionEvent,
	/* 14 */ conditionEvent,
	/* 15 */ conditionEvent,
	/* 16 */ conditionEvent,
	/* 17 */ conditionEvent,
	/* 18 */ conditionHasUndo,
	/* 19 */ conditionHasRedo,
};
#endif
#ifdef FUSION_EXPRESSIONS
std::vector<fusion::expression_func_pointer> const RunData::EXPRESSIONS
{
	/*  0 */ expressionGetItemValueG,
	/*  1 */ expressionGetItemStringG,
	/*  2 */ expressionGetXG,
	/*  3 */ expressionGetYG,
	/*  4 */ expressionNthItemG,
	/*  5 */ expressionNthItemValueG,
	/*  6 */ expressionNthItemStringG,
	/*  7 */ expressionItemCountG,
	/*  8 */ expressionGetItemValue,
	/*  9 */ expressionGetItemString,
	/* 10 */ expressionGetX,
	/* 11 */ expressionGetY,
	/* 12 */ expressionNthGroup,
	/* 13 */ expressionNthItem,
	/* 14 */ expressionNthItemString,
	/* 15 */ expressionNthItemValue,
	/* 16 */ expressionGroupCount,
	/* 17 */ expressionItemCount,
	/* 18 */ expressionTotalItems,
	/* 19 */ expressionSearchResultCounts,
	/* 20 */ expressionGetNthSearchResultGroup,
	/* 21 */ expressionGetNthSearchResultItem,
	/* 22 */ expressionGetNthSearchResultItemValue,
	/* 23 */ expressionGetNthSearchResultItemString,
	/* 24 */ expressionGetNthSearchResultPath,
	/* 25 */ expressionWriteStream,
	/* 26 */ expressionHashString,
	/* 27 */ expressionEncryptString,
	/* 28 */ expressionEscapeString,
	/* 29 */ expressionUnEscapeString,
	/* 30 */ expressionInnerProduct,
	/* 31 */ expressionInnerProductStr,
	/* 32 */ expressionNthSortedName,
	/* 33 */ expressionNthSortedValue,
	/* 34 */ expressionGetCSV,
	/* 35 */ expressionNthOverallItem,
	/* 36 */ expressionUniqueItemCount,
	/* 37 */ expressionGetItemArray,
	/* 38 */ expressionCurGroup,
	/* 39 */ expressionCurGroupString,
	/* 40 */ expressionFname,
};
#endif
