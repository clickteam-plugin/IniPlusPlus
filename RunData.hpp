#pragma once

#include "Settings.hpp"

#include "FusionAPI.hpp"
#include "lSDK/UnicodeUtilities.hpp"

#include <cassert>
#include <concepts>
#include <deque>
#include <filesystem>
#include <map>
#include <memory>
#include <set>
#include <type_traits>
#include <tuple>
#include <variant>
#include <vector>

/**
Basic manual implementation in lieu of a proper existing library. Feature incomplete, lightly tested.
*/
template<typename Key, typename T>
struct MapWithOrder final
{
	using map_t = std::map<Key, T, std::less<>>;
	using order_t = std::vector<typename map_t::iterator>;
private:
	map_t map;
	order_t order;
public:
	using key_type = typename map_t::key_type;
	using mapped_type = typename map_t::mapped_type;
	using value_type = typename map_t::value_type;
	using size_type = typename map_t::size_type;
	using difference_type = typename map_t::difference_type;
	using key_compare = typename map_t::key_compare;
	using allocator_type = typename map_t::allocator_type;
	using reference = typename map_t::reference;
	using const_reference = typename map_t::const_reference;
	using pointer = typename map_t::pointer;
	using const_pointer = typename map_t::const_pointer;
	template<bool Const>
	struct iterator_template
	{
		using order_it_t = std::conditional_t<Const, typename order_t::const_iterator, typename order_t::iterator>;
		using map_it_t = std::conditional_t<Const, typename map_t::const_iterator, typename map_t::iterator>;
		using difference_type = typename order_it_t::difference_type;
		using value_type = typename map_it_t::value_type;
		using pointer = typename map_it_t::pointer;
		using reference = typename map_it_t::reference;
		using iterator_category = typename order_it_t::iterator_category;

		order_it_t it;

		constexpr iterator_template &operator++(   ) noexcept(noexcept(++it)) { ++it; return *this; }
		constexpr iterator_template &operator--(   ) noexcept(noexcept(--it)) { --it; return *this; }
		constexpr iterator_template  operator++(int) noexcept(noexcept(++it)) { iterator_template t = *this; ++it; return t; }
		constexpr iterator_template  operator--(int) noexcept(noexcept(--it)) { iterator_template t = *this; --it; return t; }
		constexpr iterator_template &operator+=(difference_type n) noexcept(noexcept(it += n)) { it += n; return *this; }
		constexpr iterator_template &operator-=(difference_type n) noexcept(noexcept(it -= n)) { it -= n; return *this; }
		constexpr iterator_template operator+(difference_type n) noexcept(noexcept(it + n)) { return {it + n}; }
		constexpr iterator_template operator-(difference_type n) noexcept(noexcept(it - n)) { return {it - n}; }
		constexpr bool operator==(iterator_template const &b) const noexcept(noexcept(it == b.it)) { return it == b.it; }
		constexpr bool operator!=(iterator_template const &b) const noexcept(noexcept(it != b.it)) { return it != b.it; }
		constexpr bool operator< (iterator_template const &b) const noexcept(noexcept(it <  b.it)) { return it <  b.it; }
		constexpr bool operator> (iterator_template const &b) const noexcept(noexcept(it >  b.it)) { return it >  b.it; }
		constexpr bool operator<=(iterator_template const &b) const noexcept(noexcept(it <= b.it)) { return it <= b.it; }
		constexpr bool operator>=(iterator_template const &b) const noexcept(noexcept(it >= b.it)) { return it >= b.it; }
		constexpr value_type &operator*() const noexcept(noexcept(**it)) { return **it; }
		constexpr value_type &operator[](difference_type n) const noexcept(noexcept(*(it[n]))) { return *(it[n]); }
		constexpr map_it_t operator->() const noexcept(noexcept(*it)) { return *it; }
	};
	using iterator = iterator_template<false>;
	using const_iterator = iterator_template<true>;
	using node_type = typename map_t::node_type;
	using insert_return_type = typename map_t::insert_return_type;
	using value_compare = typename map_t::value_compare;

