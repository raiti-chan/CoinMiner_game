#include "big_integer.h"

#include <vector>
#include <iostream>
#include <exception>
#include <cctype>

namespace lib {

	big_integer::big_integer() noexcept {
		this->_values = new decimal_vector;

		//std::cout << "init : " << std::hex << this->_values << "\n";
	}

	big_integer::~big_integer() noexcept {
		if (this->_values != nullptr) {
			//std::cout << "delete : " << std::hex << this->_values << "\n";
			delete this->_values;
			this->_values = nullptr;
		}
	}

	big_integer::big_integer(const big_integer& value) noexcept {
		if (value._digit_count == 0) return;
		this->_values = new decimal_vector(*value._values);
		//std::cout << "init : " << std::hex << this->_values << "\n";

		this->_neg_sign = value._neg_sign;
		this->_digit_count = value._digit_count;
	}

	big_integer::big_integer(big_integer&& value) noexcept {
		this->_values = value._values;
		value._values = nullptr;

		this->_neg_sign = value._neg_sign;
		this->_digit_count = value._digit_count;
	}


	big_integer::big_integer(unsigned char value) noexcept : big_integer(static_cast<unsigned long long>(value)) {}
	big_integer::big_integer(char value) noexcept : big_integer(static_cast<long long>(value)) {}
	big_integer::big_integer(unsigned short value) noexcept : big_integer(static_cast<unsigned long long>(value)) {}
	big_integer::big_integer(short value) noexcept : big_integer(static_cast<long long>(value)) {}
	big_integer::big_integer(unsigned int value) noexcept : big_integer(static_cast<unsigned long long>(value)) {}
	big_integer::big_integer(int value) noexcept : big_integer(static_cast<long long>(value)) {}
	big_integer::big_integer(unsigned long value) noexcept : big_integer(static_cast<unsigned long long>(value)) {}
	big_integer::big_integer(long value) noexcept : big_integer(static_cast<long long>(value)) {}

	big_integer::big_integer(unsigned long long value) noexcept : big_integer() {
		if (value == 0) return;
		unsigned int digit = 0;
		do {
			if (digit % 2 == 0) {
				this->_values->push_back(double_value_uc{ static_cast<unsigned char>(value % 10) });
			} else {
				(*this->_values)[digit / 2].dv.r = static_cast<unsigned char>(value % 10);
			}

			value /= 10;
			digit++;
		} while (value);
		this->_digit_count = digit;
	}

	big_integer::big_integer(long long value) noexcept : big_integer() {
		if (value == 0) return;
		unsigned int digit = 0;
		if (value < 0) {
			this->_neg_sign = true;
			value = -value;
		}
		do {
			if (digit % 2 == 0) {
				this->_values->push_back(double_value_uc{ static_cast<unsigned char>(value % 10) });
			} else {
				(*this->_values)[digit / 2].dv.r = static_cast<unsigned char>(value % 10);
			}
			value /= 10;
			digit++;
		} while (value);
		this->_digit_count = digit;
	}


	big_integer& big_integer::operator = (const big_integer& value) noexcept {
		if (this->_values != nullptr) {
			//std::cout << "delete : " << std::hex << this->_values << "\n";
			delete this->_values;
		}
		this->_values = new decimal_vector(*value._values);

		this->_neg_sign = value._neg_sign;
		this->_digit_count = value._digit_count;
		return *this;
	}

	big_integer& big_integer::operator = (big_integer&& value) noexcept {
		if (this->_values != nullptr) {
			//std::cout << "delete : " << std::hex << this->_values << "\n";
			delete this->_values;
		}
		this->_values = value._values;
		value._values = nullptr;

		this->_neg_sign = value._neg_sign;
		this->_digit_count = value._digit_count;
		return *this;
	}


	big_integer big_integer::to_big_integer(const char* src) {
		int index = 0;
		big_integer bi;
		while (std::isspace(src[index]) || src[index] == '0') index++;
		if (src[index] == '-') {
			bi._neg_sign = true;
			index++;
		}
		int base_index = index;
		while (src[index] != '\0') {
			char v = src[index] - '0';
			if (v < 0 || v > 9) break;
			index++;
		}
		int digit_count = index - base_index;
		int mod = digit_count % 2;
		int vec_index = (digit_count - 1) / 2;
		bi._values->resize((digit_count + 1) / 2);
		bi._digit_count = digit_count;
		for (int i = 0; i < digit_count; i++, vec_index--) {
			double_value_uc v;
			v.value = 0;
			if (mod == 1) {
				v.dv.l = src[base_index + i] - '0';
				(*bi._values)[vec_index].value = v.value;
				mod = 0;
				continue;
			}
			v.dv.r = src[base_index + i] - '0';
			i++;
			v.dv.l = src[base_index + i] - '0';
			(*bi._values)[vec_index].value = v.value;
		}
		return bi;
	}

