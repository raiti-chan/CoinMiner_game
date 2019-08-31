#pragma once

#include "std_light_header.h"


namespace lib {
	
	using double_value = struct _double_value {
		unsigned char l : 4;
		unsigned char r : 4;
	};

	using double_value_uc = union _double_value_uc {
		unsigned char value = 0;
		double_value dv;
	};

	class big_integer {

		using decimal_vector = std_lib::vector<double_value_uc>;

		bool _neg_sign = false;
		unsigned int _digit_count = 0;
		decimal_vector* _values = nullptr;

		big_integer() noexcept;

		void _inc_or_dec(bool is_inc);

	public:

		~big_integer() noexcept;

		big_integer(const big_integer& value) noexcept;
		big_integer(big_integer&& value) noexcept;

		big_integer(unsigned char value) noexcept;
		big_integer(char value) noexcept;
		big_integer(unsigned short value) noexcept;
		big_integer(short value) noexcept;
		big_integer(unsigned int value) noexcept;
		big_integer(int value) noexcept;
		big_integer(unsigned long value) noexcept;
		big_integer(long value) noexcept;
		big_integer(unsigned long long value) noexcept;
		big_integer(long long value) noexcept;

		big_integer& operator =(const big_integer& value) noexcept;
		big_integer& operator =(big_integer&& value) noexcept;

		big_integer operator ++(int) noexcept;
		big_integer operator --(int) noexcept;

		big_integer& operator ++() noexcept;
		big_integer& operator --() noexcept;

		big_integer operator -() const noexcept;
		big_integer operator +() const noexcept;

		static big_integer to_big_integer(const char* str);
		
		static unsigned int to_value_index(const unsigned int index) { return index / 2; }
		static unsigned int to_value_index(const unsigned int index, unsigned char& mod) {
			mod = index & 0x1;
			return index / 2;
		}

		friend bool operator <(const big_integer& v1, const big_integer& v2) noexcept;
		friend inline bool operator >(const big_integer& v1, const big_integer& v2) noexcept { return v2 < v1; }
		friend inline bool operator <=(const big_integer& v1, const big_integer& v2) noexcept { return !(v1 > v2); }
		friend inline bool operator >=(const big_integer& v1, const big_integer& v2) noexcept { return !(v1 < v2); }

		friend bool operator ==(const big_integer& v1, const big_integer& v2) noexcept;
		friend inline bool operator !=(const big_integer& v1, const big_integer& v2) noexcept { return !(v1 == v2); }

		friend std::ostream& operator <<(std::ostream& os, const big_integer& value) noexcept;

	};

	namespace literal {
		namespace meta_function {
			template<char...V>
			constexpr bool _point_check(int i) {
				using c_str = char[];
				constexpr c_str str{ V..., '\0' };
				return str[i] == '\0' ? true : str[i] == '.' ? false : _point_check<V...>(i + 1);
			}
		}

		template<char...V>
		lib::big_integer operator "" _bi() {
			static_assert(meta_function::_point_check<V...>(0), "big_integer literal error.\n");
			using c_str = char[];
			constexpr c_str str{ V..., '\0' };
			return lib::big_integer::to_big_integer(str);
		}
	}

}