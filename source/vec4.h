#ifndef __VEC4_H__
#define __VEC4_H__

#include <cstdio>

#include <emmintrin.h>

#include "swizzle4.h"

class vec4
{
	private:
			// Most compilers don't use pshufd (SSE2) when _mm_shuffle(x, x, mask) is used
			// This macro saves 2-3 movaps instructions when shuffling
			// This has to be a macro since mask HAS to be an immidiate value
		#define _mm_shufd(xmm, mask) _mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(xmm), mask))

		typedef _swizzle4_maker<float, vec4> _swzl;

		// ----------------------------------------------------------------- //

	public:
			// Empty constructor
		inline vec4() {
			m = _mm_setzero_ps();
		}

			// Fill constructor
		explicit inline vec4(float f) {
			m = _mm_set1_ps(f);
		}

			// 4 var init constructor
		inline vec4(float _x, float _y, float _z, float _w) {
			m = _mm_setr_ps(_x, _y, _z, _w);
		}

			// Float array constructor
		inline vec4(const float* fv) {
			m = _mm_loadu_ps(fv);
		}

			// Copy constructor
		inline vec4(const vec4 &v) {
			m = v.m;
		}

			// SSE compatible constructor
		inline vec4(const __m128 &_m) {
			m = _m;
		}

		// ----------------------------------------------------------------- //

			// Read-write swizzle
		template<unsigned mask>
		inline _swzl::rw<mask> shuffle4_rw() {
			return _swzl::rw<mask>(*this);
		}

			// Read-write swizzle, const, actually read only
		template<unsigned mask>
		inline _swzl::ro<mask> shuffle4_rw() const {
			return _swzl::ro<mask>(*this);
		}

			// Read-only swizzle
		template<unsigned mask>
		inline _swzl::ro<mask> shuffle4_ro() const {
			return _swzl::ro<mask>(*this);
		}

		// ----------------------------------------------------------------- //

			// Write direct access operator
		inline float& operator[](int index) {
			return ((float*)this)[index];
		}

			// Read direct access operator
		inline const float& operator[](int index) const {
			return ((const float*)this)[index];
		}

			// Cast operator
		inline operator float* () {
			return (float*)this;
		}

			// Const cast operator
		inline operator const float* () const {
			return (const float*)this;
		}

		// ----------------------------------------------------------------- //

		friend inline vec4& operator += (vec4 &v, float f) {
			v.m = _mm_add_ps(v.m, _mm_set1_ps(f));
			return v;
		}

		friend inline vec4& operator += (vec4 &v0, const vec4 &v1) {
			v0.m = _mm_add_ps(v0.m, v1.m);
			return v0;
		}

		friend inline vec4& operator -= (vec4 &v, float f) {
			v.m = _mm_sub_ps(v.m, _mm_set1_ps(f));
			return v;
		}

		friend inline vec4& operator -= (vec4 &v0, const vec4 &v1) {
			v0.m = _mm_sub_ps(v0.m, v1.m);
			return v0;
		}

		friend inline vec4& operator *= (vec4 &v, float f) {	
			v.m = _mm_mul_ps(v.m, _mm_set1_ps(f));
			return v;
		}

		friend inline vec4& operator *= (vec4 &v0, const vec4 &v1) {
			v0.m = _mm_mul_ps(v0.m, v1.m);
			return v0;
		}

		friend inline vec4& operator /= (vec4 &v, float f) {
			v.m = _mm_div_ps(v.m, _mm_set1_ps(f));
			return v;
		}

		friend inline vec4& operator /= (vec4 &v0, const vec4 &v1) {
			v0.m = _mm_div_ps(v0.m, v1.m);
			return v0;
		}

		// ----------------------------------------------------------------- //

		friend inline const vec4 operator + (float f, const vec4 &v) {
			return _mm_add_ps(_mm_set1_ps(f), v.m);
		}

		friend inline const vec4 operator + (const vec4 &v, float f) {
			return _mm_add_ps(v.m, _mm_set1_ps(f));
		}

		friend inline const vec4 operator + (const vec4 &v0, const vec4 &v1) {
			return _mm_add_ps(v0.m, v1.m);
		}

		friend inline const vec4 operator - (const vec4 &v) {
			return _mm_xor_ps(v.m, _mm_set1_ps(-0.f));
		}