	big_integer big_integer::operator++(int) noexcept {
		big_integer r(*this);
		++(*this);
		return r;
	}

	big_integer big_integer::operator--(int) noexcept {
		big_integer r(*this);
		--(*this);
		return r;
	}

	big_integer& big_integer::operator++() noexcept {
		if (this->_digit_count == 0) {
			this->_digit_count = 1;
			this->_neg_sign = false;
			this->_values->resize(1);
			(*this->_values)[0].dv.l = 1;
			return *this;
		}
		if (!this->_neg_sign) {
			(*this->_values)[0].dv.l++;
			int i = 0;
			int digit = 1;
			while ((*this->_values)[i].dv.l > 9) {
				(*this->_values)[i].dv.l -= 10;
				(*this->_values)[i].dv.r++;
				digit++;
				if (this->_digit_count < digit) this->_digit_count++;
				if ((*this->_values)[i].dv.r > 9) {
					(*this->_values)[i].dv.r -= 10;

					if (this->_values->size() < i + 1 + 1) {
						this->_values->push_back(double_value_uc {1});
						this->_digit_count++;
						break;
					}
					i++;
					(*this->_values)[i].dv.l++;
					digit++;
					if (this->_digit_count < digit) this->_digit_count++;
				} else {
					break;
				}
			}
			/*if ((*this->_values)[0].dv.l > 9) {
				(*this->_values)[0].dv.l -= 10;
				(*this->_values)[0].dv.r++;
				if (this->_digit_count < 2) this->_digit_count++;
				int i = 1;
				while ((*this->_values)[i].dv.l > 9) {

				}

			}*/
		}
		return *this;
	}

	big_integer& big_integer::operator--() noexcept {
		if (this->_digit_count == 0) {
			this->_digit_count = 1;
			this->_neg_sign = true;
			this->_values->resize(1);
			(*this->_values)[0].dv.l = 1;
			return *this;
		}
		return *this;
	}

	big_integer big_integer::operator-() const noexcept {
		big_integer r(*this);
		r._neg_sign = true;
		return r;
	}

	big_integer big_integer::operator+() const noexcept {
		return big_integer(*this);
	}

	bool operator == (const big_integer& v1, const big_integer& v2) noexcept {
		if (v1._digit_count == 0 && v2._digit_count == 0) return true;
		if (v1._neg_sign != v2._neg_sign) return false;
		if (v1._digit_count != v2._digit_count) return false;
		int size_v1 = v1._values->size();
		int size_v2 = v2._values->size();
		int& max_count = size_v1 < size_v2 ? size_v1 : size_v2;
		for (int i = 0; i < max_count; i++) {
			if ((*v1._values)[i].value != (*v2._values)[i].value) return false;
		}
		return true;
	}

	bool operator < (const big_integer& v1, const big_integer& v2) noexcept {
		if (v1._neg_sign == true && v2._neg_sign == false) {
			return true;
		}
		if (v1._neg_sign == false && v2._neg_sign == true) {
			return false;
		}
		if (v1._neg_sign == false) {
			if (v1._digit_count < v2._digit_count) {
				return true;
			}
			if (v1._digit_count > v2._digit_count) {
				return false;
			}
			for (int i = v1._values->size() - 1; i >= 0; i--) {
				if ((*v1._values)[i].value < (*v2._values)[i].value) {
					return true;
				}
				if ((*v1._values)[i].value > (*v2._values)[i].value) {
					return false;
				}
			}
		} else {
			if (v1._digit_count > v2._digit_count) {
				return true;
			}
			if (v1._digit_count < v2._digit_count) {
				return false;
			}
			for (int i = v1._values->size() - 1; i >= 0; i--) {
				if ((*v1._values)[i].value > (*v2._values)[i].value) {
					return true;
				}
				if ((*v1._values)[i].value < (*v2._values)[i].value) {
					return false;
				}
			}
		}
		return false;
	}

	std::ostream& operator << (std::ostream& os, const big_integer& value) noexcept {
		if (value._digit_count == 0) {
			os << 0;
			return os;
		}
		if (value._neg_sign) os << '-';

		int index = (value._digit_count - 1) / 2;
		int mod = value._digit_count % 2;
		for (; index >= 0; index--) {
			if (mod == 1) {
				os << static_cast<int>((*value._values)[index].dv.l);
				mod = 0;
			} else {
				os << static_cast<int>((*value._values)[index].dv.r)
					<< static_cast<int>((*value._values)[index].dv.l);
			}
		}
		return os;
	}
}