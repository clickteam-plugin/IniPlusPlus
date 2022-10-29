#include "FusionAPI.hpp"
#include "EditData.hpp"
#include "RunData.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <set>

#include <Shlobj.h>

using namespace std::string_literals;
using namespace std::string_view_literals;

[[nodiscard]] static std::filesystem::path longify_path(std::filesystem::path p)
{
	if(p.has_root_path())
	{
		auto const root_path{p.root_path()};
		if(!root_path.native().starts_with(LR"path(\\?)path"sv)
		&& !root_path.native().starts_with(LR"path(\\.)path"sv)
		&& !root_path.native().starts_with(LR"path(\??)path"sv))
		{
			p = LR"path(\\?\)path"s + p.native();
		}
	}
	return p;
}

[[nodiscard]] static std::filesystem::path get_known_folder(::KNOWNFOLDERID const& id)
{
	struct raii final
	{
		::PWSTR raw{};
		raii(::KNOWNFOLDERID const& id) noexcept { std::ignore = ::SHGetKnownFolderPath(id, {}, {}, &raw); }
		~raii() noexcept { ::CoTaskMemFree(raw); }
	} const r(id);
	if(r.raw && *r.raw)
	{
		return longify_path(r.raw);
	}
	return longify_path(std::filesystem::current_path());
}
enum struct DefaultFolderProp
: std::int32_t
{
	Windows = 0, //Windows Folder
	Current = 1, //(Initial) Current Folder
	Program = 2, //Program Folder
	Appdata = 3, //Application Data (User)
	AppdataCommon = 4, //Application Data (Common)
};
[[nodiscard]] std::filesystem::path get_default_path(std::int32_t const type, RunHeader const& rh)
{
	switch(DefaultFolderProp{type})
	{
		case DefaultFolderProp::Windows: return get_known_folder(::FOLDERID_Windows);
		case DefaultFolderProp::Current: break;
		case DefaultFolderProp::Program:
		{
			CRunApp const* app = rh.rhApp;
			while(app->m_pParentApp)
			{
				app = app->m_pParentApp;
			}
			if((app->m_hdr.gaFlags & GA_ONEFILE) == 0 && app->m_editorFileName != nullptr)
			{
				return longify_path(std::filesystem::path(app->m_editorFileName).parent_path());
			}
			return longify_path(std::filesystem::path(rh.rhApp->m_appFileName).parent_path());
		}
		case DefaultFolderProp::Appdata: return get_known_folder(::FOLDERID_RoamingAppData);
		case DefaultFolderProp::AppdataCommon: return get_known_folder(::FOLDERID_ProgramData);
		default: break;
	}
	return longify_path(std::filesystem::current_path());
}

static auto load_text_file(RunData const* const run_data, lSDK::char_t const* const filename) noexcept
{
	return std::unique_ptr<lSDK::char_t, free_via_mv>
	{
		mvLoadTextFile(run_data->rHo.hoAdRunHeader->rh4.rh4Mv, filename, CHARENC_DEFAULT, FALSE),
		free_via_mv   {run_data->rHo.hoAdRunHeader->rh4.rh4Mv}
	};
}
static auto save_text_file(RunData const* const run_data, lSDK::char_t const* const filename, lSDK::char_t const* const text) noexcept
{
	return mvSaveTextFile(run_data->rHo.hoAdRunHeader->rh4.rh4Mv, filename, text, CHARENC_DEFAULT);
}

