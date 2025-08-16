#pragma once
#include <ostream>
#include <glm/glm.hpp>

template<glm::length_t L, typename T, glm::qualifier Q>
inline std::ostream& operator<<(std::ostream& os, const glm::vec<L, T, Q>& v){
	os << "(";
	for (glm::length_t i = 0; i < L; i++){
		os << v[i];
		if(i + 1 < L)
			os << ", ";
	}
	os << ")";
	return os;
}


