#ifndef TYPES_H
#define TYPES_H

#ifdef NTLLIB
#include <NTL/ZZX.h>
#endif

#ifdef PARI
#include <pari/pari.h>
#endif

static const bool N_LESS_64		= true;

typedef unsigned long set_t;	// This represents a set. Only supports n < 64 vertices
static const unsigned long EMPTY_SET	= 0ul;

typedef unsigned long u_int_t;
static const u_int_t U_ONE		= 1ul;
static const u_int_t U_ZERO		= 0ul;

typedef signed long sign_t;
static const sign_t S_ONE		= 1l;
static const sign_t S_N_ONE		= -1l;

static std::string ZERO_STR("0");

#ifdef NTLLIB
typedef void* sp;
typedef NTL::ZZX rval_t;
#endif
#ifdef PARI
typedef GEN rval_t;
typedef pari_sp sp;
#endif

template <class T>
class PointerList {
private:
	T* list;
public:
	PointerList(const int s) : size(s) {
		list = new T[s];
	}
	~PointerList() {
		delete[] list;
	}
	const u_int_t size;
	T& operator[](const int i) { return list[i]; }
};

typedef PointerList<set_t> set_list_t;
typedef PointerList<rval_t> rval_list_t;

#endif