	MapWithOrder() = default;
	MapWithOrder(MapWithOrder const &from)
	{
		for(auto &p : from.map)
		{
			insert(p);
		}
	}
	MapWithOrder(MapWithOrder &&) noexcept = default;
	MapWithOrder &operator=(MapWithOrder const &from)
	{
		clear();
		for(auto &p : from.map)
		{
			insert(p);
		}
		return *this;
	}
	MapWithOrder &operator=(MapWithOrder &&) noexcept = default;
	~MapWithOrder() noexcept = default;

	mapped_type       &at(key_type const &k)       { return map.at(k); }
	mapped_type const &at(key_type const &k) const { return map.at(k); }

	mapped_type &operator[](key_type const &k) = delete;
	mapped_type &operator[](key_type &&k) = delete;

	iterator begin() noexcept { return iterator{order.begin()}; }
	const_iterator begin() const noexcept { return const_iterator{order.begin()}; }
	const_iterator cbegin() const noexcept { return const_iterator{order.cbegin()}; }
	iterator end() noexcept { return iterator{order.end()}; }
	const_iterator end() const noexcept { return const_iterator{order.end()}; }
	const_iterator cend() const noexcept { return const_iterator{order.cend()}; }

	[[nodiscard]] bool empty() const noexcept { return map.empty(); }
	size_type size() const noexcept { return map.size(); }
	size_type max_size() const noexcept { return map.max_size(); }

	void clear() noexcept
	{
		order.clear();
		map.clear();
	}

private:
	std::pair<iterator, bool> handle_insert_result(std::pair<typename map_t::iterator, bool> const &r)
	{
		if(r.second)
		{
			order.emplace_back(r.first);
			return {iterator{order.begin()+(order.size()-1)}, true};
		}
		auto it = std::find(order.begin(), order.end(), r.first);
		assert(it != order.end());
		return {iterator{std::move(it)}, false};
	}
public:
	template<typename P>
	std::pair<iterator, bool> insert(P &&v)
	{
		return handle_insert_result(map.insert(std::forward<P>(v)));
	}
	template<typename M>
	std::pair<iterator, bool> insert_or_assign(key_type const &k, M &&v)
	{
		return handle_insert_result(map.insert_or_assign(k, std::forward<M>(v)));
	}
	template<typename M>
	std::pair<iterator, bool> insert_or_assign(key_type &&k, M &&v)
	{
		return handle_insert_result(map.insert_or_assign(std::move(k), std::forward<M>(v)));
	}
	template<typename... Args>
	std::pair<iterator, bool> emplace(Args &&... args)
	{
		return handle_insert_result(map.emplace(std::forward<Args>(args)...));
	}
	template<typename... Args>
	std::pair<iterator, bool> try_emplace(key_type const &k, Args &&... args)
	{
		return handle_insert_result(map.try_emplace(k, std::forward<Args>(args)...));
	}
	template<typename... Args>
	std::pair<iterator, bool> try_emplace(key_type &&k, Args &&... args)
	{
		return handle_insert_result(map.try_emplace(std::move(k), std::forward<Args>(args)...));
	}

	iterator erase(const_iterator it)
	{
		map.erase(*it.it);
		return {order.erase(it.it)};
	}
	iterator erase(iterator it)
	{
		map.erase(*it.it);
		return {order.erase(it.it)};
	}
	size_type erase(key_type const &k)
	{
		auto it = map.find(k);
		if(it == map.end())
		{
			return 0;
		}
		auto ot = std::find(order.begin(), order.end(), it);
		assert(ot != order.end());
		order.erase(ot);
		map.erase(it);
		return 1;
	}

	template<typename K>
	size_type count(K const &k) const
	{
		return map.count(k);
	}

	template<typename K>
	iterator find(K const &k)
	{
		auto it = map.find(k);
		if(it == map.end())
		{
			return {order.end()};
		}
		auto ot = std::find(order.begin(), order.end(), it);
		assert(ot != order.end());
		return {ot};
	}
	template<typename K>
	const_iterator find(K const &k) const
	{
		auto it = map.find(k);
		if(it == map.end())
		{
			return {order.end()};
		}
		auto ot = std::find(order.begin(), order.end(), it);
		assert(ot != order.end());
		return {ot};
	}
};

struct free_via_mv final
{
	mv* mV;
	void operator()(void* const p) const noexcept
	{
		mvFree(mV, p);
	}
};

struct Doer
{
	virtual ~Doer() noexcept = default;