		friend inline const vec4 operator - (float f, const vec4 &v) {
			return _mm_sub_ps( _mm_set1_ps(f), v.m);
		}

		friend inline const vec4 operator - (const vec4 &v, float f) {
			return _mm_sub_ps(v.m, _mm_set1_ps(f));
		}

		friend inline const vec4 operator - (const vec4 &v0, const vec4 &v1) {
			return _mm_sub_ps(v0.m, v1.m);
		}

		friend inline const vec4 operator * (float f, const vec4 &v) {
			return _mm_mul_ps(_mm_set1_ps(f), v.m);
		}

		friend inline const vec4 operator * (const vec4 &v, float f) {
			return _mm_mul_ps(v.m, _mm_set1_ps(f));
		}

		friend inline const vec4 operator * (const vec4 &v0, const vec4 &v1) {
			return _mm_mul_ps(v0.m, v1.m);
		}

		friend inline const vec4 operator / (float f, const vec4 &v) {
			return _mm_div_ps(_mm_set1_ps(f), v.m);
		}

		friend inline const vec4 operator / (const vec4 &v, float f) {
			return _mm_div_ps(v.m, _mm_set1_ps(f));
		}

		friend inline const vec4 operator / (const vec4 &v0, const vec4 &v1) {
			return _mm_div_ps(v0.m, v1.m);
		}

		// ----------------------------------------------------------------- //

		friend inline const vec4 sqrt(const vec4 &v) {
			return _mm_sqrt_ps(v.m);
		}

		friend inline const vec4 inversesqrt(const vec4 &v) {
			return _mm_div_ps(_mm_set1_ps(1.0f), _mm_sqrt_ps(v.m));
		}

		// ----------------------------------------------------------------- //

		friend inline const vec4 abs(const vec4 &v) {
			return _mm_andnot_ps(_mm_set1_ps(-0.f), v.m);
		}

		friend inline const vec4 ceil(const vec4 &v) {
			return _mm_cvtepi32_ps(_mm_cvtps_epi32(
								   _mm_add_ps(v.m, _mm_set1_ps(0.5f))));
		}

		friend inline const vec4 clamp(const vec4 &v0, float f1, float f2) {
			return _mm_max_ps(_mm_set1_ps(f1),
							  _mm_min_ps(_mm_set1_ps(f2), v0.m));
		}

		friend inline const vec4 clamp(const vec4 &v0,
									   const vec4 &v1, const vec4 &v2) {
			return _mm_max_ps(v1.m, _mm_min_ps(v2.m, v0.m));
		}

		friend inline const vec4 floor(const vec4 &v) {
			return _mm_cvtepi32_ps(_mm_srai_epi32(_mm_cvtps_epi32(_mm_sub_ps(
								   _mm_add_ps(v.m, v.m), _mm_set1_ps(0.5f))), 1));
		}

		friend inline const vec4 fract(const vec4 &v) {
			return _mm_sub_ps(v.m, _mm_cvtepi32_ps(_mm_srai_epi32(_mm_cvtps_epi32(
								   _mm_sub_ps(_mm_add_ps(v.m, v.m),
											  _mm_set1_ps(0.5f))), 1)));
		}

		friend inline const vec4 max(const vec4 &v, float f) {
			return _mm_max_ps(v.m, _mm_set1_ps(f));
		}

		friend inline const vec4 max(const vec4 &v0, const vec4 &v1) {
			return _mm_max_ps(v0.m, v1.m);
		}

		friend inline const vec4 min(const vec4 &v, float f) {
			return _mm_min_ps(v.m, _mm_set1_ps(f));
		}

		friend inline const vec4 min(const vec4 &v0, const vec4 &v1) {
			return _mm_min_ps(v0.m, v1.m);
		}

		friend inline const vec4 mix(const vec4 &v0, const vec4 &v1,
									 float f) {
			__m128 ff = _mm_set1_ps(f);
			return _mm_add_ps(_mm_mul_ps(v0.m, _mm_sub_ps(_mm_set1_ps(1.f), ff)),
							  _mm_mul_ps(v1.m, ff));
		}