//seems like Jax used an encryption algorithm by Rodney Thayer https://www.codeproject.com/Articles/2645/Fast-Simple-Secure-Encryption-Routine
static constexpr void thayer_crypt(std::string& inp, std::string_view const key) noexcept
{
	if(std::empty(key))
	{
		return;
	}

	constexpr std::size_t const SBOX_SIZE{256};
	std::array<char, SBOX_SIZE> sbox{}, sbox2{};
	for(std::size_t i{0}; i < std::size(sbox); ++i)
	{
		static_assert(std::is_signed_v<char>);
		sbox.at(i) = static_cast<char>(i);
	}
	for(std::size_t i{0}; i < std::size(sbox2); ++i)
	{
		sbox2.at(i) = key.at(i % std::size(key));
	}
	for(std::size_t i{0}, j{0}; i < SBOX_SIZE; ++i)
	{
		j = ((j + static_cast<std::uint32_t>(sbox.at(i)) + static_cast<std::uint32_t>(sbox2.at(i))) % SBOX_SIZE);
		std::swap(sbox.at(i), sbox.at(j));
	}
	for(std::size_t x{0}, i{0}, j{0}; x < std::size(inp); ++x)
	{
		i = ((i + 1) % SBOX_SIZE);
		j = ((j + static_cast<std::uint32_t>(sbox[i])) % SBOX_SIZE);
		std::swap(sbox.at(i), sbox.at(j));
		auto const t{(static_cast<std::uint32_t>(sbox.at(i)) + static_cast<std::uint32_t>(sbox.at(j))) % SBOX_SIZE};
		auto const k{sbox.at(t)};
		inp[x] = (inp[x] ^ k);
	}
}
void Data::toggle_encryption(std::string& d, std::string_view key) noexcept
{
	return thayer_crypt(d, key);
}