	virtual void undo() const = 0;
	virtual void redo() const = 0;
};
using Value_t = lSDK::string_t;
using ItemName_t = lSDK::string_t;
using GroupName_t = lSDK::string_t;
using OrderedGroup_t = MapWithOrder<ItemName_t, Value_t>;
using OrderedIni_t = MapWithOrder<GroupName_t, OrderedGroup_t>;
using UnorderedGroup_t = std::map<ItemName_t, Value_t, std::less<>>;
using UnorderedIni_t = std::map<GroupName_t, UnorderedGroup_t, std::less<>>;
using GroupPtr_t = std::variant<OrderedGroup_t*, UnorderedGroup_t*>; //never null, but variants can't hold references
using ConstGroupPtr_t = std::variant<OrderedGroup_t const*, UnorderedGroup_t const*>; //can be null
using Ini_t = std::variant<OrderedIni_t, UnorderedIni_t>;
using SearchResults_t = std::set<std::tuple<GroupName_t, ItemName_t, Value_t>>;
struct Data final
{
	Ini_t ini;
	SearchResults_t results;
	std::deque<std::shared_ptr<Doer>> undos, redos;
	bool pending_autosave{false};
	RuntimeSettingsGlobal settings;

	Data(RuntimeSettingsGlobal settings_) noexcept
	: settings{std::move(settings_)}
	{
	}

	[[nodiscard]] GroupPtr_t get_group_or_create(lSDK::string_view_t const group)
	{
		return std::visit([group]<typename IniT>(IniT& ini)
		-> GroupPtr_t
		{
			if(auto const it{ini.find(group)}; it != std::end(ini))
			{
				return GroupPtr_t{std::in_place_type<typename IniT::mapped_type*>, &it->second};
			}
			return GroupPtr_t{std::in_place_type<typename IniT::mapped_type*>, &ini.emplace(GroupName_t{group}, typename IniT::mapped_type{}).first->second};
		}, ini);
	}
	[[nodiscard]] ConstGroupPtr_t get_group_if_exists(lSDK::string_view_t const group) const noexcept
	{
		return std::visit([group]<typename IniT>(IniT const& ini)
		-> ConstGroupPtr_t
		{
			if(auto const it{ini.find(group)}; it != std::cend(ini))
			{
				return ConstGroupPtr_t{std::in_place_type<typename IniT::mapped_type const*>, &it->second};
			}
			return ConstGroupPtr_t{std::in_place_type<typename IniT::mapped_type const*>, nullptr};
		}, ini);
	}
	void erase_group(lSDK::string_view_t const group)
	{
		std::visit([group](auto& ini)
		{
			if(auto const it{ini.find(group)}; it != std::end(ini))
			{
				ini.erase(it);
			}
		}, ini);
	}
	[[nodiscard]] Value_t& get_value_or_create(lSDK::string_view_t const group, lSDK::string_view_t const item)
	{
		return std::visit([item](auto* g)
		-> Value_t&
		{
			if(auto const it{g->find(item)}; it != std::end(*g))
			{
				return it->second;
			}
			return g->emplace(ItemName_t{item}, Value_t{}).first->second;
		}, get_group_or_create(group));
	}
	[[nodiscard]] Value_t const* get_value_if_exists(lSDK::string_view_t const group, lSDK::string_view_t const item) const noexcept
	{
		return std::visit([item](auto const* g)
		-> Value_t const*
		{
			if(g != nullptr)
			{
				if(auto const it{g->find(item)}; it != std::end(*g))
				{
					return &(it->second);
				}
			}
			return nullptr;
		}, get_group_if_exists(group));
	}
	[[nodiscard]] Value_t const& get_value_or(lSDK::string_view_t const group, lSDK::string_view_t const item, Value_t const& fallback_value) const noexcept
	{
		if(auto const v{get_value_if_exists(group, item)})
		{
			return *v;
		}
		return fallback_value;
	}
	void create_value(lSDK::string_view_t const group, lSDK::string_t item, Value_t value)
	{
		std::visit([item = std::move(item), value = std::move(value)](auto* g) mutable
		{
			std::ignore = g->emplace(std::move(item), std::move(value));
		}, get_group_or_create(group));
	}
	[[nodiscard]] bool has_group(lSDK::string_view_t const group) const noexcept
	{
		return std::visit([group](auto const& ini) noexcept -> bool { return ini.find(group) != std::end(ini); }, ini);
	}
	[[nodiscard]] bool has_item(lSDK::string_view_t const group, lSDK::string_view_t const item) const noexcept
	{
		return std::visit([group, item](auto const& ini) noexcept
		-> bool
		{
			if(auto const it{ini.find(group)}; it != std::end(ini))
			{
				return it->second.find(item) != std::end(it->second);
			}
			return false;
		}, ini);
	}