		friend inline const vec4 mix(const vec4 &v0, const vec4 &v1,
									 const vec4 &v2) {
			return _mm_add_ps(_mm_mul_ps(v0.m, _mm_sub_ps(_mm_set1_ps(1.f), v1.m)),
							  _mm_mul_ps(v1.m, v2.m));
		}

		friend inline const vec4 mod(const vec4 &v, float f) {
			__m128 ff = _mm_set1_ps(f);
			__m128 d = _mm_div_ps(v.m, ff);
			return _mm_sub_ps(v.m, _mm_mul_ps(ff, _mm_cvtepi32_ps(_mm_srai_epi32(
								   _mm_cvtps_epi32(_mm_sub_ps(_mm_add_ps(d, d),
												   _mm_set1_ps(0.5f))), 1))));
		}

		friend inline const vec4 mod(const vec4 &v0, const vec4 &v1) {
			__m128 d = _mm_div_ps(v0.m, v1.m);
			return _mm_sub_ps(v0.m, _mm_mul_ps(v1.m, _mm_cvtepi32_ps(_mm_srai_epi32(
									_mm_cvtps_epi32(_mm_sub_ps(_mm_add_ps(d, d),
													_mm_set1_ps(0.5f))), 1))));
		}

		friend inline const vec4 modf(const vec4 &v0, vec4 &v1) {
			v1.m = _mm_or_ps(_mm_cvtepi32_ps(_mm_cvttps_epi32(v0.m)),
							 _mm_and_ps(_mm_set1_ps(-0.f), v0.m));
			return _mm_sub_ps(v0.m, v1.m);
		}

		friend inline const vec4 round(const vec4 &v) {
			return _mm_cvtepi32_ps(_mm_cvtps_epi32(v.m));
		}

		friend inline const vec4 roundEven(const vec4 &v) {
			__m128 f = _mm_cvtepi32_ps(_mm_cvttps_epi32(v.m));
			return _mm_add_ps(_mm_or_ps(_mm_and_ps(_mm_set1_ps(-0.f), v.m),
							  _mm_cvtepi32_ps(_mm_and_si128(_mm_cvtps_epi32(f),
											  _mm_set1_epi32(1)))), f);
		}

		friend inline const vec4 sign(const vec4 &v) {
			return _mm_and_ps(_mm_or_ps(_mm_and_ps(v.m, _mm_set1_ps(-0.f)),
										_mm_set1_ps(1.0f)),
							  _mm_cmpneq_ps(v.m, _mm_setzero_ps()));
		}

		friend inline const vec4 smoothstep(float f1, float f2,
											const vec4 &v) {
			__m128 ff1 = _mm_set1_ps(f1);
			 __m128 c = _mm_max_ps(_mm_min_ps(_mm_div_ps(_mm_sub_ps(v.m, ff1),
								   _mm_sub_ps(_mm_set1_ps(f2), ff1)),
								   _mm_set1_ps(1.f)), _mm_setzero_ps());
			return _mm_mul_ps(_mm_mul_ps(c, c),
							  _mm_sub_ps(_mm_set1_ps(3.0f), _mm_add_ps(c, c)));
		}

		friend inline const vec4 smoothstep(const vec4 &v0,
											const vec4 &v1, const vec4 &v2) {
			 __m128 c = _mm_max_ps(_mm_min_ps(_mm_div_ps(_mm_sub_ps(v2.m, v0.m),
								   _mm_sub_ps(v1.m, v0.m)), _mm_set1_ps(1.f)),
								   _mm_setzero_ps());
			return _mm_mul_ps(_mm_mul_ps(c, c),
							  _mm_sub_ps(_mm_set1_ps(3.0f), _mm_add_ps(c, c)));
		}

		friend inline const vec4 step(float f, const vec4 &v) {
			return _mm_and_ps(_mm_cmple_ps(v.m, _mm_set1_ps(f)),
							  _mm_set1_ps(1.0f));
		}

		friend inline const vec4 step(const vec4 &v0, const vec4 &v1) {
			return _mm_and_ps(_mm_cmple_ps(v0.m, v1.m), _mm_set1_ps(1.0f));
		}

		friend inline const vec4 trunc(const vec4 &v) {
			return _mm_cvtepi32_ps(_mm_cvttps_epi32(v.m));
		}