[[nodiscard]] static constexpr auto is_end_escaped(lSDK::string_view_t const s) noexcept
{
	if(auto const last_non_escape{s.find_last_not_of(TSL('\\'))}; last_non_escape != s.npos)
	{
		return ((std::size(s) - last_non_escape) % 2) == 0;
	}
	return (std::size(s) % 2) == 1;
}
#ifndef NDEBUG
static_assert(is_end_escaped(TSL(""sv)) == false);
static_assert(is_end_escaped(TSL("\\"sv)) == true);
static_assert(is_end_escaped(TSL("\\\\"sv)) == false);
static_assert(is_end_escaped(TSL("\\\\\\"sv)) == true);
static_assert(is_end_escaped(TSL("blah"sv)) == false);
static_assert(is_end_escaped(TSL("blah\\"sv)) == true);
static_assert(is_end_escaped(TSL("blah\\\\"sv)) == false);
static_assert(is_end_escaped(TSL("blah\\\\\\"sv)) == true);
static_assert(is_end_escaped(TSL("\\blah"sv)) == false);
static_assert(is_end_escaped(TSL("\\blah\\"sv)) == true);
static_assert(is_end_escaped(TSL("\\blah\\\\"sv)) == false);
static_assert(is_end_escaped(TSL("\\blah\\\\\\"sv)) == true);
#endif
struct LineAndEnding final
{
	lSDK::string_view_t line, ending;
	[[nodiscard]] friend constexpr auto operator<=>(LineAndEnding const&, LineAndEnding const&) noexcept = default;
};
[[nodiscard]] static constexpr auto read_line(lSDK::string_view_t const s, std::optional<lSDK::string_view_t> const newline, bool const honor_eol_escapes) noexcept
{
	LineAndEnding ret;
	std::size_t next{0};
	while(std::size(ret.line) < std::size(s))
	{
		if(newline)
		{
			ret.line = s.substr(0, s.find(*newline, next));
			if(std::size(ret.line) < std::size(s))
			{
				ret.ending = s.substr(std::size(ret.line), std::size(*newline));
			}
			next = std::size(ret.line) + std::size(*newline);
		}
		else
		{
			auto const rn{s.find_first_of(TSL("\r\n"sv), next)};
			ret.line = s.substr(0, rn);
			if(rn != s.npos)
			{
				ret.ending = s.substr(rn, 1);
				next = rn + 1;
				if(next < std::size(s))
				{
					//only skip one \r\n pair (at most two chars)
					if((s[rn] == TSL('\r') && s[next] == TSL('\n'))
					|| (s[rn] == TSL('\n') && s[next] == TSL('\r')))
					{
						ret.ending = s.substr(rn, 2);
						++next;
					}
				}
			}
			else
			{
				ret.ending = s.substr(std::size(s));
			}
		}
		if(!honor_eol_escapes)
		{
			break;
		}
		if(is_end_escaped(ret.line))
		{
			ret.ending.remove_prefix(std::size(ret.ending));
		}
		else
		{
			break;
		}
	}
	return ret;
}
#ifndef NDEBUG
static_assert(read_line(TSL("\\"sv), std::nullopt, true) == LineAndEnding{TSL("\\"sv), TSL(""sv)});
static_assert(read_line(TSL("\\\n"sv), std::nullopt, true) == LineAndEnding{TSL("\\\n"sv), TSL(""sv)});
static_assert(read_line(TSL("abc\ndef"sv), std::nullopt, true) == LineAndEnding{TSL("abc"sv), TSL("\n"sv)});
static_assert(read_line(TSL("abc\\\ndef\nghi"sv), std::nullopt, true) == LineAndEnding{TSL("abc\\\ndef"sv), TSL("\n"sv)});
static_assert(read_line(TSL("abc\\\r\ndef\r\nghi"sv), std::nullopt, true) == LineAndEnding{TSL("abc\\\r\ndef"sv), TSL("\r\n"sv)});
static_assert(read_line(TSL("abc\\\n\nghi"sv), std::nullopt, true) == LineAndEnding{TSL("abc\\\n"sv), TSL("\n"sv)});
static_assert(read_line(TSL("abc\\\r\n\r\nghi"sv), std::nullopt, true) == LineAndEnding{TSL("abc\\\r\n"sv), TSL("\r\n"sv)});
static_assert(read_line(TSL("abc\\\r\n\nghi"sv), std::nullopt, true) == LineAndEnding{TSL("abc\\\r\n"sv), TSL("\n"sv)});
static_assert(read_line(TSL("abc\\\r\n\rghi"sv), std::nullopt, true) == LineAndEnding{TSL("abc\\\r\n"sv), TSL("\r"sv)});
static_assert(read_line(TSL("abcdefghi"sv), TSL("def"sv), true) == LineAndEnding{TSL("abc"sv), TSL("def"sv)});
static_assert(read_line(TSL("abc\\defghi"sv), TSL("def"sv), true) == LineAndEnding{TSL("abc\\defghi"sv), TSL(""sv)});
static_assert(read_line(TSL("abc\\defdefghi"sv), TSL("def"sv), true) == LineAndEnding{TSL("abc\\def"sv), TSL("def"sv)});
static_assert(read_line(TSL("abc\\ddddddghi"sv), TSL("ddd"sv), true) == LineAndEnding{TSL("abc\\ddd"sv), TSL("ddd"sv)});
static_assert(read_line(TSL("abc\\dddddghi"sv), TSL("ddd"sv), true) == LineAndEnding{TSL("abc\\dddddghi"sv), TSL(""sv)});
#endif
[[nodiscard]] static constexpr auto is_comment_or_empty(lSDK::string_view_t const s) noexcept
{
	auto const nonwhitespace{s.find_first_not_of(TSL(" \t"sv))};
	if(nonwhitespace == s.npos
	|| s[nonwhitespace] == TSL(';')
	|| s[nonwhitespace] == TSL('\r')
	|| s[nonwhitespace] == TSL('\n'))
	{
		return true;
	}
	return false;
}
[[nodiscard]] static constexpr auto skip_line(lSDK::string_view_t const s, std::optional<lSDK::string_view_t> const newline) noexcept //doesn't honor escapes
{
	if(newline)
	{
		auto const nl{s.find(*newline)};
		if(nl != s.npos)
		{
			return s.substr(nl + std::size(*newline));
		}
	}
	else
	{
		auto const rn{s.find_first_of(TSL("\r\n"sv))};
		if(rn != s.npos)
		{
			if(rn + 1 < std::size(s))
			{
				//only skip one \r\n pair (at most two chars)
				if((s[rn] == TSL('\r') && s[rn + 1] == TSL('\n'))
				|| (s[rn] == TSL('\n') && s[rn + 1] == TSL('\r')))
				{
					return s.substr(rn + 2);
				}
			}
			return s.substr(rn + 1);
		}
	}
	return s.substr(std::size(s));
}
#ifndef NDEBUG
static_assert(skip_line(TSL("abc"sv), std::nullopt) == TSL(""sv));
static_assert(skip_line(TSL("abc\ndef"sv), std::nullopt) == TSL("def"sv));
static_assert(skip_line(TSL("abc\rdef"sv), std::nullopt) == TSL("def"sv));
static_assert(skip_line(TSL("abc\r\ndef"sv), std::nullopt) == TSL("def"sv));
static_assert(skip_line(TSL("abc\r\n\ndef"sv), std::nullopt) == TSL("\ndef"sv));
static_assert(skip_line(TSL("abcddddddghi"sv), TSL("ddd"sv)) == TSL("dddghi"sv));
static_assert(skip_line(TSL("abcdddddghi"sv), TSL("ddd"sv)) == TSL("ddghi"sv));
#endif
[[nodiscard]] static constexpr auto trim(lSDK::string_view_t s) noexcept
{
	{
		auto const p{s.find_first_not_of(TSL(" \t"sv))};
		if(p == s.npos)
		{
			return s.substr(std::size(s));
		}
		s.remove_prefix(p);
	}
	{
		auto const p{s.find_last_not_of(TSL(" \t"sv))};
		if(p != s.npos)
		{
			return s.substr(0, p + 1);
		}
	}
	return s;
}
#ifndef NDEBUG
static_assert(trim(TSL(" a "sv)) == TSL("a"sv));
static_assert(trim(TSL("  "sv)) == TSL(""sv));
#endif
[[nodiscard]] static auto unescape(lSDK::string_view_t s, std::optional<lSDK::string_view_t> const newline)
{
	lSDK::string_t result;
	result.reserve(std::size(s));
	while(!std::empty(s))
	{
		if(auto const p{s.find(TSL('\\'))}; p == s.npos)
		{
			result.append(s);
			break;
		}
		else if(p > 0)
		{
			result.append(s.substr(0, p));
			s.remove_prefix(p);
		}
		if(s.starts_with(TSL("\\n"sv)))
		{
			if(newline)
			{
				result.append(*newline);
			}
			else
			{
				result += TSL('\n');
			}
			s.remove_prefix(2);
		}
		else if(s.starts_with(TSL("\\t"sv)))
		{
			result += TSL('\t');
			s.remove_prefix(2);
		}
		else if(s.starts_with(TSL("\\_"sv)))
		{
			result += TSL(' ');
			s.remove_prefix(2);
		}
		else if(s.starts_with(TSL("\\q"sv)))
		{
			result += TSL('"');
			s.remove_prefix(2);
		}
		else
		{
			s.remove_prefix(1);
			if(!std::empty(s))
			{
				result += s.front();
				s.remove_prefix(1);
			}
		}
	}
	return result;
}
void Data::load(lSDK::string_view_t s)
{
	undos.clear();
	redos.clear();
	std::vector<lSDK::string_t> group_path;
	lSDK::string_t current_group_name;
	bool skip_this_group{false};
	while(!std::empty(s))
	{
		if(is_comment_or_empty(s))
		{
			s = skip_line(s, settings.newline);
			continue;
		}

		auto [line, ending]{read_line(s, settings.newline, false)};
		s.remove_prefix(std::size(line) + std::size(ending));
		std::size_t const depth{settings.subgroups*line.find_first_not_of('\t')}; //0 if subgroups are disabled, otherwise count of number of indents
		group_path.resize(depth + 1);
		line = trim(line);
		assert(!std::empty(line)); //should've been handled above

		if(line.front() == TSL('[')) //group
		{
			if(line.back() != TSL(']'))
			{
				//strange... garbage line?
				continue;
			}
			line.remove_prefix(1);
			line.remove_suffix(1);
			if(settings.esc_group)
			{
				group_path.back() = unescape(line, settings.newline);
			}
			else
			{
				group_path.back() = line;
			}
			current_group_name.resize(std::size(group_path.front()));
			current_group_name.replace(0, std::size(group_path.front()), group_path.front());
			for(std::size_t i{1}; i < std::size(group_path); ++i)
			{
				current_group_name += TSL('/');
				current_group_name += group_path.at(i);
			}
			switch(settings.repeat_mode_group)
			{
				case RepeatModeGroup::TakeFirst: skip_this_group = has_group(current_group_name); break;
				case RepeatModeGroup::TakeLast: skip_this_group = false; erase_group(current_group_name); break;
				case RepeatModeGroup::Rename:
				{
					skip_this_group = false;
					current_group_name.reserve(std::size(current_group_name) + 2);
					current_group_name += TSL('.');
					std::size_t n{1};
					std::size_t const original_len{std::size(current_group_name)};
					while(has_group((current_group_name += std::to_wstring(n))))
					{
						++n;
						current_group_name.resize(original_len);
					}
				} break;
				case RepeatModeGroup::GroupMerge: skip_this_group = false; break;
			}
			if(!skip_this_group && settings.empty_groups)
			{
				//create the group in advance if it doesn't already exist
				std::ignore = get_group_or_create(current_group_name);
			}
			continue;
		}
		if(skip_this_group)
		{
			continue;
		}

		//item
		std::size_t equals_pos{};
		while(true)
		{
			equals_pos = line.find(TSL('='), equals_pos);
			if(equals_pos == line.npos
			|| !is_end_escaped(line.substr(0, equals_pos)))
			{
				break;
			}
			else
			{
				++equals_pos;
			}
		}
		if(equals_pos == line.npos)
		{
			//strange... garbage line?
			continue;
		}
		auto const item_name{trim(line.substr(0, equals_pos))};
		auto const item_value{trim(line.substr(equals_pos + 1))};
		auto item_value_unescaped{settings.esc_value? unescape(item_value, settings.newline) : lSDK::string_t{item_value}}; //yes, this is done before checking if it is quoted... ask Jax why
		if(!settings.never_quote)
		{
			if(std::size(item_value_unescaped) >= 2
			&& item_value_unescaped.starts_with(TSL('"'))
			&& item_value_unescaped.  ends_with(TSL('"')))
			{
				item_value_unescaped.pop_back();
				std::ignore = item_value_unescaped.erase(0);
			}
		}
		auto item_name_unescaped{settings.esc_item? unescape(item_name, settings.newline) : lSDK::string_t{item_name}};
		if(has_item(current_group_name, item_name_unescaped))
		{
			switch(settings.repeat_mode_item)
			{
				case RepeatModeItem::TakeFirst: break;
				case RepeatModeItem::TakeLast: get_value_or_create(current_group_name, item_name_unescaped) = item_value_unescaped; break;
				case RepeatModeItem::Rename:
				{
					item_name_unescaped.reserve(std::size(item_name_unescaped) + 2);
					item_name_unescaped += TSL('.');
					std::size_t n{1};
					std::size_t const original_len{std::size(item_name_unescaped)};
					while(has_item(current_group_name, (item_name_unescaped += std::to_wstring(n))))
					{
						++n;
						item_name_unescaped.resize(original_len);
					}
					create_value(current_group_name, std::move(item_name_unescaped), std::move(item_value_unescaped));
				} break;
			}
		}
		else
		{
			create_value(current_group_name, std::move(item_name_unescaped), std::move(item_value_unescaped));
		}
	}
}
void Data::load(RunData const* const run_data, std::filesystem::path const& file_path)
{
	if(settings.encrypt_key)
	{
		if(std::ifstream ifs{file_path, std::ios::binary})
		if(ifs.seekg({}, std::ios::end))
		if(auto const end_pos{ifs.tellg()}; end_pos > 0)
		if(auto ini_data{std::string(static_cast<std::size_t>(end_pos), '\0')}
		;  ifs.seekg({}, std::ios::beg))
		if(ifs.read(std::data(ini_data), std::size(ini_data)))
		{
			ifs.close();
			toggle_encryption(ini_data, *settings.encrypt_key);
			auto const ini_unicode{lSDK::wide_from_narrow(ini_data)};
			ini_data.clear();
			return load(lSDK::string_view_t{ini_unicode});
		}
		else assert(false);
		else assert(false);
		else assert(false);
		else assert(false);
		else assert(false);
	}
	else if(auto const ini_unicode{load_text_file(run_data, file_path.wstring().c_str())})
	{
		return load(lSDK::string_view_t{ini_unicode.get()});
	}
	else assert(!exists(file_path));
}
static auto split(lSDK::string_view_t s, lSDK::string_view_t const by)
{
	std::vector<lSDK::string_view_t> ret;
	while(!std::empty(s))
	{
		auto const p{s.find(by)};
		ret.emplace_back(s.substr(0, p));
		if(p == s.npos)
		{
			break;
		}
		s.remove_prefix(p + std::size(by));
	}
	return ret;
}
static auto escape(lSDK::string_view_t s, std::optional<lSDK::string_view_t> newline, bool const spaces, bool const equals)
{
	if(!newline)
	{
		newline.emplace(TSL("\n"sv));
	}
	lSDK::string_t result;
	result.reserve(std::size(s));
	while(!std::empty(s))
	{
		if(newline)
		{
			if(s.front() == TSL('\\'))
			{
				result += TSL("\\\\"sv);
				s.remove_prefix(1);
			}
			else if(s.front() == TSL('"'))
			{
				result += TSL("\\q"sv);
				s.remove_prefix(1);
			}
			else if(s.front() == TSL('\t'))
			{
				result += TSL("\\t"sv);
				s.remove_prefix(1);
			}
			else if(s.starts_with(*newline))
			{
				result += TSL("\\n"sv);
				s.remove_prefix(std::size(*newline));
			}
			else if(spaces
			&& ((std::empty(result) && s.front() == TSL(' '))
			||  (std::size(s) == 1  && s.front() == TSL(' '))))
			{
				result += TSL("\\_"sv);
				s.remove_prefix(1);
			}
			else if(equals && s.front() == TSL('='))
			{
				result += TSL("\\="sv);
				s.remove_prefix(1);
			}
		}
	}
	return result;
}
lSDK::string_t Data::stringify() const
{
	return std::visit([this](auto const& ini)
	{
		lSDK::string_t result;
		for(auto const& [group_names, group] : ini)
		{
			if(!std::empty(result))
			{
				result += settings.newline.value_or(TSL("\r\n"s));
			}
			if constexpr(false) //TODO subgroups
			{
				auto const subgroups{split(group_names, TSL("/"sv))};
				std::size_t const depth{settings.subgroups*std::size(subgroups)};
				auto const indentation{lSDK::string_t(depth, TSL('\t'))};
				result += indentation;
			}
			auto const group_name{settings.save_repeated? lSDK::string_view_t(group_names).substr(0, group_names.rfind(TSL('.'))) : group_names}; //ask Jax why this is inverted
			result += TSL('[');
			if(settings.esc_group)
			{
				result += escape(group_names, settings.newline, false, false);
			}
			else
			{
				result += group_names;
			}
			result += TSL(']');
			result += settings.newline.value_or(TSL("\r\n"s));
			for(auto const& [item_name_, item_value] : group)
			{
				auto const item_name{settings.save_repeated? lSDK::string_view_t(item_name_).substr(0, item_name_.rfind(TSL('.'))) : item_name_}; //ask Jax why this is inverted
				if(settings.esc_item)
				{
					result += escape(item_name, settings.newline, true, true);
				}
				else
				{
					result += item_name;
				}
				result += TSL('=');
				if(settings.esc_value)
				{
					result += escape(item_value, settings.newline, settings.never_quote, false);
				}
				else
				{
					result += item_value;
				}
				result += settings.newline.value_or(TSL("\r\n"s));
			}
		}
		return result;
	}, ini);
}
void Data::save(RunData const* const run_data, std::filesystem::path const& file_path) const
{
	if(run_data->settings->auto_create_dirs)
	{
		std::error_code ec{};
		create_directories(file_path.parent_path(), ec);
		std::ignore = ec;
	}
	if(settings.encrypt_key)
	{
		auto ini_data{lSDK::narrow_from_wide(stringify())};
		toggle_encryption(ini_data, *settings.encrypt_key);
		std::ignore = std::ofstream(file_path, std::ios::binary).write(std::data(ini_data), std::size(ini_data));
	}
	else
	{
		std::ignore = save_text_file(run_data, file_path.wstring().c_str(), stringify().c_str());
	}
}

