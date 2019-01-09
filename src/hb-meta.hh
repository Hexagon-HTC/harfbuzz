/*
 * Copyright © 2018  Google, Inc.
 *
 *  This is part of HarfBuzz, a text shaping library.
 *
 * Permission is hereby granted, without written agreement and without
 * license or royalty fees, to use, copy, modify, and distribute this
 * software and its documentation for any purpose, provided that the
 * above copyright notice and the following two paragraphs appear in
 * all copies of this software.
 *
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES
 * ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN
 * IF THE COPYRIGHT HOLDER HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * THE COPYRIGHT HOLDER SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING,
 * BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE COPYRIGHT HOLDER HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 * Google Author(s): Behdad Esfahbod
 */

#ifndef HB_META_HH
#define HB_META_HH

#include "hb.hh"


/*
 * C++ Template Meta-programming.
 */


template <typename T> static inline T hb_declval ();
#define hb_declval(T) (hb_declval<T> ())

template <typename T> struct hb_match_const { typedef T type; enum { value = false }; };
template <typename T> struct hb_match_const<const T> { typedef T type; enum { value = true }; };
#define hb_remove_const(T) typename hb_match_const<T>::type
#define hb_is_const(T) hb_match_const<T>::value
template <typename T> struct hb_match_reference { typedef T type; enum { value = false }; };
template <typename T> struct hb_match_reference<T &> { typedef T type; enum { value = true }; };
#define hb_remove_reference(T) typename hb_match_reference<T>::type
#define hb_is_reference(T) hb_match_reference<T>::value
template <typename T> struct hb_match_pointer { typedef T type; enum { value = false }; };
template <typename T> struct hb_match_pointer<T *> { typedef T type; enum { value = true }; };
#define hb_remove_pointer(T) typename hb_match_pointer<T>::type
#define hb_is_pointer(T) hb_match_pointer<T>::value


/* Void!  For when we need a expression-type of void. */
struct hb_void_t { typedef void value; };

/* Bool!  For when we need to evaluate type-dependent expressions
 * in a template argument. */
template <bool b> struct hb_bool_tt { enum { value = b }; };
typedef hb_bool_tt<true> hb_true_t;
typedef hb_bool_tt<false> hb_false_t;


template<bool B, typename T = void>
struct hb_enable_if {};

template<typename T>
struct hb_enable_if<true, T> { typedef T type; };

#define hb_enable_if(Cond) typename hb_enable_if<(Cond)>::type* = nullptr


/*
 * Meta-functions.
 */

template <typename T> struct hb_is_signed;
template <> struct hb_is_signed<signed char> { enum { value = true }; };
template <> struct hb_is_signed<signed short> { enum { value = true }; };
template <> struct hb_is_signed<signed int> { enum { value = true }; };
template <> struct hb_is_signed<signed long> { enum { value = true }; };
template <> struct hb_is_signed<unsigned char> { enum { value = false }; };
template <> struct hb_is_signed<unsigned short> { enum { value = false }; };
template <> struct hb_is_signed<unsigned int> { enum { value = false }; };
template <> struct hb_is_signed<unsigned long> { enum { value = false }; };
/* We need to define hb_is_signed for the typedefs we use on pre-Visual
 * Studio 2010 for the int8_t type, since __int8/__int64 is not considered
 * the same as char/long.  The previous lines will suffice for the other
 * types, though.  Note that somehow, unsigned __int8 is considered same
 * as unsigned char.
 * https://github.com/harfbuzz/harfbuzz/pull/1499
 */
#if defined(_MSC_VER) && (_MSC_VER < 1600)
template <> struct hb_is_signed<__int8> { enum { value = true }; };
#endif
#define hb_is_signed(T) hb_is_signed<T>::value

template <bool is_signed> struct hb_signedness_int;
template <> struct hb_signedness_int<false> { typedef unsigned int value; };
template <> struct hb_signedness_int<true>  { typedef   signed int value; };
#define hb_signedness_int(T) hb_signedness_int<T>::value


#endif /* HB_META_HH */