		// ----------------------------------------------------------------- //

		friend inline float distance(const vec4 &v0, const vec4 &v1) {
			__m128 l = _mm_sub_ps(v0.m, v1.m);
			l = _mm_mul_ps(l, l);
			l = _mm_add_ps(l, _mm_shufd(l, 0x4E));
			return _mm_cvtss_f32(_mm_sqrt_ss(_mm_add_ss(l,
											 _mm_shufd(l, 0x11))));
		}

		friend inline float dot(const vec4 &v0, const vec4 &v1) {
			__m128 l = _mm_mul_ps(v0.m, v1.m);
			l = _mm_add_ps(l, _mm_shufd(l, 0x4E));
			return _mm_cvtss_f32(_mm_add_ss(l, _mm_shufd(l, 0x11)));
		}

		friend inline const vec4 faceforward(const vec4 &v0,
											 const vec4 &v1, const vec4 &v2) {
			__m128 l = _mm_mul_ps(v2.m, v1.m);
			l = _mm_add_ps(l, _mm_shufd(l, 0x4E));
			return _mm_xor_ps(_mm_and_ps(_mm_cmpnlt_ps(
					_mm_add_ps(l, _mm_shufd(l, 0x11)),
					_mm_setzero_ps()), _mm_set1_ps(-0.f)), v0.m);
		}

		friend inline float length(const vec4 &v) {
			__m128 l = _mm_mul_ps(v.m, v.m);
			l = _mm_add_ps(l, _mm_shufd(l, 0x4E));
			return _mm_cvtss_f32(_mm_sqrt_ss(_mm_add_ss(l,
											 _mm_shufd(l, 0x11))));
		}

		friend inline const vec4 normalize(const vec4 &v) {
			__m128 l = _mm_mul_ps(v.m, v.m);
			l = _mm_add_ps(l, _mm_shufd(l, 0x4E));
			return _mm_div_ps(v.m, _mm_sqrt_ps(_mm_add_ps(l,
											   _mm_shufd(l, 0x11))));
		}

		friend inline const vec4 reflect(const vec4 &v0, const vec4 &v1) {
			__m128 l = _mm_mul_ps(v1.m, v0.m);
			l = _mm_add_ps(l, _mm_shufd(l, 0x4E));
			l = _mm_add_ps(l, _mm_shufd(l, 0x11));
			return _mm_sub_ps(v0.m, _mm_mul_ps(_mm_add_ps(l, l), v1.m));
		}

		friend inline const vec4 refract(const vec4 &v0, const vec4 &v1,
										 float f) {
			__m128 o = _mm_set1_ps(1.0f);
			__m128 e = _mm_set1_ps(f);
			__m128 d = _mm_mul_ps(v1.m, v0.m);
			d = _mm_add_ps(d, _mm_shufd(d, 0x4E));
			d = _mm_add_ps(d, _mm_shufd(d, 0x11));
			__m128 k = _mm_sub_ps(o, _mm_mul_ps(_mm_mul_ps(e, e),
									 _mm_sub_ps(o, _mm_mul_ps(d, d))));
			return _mm_and_ps(_mm_cmpnlt_ps(k, _mm_setzero_ps()),
							  _mm_mul_ps(_mm_mul_ps(e, _mm_sub_ps(v0.m,
							  _mm_mul_ps(_mm_mul_ps(e, d), _mm_sqrt_ps(k)))),
										 v1.m));
		}

		// ----------------------------------------------------------------- //

		friend inline bool operator == (const vec4 &v0, const vec4 &v1) {
			return (_mm_movemask_ps(_mm_cmpeq_ps(v0.m, v1.m)) == 0xF);
		}

		friend inline bool operator != (const vec4 &v0, const vec4 &v1) {
			return (_mm_movemask_ps(_mm_cmpneq_ps(v0.m, v1.m)) != 0x0);
		}

		// ----------------------------------------------------------------- //

		union {
				// Vertex / Vector 
			struct {
				float x, y, z, w;
			};
				// Color
			struct {
				float r, g, b, a;
			};
				// Texture coordinates
			struct {
				float s, t, p, q;
			};

				// SSE register
			__m128	m;
		};

		// Avoid pollution
	#undef _mm_shufd
};

#endif