#ifdef FUSION_GET_RUNTIME_STRUCTURE_SIZE
auto FUSION_API GetRunObjectDataSize(RunHeader* const run_header, SerializedEditData const* const serialized_edit_data) noexcept
-> std::uint16_t
{
	return sizeof(RunData);
}
#endif

#ifdef FUSION_FINISH_RUNTIME_STRUCTURE_CONSTRUCTION
auto FUSION_API CreateRunObject(RunData* const run_data, SerializedEditData const* const serialized_edit_data, createObjectInfo* const create_object_info) noexcept
-> std::int16_t
{
	run_data->settings = nullptr;
	run_data->local_data = nullptr;
	run_data->data = nullptr;
	try
	{
		EdittimeSettings edittime_settings{serialized_edit_data->deserialize(run_data->rHo.hoAdRunHeader->rhApp->m_dwCodePage)};
		run_data->settings = new RuntimeSettingsLocal{edittime_settings, *(run_data->rHo.hoAdRunHeader)};
		RuntimeSettingsGlobal settings_global{edittime_settings, *(run_data->settings), *(run_data->rHo.hoAdRunHeader)};
		if(edittime_settings.globalObject)
		{
			run_data->local_data = nullptr;
			if(auto const it{Data::global.find(edittime_settings.globalKey)}; it != std::end(Data::global))
			{
				run_data->data = &(it->second);
				run_data->data->settings = std::move(settings_global);
				//yes, this means if multiple Ini++ in the same frame share the same global data slot, only one of their settings will stick... ask Jax why
			}
			else
			{
				run_data->data = &(Data::global.emplace(
					std::move(edittime_settings.globalKey),
					std::move(settings_global)
				).first->second);
			}
		}
		else
		{
			run_data->local_data = new Data{std::move(settings_global)};
			run_data->data = run_data->local_data;
		}
		if(!std::empty(edittime_settings.defaultText))
		{
			run_data->data->load(lSDK::string_view_t{edittime_settings.defaultText});
		}
		//yes, even though the property description says the default text is applied after, it's actually applied first... ask Jax why
		if(run_data->data->settings.ini_path)
		{
			run_data->data->load(run_data);
		}
		return FUSION_FINISH_RUNTIME_STRUCTURE_CONSTRUCTION_SUCCESS;
	}
	catch(...)
	{
		delete run_data->local_data;
		delete run_data->settings;
	}
	return FUSION_FINISH_RUNTIME_STRUCTURE_CONSTRUCTION_FAILURE;
}
#endif
#ifdef FUSION_BEGIN_RUNTIME_STRUCTURE_DESTRUCTION
auto FUSION_API DestroyRunObject(RunData* const run_data, std::int32_t const fast) noexcept
-> std::int16_t
{
	delete run_data->local_data;
	delete run_data->settings;
	return FUSION_BEGIN_RUNTIME_STRUCTURE_DESTRUCTION_SUCCESS;
}
#endif

