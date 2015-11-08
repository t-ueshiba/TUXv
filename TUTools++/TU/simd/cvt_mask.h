/*
 *  $Id$
 */
#if !defined(__TU_SIMD_CVT_MASK_H)
#define	__TU_SIMD_CVT_MASK_H

#include "TU/simd/cvt.h"

namespace TU
{
namespace simd
{
/************************************************************************
*  Mask conversion operators						*
************************************************************************/
//! T型マスクベクトルのI番目の部分をより大きなS型マスクベクトルに型変換する．
/*!
  整数ベクトル間の変換の場合，SのサイズはTの2/4/8倍である．また，S, Tは
  符号付き／符号なしのいずれでも良い．
  \param x	変換されるマスクベクトル
  \return	変換されたマスクベクトル
*/
template <class S, size_t I=0, class T> vec<S>	cvt_mask(vec<T> x)	;

//! 2つのT型整数マスクベクトルをより小さなS型整数マスクベクトルに型変換する．
/*!
  SのサイズはTの倍である．また，S, Tは符号付き／符号なしのいずれでも良い．
  \param x	変換されるマスクベクトル
  \param y	変換されるマスクベクトル
  \return	xが変換されたものを下位，yが変換されたものを上位に
		配したマスクベクトル
*/
template <class S, class T> vec<S>	cvt_mask(vec<T> x, vec<T> y)	;

/************************************************************************
*  Converting mask vec tuples						*
************************************************************************/
namespace detail
{
  template <class S, size_t I>
  struct generic_cvt_mask
  {
      vec<S>	operator ()(vec<S> x) const
		{
		    return x;
		}
      template <class T_>
      vec<S>	operator ()(vec<T_> x) const
		{
		    return cvt_mask<S, I>(x);
		}
      template <class T_>
      vec<S>	operator ()(vec<T_> x, vec<T_> y) const
		{
		    return cvt_mask<S>(x, y);
		}
  };
}
    
template <class S, size_t I=0, class HEAD, class TAIL> inline auto
cvt_mask(const boost::tuples::cons<HEAD, TAIL>& x)
    -> decltype(boost::tuples::cons_transform(
		    x, detail::generic_cvt_mask<S, I>()))
{
    return boost::tuples::cons_transform(x, detail::generic_cvt_mask<S, I>());
}
    
template <class S, class H1, class T1, class H2, class T2> inline auto
cvt_mask(const boost::tuples::cons<H1, T1>& x,
	 const boost::tuples::cons<H2, T2>& y)
    -> decltype(boost::tuples::cons_transform(x, y,
					      detail::generic_cvt_mask<S, 0>()))
{
    return boost::tuples::cons_transform(x, y,
					 detail::generic_cvt_mask<S, 0>());
}
    
/************************************************************************
*  Converting packs							*
************************************************************************/
namespace detail
{
  template <class S, class T>
  inline typename std::enable_if<(2*vec<S>::size == vec<T>::size),
				 std::pair<vec<S>, vec<S> > >::type
  cvtup_mask(const vec<T>& x)
  {
      return std::make_pair(cvt_mask<S, 0>(x), cvt_mask<S, 1>(x));
  }

  template <class S, class T>
  inline typename std::enable_if<(vec<S>::size == vec<T>::size), vec<S> >::type
  cvtup_mask(const vec<T>& x)
  {
      return cvt_mask<S>(x);
  }

  template <class S, class PACK> inline pack_target<S, std::pair<PACK, PACK> >
  cvtup_mask(const std::pair<PACK, PACK>& x)
  {
      return std::make_pair(cvtup_mask<S>(x.first), cvtup_mask<S>(x.second));
  }

  template <class S, class T>
  inline typename std::enable_if<(vec<S>::size == 2*vec<T>::size),
				 vec<S> >::type
  cvtdown_mask(const std::pair<vec<T>, vec<T> >& x)
  {
      return cvt_mask<S>(x.first, x.second);
  }
    
  template <class S, class PACK> inline pack_target<S, std::pair<PACK, PACK> >
  cvtdown_mask(const std::pair<PACK, PACK>& x)
  {
      return std::make_pair(cvtdown_mask<S>(x.first),
			    cvtdown_mask<S>(x.second));
  }
}
    
}	// namespace simd
}	// namespace TU

#if defined(MMX)
#  include "TU/simd/intel/cvt_mask.h"
#elif defined(NEON)
#  include "TU/simd/arm/cvt_mask.h"
#endif

#endif	// !__TU_SIMD_CVT_MASK_H=