	static void toggle_encryption(std::string&, std::string_view) noexcept;

	void load(lSDK::string_view_t);
	void load(RunData const*, std::filesystem::path const&);
	void load(RunData const* const run_data)
	{
		if(settings.ini_path)
		{
			settings.ini_path->make_preferred();
			return load(run_data, *settings.ini_path);
		}
	}
	lSDK::string_t stringify() const;
	void save(RunData const*, std::filesystem::path const&) const;
	void save(RunData const* const run_data) const
	{
		if(settings.ini_path && !settings.read_only)
		{
			return save(run_data, *settings.ini_path);
		}
	}
	void autosave(RunData const* const run_data)
	{
		if(settings.auto_save && pending_autosave)
		{
			pending_autosave = false;
			return save(run_data);
		}
	}
	void close(RunData const* const run_data)
	{
		redos.clear();
		undos.clear();
		autosave(run_data);
		settings.ini_path.reset();
		std::visit([](auto& ini) noexcept { ini.clear(); }, ini);
		settings.read_only = false;
		pending_autosave = false;
	}

	template<std::derived_from<Doer> D, typename... Args>
	void do_doer(Args&&... args)
	{
		redos.clear();
		undos.emplace_back(std::shared_ptr<Doer>(new D{std::forward<Args>(args)...}));
		undos.back()->redo();
		pending_autosave = true;
		while(std::size(undos) > settings.undos)
		{
			undos.pop_front();
		}
	}
	void undo()
	{
		if(std::empty(undos))
		{
			return;
		}
		redos.emplace_back(std::move(undos.back()));
		undos.pop_back();
		redos.back()->undo();
		pending_autosave = true;
		while(std::size(redos) > settings.redos)
		{
			redos.pop_front();
		}
	}
	void redo()
	{
		if(std::empty(redos))
		{
			return;
		}
		undos.emplace_back(std::move(redos.back()));
		redos.pop_back();
		undos.back()->redo();
		pending_autosave = true;
		while(std::size(undos) > settings.undos)
		{
			undos.pop_front();
		}
	}

	using global_t = std::map<lSDK::string_t, Data, std::less<>>;
	static global_t global;
};

struct RunData final
{
	headerObject rHo;
	rCom rc;
	rMvt rm;
	rVal rv;

	RuntimeSettingsLocal* settings; //owning
	Data* local_data; //owning
	Data* data; //non-owning

	RunData() = delete;
	RunData(RunData const &) = delete;
	RunData(RunData &&) = delete;
	RunData &operator=(RunData &&) = delete;
	RunData &operator=(RunData const &) = delete;
	~RunData() = delete;

	#ifdef FUSION_ACTIONS
	static std::vector<fusion::action_func_pointer> const ACTIONS;
	#endif
	#ifdef FUSION_CONDITIONS
	static std::vector<fusion::condition_func_pointer> const CONDITIONS;
	#endif
	#ifdef FUSION_EXPRESSIONS
	static std::vector<fusion::expression_func_pointer> const EXPRESSIONS;
	#endif
};
static_assert(fusion::is_valid_rundata_structure<FUSION_OEFLAGS>());

#ifndef NDEBUG
#define NOT_YET_IMPLEMENTED { static bool shown{false}; if(!shown){ assert(!__func__); shown = true; } }
#elif !defined(FUSION_RUNTIME_ONLY)
#define NOT_YET_IMPLEMENTED { static bool shown{false}; if(!shown){ std::ignore = ::MessageBoxA({}, __func__, "Ini++ Unicode: unimplemented", MB_ICONWARNING|MB_OK); shown = true; } }
#else
#define NOT_YET_IMPLEMENTED
#endif