#ifdef FUSION_ON_TICK
auto FUSION_API HandleRunObject(RunData* const run_data) noexcept
-> std::int16_t
{
	run_data->data->autosave(run_data);
	return FUSION_ON_TICK_DONT_DRAW;
}
#endif

#ifdef FUSION_ON_DRAW
auto FUSION_API DisplayRunObject(RunData* const run_data) noexcept
-> std::int16_t
{
	return FUSION_ON_DRAW_SUCCESS;
}
#endif

#ifdef FUSION_SERIALIZE_RUNTIME
auto FUSION_API SaveRunObject(RunData* const run_data, HANDLE const file_handle) noexcept
-> fusion::boolean
{
	//TODO
	return FUSION_SERIALIZE_RUNTIME_SUCCESS;
}
#endif
#ifdef FUSION_DESERIALIZE_RUNTIME
auto FUSION_API LoadRunObject(RunData* const run_data, HANDLE const file_handle) noexcept
-> fusion::boolean
{
	//TODO
	return FUSION_DESERIALIZE_RUNTIME_SUCCESS;
}
#endif

#ifdef FUSION_PAUSE_RUNTIME
auto FUSION_API PauseRunObject(RunData* const run_data) noexcept
-> std::int16_t
{
	return FUSION_PAUSE_RUNTIME_SUCCESS;
}
#endif
#ifdef FUSION_UNPAUSE_RUNTIME
auto FUSION_API ContinueRunObject(RunData* const run_data) noexcept
-> std::int16_t
{
	return FUSION_UNPAUSE_RUNTIME_SUCCESS;
}
#endif

Data::global_t Data::global;

#ifdef FUSION_START_APPLICATION
void FUSION_API StartApp(mv* const mV, CRunApp* const app) noexcept
{
}
#endif
#ifdef FUSION_END_APPLICATION
void FUSION_API EndApp(mv* const mV, CRunApp* const app) noexcept
{
	if(app->m_pParentApp == nullptr)
	{
		Data::global.clear();
	}
}
#endif
