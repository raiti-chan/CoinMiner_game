#pragma once

namespace std {
	
	template <class _Elem> struct char_traits;
	template <class _Elem, class _Traits> class basic_ostream;
	using ostream = std::basic_ostream<char, std::char_traits<char>>;


	template < class T > class allocator;
	template <class _Ty, class _Alloc> class vector;

}

namespace std_lib {
	template <class _Ty, class _Alloc = std::allocator<_Ty>>
	using vector = std::vector<_Ty, _Alloc>;

	using ostream = std::ostream;
}