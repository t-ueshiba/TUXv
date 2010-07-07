/*
 *  平成14-19年（独）産業技術総合研究所 著作権所有
 *  
 *  創作者：植芝俊夫
 *
 *  本プログラムは（独）産業技術総合研究所の職員である植芝俊夫が創作し，
 *  （独）産業技術総合研究所が著作権を所有する秘密情報です．著作権所有
 *  者による許可なしに本プログラムを使用，複製，改変，第三者へ開示する
 *  等の行為を禁止します．
 *  
 *  このプログラムによって生じるいかなる損害に対しても，著作権所有者お
 *  よび創作者は責任を負いません。
 *
 *  Copyright 2002-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the copyright holder are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holder or the creator are not responsible for any
 *  damages caused by using this program.
 *  
 *  $Id: Vector++.h,v 1.38 2010-07-07 03:02:40 ueshiba Exp $
 */
#ifndef __TUVectorPP_h
#define __TUVectorPP_h

#include "TU/Array++.h"
#include <cmath>
#include <iomanip>

namespace TU
{
/************************************************************************
*  class Rotation							*
************************************************************************/
//! 2次元超平面内での回転を表すクラス
/*!
  具体的には
  \f[
    \TUvec{R}{}(p, q; \theta) \equiv
    \begin{array}{r@{}l}
      & \begin{array}{ccccccccccc}
        & & \makebox[4.0em]{} & p & & & \makebox[3.8em]{} & q & & &
      \end{array} \\
      \begin{array}{l}
        \\ \\ \\ \raisebox{1.5ex}{$p$} \\ \\ \\ \\ \raisebox{1.5ex}{$q$} \\ \\ \\
      \end{array} &
      \TUbeginarray{ccccccccccc}
	1 \\
	& \ddots \\
	& & 1 \\
	& & & \cos\theta & & & & -\sin\theta \\
	& & & & 1 \\
	& & & & & \ddots \\
	& & & & & & 1 \\
	& & & \sin\theta & & & & \cos\theta \\
	& & & & & & & & 1\\
	& & & & & & & & & \ddots \\
	& & & & & & & & & & 1
      \TUendarray
    \end{array}
  \f]
  なる回転行列で表される．
*/
class __PORT Rotation
{
  public:
    Rotation(u_int p, u_int q, double x, double y)	;
    Rotation(u_int p, u_int q, double theta)		;

  //! p軸を返す．
  /*!
    \return	p軸のindex
  */
    u_int	p()				const	{return _p;}

  //! q軸を返す．
  /*!
    \return	q軸のindex
  */
    u_int	q()				const	{return _q;}

  //! 回転角生成ベクトルの長さを返す．
  /*!
    \return	回転角生成ベクトル(x, y)に対して\f$\sqrt{x^2 + y^2}\f$
  */
    double	length()			const	{return _l;}

  //! 回転角のcos値を返す．
  /*!
    \return	回転角のcos値
  */
    double	cos()				const	{return _c;}

  //! 回転角のsin値を返す．
  /*!
    \return	回転角のsin値
  */
    double	sin()				const	{return _s;}
    
  private:
    const u_int	_p, _q;				// rotation axis
    double	_l;				// length of (x, y)
    double	_c, _s;				// cos & sin
};

/************************************************************************
*  class Vector<T>							*
************************************************************************/
template <class T, class B, class R>	class Matrix;

//! T型の要素を持つベクトルを表すクラス
/*!
  \param T	要素の型
  \param B	バッファ
*/
template <class T, class B=Buf<T> >
class Vector : public Array<T, B>
{
  public:
    typedef typename Array<T, B>::value_type		value_type;
    typedef typename Array<T, B>::difference_type	difference_type;
    typedef typename Array<T, B>::reference		reference;
    typedef typename Array<T, B>::const_reference	const_reference;
    typedef typename Array<T, B>::pointer		pointer;
    typedef typename Array<T, B>::const_pointer		const_pointer;
    typedef typename Array<T, B>::iterator		iterator;
    typedef typename Array<T, B>::const_iterator	const_iterator;
    typedef typename Array<T, B>::reverse_iterator	reverse_iterator;
    typedef typename Array<T, B>::const_reverse_iterator
							const_reverse_iterator;
    
  public:
    Vector()								;
    explicit Vector(u_int d)						;
    Vector(T* p, u_int d)						;
    template <class B2>
    Vector(const Vector<T, B2>& v, u_int i, u_int d)			;
    template <class T2, class B2>
    Vector(const Vector<T2, B2>& v)					;
    template <class T2, class B2>
    Vector&		operator =(const Vector<T2, B2>& v)		;

    using		Array<T, B>::begin;
    using		Array<T, B>::end;
    using		Array<T, B>::size;
    using		Array<T, B>::dim;
  //    using		Array<T, B>::operator pointer;
  //    using		Array<T, B>::operator const_pointer;

    const Vector<T>	operator ()(u_int i, u_int d)		const	;
    Vector<T>		operator ()(u_int i, u_int d)			;
    Vector&		operator  =(const T& c)				;
    Vector&		operator *=(double c)				;
    Vector&		operator /=(double c)				;
    template <class T2, class B2>
    Vector&		operator +=(const Vector<T2, B2>& v)		;
    template <class T2, class B2>
    Vector&		operator -=(const Vector<T2, B2>& v)		;
    template <class T2, class B2>
    Vector&		operator ^=(const Vector<T2, B2>& V)		;
    template <class T2, class B2, class R2>
    Vector&		operator *=(const Matrix<T2, B2, R2>& m)	;
    Vector		operator  -()				const	;
    T			square()				const	;
    double		length()				const	;
    template <class T2, class B2>
    T			sqdist(const Vector<T2, B2>& v)		const	;
    template <class T2, class B2>
    double		dist(const Vector<T2, B2>& v)		const	;
    Vector&		normalize()					;
    Vector		normal()				const	;
    template <class T2, class B2, class R2>
    Vector&		solve(const Matrix<T2, B2, R2>& m)		;
    Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3> >
			skew()					const	;
    Vector<T>		homogeneous()				const	;
    Vector<T>		inhomogeneous()				const	;
    void		resize(u_int d)					;
    void		resize(T* p, u_int d)				;
};

//! ベクトルを生成し，全要素を0で初期化する．
template <class T, class B>
Vector<T, B>::Vector()
    :Array<T, B>()
{
    *this = 0;
}

//! 指定された次元のベクトルを生成し，全要素を0で初期化する．
/*!
  \param d	ベクトルの次元
*/
template <class T, class B> inline
Vector<T, B>::Vector(u_int d)
    :Array<T, B>(d)
{
    *this = 0;
}

//! 外部記憶領域と次元を指定してベクトルを生成する．
/*!
  \param p	外部記憶領域へのポインタ
  \param d	ベクトルの次元
*/
template <class T, class B> inline
Vector<T, B>::Vector(T* p, u_int d)
    :Array<T, B>(p, d)
{
}

//! 与えられたベクトルと記憶領域を共有する部分ベクトルを生成する．
/*!
  \param v	元のベクトル
  \param i	部分ベクトルの第0要素を指定するindex
  \param d	部分ベクトルの次元
*/
template <class T, class B> template <class B2> inline
Vector<T, B>::Vector(const Vector<T, B2>& v, u_int i, u_int d)
    :Array<T, B>(v, i, d)
{
}

//! 他のベクトルと同一要素を持つベクトルを作る(コピーコンストラクタの拡張)．
/*!
  \param v	コピー元ベクトル
*/
template <class T, class B> template <class T2, class B2> inline
Vector<T, B>::Vector(const Vector<T2, B2>& v)
    :Array<T, B>(v)
{
}
    
//! 他のベクトルを自分に代入する(代入演算子の拡張)．
/*!
  \param v	コピー元ベクトル
  \return	このベクトル
*/
template <class T, class B> template <class T2, class B2> inline Vector<T, B>&
Vector<T, B>::operator =(const Vector<T2, B2>& v)
{
    Array<T, B>::operator =(v);
    return *this;
}

//! このベクトルと記憶領域を共有した部分ベクトルを生成する．
/*!
    \param i	部分ベクトルの第0要素を指定するindex
    \param d	部分ベクトルの次元
    \return	生成された部分ベクトル
*/
template <class T, class B> inline Vector<T>
Vector<T, B>::operator ()(u_int i, u_int d)
{
    return Vector<T>(*this, i, d);
}

//! このベクトルと記憶領域を共有した部分ベクトルを生成する．
/*!
    \param i	部分ベクトルの第0要素を指定するindex
    \param d	部分ベクトルの次元
    \return	生成された部分ベクトル
*/
template <class T, class B> inline const Vector<T>
Vector<T, B>::operator ()(u_int i, u_int d) const
{
    return Vector<T>(*this, i, d);
}

//! このベクトルの全ての要素に同一の数値を代入する．
/*!
  \param c	代入する数値
  \return	このベクトル
*/
template <class T, class B> inline Vector<T, B>&
Vector<T, B>::operator =(const T& c)
{
    Array<T, B>::operator =(c);
    return *this;
}

//! このベクトルに指定された数値を掛ける．
/*!
  \param c	掛ける数値
  \return	このベクトル，すなわち\f$\TUvec{u}{}\leftarrow c\TUvec{u}{}\f$
*/
template <class T, class B> inline Vector<T, B>&
Vector<T, B>::operator *=(double c)
{
    Array<T, B>::operator *=(c);
    return *this;
}

//! このベクトルを指定された数値で割る．
/*!
  \param c	割る数値
  \return	このベクトル，すなわち
		\f$\TUvec{u}{}\leftarrow \frac{\TUvec{u}{}}{c}\f$
*/
template <class T, class B> inline Vector<T, B>&
Vector<T, B>::operator /=(double c)
{
    Array<T, B>::operator /=(c);
    return *this;
}

//! このベクトルに他のベクトルを足す．
/*!
  \param v	足すベクトル
  \return	このベクトル，すなわち
		\f$\TUvec{u}{}\leftarrow \TUvec{u}{} + \TUvec{v}{}\f$
*/
template <class T, class B> template <class T2, class B2> inline Vector<T, B>&
Vector<T, B>::operator +=(const Vector<T2, B2>& v)
{
    Array<T, B>::operator +=(v);
    return *this;
}

//! このベクトルから他のベクトルを引く．
/*!
  \param v	引くベクトル
  \return	このベクトル，すなわち
		\f$\TUvec{u}{}\leftarrow \TUvec{u}{} - \TUvec{v}{}\f$
*/
template <class T, class B> template <class T2, class B2> inline Vector<T, B>&
Vector<T, B>::operator -=(const Vector<T2, B2>& v)
{
    Array<T, B>::operator -=(v);
    return *this;
}

//! このベクトルと他の3次元ベクトルとのベクトル積をとる．
/*!
    \param v	他のベクトル
    \return	このベクトル，すなわち
		\f$\TUvec{u}{}\leftarrow \TUvec{u}{}\times\TUvec{v}{}\f$
    \throw std::invalid_argument	このベクトルとvが3次元でない場合に送出
*/
template <class T, class B> template <class T2, class B2> Vector<T, B>&
Vector<T, B>::operator ^=(const Vector<T2, B2>& v)	// outer product
{
    check_dim(v.dim());
    if (dim() != 3)
	throw std::invalid_argument("TU::Vector<T, B>::operator ^=: dimension must be 3");
    Vector<T, FixedSizedBuf<T, 3u> > tmp(*this);
    (*this)[0] = tmp[1] * v[2] - tmp[2] * v[1];
    (*this)[1] = tmp[2] * v[0] - tmp[0] * v[2];
    (*this)[2] = tmp[0] * v[1] - tmp[1] * v[0];
    return *this;
}

//! このベクトルの右から行列を掛ける．
/*!
  \param m	掛ける行列
  \return	このベクトル，すなわち
		\f$\TUtvec{u}{} \leftarrow \TUtvec{u}{}\TUvec{M}{}\f$
*/
template <class T, class B> template <class T2, class B2, class R2>
inline Vector<T, B>&
Vector<T, B>::operator *=(const Matrix<T2, B2, R2>& m)
{
    return *this = *this * m;
}

//! このベクトルの符号を反転したベクトルを返す．
/*!
  \return	符号を反転したベクトル，すなわち\f$-\TUvec{u}{}\f$
*/
template <class T, class B> inline Vector<T, B>
Vector<T, B>::operator -() const
{
    return Vector(*this) *= -1;
}

//! このベクトルの長さの2乗を返す．
/*!
  \return	ベクトルの長さの2乗，すなわち\f$\TUnorm{\TUvec{u}{}}^2\f$
*/
template <class T, class B> inline T
Vector<T, B>::square() const
{
    return *this * *this;
}

//! このベクトルの長さを返す．
/*!
  \return	ベクトルの長さ，すなわち\f$\TUnorm{\TUvec{u}{}}\f$
*/
template <class T, class B> inline double
Vector<T, B>::length() const
{
    return sqrt(double(square()));
}

//! このベクトルと他のベクトルの差の長さの2乗を返す．
/*!
  \param v	比較対象となるベクトル
  \return	ベクトル間の差の2乗，すなわち
		\f$\TUnorm{\TUvec{u}{} - \TUvec{v}{}}^2\f$
*/
template <class T, class B> template <class T2, class B2> inline T
Vector<T, B>::sqdist(const Vector<T2, B2>& v) const
{
    return (*this - v).square();
}

//! このベクトルと他のベクトルの差の長さを返す．
/*!
  \param v	比較対象となるベクトル
  \return	ベクトル間の差，すなわち
		\f$\TUnorm{\TUvec{u}{} - \TUvec{v}{}}\f$
*/
template <class T, class B> template <class T2, class B2> inline double
Vector<T, B>::dist(const Vector<T2, B2>& v) const
{
    return sqrt(sqdist(v));
}

//! このベクトルの長さを1に正規化する．
/*!
  \return	このベクトル，すなわち
		\f$
		  \TUvec{u}{}\leftarrow\frac{\TUvec{u}{}}{\TUnorm{\TUvec{u}{}}}
		\f$
*/
template <class T, class B> inline Vector<T, B>&
Vector<T, B>::normalize()
{
    return *this /= length();
}

//! このベクトルの長さを1に正規化したベクトルを返す．
/*!
  \return	長さを正規化したベクトル，すなわち
		\f$\frac{\TUvec{u}{}}{\TUnorm{\TUvec{u}{}}}\f$
*/
template <class T, class B> inline Vector<T, B>
Vector<T, B>::normal() const
{
    return Vector(*this).normalize();
}

//! この3次元ベクトルから3x3反対称行列を生成する．
/*!
  \return	生成された反対称行列，すなわち
  \f[
    \TUskew{u}{} \equiv
    \TUbeginarray{ccc}
      & -u_2 & u_1 \\ u_2 & & -u_0 \\ -u_1 & u_0 &
    \TUendarray
  \f]
  \throw std::invalid_argument	3次元ベクトルでない場合に送出
*/
template <class T, class B>
Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3> >
Vector<T, B>::skew() const
{
    if (dim() != 3)
	throw std::invalid_argument("TU::Vector<T, B>::skew: dimension must be 3");
    Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3 > >	r;
    r[2][1] = (*this)[0];
    r[0][2] = (*this)[1];
    r[1][0] = (*this)[2];
    r[1][2] = -r[2][1];
    r[2][0] = -r[0][2];
    r[0][1] = -r[1][0];
    return r;
}

//! 非同次座標を表すベクトルに対し，値1を持つ成分を最後に付加した同次座標ベクトルを返す．
/*!
  \return	同次化されたベクトル
*/
template <class T, class B> inline Vector<T>
Vector<T, B>::homogeneous() const
{
    Vector<T>	v(dim() + 1);
    v(0, dim()) = *this;
    v[dim()]	= 1;
    return v;
}

//! 同次座標を表すベクトルに対し，各成分を最後の成分で割った非同次座標ベクトルを返す．
/*!
  \return	非同次化されたベクトル
*/
template <class T, class B> inline Vector<T>
Vector<T, B>::inhomogeneous() const
{
    return (*this)(0, dim()-1) / (*this)[dim()-1];
}

//! ベクトルの次元を変更し，全要素を0に初期化する．
/*!
  ただし，他のオブジェクトと記憶領域を共有しているベクトルの次元を
  変更することはできない．
  \param d	新しい次元
*/
template <class T, class B> inline void
Vector<T, B>::resize(u_int d)
{
    Array<T, B>::resize(d);
    *this = 0;
}

//! ベクトルが内部で使用する記憶領域を指定したものに変更する．
/*!
  \param p	新しい記憶領域へのポインタ
  \param siz	新しい次元
*/
template <class T, class B> inline void
Vector<T, B>::resize(T* p, u_int d)
{
    Array<T, B>::resize(p, d);
}

/************************************************************************
*  class Matrix<T, B, R>						*
************************************************************************/
template <class T>	class TriDiagonal;
template <class T>	class SVDecomposition;

//! T型の要素を持つ行列を表すクラス
/*!
  各行がT型の要素を持つベクトル#TU::Vector<T>になっている．
  \param T	要素の型
  \param B	バッファ
  \param R	行バッファ
*/
template <class T, class B=Buf<T>, class R=Buf<Vector<T> > >
class Matrix : public Array2<Vector<T>, B, R>
{
  public:
    typedef typename Array2<Vector<T>, B, R>::value_type	value_type;
    typedef typename Array2<Vector<T>, B, R>::difference_type	difference_type;
    typedef typename Array2<Vector<T>, B, R>::reference		reference;
    typedef typename Array2<Vector<T>, B, R>::const_reference	const_reference;
    typedef typename Array2<Vector<T>, B, R>::pointer		pointer;
    typedef typename Array2<Vector<T>, B, R>::const_pointer	const_pointer;
    typedef typename Array2<Vector<T>, B, R>::iterator		iterator;
    typedef typename Array2<Vector<T>, B, R>::const_iterator	const_iterator;
    
  public:
    Matrix()								;
    Matrix(u_int r, u_int c)						;
    Matrix(T* p, u_int r, u_int c)					;
    template <class B2, class R2>
    Matrix(const Matrix<T, B2, R2>& m, u_int i, u_int j, u_int r, u_int c)	;
    template <class T2, class B2, class R2>
    Matrix(const Matrix<T2, B2, R2>& m)					;
    template <class T2, class B2, class R2>
    Matrix&		operator =(const Matrix<T2, B2, R2>& m)		;

    using		Array2<Vector<T>, B, R>::begin;
    using		Array2<Vector<T>, B, R>::end;
    using		Array2<Vector<T>, B, R>::size;
    using		Array2<Vector<T>, B, R>::dim;
    using		Array2<Vector<T>, B, R>::nrow;
    using		Array2<Vector<T>, B, R>::ncol;

			operator const Vector<T>()		const	;
    const Matrix<T>	operator ()(u_int i, u_int j,
				    u_int r, u_int c)		const	;
    Matrix<T>		operator ()(u_int i, u_int j,
				    u_int r, u_int c)			;
    Matrix&		operator  =(const T& c)				;
    Matrix&		operator *=(double c)				;
    Matrix&		operator /=(double c)				;
    template <class T2, class B2, class R2>
    Matrix&		operator +=(const Matrix<T2, B2, R2>& m)	;
    template <class T2, class B2, class R2>
    Matrix&		operator -=(const Matrix<T2, B2, R2>& m)	;
    template <class T2, class B2, class R2>
    Matrix&		operator *=(const Matrix<T2, B2, R2>& m)	;
    template <class T2, class B2>
    Matrix&		operator ^=(const Vector<T2, B2>& v)		;
    Matrix		operator  -()				const	;
    Matrix&		diag(T c)					;
    Matrix<T>		trns()					const	;
    Matrix		inv()					const	;
    template <class T2, class B2, class R2>
    Matrix&		solve(const Matrix<T2, B2, R2>& m)		;
    T			det()					const	;
    T			det(u_int p, u_int q)			const	;
    T			trace()					const	;
    Matrix		adj()					const	;
    Matrix<T>		pinv(T cndnum=1.0e5)			const	;
    Matrix<T>		eigen(Vector<T>& eval, bool abs=true)	const	;
    Matrix<T>		geigen(const Matrix<T>& BB,
			       Vector<T>& eval, bool abs=true)	const	;
    Matrix		cholesky()				const	;
    Matrix&		normalize()					;
    Matrix&		rotate_from_left(const Rotation& r)		;
    Matrix&		rotate_from_right(const Rotation& r)		;
    T			square()				const	;
    double		length()				const	;
    Matrix&		symmetrize()					;
    Matrix&		antisymmetrize()				;
    void		rot2angle(T& theta_x,
				  T& theta_y, T& theta_z)	const	;
    Vector<T, FixedSizedBuf<T, 3> >
			rot2axis(T& c, T& s)			const	;
    Vector<T, FixedSizedBuf<T, 3> >
			rot2axis()				const	;

    static Matrix	I(u_int d)					;
    template <class T2, class B2>
    static Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3> >
			Rt(const Vector<T2, B2>& n, T c, T s)		;
    template <class T2, class B2>
    static Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3> >
			Rt(const Vector<T2, B2>& axis)			;

    void		resize(u_int r, u_int c)			;
    void		resize(T* p, u_int r, u_int c)			;
};

//! 行列を生成し，全要素を0で初期化する．
template <class T, class B, class R> inline
Matrix<T, B, R>::Matrix()
    :Array2<Vector<T>, B, R>()
{
    *this = 0;
}

//! 指定されたサイズの行列を生成し，全要素を0で初期化する．
/*!
  \param r	行列の行数
  \param c	行列の列数
*/
template <class T, class B, class R> inline
Matrix<T, B, R>::Matrix(u_int r, u_int c)
    :Array2<Vector<T>, B, R>(r, c)
{
    *this = 0;
}

//! 外部記憶領域とサイズを指定して行列を生成する．
/*!
  \param p	外部記憶領域へのポインタ
  \param r	行列の行数
  \param c	行列の列数
*/
template <class T, class B, class R> inline
Matrix<T, B, R>::Matrix(T* p, u_int r, u_int c)
    :Array2<Vector<T>, B, R>(p, r, c)
{
}

//! 与えられた行列と記憶領域を共有する部分行列を生成する．
/*!
  \param m	元の行列
  \param i	部分行列の第0行を指定するindex
  \param j	部分行列の第0列を指定するindex
  \param r	部分行列の行数
  \param c	部分行列の列数
*/
template <class T, class B, class R> template <class B2, class R2> inline
Matrix<T, B, R>::Matrix(const Matrix<T, B2, R2>& m,
			u_int i, u_int j, u_int r, u_int c)
    :Array2<Vector<T>, B, R>(m, i, j, r, c)
{
}

//! 他の行列と同一要素を持つ行列を作る(コピーコンストラクタの拡張)．
/*!
  \param m	コピー元行列
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline
Matrix<T, B, R>::Matrix(const Matrix<T2, B2, R2>& m)
    :Array2<Vector<T>, B, R>(m)
{
}

//! 他の行列を自分に代入する(代入演算子の拡張)．
/*!
  \param m	コピー元行列
  \return	この行列
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline Matrix<T, B, R>&
Matrix<T, B, R>::operator =(const Matrix<T2, B2, R2>& m)
{
    Array2<Vector<T>, B, R>::operator =(m);
    return *this;
}

//! この行列の行を並べて記憶領域を共有するベクトルを生成する．
/*!
  全行の記憶領域は連続していなければならない．
  \return	記憶領域を共有するベクトル
*/
template <class T, class B, class R> inline
Matrix<T, B, R>::operator const Vector<T>() const
{
    return Vector<T>(const_cast<T*>((const T*)*this), nrow()*ncol());
}

//! この行列と記憶領域を共有した部分行列を生成する．
/*!
    \param i	部分行列の左上隅要素となる行を指定するindex
    \param j	部分行列の左上隅要素となる列を指定するindex
    \param r	部分行列の行数
    \param c	部分行列の列数
    \return	生成された部分行列
*/
template <class T, class B, class R> inline Matrix<T>
Matrix<T, B, R>::operator ()(u_int i, u_int j, u_int r, u_int c)
{
    return Matrix<T>(*this, i, j, r, c);
}

//! この行列と記憶領域を共有した部分行列を生成する．
/*!
    \param i	部分行列の左上隅要素となる行を指定するindex
    \param j	部分行列の左上隅要素となる列を指定するindex
    \param r	部分行列の行数
    \param c	部分行列の列数
    \return	生成された部分行列
*/
template <class T, class B, class R> inline const Matrix<T>
Matrix<T, B, R>::operator ()(u_int i, u_int j, u_int r, u_int c) const
{
    return Matrix<T>(*this, i, j, r, c);
}

//! この行列の全ての要素に同一の数値を代入する．
/*!
  \param c	代入する数値
  \return	この行列
*/
template <class T, class B, class R> inline Matrix<T, B, R>&
Matrix<T, B, R>::operator =(const T& c)
{
    Array2<Vector<T>, B, R>::operator =(c);
    return *this;
}

//! この行列に指定された数値を掛ける．
/*!
  \param c	掛ける数値
  \return	この行列，すなわち\f$\TUvec{A}{}\leftarrow c\TUvec{A}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>&
Matrix<T, B, R>::operator *=(double c)
{
    Array2<Vector<T>, B, R>::operator *=(c);
    return *this;
}

//! この行列を指定された数値で割る．
/*!
  \param c	割る数値
  \return	この行列，すなわち
		\f$\TUvec{A}{}\leftarrow \frac{\TUvec{A}{}}{c}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>&
Matrix<T, B, R>::operator /=(double c)
{
    Array2<Vector<T>, B, R>::operator /=(c);
    return *this;
}

//! この行列に他の行列を足す．
/*!
  \param m	足す行列
  \return	この行列，すなわち
		\f$\TUvec{A}{}\leftarrow \TUvec{A}{} + \TUvec{M}{}\f$
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline Matrix<T, B, R>&
Matrix<T, B, R>::operator +=(const Matrix<T2, B2, R2>& m)
{
    Array2<Vector<T>, B, R>::operator +=(m);
    return *this;
}

//! この行列から他の行列を引く．
/*!
  \param m	引く行列
  \return	この行列，すなわち
		\f$\TUvec{A}{}\leftarrow \TUvec{A}{} - \TUvec{M}{}\f$
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline Matrix<T, B, R>&
Matrix<T, B, R>::operator -=(const Matrix<T2, B2, R2>& m)
{
    Array2<Vector<T>, B, R>::operator -=(m);
    return *this;
}

//! この行列に他の行列を掛ける．
/*!
  \param m	掛ける行列
  \return	この行列，すなわち
		\f$\TUvec{A}{}\leftarrow \TUvec{A}{}\TUvec{M}{}\f$
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
inline Matrix<T, B, R>&
Matrix<T, B, R>::operator *=(const Matrix<T2, B2, R2>& m)
{
    return *this = *this * m;
}

//! この?x3行列の各行と3次元ベクトルとのベクトル積をとる．
/*!
  \param v	3次元ベクトル
  \return	この行列，すなわち
		\f$\TUvec{A}{}\leftarrow(\TUtvec{A}{}\times\TUvec{v}{})^\top\f$
*/
template <class T, class B, class R> template <class T2, class B2>
Matrix<T, B, R>&
Matrix<T, B, R>::operator ^=(const Vector<T2, B2>& v)
{
    for (u_int i = 0; i < nrow(); ++i)
	(*this)[i] ^= v;
    return *this;
}

//! この行列の符号を反転した行列を返す．
/*!
  \return	符号を反転した行列，すなわち\f$-\TUvec{A}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>
Matrix<T, B, R>::operator -() const
{
    return Matrix(*this) *= -1;
}

//! この正方行列を全て同一の対角成分値を持つ対角行列にする．
/*!
  \param c	対角成分の値
  \return	この行列，すなわち\f$\TUvec{A}{} \leftarrow \diag(c,\ldots,c)\f$
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::diag(T c)
{
    check_dim(ncol());
    *this = 0;
    for (u_int i = 0; i < nrow(); ++i)
	(*this)[i][i] = c;
    return *this;
}

//! この行列の転置行列を返す．
/*!
  \return	転置行列，すなわち\f$\TUtvec{A}{}\f$
*/
template <class T, class B, class R> Matrix<T>
Matrix<T, B, R>::trns() const
{
    Matrix<T> val(ncol(), nrow());
    for (u_int i = 0; i < nrow(); ++i)
	for (u_int j = 0; j < ncol(); ++j)
	    val[j][i] = (*this)[i][j];
    return val;
}

//! この行列の逆行列を返す．
/*!
  \return	逆行列，すなわち\f$\TUinv{A}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>
Matrix<T, B, R>::inv() const
{
    return I(nrow()).solve(*this);
}

//! この行列の小行列式を返す．
/*!
  \param p	元の行列から取り除く行を指定するindex
  \param q	元の行列から取り除く列を指定するindex
  \return	小行列式，すなわち\f$\det\TUvec{A}{pq}\f$
*/
template <class T, class B, class R> T
Matrix<T, B, R>::det(u_int p, u_int q) const
{
    Matrix<T>		d(nrow()-1, ncol()-1);
    for (u_int i = 0; i < p; ++i)
    {
	for (u_int j = 0; j < q; ++j)
	    d[i][j] = (*this)[i][j];
	for (u_int j = q; j < d.ncol(); ++j)
	    d[i][j] = (*this)[i][j+1];
    }
    for (u_int i = p; i < d.nrow(); ++i)
    {
	for (u_int j = 0; j < q; ++j)
	    d[i][j] = (*this)[i+1][j];
	for (u_int j = q; j < d.ncol(); ++j)
	    d[i][j] = (*this)[i+1][j+1];
    }
    return d.det();
}

//! この正方行列のtraceを返す．
/*!
  \return			trace, すなわち\f$\trace\TUvec{A}{}\f$
  \throw std::invalid_argument	正方行列でない場合に送出
*/
template <class T, class B, class R> T
Matrix<T, B, R>::trace() const
{
    if (nrow() != ncol())
        throw
	  std::invalid_argument("TU::Matrix<T>::trace(): not square matrix!!");
    T	val = 0.0;
    for (u_int i = 0; i < nrow(); ++i)
	val += (*this)[i][i];
    return val;
}

//! この行列の余因子行列を返す．
/*!
  \return	余因子行列，すなわち
		\f$\TUtilde{A}{} = (\det\TUvec{A}{})\TUinv{A}{}\f$
*/
template <class T, class B, class R> Matrix<T, B, R>
Matrix<T, B, R>::adj() const
{
    Matrix<T, B, R>	val(nrow(), ncol());
    for (u_int i = 0; i < val.nrow(); ++i)
	for (u_int j = 0; j < val.ncol(); ++j)
	    val[i][j] = ((i + j) % 2 ? -det(j, i) : det(j, i));
    return val;
}

//! この行列の疑似逆行列を返す．
/*!
  \param cndnum	最大特異値に対する絶対値の割合がこれに達しない基底は無視
  \return	疑似逆行列，すなわち与えられた行列の特異値分解を
		\f$\TUvec{A}{} = \TUvec{V}{}\diag(\sigma_0,\ldots,\sigma_{n-1})
		\TUtvec{U}{}\f$とすると
		\f[
		  \TUvec{u}{0}\sigma_0^{-1}\TUtvec{v}{0} + \cdots +
		  \TUvec{u}{r}\sigma_{r-1}^{-1}\TUtvec{v}{r-1},
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \TUabs{\sigma_1} > \epsilon\TUabs{\sigma_0},\ldots,
		  \TUabs{\sigma_{r-1}} > \epsilon\TUabs{\sigma_0}
		\f]
*/
template <class T, class B, class R> Matrix<T>
Matrix<T, B, R>::pinv(T cndnum) const
{
    SVDecomposition<T>	svd(*this);
    Matrix<T>		val(svd.ncol(), svd.nrow());
    
    for (u_int i = 0; i < svd.diagonal().dim(); ++i)
	if (fabs(svd[i]) * cndnum > fabs(svd[0]))
	    val += (svd.Ut()[i] / svd[i]) % svd.Vt()[i];

    return val;
}

//! この対称行列の固有値と固有ベクトルを返す．
/*!
    \param eval	固有値が返される
    \param abs	固有値を絶対値の大きい順に並べるならtrue, 値の大きい順に
		並べるならfalse
    \return	各行が固有ベクトルから成る回転行列，すなわち
		\f[
		  \TUvec{A}{}\TUvec{U}{} =
		  \TUvec{U}{}\diag(\lambda_0,\ldots,\lambda_{n-1}),
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \TUtvec{U}{}\TUvec{U}{} = \TUvec{I}{n},~\det\TUvec{U}{} = 1
		\f]
		なる\f$\TUtvec{U}{}\f$
*/
template <class T, class B, class R> Matrix<T>
Matrix<T, B, R>::eigen(Vector<T>& eval, bool abs) const
{
    TriDiagonal<T>	tri(*this);

    tri.diagonalize(abs);
    eval = tri.diagonal();

    return tri.Ut();
}

//! この対称行列の一般固有値と一般固有ベクトルを返す．
/*!
    \param B	もとの行列と同一サイズの正値対称行列
    \param eval	一般固有値が返される
    \param abs	一般固有値を絶対値の大きい順に並べるならtrue, 値の大きい順に
		並べるならfalse
    \return	各行が一般固有ベクトルから成る正則行列
		（ただし直交行列ではない），すなわち
		\f[
		  \TUvec{A}{}\TUvec{U}{} =
		  \TUvec{B}{}\TUvec{U}{}\diag(\lambda_0,\ldots,\lambda_{n-1}),
		  {\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \TUtvec{U}{}\TUvec{B}{}\TUvec{U}{} = \TUvec{I}{n}
		\f]
		なる\f$\TUtvec{U}{}\f$
*/
template <class T, class B, class R> Matrix<T>
Matrix<T, B, R>::geigen(const Matrix<T>& BB, Vector<T>& eval, bool abs) const
{
    Matrix<T>	Ltinv = BB.cholesky().inv(), Linv = Ltinv.trns();
    Matrix<T>	Ut = (Linv * (*this) * Ltinv).eigen(eval, abs);
    
    return Ut * Linv;
}

//! この正値対称行列のCholesky分解（上半三角行列）を返す．
/*!
  計算においては，もとの行列の上半部分しか使わない
  \return	\f$\TUvec{A}{} = \TUvec{L}{}\TUtvec{L}{}\f$なる
		\f$\TUtvec{L}{}\f$（上半三角行列）
  \throw std::invalid_argument	正方行列でない場合に送出
  \throw std::runtime_error	正値でない場合に送出
*/
template <class T, class B, class R> Matrix<T, B, R>
Matrix<T, B, R>::cholesky() const
{
    if (nrow() != ncol())
        throw
	    std::invalid_argument("TU::Matrix<T>::cholesky(): not square matrix!!");

    Matrix<T, B, R>	Lt(*this);
    for (u_int i = 0; i < nrow(); ++i)
    {
	T d = Lt[i][i];
	if (d <= 0)
	    throw std::runtime_error("TU::Matrix<T>::cholesky(): not positive definite matrix!!");
	for (u_int j = 0; j < i; ++j)
	    Lt[i][j] = 0;
	Lt[i][i] = d = sqrt(d);
	for (u_int j = i + 1; j < ncol(); ++j)
	    Lt[i][j] /= d;
	for (u_int j = i + 1; j < nrow(); ++j)
	    for (u_int k = j; k < ncol(); ++k)
		Lt[j][k] -= (Lt[i][j] * Lt[i][k]);
    }
    
    return Lt;
}

//! この行列のノルムを1に正規化する．
/*!
    \return	この行列，すなわち
		\f$
		  \TUvec{A}{}\leftarrow\frac{\TUvec{A}{}}{\TUnorm{\TUvec{A}{}}}
		\f$
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::normalize()
{
    T	sum = 0.0;
    for (u_int i = 0; i < nrow(); ++i)
	sum += (*this)[i] * (*this)[i];
    return *this /= sqrt(sum);
}

//! この行列の左から（転置された）回転行列を掛ける．
/*!
    \return	この行列，すなわち
		\f$\TUvec{A}{}\leftarrow\TUtvec{R}{}\TUvec{A}{}\f$
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::rotate_from_left(const Rotation& r)
{
    for (u_int j = 0; j < ncol(); ++j)
    {
	const T	tmp = (*this)[r.p()][j];
	
	(*this)[r.p()][j] =  r.cos()*tmp + r.sin()*(*this)[r.q()][j];
	(*this)[r.q()][j] = -r.sin()*tmp + r.cos()*(*this)[r.q()][j];
    }
    return *this;
}

//! この行列の右から回転行列を掛ける．
/*!
    \return	この行列，すなわち
		\f$\TUvec{A}{}\leftarrow\TUvec{A}{}\TUvec{R}{}\f$
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::rotate_from_right(const Rotation& r)
{
    for (u_int i = 0; i < nrow(); ++i)
    {
	const T	tmp = (*this)[i][r.p()];
	
	(*this)[i][r.p()] =  tmp*r.cos() + (*this)[i][r.q()]*r.sin();
	(*this)[i][r.q()] = -tmp*r.sin() + (*this)[i][r.q()]*r.cos();
    }
    return *this;
}

//! この行列の2乗ノルムの2乗を返す．
/*!
    \return	行列の2乗ノルムの2乗，すなわち\f$\TUnorm{\TUvec{A}{}}^2\f$
*/
template <class T, class B, class R> T
Matrix<T, B, R>::square() const
{
    T	val = 0.0;
    for (u_int i = 0; i < nrow(); ++i)
	val += (*this)[i] * (*this)[i];
    return val;
}

//! この行列の2乗ノルムを返す．
/*!
  \return	行列の2乗ノルム，すなわち\f$\TUnorm{\TUvec{A}{}}\f$
*/
template <class T, class B, class R> inline double
Matrix<T, B, R>::length() const
{
    return sqrt(square());
}

//! この行列の下半三角部分を上半三角部分にコピーして対称化する．
/*!
    \return	この行列
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::symmetrize()
{
    for (u_int i = 0; i < nrow(); ++i)
	for (u_int j = 0; j < i; ++j)
	    (*this)[j][i] = (*this)[i][j];
    return *this;
}

//! この行列の下半三角部分の符号を反転し，上半三角部分にコピーして反対称化する．
/*!
    \return	この行列
*/
template <class T, class B, class R> Matrix<T, B, R>&
Matrix<T, B, R>::antisymmetrize()
{
    for (u_int i = 0; i < nrow(); ++i)
    {
	(*this)[i][i] = 0.0;
	for (u_int j = 0; j < i; ++j)
	    (*this)[j][i] = -(*this)[i][j];
    }
    return *this;
}

//! この3次元回転行列から各軸周りの回転角を取り出す．
/*!
  この行列を\f$\TUtvec{R}{}\f$とすると，
  \f[
    \TUvec{R}{} =
    \TUbeginarray{ccc}
      \cos\theta_z & -\sin\theta_z & \\
      \sin\theta_z &  \cos\theta_z & \\
      & & 1
    \TUendarray
    \TUbeginarray{ccc}
       \cos\theta_y & & \sin\theta_y \\
       & 1 & \\
      -\sin\theta_y & & \cos\theta_y
    \TUendarray
    \TUbeginarray{ccc}
      1 & & \\
      & \cos\theta_x & -\sin\theta_x \\
      & \sin\theta_x &  \cos\theta_x
    \TUendarray
  \f]
  なる\f$\theta_x, \theta_y, \theta_z\f$が回転角となる．
 \param theta_x	x軸周りの回転角(\f$ -\pi \le \theta_x \le \pi\f$)を返す．
 \param theta_y	y軸周りの回転角
	(\f$ -\frac{\pi}{2} \le \theta_y \le \frac{\pi}{2}\f$)を返す．
 \param theta_z	z軸周りの回転角(\f$ -\pi \le \theta_z \le \pi\f$)を返す．
 \throw invalid_argument	3次元正方行列でない場合に送出
*/
template <class T, class B, class R> void
Matrix<T, B, R>::rot2angle(T& theta_x, T& theta_y, T& theta_z) const
{
    using namespace	std;
    
    if (nrow() != 3 || ncol() != 3)
	throw invalid_argument("TU::Matrix<T>::rot2angle: input matrix must be 3x3!!");

    if ((*this)[0][0] == 0.0 && (*this)[0][1] == 0.0)
    {
	theta_x = atan2(-(*this)[2][1], (*this)[1][1]);
	theta_y = ((*this)[0][2] < 0.0 ? M_PI / 2.0 : -M_PI / 2.0);
	theta_z = 0.0;
    }
    else
    {
	theta_x = atan2((*this)[1][2], (*this)[2][2]);
	theta_y = -asin((*this)[0][2]);
	theta_z = atan2((*this)[0][1], (*this)[0][0]);
    }
}

//! この3次元回転行列から回転角と回転軸を取り出す．
/*!
  この行列を\f$\TUtvec{R}{}\f$とすると，
  \f[
    \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
    + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
    - \TUskew{n}{}\sin\theta
  \f]
  なる\f$\theta\f$と\f$\TUvec{n}{}\f$がそれぞれ回転角と回転軸となる．
 \param c	回転角のcos値，すなわち\f$\cos\theta\f$を返す．
 \param s	回転角のsin値，すなわち\f$\sin\theta\f$を返す．
 \return	回転軸を表す3次元単位ベクトル，すなわち\f$\TUvec{n}{}\f$
 \throw std::invalid_argument	3x3行列でない場合に送出
*/
template <class T, class B, class R> Vector<T, FixedSizedBuf<T, 3> >
Matrix<T, B, R>::rot2axis(T& c, T& s) const
{
    if (nrow() != 3 || ncol() != 3)
	throw std::invalid_argument("TU::Matrix<T>::rot2axis: input matrix must be 3x3!!");

  // Compute cosine and sine of rotation angle.
    const T	trace = (*this)[0][0] + (*this)[1][1] + (*this)[2][2];
    c = (trace - 1.0) / 2.0;
    s = sqrt((trace + 1.0)*(3.0 - trace)) / 2.0;

  // Compute rotation axis.
    Vector<T, FixedSizedBuf<T, 3> >	n;
    n[0] = (*this)[1][2] - (*this)[2][1];
    n[1] = (*this)[2][0] - (*this)[0][2];
    n[2] = (*this)[0][1] - (*this)[1][0];
    n.normalize();

    return n;
}

//! この3次元回転行列から回転角と回転軸を取り出す．
/*!
  この行列を\f$\TUtvec{R}{}\f$とすると，
  \f[
    \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
    + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
    - \TUskew{n}{}\sin\theta
  \f]
  なる\f$\theta\f$と\f$\TUvec{n}{}\f$がそれぞれ回転角と回転軸となる．
 \return			回転角と回転軸を表す3次元ベクトル，すなわち
				\f$\theta\TUvec{n}{}\f$
 \throw invalid_argument	3x3行列でない場合に送出
*/
template <class T, class B, class R> Vector<T, FixedSizedBuf<T, 3u> >
Matrix<T, B, R>::rot2axis() const
{
    if (nrow() != 3 || ncol() != 3)
	throw std::invalid_argument("TU::Matrix<T>::rot2axis: input matrix must be 3x3!!");

    Vector<T, FixedSizedBuf<T, 3u> >	axis;
    axis[0] = ((*this)[1][2] - (*this)[2][1]) * 0.5;
    axis[1] = ((*this)[2][0] - (*this)[0][2]) * 0.5;
    axis[2] = ((*this)[0][1] - (*this)[1][0]) * 0.5;
    const T	s = sqrt(axis.square());
    if (s + 1.0 == 1.0)		// s << 1 ?
	return axis;
    const T	trace = (*this)[0][0] + (*this)[1][1] + (*this)[2][2];
    if (trace > 1.0)		// cos > 0 ?
	return  axis *= ( asin(s) / s);
    else
	return  axis *= (-asin(s) / s);
}

//! 単位正方行列を生成する．
/*!
  \param d	単位正方行列の次元
  \return	単位正方行列
*/
template <class T, class B, class R> inline Matrix<T, B, R>
Matrix<T, B, R>::I(u_int d)
{
    return Matrix<T, B, R>(d, d).diag(1.0);
}

//! 3次元回転行列を生成する．
/*!
  \param n	回転軸を表す3次元単位ベクトル
  \param c	回転角のcos値
  \param s	回転角のsin値
  \return	生成された回転行列，すなわち
		\f[
		  \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
		  + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
		  - \TUskew{n}{}\sin\theta
		\f]
*/
template <class T, class B, class R> template <class T2, class B2>
Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3> >
Matrix<T, B, R>::Rt(const Vector<T2, B2>& n, T c, T s)
{
    if (n.dim() != 3)
	throw std::invalid_argument("TU::Matrix<T, B, R>::Rt: dimension of the argument \'n\' must be 3");
    Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3 > > Qt = n % n;
    Qt *= (1.0 - c);
    Qt[0][0] += c;
    Qt[1][1] += c;
    Qt[2][2] += c;
    Qt[0][1] += n[2] * s;
    Qt[0][2] -= n[1] * s;
    Qt[1][0] -= n[2] * s;
    Qt[1][2] += n[0] * s;
    Qt[2][0] += n[1] * s;
    Qt[2][1] -= n[0] * s;

    return Qt;
}

//! 3次元回転行列を生成する．
/*!
  \param axis	回転角と回転軸を表す3次元ベクトル
  \return	生成された回転行列，すなわち
		\f[
		  \TUtvec{R}{} \equiv \TUvec{I}{3}\cos\theta
		  + \TUvec{n}{}\TUtvec{n}{}(1 - \cos\theta)
		  - \TUskew{n}{}\sin\theta,{\hskip 1em}\mbox{where}{\hskip 0.5em}
		  \theta = \TUnorm{\TUvec{a}{}},~
		  \TUvec{n}{} = \frac{\TUvec{a}{}}{\TUnorm{\TUvec{a}{}}}
		\f]
*/
template <class T, class B, class R> template <class T2, class B2>
Matrix<T, FixedSizedBuf<T, 9>, FixedSizedBuf<Vector<T>, 3> >
Matrix<T, B, R>::Rt(const Vector<T2, B2>& axis)
{
    const T	theta = axis.length();
    if (theta + 1.0 == 1.0)		// theta << 1 ?
	return I(3);
    else
    {
	T	c = cos(theta), s = sin(theta);
	return Rt(axis / theta, c, s);
    }
}

//! 行列のサイズを変更し，0に初期化する．
/*!
  \param r	新しい行数
  \param c	新しい列数
*/
template <class T, class B, class R> inline void
Matrix<T, B, R>::resize(u_int r, u_int c)
{
    Array2<Vector<T>, B, R>::resize(r, c);
    *this = 0;
}

//! 行列の内部記憶領域とサイズを変更する．
/*!
  \param p	新しい内部記憶領域へのポインタ
  \param r	新しい行数
  \param c	新しい列数
*/
template <class T, class B, class R> inline void
Matrix<T, B, R>::resize(T* p, u_int r, u_int c)
{
    Array2<Vector<T>, B, R>::resize(p, r, c);
}

/************************************************************************
*  numerical operators							*
************************************************************************/
//! 2つのベクトルの足し算
/*!
  \param v	第1引数
  \param w	第2引数
  \return	結果を格納したベクトル，すなわち\f$\TUvec{v}{}+\TUvec{w}{}\f$
*/
template <class T1, class B1, class T2, class B2> inline Vector<T1, B1>
operator +(const Vector<T1, B1>& v, const Vector<T2, B2>& w)
{
    return Vector<T1, B1>(v) += w;
}

//! 2つのベクトルの引き算
/*!
  \param v	第1引数
  \param w	第2引数
  \return	結果を格納したベクトル，すなわち\f$\TUvec{v}{}-\TUvec{w}{}\f$
*/
template <class T1, class B1, class T2, class B2> inline Vector<T1, B1>
operator -(const Vector<T1, B1>& v, const Vector<T2, B2>& w)
{
    return Vector<T1, B1>(v) -= w;
}

//! ベクトルに定数を掛ける．
/*!
  \param c	掛ける定数
  \param v	ベクトル
  \return	結果を格納したベクトル，すなわち\f$c\TUvec{v}{}\f$
*/
template <class T, class B> inline Vector<T, B>
operator *(double c, const Vector<T, B>& v)
{
    return Vector<T, B>(v) *= c;
}

//! ベクトルに定数を掛ける．
/*!
  \param v	ベクトル
  \param c	掛ける定数
  \return	結果を格納したベクトル，すなわち\f$c\TUvec{v}{}\f$
*/
template <class T, class B> inline Vector<T, B>
operator *(const Vector<T, B>& v, double c)
{
    return Vector<T, B>(v) *= c;
}

//! ベクトルの各要素を定数で割る．
/*!
  \param v	ベクトル
  \param c	割る定数
  \return	結果を格納したベクトル，すなわち\f$\frac{1}{c}\TUvec{v}{}\f$
*/
template <class T, class B> inline Vector<T, B>
operator /(const Vector<T, B>& v, double c)
{
    return Vector<T, B>(v) /= c;
}

//! 2つの行列の足し算
/*!
  \param m	第1引数
  \param n	第2引数
  \return	結果を格納した行列，すなわち\f$\TUvec{M}{}+\TUvec{N}{}\f$
*/
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline Matrix<T1, B1, R1>
operator +(const Matrix<T1, B1, R1>& m, const Matrix<T2, B2, R2>& n)
{
    return Matrix<T1, B1, R1>(m) += n;
}

//! 2つの行列の引き算
/*!
  \param m	第1引数
  \param n	第2引数
  \return	結果を格納した行列，すなわち\f$\TUvec{M}{}-\TUvec{N}{}\f$
*/
template <class T1, class B1, class R1, class T2, class B2, class R2>
inline Matrix<T1, B1, R1>
operator -(const Matrix<T1, B1, R1>& m, const Matrix<T2, B2, R2>& n)
{
    return Matrix<T1, B1, R1>(m) -= n;
}

//! 行列に定数を掛ける．
/*!
  \param c	掛ける定数
  \param m	行列
  \return	結果を格納した行列，すなわち\f$c\TUvec{M}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>
operator *(double c, const Matrix<T, B, R>& m)
{
    return Matrix<T, B, R>(m) *= c;
}

//! 行列に定数を掛ける．
/*!
  \param m	行列
  \param c	掛ける定数
  \return	結果を格納した行列，すなわち\f$c\TUvec{M}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>
operator *(const Matrix<T, B, R>& m, double c)
{
    return Matrix<T, B, R>(m) *= c;
}

//! 行列の各要素を定数で割る．
/*!
  \param m	行列
  \param c	割る定数
  \return	結果を格納した行列，すなわち\f$\frac{1}{c}\TUvec{M}{}\f$
*/
template <class T, class B, class R> inline Matrix<T, B, R>
operator /(const Matrix<T, B, R>& m, double c)
{
    return Matrix<T, B, R>(m) /= c;
}

//! 2つの3次元ベクトルのベクトル積
/*!
  \param v	第1引数
  \param w	第2引数
  \return	ベクトル積，すなわち\f$\TUvec{v}{}\times\TUvec{w}{}\f$
*/
template <class T1, class B1, class T2, class B2>
inline Vector<T1, FixedSizedBuf<T1, 3> >
operator ^(const Vector<T1, B1>& v, const Vector<T2, B2>& w)
{
    return Vector<T1, FixedSizedBuf<T1, 3> >(v) ^= w;
}

//! 2つのベクトルの内積
/*!
  \param v	第1引数
  \param w	第2引数
  \return	内積，すなわち\f$\TUtvec{v}{}\TUvec{w}{}\f$
*/
template <class T1, class B1, class T2, class B2> T1
operator *(const Vector<T1, B1>& v, const Vector<T2, B2>& w)
{
    v.check_dim(w.dim());
    T1	val = 0;
    for (u_int i = 0; i < v.dim(); ++i)
	val += v[i] * w[i];
    return val;
}

//! ベクトルと行列の積
/*!
  \param v	ベクトル
  \param m	行列
  \return	結果のベクトル，すなわち\f$\TUtvec{v}{}\TUvec{M}{}\f$
*/
template <class T1, class B1, class T2, class B2, class R2> Vector<T1>
operator *(const Vector<T1, B1>& v, const Matrix<T2, B2, R2>& m)
{
    v.check_dim(m.nrow());
    Vector<T1> val(m.ncol());
    for (u_int j = 0; j < m.ncol(); ++j)
	for (u_int i = 0; i < m.nrow(); ++i)
	    val[j] += v[i] * m[i][j];
    return val;
}

//! 2つのベクトルの外積
/*!
  \param v	第1引数
  \param w	第2引数
  \return	結果の行列，すなわち\f$\TUvec{v}{}\TUtvec{w}{}\f$
*/
template <class T1, class B1, class T2, class B2> Matrix<T1>
operator %(const Vector<T1, B1>& v, const Vector<T2, B2>& w)
{
    Matrix<T1>	val(v.dim(), w.dim());
    for (u_int i = 0; i < v.dim(); ++i)
	for (u_int j = 0; j < w.dim(); ++j)
	    val[i][j] = v[i] * w[j];
    return val;
}

//! 3次元ベクトルと3x?行列の各列とのベクトル積
/*!
  \param v			3次元ベクトル
  \param m			3x?行列
  \return			結果の3x?行列，すなわち
				\f$\TUvec{v}{}\times\TUvec{M}{}\f$
  \throw std::invalid_argument	vが3次元ベクトルでないかmが3x?行列でない場合に
				送出
*/
template <class T1, class B1, class T2, class B2, class R2> Matrix<T2, B2, R2>
operator ^(const Vector<T1, B1>& v, const Matrix<T2, B2, R2>& m)
{
    v.check_dim(m.nrow());
    if (v.dim() != 3)
	throw std::invalid_argument("operator ^(const Vecotr<T>&, const Matrix<T, B, R>&): dimension of vector must be 3!!");
    Matrix<T2, B2, R2>	val(m.nrow(), m.ncol());
    for (u_int j = 0; j < val.ncol(); ++j)
    {
	val[0][j] = v[1] * m[2][j] - v[2] * m[1][j];
	val[1][j] = v[2] * m[0][j] - v[0] * m[2][j];
	val[2][j] = v[0] * m[1][j] - v[1] * m[0][j];
    }
    return val;
}

//! 2つの行列の積
/*!
  \param m	第1引数
  \param n	第2引数
  \return	結果の行列，すなわち\f$\TUvec{M}{}\TUvec{N}{}\f$
*/
template <class T1, class B1, class R1, class T2, class B2, class R2>
Matrix<T1>
operator *(const Matrix<T1, B1, R1>& m, const Matrix<T2, B2, R2>& n)
{
    n.check_dim(m.ncol());
    Matrix<T1>	val(m.nrow(), n.ncol());
    for (u_int i = 0; i < m.nrow(); ++i)
	for (u_int j = 0; j < n.ncol(); ++j)
	    for (u_int k = 0; k < m.ncol(); ++k)
		val[i][j] += m[i][k] * n[k][j];
    return val;
}

//! 行列とベクトルの積
/*!
  \param m	行列
  \param v	ベクトル
  \return	結果のベクトル，すなわち\f$\TUvec{M}{}\TUvec{v}{}\f$
*/
template <class T1, class B1, class R1, class T2, class B2> Vector<T1>
operator *(const Matrix<T1, B1, R1>& m, const Vector<T2, B2>& v)
{
    Vector<T1>	val(m.nrow());
    for (u_int i = 0; i < m.nrow(); ++i)
	val[i] = m[i] * v;
    return val;
}

//! ?x3行列の各行と3次元ベクトルのベクトル積
/*!
  \param m	?x3行列
  \param v	3次元ベクトル
  \return	結果の行列，すなわち\f$(\TUtvec{M}{}\times\TUvec{v}{})^\top\f$
*/
template <class T1, class B1, class R1, class T2, class B2>
inline Matrix<T1, B1, R1>
operator ^(const Matrix<T1, B1, R1>& m, const Vector<T2, B2>& v)
{
    return Matrix<T1, B1, R1>(m) ^= v;
}

/************************************************************************
*  class LUDecomposition<T>						*
************************************************************************/
//! 正方行列のLU分解を表すクラス
template <class T>
class LUDecomposition : private Array2<Vector<T> >
{
  public:
    template <class T2, class B2, class R2>
    LUDecomposition(const Matrix<T2, B2, R2>&)		;

    template <class T2, class B2>
    void	substitute(Vector<T2, B2>&)	const	;

  //! もとの正方行列の行列式を返す．
  /*!
    \return	もとの正方行列の行列式
  */
    T		det()				const	{return _det;}
    
  private:
    using	Array2<Vector<T> >::nrow;
    using	Array2<Vector<T> >::ncol;
    
    Array<int>	_index;
    T		_det;
};

//! 与えられた正方行列のLU分解を生成する．
/*!
 \param m			LU分解する正方行列
 \throw std::invalid_argument	mが正方行列でない場合に送出
*/
template <class T> template <class T2, class B2, class R2>
LUDecomposition<T>::LUDecomposition(const Matrix<T2, B2, R2>& m)
    :Array2<Vector<T> >(m), _index(ncol()), _det(1.0)
{
    using namespace	std;
    
    if (nrow() != ncol())
        throw invalid_argument("TU::LUDecomposition<T>::LUDecomposition: not square matrix!!");

    for (u_int j = 0; j < ncol(); ++j)	// initialize column index
	_index[j] = j;			// for explicit pivotting

    Vector<T>	scale(ncol());
    for (u_int j = 0; j < ncol(); ++j)	// find maximum abs. value in each col.
    {					// for implicit pivotting
	T max = 0.0;

	for (u_int i = 0; i < nrow(); ++i)
	{
	    const T tmp = fabs((*this)[i][j]);
	    if (tmp > max)
		max = tmp;
	}
	scale[j] = (max != 0.0 ? 1.0 / max : 1.0);
    }

    for (u_int i = 0; i < nrow(); ++i)
    {
	for (u_int j = 0; j < i; ++j)		// left part (j < i)
	{
	    T& sum = (*this)[i][j];
	    for (u_int k = 0; k < j; ++k)
		sum -= (*this)[i][k] * (*this)[k][j];
	}

	u_int	jmax;
	T	max = 0.0;
	for (u_int j = i; j < ncol(); ++j)  // diagonal and right part (i <= j)
	{
	    T& sum = (*this)[i][j];
	    for (u_int k = 0; k < i; ++k)
		sum -= (*this)[i][k] * (*this)[k][j];
	    const T tmp = fabs(sum) * scale[j];
	    if (tmp >= max)
	    {
		max  = tmp;
		jmax = j;
	    }
	}
	if (jmax != i)			// pivotting required ?
	{
	    for (u_int k = 0; k < nrow(); ++k)	// swap i-th and jmax-th column
		swap((*this)[k][i], (*this)[k][jmax]);
	    swap(_index[i], _index[jmax]);	// swap column index
	    swap(scale[i], scale[jmax]);	// swap colum-wise scale factor
	    _det = -_det;
	}

	_det *= (*this)[i][i];

	if ((*this)[i][i] == 0.0)	// singular matrix ?
	    break;

	for (u_int j = i + 1; j < nrow(); ++j)
	    (*this)[i][j] /= (*this)[i][i];
    }
}

//! もとの正方行列を係数行列とした連立1次方程式を解く．
/*!
  \param b			もとの正方行列\f$\TUvec{M}{}\f$と同じ次
				元を持つベクトル．\f$\TUtvec{b}{} =
				\TUtvec{x}{}\TUvec{M}{}\f$の解に変換さ
				れる．
  \throw std::invalid_argument	ベクトルbの次元がもとの正方行列の次元に一致
				しない場合に送出
  \throw std::runtime_error	もとの正方行列が正則でない場合に送出
*/
template <class T> template <class T2, class B2> void
LUDecomposition<T>::substitute(Vector<T2, B2>& b) const
{
    if (b.dim() != ncol())
	throw std::invalid_argument("TU::LUDecomposition<T>::substitute: Dimension of given vector is not equal to mine!!");
    
    Vector<T2, B2>	tmp(b);
    for (u_int j = 0; j < b.dim(); ++j)
	b[j] = tmp[_index[j]];

    for (u_int j = 0; j < b.dim(); ++j)		// forward substitution
	for (u_int i = 0; i < j; ++i)
	    b[j] -= b[i] * (*this)[i][j];
    for (u_int j = b.dim(); j-- > 0; )		// backward substitution
    {
	for (u_int i = b.dim(); --i > j; )
	    b[j] -= b[i] * (*this)[i][j];
	if ((*this)[j][j] == 0.0)		// singular matrix ?
	    throw std::runtime_error("TU::LUDecomposition<T>::substitute: singular matrix !!");
	b[j] /= (*this)[j][j];
    }
}

//! 連立1次方程式を解く．
/*!
  \param m	正則な正方行列
  \return	\f$\TUtvec{u}{} = \TUtvec{x}{}\TUvec{M}{}\f$
		の解を納めたこのベクトル，すなわち
		\f$\TUtvec{u}{} \leftarrow \TUtvec{u}{}\TUinv{M}{}\f$
*/
template <class T, class B> template <class T2, class B2, class R2>
inline Vector<T, B>&
Vector<T, B>::solve(const Matrix<T2, B2, R2>& m)
{
    LUDecomposition<T2>(m).substitute(*this);
    return *this;
}

//! 連立1次方程式を解く．
/*!
  \param m	正則な正方行列
  \return	\f$\TUvec{A}{} = \TUvec{X}{}\TUvec{M}{}\f$
		の解を納めたこの行列，すなわち
		\f$\TUvec{A}{} \leftarrow \TUvec{A}{}\TUinv{M}{}\f$
*/
template <class T, class B, class R> template <class T2, class B2, class R2>
Matrix<T, B, R>&
Matrix<T, B, R>::solve(const Matrix<T2, B2, R2>& m)
{
    LUDecomposition<T2>	lu(m);
    
    for (u_int i = 0; i < nrow(); ++i)
	lu.substitute((*this)[i]);
    return *this;
}

//! この行列の行列式を返す．
/*!
  \return	行列式，すなわち\f$\det\TUvec{A}{}\f$
*/
template <class T, class B, class R> inline T
Matrix<T, B, R>::det() const
{
    return LUDecomposition<T>(*this).det();
}

/************************************************************************
*  class Householder<T>							*
************************************************************************/
template <class T>	class QRDecomposition;
template <class T>	class BiDiagonal;

//! Householder変換を表すクラス
template <class T>
class Householder : public Matrix<T>
{
  private:
    Householder(u_int dd, u_int d)
	:Matrix<T>(dd, dd), _d(d), _sigma(Matrix<T>::nrow())	{}
    template <class T2, class B2, class R2>
    Householder(const Matrix<T2, B2, R2>& a, u_int d)		;

    using		Matrix<T>::dim;
    
    void		apply_from_left(Matrix<T>& a, u_int m)	;
    void		apply_from_right(Matrix<T>& a, u_int m)	;
    void		apply_from_both(Matrix<T>& a, u_int m)	;
    void		make_transformation()			;
    const Vector<T>&	sigma()				const	{return _sigma;}
    Vector<T>&		sigma()					{return _sigma;}
    bool		sigma_is_zero(u_int m, T comp)	const	;

  private:
    const u_int		_d;		// deviation from diagonal element
    Vector<T>		_sigma;

    friend class	QRDecomposition<T>;
    friend class	TriDiagonal<T>;
    friend class	BiDiagonal<T>;
};

template <class T> template <class T2, class B2, class R2>
Householder<T>::Householder(const Matrix<T2, B2, R2>& a, u_int d)
    :Matrix<T>(a), _d(d), _sigma(dim())
{
    if (a.nrow() != a.ncol())
	throw std::invalid_argument("TU::Householder<T>::Householder: Given matrix must be square !!");
}

template <class T> void
Householder<T>::apply_from_left(Matrix<T>& a, u_int m)
{
    if (a.nrow() < dim())
	throw std::invalid_argument("TU::Householder<T>::apply_from_left: # of rows of given matrix is smaller than my dimension !!");
    
    T	scale = 0.0;
    for (u_int i = m+_d; i < dim(); ++i)
	scale += fabs(a[i][m]);
	
    if (scale != 0.0)
    {
	T	h = 0.0;
	for (u_int i = m+_d; i < dim(); ++i)
	{
	    a[i][m] /= scale;
	    h += a[i][m] * a[i][m];
	}

	const T	s = (a[m+_d][m] > 0.0 ? sqrt(h) : -sqrt(h));
	h	     += s * a[m+_d][m];			// H = u^2 / 2
	a[m+_d][m]   += s;				// m-th col <== u
	    
	for (u_int j = m+1; j < a.ncol(); ++j)
	{
	    T	p = 0.0;
	    for (u_int i = m+_d; i < dim(); ++i)
		p += a[i][m] * a[i][j];
	    p /= h;					// p[j] (p' = u'A / H)
	    for (u_int i = m+_d; i < dim(); ++i)
		a[i][j] -= a[i][m] * p;			// A = A - u*p'
	    a[m+_d][j] = -a[m+_d][j];
	}
	    
	for (u_int i = m+_d; i < dim(); ++i)
	    (*this)[m][i] = scale * a[i][m];		// copy u
	_sigma[m+_d] = scale * s;
    }
}

template <class T> void
Householder<T>::apply_from_right(Matrix<T>& a, u_int m)
{
    if (a.ncol() < dim())
	throw std::invalid_argument("Householder<T>::apply_from_right: # of column of given matrix is smaller than my dimension !!");
    
    T	scale = 0.0;
    for (u_int j = m+_d; j < dim(); ++j)
	scale += fabs(a[m][j]);
	
    if (scale != 0.0)
    {
	T	h = 0.0;
	for (u_int j = m+_d; j < dim(); ++j)
	{
	    a[m][j] /= scale;
	    h += a[m][j] * a[m][j];
	}

	const T	s = (a[m][m+_d] > 0.0 ? sqrt(h) : -sqrt(h));
	h	     += s * a[m][m+_d];			// H = u^2 / 2
	a[m][m+_d]   += s;				// m-th row <== u

	for (u_int i = m+1; i < a.nrow(); ++i)
	{
	    T	p = 0.0;
	    for (u_int j = m+_d; j < dim(); ++j)
		p += a[i][j] * a[m][j];
	    p /= h;					// p[i] (p = Au / H)
	    for (u_int j = m+_d; j < dim(); ++j)
		a[i][j] -= p * a[m][j];			// A = A - p*u'
	    a[i][m+_d] = -a[i][m+_d];
	}
	    
	for (u_int j = m+_d; j < dim(); ++j)
	    (*this)[m][j] = scale * a[m][j];		// copy u
	_sigma[m+_d] = scale * s;
    }
}

template <class T> void
Householder<T>::apply_from_both(Matrix<T>& a, u_int m)
{
    Vector<T>		u = a[m](m+_d, a.ncol()-m-_d);
    T		scale = 0.0;
    for (u_int j = 0; j < u.dim(); ++j)
	scale += fabs(u[j]);
	
    if (scale != 0.0)
    {
	u /= scale;

	T		h = u * u;
	const T	s = (u[0] > 0.0 ? sqrt(h) : -sqrt(h));
	h	     += s * u[0];			// H = u^2 / 2
	u[0]	     += s;				// m-th row <== u

	Matrix<T>	A = a(m+_d, m+_d, a.nrow()-m-_d, a.ncol()-m-_d);
	Vector<T>	p = _sigma(m+_d, dim()-m-_d);
	for (u_int i = 0; i < A.nrow(); ++i)
	    p[i] = (A[i] * u) / h;			// p = Au / H

	const T	k = (u * p) / (h + h);		// K = u*p / 2H
	for (u_int i = 0; i < A.nrow(); ++i)
	{				// m-th col of 'a' is used as 'q'
	    a[m+_d+i][m] = p[i] - k * u[i];		// q = p - Ku
	    for (u_int j = 0; j <= i; ++j)		// A = A - uq' - qu'
		A[j][i] = (A[i][j] -= (u[i]*a[m+_d+j][m] + a[m+_d+i][m]*u[j]));
	}
	for (u_int j = 1; j < A.nrow(); ++j)
	    A[j][0] = A[0][j] = -A[0][j];

	for (u_int j = m+_d; j < a.ncol(); ++j)
	    (*this)[m][j] = scale * a[m][j];		// copy u
	_sigma[m+_d] = scale * s;
    }
}

template <class T> void
Householder<T>::make_transformation()
{
    for (u_int m = dim(); m-- > 0; )
    {
	for (u_int i = m+1; i < dim(); ++i)
	    (*this)[i][m] = 0.0;

	if (_sigma[m] != 0.0)
	{
	    for (u_int i = m+1; i < dim(); ++i)
	    {
		T	g = 0.0;
		for (u_int j = m+1; j < dim(); ++j)
		    g += (*this)[i][j] * (*this)[m-_d][j];
		g /= (_sigma[m] * (*this)[m-_d][m]);	// g[i] (g = Uu / H)
		for (u_int j = m; j < dim(); ++j)
		    (*this)[i][j] -= g * (*this)[m-_d][j];	// U = U - gu'
	    }
	    for (u_int j = m; j < dim(); ++j)
		(*this)[m][j] = (*this)[m-_d][j] / _sigma[m];
	    (*this)[m][m] -= 1.0;
	}
	else
	{
	    for (u_int j = m+1; j < dim(); ++j)
		(*this)[m][j] = 0.0;
	    (*this)[m][m] = 1.0;
	}
    }
}

template <class T> bool
Householder<T>::sigma_is_zero(u_int m, T comp) const
{
    return (T(fabs(_sigma[m])) + comp == comp);
}

/************************************************************************
*  class QRDecomposition<T>						*
************************************************************************/
//! 一般行列のQR分解を表すクラス
/*!
  与えられた行列\f$\TUvec{A}{} \in \TUspace{R}{m\times n}\f$に対して
  \f$\TUvec{A}{} = \TUtvec{R}{}\TUtvec{Q}{}\f$なる下半三角行列
  \f$\TUtvec{R}{} \in \TUspace{R}{m\times n}\f$と回転行列
  \f$\TUtvec{Q}{} \in \TUspace{R}{n\times n}\f$を求める
  （\f$\TUvec{A}{}\f$の各行を\f$\TUtvec{Q}{}\f$の行の線型結合で表現す
  る）．
 */
template <class T>
class QRDecomposition : private Matrix<T>
{
  public:
    template <class T2, class B2, class R2>
    QRDecomposition(const Matrix<T2, B2, R2>& m)	;

  //! QR分解の下半三角行列を返す．
  /*!
    \return	下半三角行列\f$\TUtvec{R}{}\f$
  */
    const Matrix<T>&	Rt()			const	{return *this;}

  //! QR分解の回転行列を返す．
  /*!
    \return	回転行列\f$\TUtvec{Q}{}\f$
  */
    const Matrix<T>&	Qt()			const	{return _Qt;}
    
  private:
    using		Matrix<T>::nrow;
    using		Matrix<T>::ncol;
    
    Householder<T>	_Qt;			// rotation matrix
};

//! 与えられた一般行列のQR分解を生成する．
/*!
 \param m	QR分解する一般行列
*/
template <class T> template <class T2, class B2, class R2>
QRDecomposition<T>::QRDecomposition(const Matrix<T2, B2, R2>& m)
    :Matrix<T>(m), _Qt(m.ncol(), 0)
{
    u_int	n = std::min(nrow(), ncol());
    for (u_int j = 0; j < n; ++j)
	_Qt.apply_from_right(*this, j);
    _Qt.make_transformation();
    for (u_int i = 0; i < n; ++i)
    {
	(*this)[i][i] = _Qt.sigma()[i];
	for (u_int j = i + 1; j < ncol(); ++j)
	    (*this)[i][j] = 0.0;
    }
}

/************************************************************************
*  class TriDiagonal<T>							*
************************************************************************/
//! 対称行列の3重対角化を表すクラス
/*!
  与えられた対称行列\f$\TUvec{A}{} \in \TUspace{R}{d\times d}\f$に対し
  て\f$\TUtvec{U}{}\TUvec{A}{}\TUvec{U}{}\f$が3重対角行列となるような回
  転行列\f$\TUtvec{U}{} \in \TUspace{R}{d\times d}\f$を求める．
 */
template <class T>
class TriDiagonal
{
  public:
    template <class T2, class B2, class R2>
    TriDiagonal(const Matrix<T2, B2, R2>& a)		;

  //! 3重対角化される対称行列の次元(= 行数 = 列数)を返す．
  /*!
    \return	対称行列の次元
  */
    u_int		dim()			const	{return _Ut.nrow();}

  //! 3重対角化を行う回転行列を返す．
  /*!
    \return	回転行列
  */
    const Matrix<T>&	Ut()			const	{return _Ut;}

  //! 3重対角行列の対角成分を返す．
  /*!
    \return	対角成分
  */
    const Vector<T>&	diagonal()		const	{return _diagonal;}

  //! 3重対角行列の非対角成分を返す．
  /*!
    \return	非対角成分
  */
    const Vector<T>&	off_diagonal()		const	{return _Ut.sigma();}

    void		diagonalize(bool abs=true)	;
    
  private:
    enum		{NITER_MAX = 30};

    bool		off_diagonal_is_zero(u_int n)		const	;
    void		initialize_rotation(u_int m, u_int n,
					    T& x, T& y)		const	;
    
    Householder<T>	_Ut;
    Vector<T>		_diagonal;
    Vector<T>&		_off_diagonal;
};

//! 与えられた対称行列を3重対角化する．
/*!
  \param a			3重対角化する対称行列
  \throw std::invalid_argument	aが正方行列でない場合に送出
*/
template <class T> template <class T2, class B2, class R2>
TriDiagonal<T>::TriDiagonal(const Matrix<T2, B2, R2>& a)
    :_Ut(a, 1), _diagonal(_Ut.nrow()), _off_diagonal(_Ut.sigma())
{
    if (_Ut.nrow() != _Ut.ncol())
        throw std::invalid_argument("TU::TriDiagonal<T>::TriDiagonal: not square matrix!!");

    for (u_int m = 0; m < dim(); ++m)
    {
	_Ut.apply_from_both(_Ut, m);
	_diagonal[m] = _Ut[m][m];
    }

    _Ut.make_transformation();
}

//! 3重対角行列を対角化する（固有値，固有ベクトルの計算）．
/*!
  対角成分は固有値となり，\f$\TUtvec{U}{}\f$の各行は固有ベクトルを与える．
  \throw std::runtime_error	指定した繰り返し回数を越えた場合に送出
  \param abs	固有値をその絶対値の大きい順に並べるのであればtrue,
		その値の大きい順に並べるのであればfalse
*/ 
template <class T> void
TriDiagonal<T>::diagonalize(bool abs)
{
    using namespace	std;
    
    for (u_int n = dim(); n-- > 0; )
    {
	int	niter = 0;
	
#ifdef TUVectorPP_DEBUG
	cerr << "******** n = " << n << " ********" << endl;
#endif
	while (!off_diagonal_is_zero(n))
	{					// n > 0 here
	    if (niter++ > NITER_MAX)
		throw runtime_error("TU::TriDiagonal::diagonalize(): Number of iteration exceeded maximum value!!");

	  /* Find first m (< n) whose off-diagonal element is 0 */
	    u_int	m = n;
	    while (!off_diagonal_is_zero(--m))	// 0 <= m < n < dim() here
	    {
	    }

	  /* Set x and y which determine initial(i = m+1) plane rotation */
	    T	x, y;
	    initialize_rotation(m, n, x, y);
	  /* Apply rotation P(i-1, i) for each i (i = m+1, n+2, ... , n) */
	    for (u_int i = m; ++i <= n; )
	    {
		Rotation	rot(i-1, i, x, y);
		
		_Ut.rotate_from_left(rot);

		if (i > m+1)
		    _off_diagonal[i-1] = rot.length();
		const T w = _diagonal[i] - _diagonal[i-1];
		const T d = rot.sin()*(rot.sin()*w
			       + 2.0*rot.cos()*_off_diagonal[i]);
		_diagonal[i-1]	 += d;
		_diagonal[i]	 -= d;
		_off_diagonal[i] += rot.sin()*(rot.cos()*w
				  - 2.0*rot.sin()*_off_diagonal[i]);
		if (i < n)
		{
		    x = _off_diagonal[i];
		    y = rot.sin()*_off_diagonal[i+1];
		    _off_diagonal[i+1] *= rot.cos();
		}
	    }
#ifdef TUVectorPP_DEBUG
	    cerr << "  niter = " << niter << ": " << off_diagonal();
#endif	    
	}
    }

  // Sort eigen values and eigen vectors.
    if (abs)
    {
	for (u_int m = 0; m < dim(); ++m)
	    for (u_int n = m+1; n < dim(); ++n)
		if (fabs(_diagonal[n]) > fabs(_diagonal[m]))	// abs. values
		{
		    swap(_diagonal[m], _diagonal[n]);
		    for (u_int j = 0; j < dim(); ++j)
		    {
			const T	tmp = _Ut[m][j];
			_Ut[m][j] = _Ut[n][j];
			_Ut[n][j] = -tmp;
		    }
		}
    }
    else
    {
	for (u_int m = 0; m < dim(); ++m)
	    for (u_int n = m+1; n < dim(); ++n)
		if (_diagonal[n] > _diagonal[m])		// raw values
		{
		    swap(_diagonal[m], _diagonal[n]);
		    for (u_int j = 0; j < dim(); ++j)
		    {
			const T	tmp = _Ut[m][j];
			_Ut[m][j] = _Ut[n][j];
			_Ut[n][j] = -tmp;
		    }
		}
    }
}

template <class T> bool
TriDiagonal<T>::off_diagonal_is_zero(u_int n) const
{
    return (n == 0 || _Ut.sigma_is_zero(n, fabs(_diagonal[n-1]) +
					   fabs(_diagonal[n])));
}

template <class T> void
TriDiagonal<T>::initialize_rotation(u_int m, u_int n, T& x, T& y) const
{
    const T	g = (_diagonal[n] - _diagonal[n-1]) /
			    (2.0*_off_diagonal[n]),
			absg = fabs(g),
			gg1 = (absg > 1.0 ?
			       absg * sqrt(1.0 + (1.0/absg)*(1.0/absg)) :
			       sqrt(1.0 + absg*absg)),
			t = (g > 0.0 ? g + gg1 : g - gg1);
    x = _diagonal[m] - _diagonal[n] - _off_diagonal[n]/t;
  //x = _diagonal[m];					// without shifting
    y = _off_diagonal[m+1];
}

/************************************************************************
*  class BiDiagonal<T>							*
************************************************************************/
//! 一般行列の2重対角化を表すクラス
/*!
  与えられた一般行列\f$\TUvec{A}{} \in \TUspace{R}{m\times n}\f$に対し
  て\f$\TUtvec{V}{}\TUvec{A}{}\TUvec{U}{}\f$が2重対角行列となるような2
  つの回転行列\f$\TUtvec{U}{} \in \TUspace{R}{n\times n}\f$,
  \f$\TUtvec{V}{} \in \TUspace{R}{m\times m}\f$を求める．\f$m \le n\f$
  の場合は下半三角な2重対角行列に，\f$m > n\f$の場合は上半三角な2重対角
  行列になる．
 */
template <class T>
class BiDiagonal
{
  public:
    template <class T2, class B2, class R2>
    BiDiagonal(const Matrix<T2, B2, R2>& a)	;

  //! 2重対角化される行列の行数を返す．
  /*!
    \return	行列の行数
  */
    u_int		nrow()		const	{return _Vt.nrow();}

  //! 2重対角化される行列の列数を返す．
  /*!
    \return	行列の列数
  */
    u_int		ncol()		const	{return _Ut.nrow();}

  //! 2重対角化を行うために右から掛ける回転行列の転置を返す．
  /*!
    \return	右から掛ける回転行列の転置
  */
    const Matrix<T>&	Ut()		const	{return _Ut;}

  //! 2重対角化を行うために左から掛ける回転行列を返す．
  /*!
    \return	左から掛ける回転行列
  */
    const Matrix<T>&	Vt()		const	{return _Vt;}

  //! 2重対角行列の対角成分を返す．
  /*!
    \return	対角成分
  */
    const Vector<T>&	diagonal()	const	{return _Dt.sigma();}

  //! 2重対角行列の非対角成分を返す．
  /*!
    \return	非対角成分
  */
    const Vector<T>&	off_diagonal()	const	{return _Et.sigma();}

    void		diagonalize()		;

  private:
    enum		{NITER_MAX = 30};
    
    bool		diagonal_is_zero(u_int n)		const	;
    bool		off_diagonal_is_zero(u_int n)		const	;
    void		initialize_rotation(u_int m, u_int n,
					    T& x, T& y)		const	;

    Householder<T>	_Dt;
    Householder<T>	_Et;
    Vector<T>&		_diagonal;
    Vector<T>&		_off_diagonal;
    T			_anorm;
    const Matrix<T>&	_Ut;
    const Matrix<T>&	_Vt;
};

//! 与えられた一般行列を2重対角化する．
/*!
  \param a	2重対角化する一般行列
*/
template <class T> template <class T2, class B2, class R2>
BiDiagonal<T>::BiDiagonal(const Matrix<T2, B2, R2>& a)
    :_Dt((a.nrow() < a.ncol() ? a.ncol() : a.nrow()), 0),
     _Et((a.nrow() < a.ncol() ? a.nrow() : a.ncol()), 1),
     _diagonal(_Dt.sigma()), _off_diagonal(_Et.sigma()), _anorm(0),
     _Ut(a.nrow() < a.ncol() ? _Dt : _Et),
     _Vt(a.nrow() < a.ncol() ? _Et : _Dt)
{
    if (nrow() < ncol())
	for (u_int i = 0; i < nrow(); ++i)
	    for (u_int j = 0; j < ncol(); ++j)
		_Dt[i][j] = a[i][j];
    else
	for (u_int i = 0; i < nrow(); ++i)
	    for (u_int j = 0; j < ncol(); ++j)
		_Dt[j][i] = a[i][j];

  /* Householder reduction to bi-diagonal (off-diagonal in lower part) form */
    for (u_int m = 0; m < _Et.dim(); ++m)
    {
	_Dt.apply_from_right(_Dt, m);
	_Et.apply_from_left(_Dt, m);
    }

    _Dt.make_transformation();	// Accumulate right-hand transformation: V
    _Et.make_transformation();	// Accumulate left-hand transformation: U

    for (u_int m = 0; m < _Et.dim(); ++m)
    {
	T	anorm = fabs(_diagonal[m]) + fabs(_off_diagonal[m]);
	if (anorm > _anorm)
	    _anorm = anorm;
    }
}

//! 2重対角行列を対角化する（特異値分解）．
/*!
  対角成分は特異値となり，\f$\TUtvec{U}{}\f$と\f$\TUtvec{V}{}\f$
  の各行はそれぞれ右特異ベクトルと左特異ベクトルを与える．
  \throw std::runtime_error	指定した繰り返し回数を越えた場合に送出
*/ 
template <class T> void
BiDiagonal<T>::diagonalize()
{
    using namespace	std;
    
    for (u_int n = _Et.dim(); n-- > 0; )
    {
	u_int	niter = 0;
	
#ifdef TUVectorPP_DEBUG
	cerr << "******** n = " << n << " ********" << endl;
#endif
	while (!off_diagonal_is_zero(n))	// n > 0 here
	{
	    if (niter++ > NITER_MAX)
		throw runtime_error("TU::BiDiagonal::diagonalize(): Number of iteration exceeded maximum value");
	    
	  /* Find first m (< n) whose off-diagonal element is 0 */
	    u_int m = n;
	    do
	    {
		if (diagonal_is_zero(m-1))
		{ // If _diagonal[m-1] is zero, make _off_diagonal[m] zero.
		    T	x = _diagonal[m], y = _off_diagonal[m];
		    _off_diagonal[m] = 0.0;
		    for (u_int i = m; i <= n; ++i)
		    {
			Rotation	rotD(m-1, i, x, -y);

			_Dt.rotate_from_left(rotD);
			
			_diagonal[i] = -y*rotD.sin()
				     + _diagonal[i]*rotD.cos();
			if (i < n)
			{
			    x = _diagonal[i+1];
			    y = _off_diagonal[i+1]*rotD.sin();
			    _off_diagonal[i+1] *= rotD.cos();
			}
		    }
		    break;	// if _diagonal[n-1] is zero, m == n here.
		}
	    } while (!off_diagonal_is_zero(--m)); // 0 <= m < n < nrow() here.
	    if (m == n)
		break;		// _off_diagonal[n] has been made 0. Retry!

	  /* Set x and y which determine initial(i = m+1) plane rotation */
	    T	x, y;
	    initialize_rotation(m, n, x, y);
#ifdef TUBiDiagonal_DEBUG
	    cerr << "--- m = " << m << ", n = " << n << "---"
		 << endl;
	    cerr << "  diagonal:     " << diagonal();
	    cerr << "  off-diagonal: " << off_diagonal();
#endif
	  /* Apply rotation P(i-1, i) for each i (i = m+1, n+2, ... , n) */
	    for (u_int i = m; ++i <= n; )
	    {
	      /* Apply rotation from left */
		Rotation	rotE(i-1, i, x, y);
		
		_Et.rotate_from_left(rotE);

		if (i > m+1)
		    _off_diagonal[i-1] = rotE.length();
		T	tmp = _diagonal[i-1];
		_diagonal[i-1]	 =  rotE.cos()*tmp
				 +  rotE.sin()*_off_diagonal[i];
		_off_diagonal[i] = -rotE.sin()*tmp
				 +  rotE.cos()*_off_diagonal[i];
		if (diagonal_is_zero(i))
		    break;		// No more Given's rotation needed.
		y		 =  rotE.sin()*_diagonal[i];
		_diagonal[i]	*=  rotE.cos();

		x = _diagonal[i-1];
		
	      /* Apply rotation from right to recover bi-diagonality */
		Rotation	rotD(i-1, i, x, y);

		_Dt.rotate_from_left(rotD);

		_diagonal[i-1] = rotD.length();
		tmp = _off_diagonal[i];
		_off_diagonal[i] =  tmp*rotD.cos() + _diagonal[i]*rotD.sin();
		_diagonal[i]	 = -tmp*rotD.sin() + _diagonal[i]*rotD.cos();
		if (i < n)
		{
		    if (off_diagonal_is_zero(i+1))
			break;		// No more Given's rotation needed.
		    y		        = _off_diagonal[i+1]*rotD.sin();
		    _off_diagonal[i+1] *= rotD.cos();

		    x		        = _off_diagonal[i];
		}
	    }
#ifdef TUVectorPP_DEBUG
	    cerr << "  niter = " << niter << ": " << off_diagonal();
#endif
	}
    }

    for (u_int m = 0; m < _Et.dim(); ++m)  // sort singular values and vectors
	for (u_int n = m+1; n < _Et.dim(); ++n)
	    if (fabs(_diagonal[n]) > fabs(_diagonal[m]))
	    {
		swap(_diagonal[m], _diagonal[n]);
		for (u_int j = 0; j < _Et.dim(); ++j)
		{
		    const T	tmp = _Et[m][j];
		    _Et[m][j] = _Et[n][j];
		    _Et[n][j] = -tmp;
		}
		for (u_int j = 0; j < _Dt.dim(); ++j)
		{
		    const T	tmp = _Dt[m][j];
		    _Dt[m][j] = _Dt[n][j];
		    _Dt[n][j] = -tmp;
		}
	    }

    u_int l = min(_Dt.dim() - 1, _Et.dim());	// last index
    for (u_int m = 0; m < l; ++m)	// ensure positivity of all singular
	if (_diagonal[m] < 0.0)		// values except for the last one.
	{
	    _diagonal[m] = -_diagonal[m];
	    for (u_int j = 0; j < _Et.dim(); ++j)
		_Et[m][j] = -_Et[m][j];

	    if (l < _Et.dim())
	    {
		_diagonal[l] = -_diagonal[l];
		for (u_int j = 0; j < _Et.dim(); ++j)
		    _Et[l][j] = -_Et[l][j];
	    }
	}
}

template <class T> bool
BiDiagonal<T>::diagonal_is_zero(u_int n) const
{
    return _Dt.sigma_is_zero(n, _anorm);
}

template <class T> bool
BiDiagonal<T>::off_diagonal_is_zero(u_int n) const
{
    return _Et.sigma_is_zero(n, _anorm);
}

template <class T> void
BiDiagonal<T>::initialize_rotation(u_int m, u_int n, T& x, T& y) const
{
    const T	g = ((_diagonal[n]     + _diagonal[n-1])*
		     (_diagonal[n]     - _diagonal[n-1])+
		     (_off_diagonal[n] + _off_diagonal[n-1])*
		     (_off_diagonal[n] - _off_diagonal[n-1]))
		  / (2.0*_diagonal[n-1]*_off_diagonal[n]),
      // Caution!! You have to ensure that _diagonal[n-1] != 0
      // as well as _off_diagonal[n].
		absg = fabs(g),
		gg1 = (absg > 1.0 ?
		       absg * sqrt(1.0 + (1.0/absg)*(1.0/absg)) :
		       sqrt(1.0 + absg*absg)),
		t = (g > 0.0 ? g + gg1 : g - gg1);
    x = ((_diagonal[m] + _diagonal[n])*(_diagonal[m] - _diagonal[n]) -
	 _off_diagonal[n]*(_off_diagonal[n] + _diagonal[n-1]/t)) / _diagonal[m];
  //x = _diagonal[m];				// without shifting
    y = _off_diagonal[m+1];
}

/************************************************************************
*  class SVDecomposition<T>						*
************************************************************************/
//! 一般行列の特異値分解を表すクラス
/*!
  与えられた一般行列\f$\TUvec{A}{} \in \TUspace{R}{m\times n}\f$に対し
  て\f$\TUtvec{V}{}\TUvec{A}{}\TUvec{U}{}\f$が対角行列となるような2つの
  回転行列\f$\TUtvec{U}{} \in \TUspace{R}{n\times n}\f$,
  \f$\TUtvec{V}{} \in \TUspace{R}{m\times m}\f$を求める．
 */
template <class T>
class SVDecomposition : private BiDiagonal<T>
{
  public:
  //! 与えられた一般行列の特異値分解を求める．
  /*!
    \param a	特異値分解する一般行列
  */
    template <class T2, class B2, class R2>
    SVDecomposition(const Matrix<T2, B2, R2>& a)
	:BiDiagonal<T>(a)		{BiDiagonal<T>::diagonalize();}

    using	BiDiagonal<T>::nrow;
    using	BiDiagonal<T>::ncol;
    using	BiDiagonal<T>::Ut;
    using	BiDiagonal<T>::Vt;
    using	BiDiagonal<T>::diagonal;

  //! 特異値を求める．
  /*!
    \param i	絶対値の大きい順に並んだ特異値の1つを指定するindex
    \return	指定されたindexに対応する特異値
  */
    const T&	operator [](int i)	const	{return diagonal()[i];}
};

/************************************************************************
*  typedefs								*
************************************************************************/
typedef Vector<short,  FixedSizedBuf<short,   2> >
	Vector2s;			//!< short型要素を持つ2次元ベクトル
typedef Vector<int,    FixedSizedBuf<int,     2> >
	Vector2i;			//!< int型要素を持つ2次元ベクトル
typedef Vector<float,  FixedSizedBuf<float,   2> >
	Vector2f;			//!< float型要素を持つ2次元ベクトル
typedef Vector<double, FixedSizedBuf<double,  2> >
	Vector2d;			//!< double型要素を持つ2次元ベクトル
typedef Vector<short,  FixedSizedBuf<short,   3> >
	Vector3s;			//!< short型要素を持つ3次元ベクトル
typedef Vector<int,    FixedSizedBuf<int,     3> >
	Vector3i;			//!< int型要素を持つ3次元ベクトル
typedef Vector<float,  FixedSizedBuf<float,   3> >
	Vector3f;			//!< float型要素を持つ3次元ベクトル
typedef Vector<double, FixedSizedBuf<double,  3> >
	Vector3d;			//!< double型要素を持つ3次元ベクトル
typedef Vector<short,  FixedSizedBuf<short,   4> >
	Vector4s;			//!< short型要素を持つ4次元ベクトル
typedef Vector<int,    FixedSizedBuf<int,     4> >
	Vector4i;			//!< int型要素を持つ4次元ベクトル
typedef Vector<float,  FixedSizedBuf<float,   4> >
	Vector4f;			//!< float型要素を持つ4次元ベクトル
typedef Vector<double, FixedSizedBuf<double,  4> >
	Vector4d;			//!< double型要素を持つ4次元ベクトル
typedef Matrix<float,  FixedSizedBuf<float,   4>,
	       FixedSizedBuf<Vector<float>,   2> >
	Matrix22f;			//!< float型要素を持つ2x2行列
typedef Matrix<double, FixedSizedBuf<double,  4>,
	       FixedSizedBuf<Vector<double>,  2> >
	Matrix22d;			//!< double型要素を持つ2x2行列
typedef Matrix<float,  FixedSizedBuf<float,   6>,
	       FixedSizedBuf<Vector<float>,   2> >
	Matrix23f;			//!< float型要素を持つ2x3行列
typedef Matrix<double, FixedSizedBuf<double,  6>,
	       FixedSizedBuf<Vector<double>,  2> >
	Matrix23d;			//!< double型要素を持つ2x3行列
typedef Matrix<float,  FixedSizedBuf<float,   9>,
	       FixedSizedBuf<Vector<float>,   3> >
	Matrix33f;			//!< float型要素を持つ3x3行列
typedef Matrix<double, FixedSizedBuf<double,  9>,
	       FixedSizedBuf<Vector<double>,  3> >
	Matrix33d;			//!< double型要素を持つ3x3行列
typedef Matrix<float,  FixedSizedBuf<float,  12>,
	       FixedSizedBuf<Vector<float>,   3> >
	Matrix34f;			//!< float型要素を持つ3x4行列
typedef Matrix<double, FixedSizedBuf<double, 12>,
	       FixedSizedBuf<Vector<double>,  3> >
	Matrix34d;			//!< double型要素を持つ3x4行列
typedef Matrix<float,  FixedSizedBuf<float,  16>,
	       FixedSizedBuf<Vector<float>,   4> >
	Matrix44f;			//!< float型要素を持つ4x4行列
typedef Matrix<double, FixedSizedBuf<double, 16>,
	       FixedSizedBuf<Vector<double>,  4> >
	Matrix44d;			//!< double型要素を持つ4x4行列

/************************************************************************
*  class Minimization1<S>						*
************************************************************************/
static const double	DEFAULT_TOL = 3.0e-8;

template <class S>
class Minimization1
{
  private:
    enum		{DEFAULT_NITER_MAX = 100};

  public:
    Minimization1(S tol = DEFAULT_TOL, int niter_max = DEFAULT_NITER_MAX)
	:_tol(tol), _niter_max(niter_max)				{}
    
    virtual S		operator ()(const S&)			const	= 0;
    S			minimize(S&, S)				const	;

  private:
    const S		_tol;
    const int		_niter_max;
};

/*
 *  Minimize 1-dimensional function using golden section search and minima
 *  is returned in x. Minimum value of the func is also returned as a return
 *  value.
 */
template <class S> S
Minimization1<S>::minimize(S& x, S w) const
{
    using namespace	std;

    const double	W = 0.38197;
    S			x1 = x, x2 = x + w,
			f1 = (*this)(x1), f2 = (*this)(x2);
    
    if (f1 < f2)			// guarantee that f1 >= f2
    {
	S	tmp = x1;		// swap x1 & x2
	x1  = x2;
	x2  = tmp;
	tmp = f1;			// swap f1 & f2
	f1  = f2;
	f2  = tmp;
    }
    S	x0;
    do
    {
	x0  = x1;
	x1  = x2;
	x2 += (1.0 / W - 1.0) * (x1 - x0);	// elongate to right
#ifdef MIN1_DEBUG
	S	f0 = f1;
#endif
	f1  = f2;
	f2  = (*this)(x2);
#ifdef MIN1_DEBUG
	std::cerr << "Bracketting: [" << x0 << ", " << x1 << ", " << x2
		  << "], (" << f0 << ", " << f1 << ", " << f2 << ")"
		  << std::endl;
#endif
    } while (f1 > f2);
    
  /* Golden section search */
    S	x3 = x2;
    if (fabs(x1 - x0) > fabs(x2 - x1))
    {
	x2  = x1;
	x1 -= W * (x2 - x0);		// insert new x1 between x0 & x2
	f2  = f1;
	f1  = (*this)(x1);
    }
    else
    {
	x2 -= (1.0 - W) * (x3 - x1);	// insert new x2 between x1 & x3
	f2  = (*this)(x2);
    }
#ifdef MIN1_DEBUG
    std::cerr << "Initial:     [" << x0 << ", " << x1 << ", " << x2
	      << ", " << x3 << "], (" << f1 << ", " << f2 << ")" << std::endl;
#endif
    int	i;
    for (i = 0;
	 i < _niter_max && fabs(x3 - x0) > _tol * (fabs(x1) + fabs(x2)); ++i)
    {
	if (f1 < f2)
	{
	    x3  = x2;			// shift x2 & x3 to left
	    x2  = x1;
	    x1 -= W * (x2 - x0);	// insert new x1 between x0 & x2
	    f2  = f1;
	    f1  = (*this)(x1);
	}
	else
	{
	    x0  = x1;			// shift x0 & x1 to right
	    x1  = x2;
	    x2 += W * (x3 - x1);	// insert new x2 between x1 & x3
	    f1  = f2;
	    f2  = (*this)(x2);
	}
#ifdef MIN1_DEBUG
	std::cerr << "Golden:      [" << x0 << ", " << x1 << ", " << x2
		  << ", " << x3 << "], (" << f1 << ", " << f2 << ")"
		  << std::endl;
#endif
    }
    if (i == _niter_max)
	throw std::runtime_error("TU::Minimization1<S>::minimize(): Too many iterations!!");

    if (f1 < f2)
    {
	x = x1;
	return f1;
    }
    else
    {
	x = x2;
	return f2;
    }
}

/************************************************************************
*  class Minimization<S, T>						*
************************************************************************/
template <class S, class T>
class Minimization
{
  private:
    class LineFunction : public Minimization1<S>
    {
      public:
	LineFunction(const Minimization<S, T>& func,
		     const T& x, const Vector<S>& h,
		     S tol, int niter_max)
	  :Minimization1<S>(tol, niter_max),
	   _func(func), _x(x), _h(h)		{}
    
	S	operator ()(const S& d)	const	{return _func(_func.proceed
							      (_x, d * _h));}
    
      private:
	const Minimization<S, T>&	_func;
	const T&			_x;
	const Vector<S>&		_h;
    };

  private:
    enum		{DEFAULT_NITER_MAX = 1000};
		 
  public:
    Minimization(S tol = DEFAULT_TOL, int niter_max=DEFAULT_NITER_MAX,
		 bool pr=false)
      :_tol(tol), _niter_max(niter_max), _print(pr)			{}
    
    virtual S		operator ()(const T&)			const	= 0;
    virtual Vector<S>	ngrad(const T& x)			const	= 0;
    virtual T		proceed(const T&, const Vector<S>&)	const	= 0;
    S			minimize(T&)					;
    S			steepest_descent(T&)				;
    S			line_minimize(T&, const Vector<S>&)	const	;

  protected:
    virtual void	update(const T&)				;
    virtual void	print(int, S, const T&)			const	;
    
  private:
    int			near_enough(S, S)			const	;
 
    const S		_tol;
    const int		_niter_max;
    const bool		_print;
};

/*
 *  Minimize multi-dimensional function using conjugate gradient method and
 *  minima is returned in x. Minimum value of the func is also returned as
 *  a return value.
 */
template <class S, class T> S
Minimization<S, T>::minimize(T& x)
{
    S		val = (*this)(x);
    Vector<S>	g   = ngrad(x), h = g;
    
    for (int i = 0; i < _niter_max; ++i)
    {
	if (_print)
	    print(i, val, x);

	const S		g_sqr = g * g;
	if (g_sqr == 0.0)
	    return val;

	const S		val_next = line_minimize(x, h);
	if (near_enough(val, val_next))
	    return val_next;
	val = val_next;

	const Vector<S>	g_next = ngrad(x);
	h = g_next + (((g_next - g) * g_next) / g_sqr) * h;
	g = g_next;
	update(x);
    }

    std::cerr << "TU::Minimization<S, T>::minimize(): Too many iterations!!"
	      << std::endl;
    return val;
}

/*
 *  Minimize multi-dimensional function using steepest descent method and
 *  minima is returned in x. Minimum value of the func is also returned as
 *  a return value.
 */
template <class S, class T> S
Minimization<S, T>::steepest_descent(T& x)
{
    S		val = (*this)(x);
    Vector<S>	g   = ngrad(x);
    
    for (int i = 0; i < _niter_max; ++i)
    {
	if (_print)
	    print(i, val, x);
	
	const S		g_sqr = g * g;
	if (g_sqr == 0.0)
	    return val;

	const S		val_next = line_minimize(x, g);
	if (near_enough(val, val_next))
	    return val_next;
	val = val_next;

	g = ngrad(x);
	update(x);
    }

    std::cerr << "TU::Minimization<S, T>::steepest_descent(): Too many iterations!!"
	      << std::endl;
    return val;
}

/*
 *  Minimize function along direction h and minima is returned in x.
 *  Minimum value of the function is also returned as a return value.
 */
template <class S, class T> S
Minimization<S, T>::line_minimize(T &x, const Vector<S>& h) const
{
    LineFunction	lfunc(*this, x, h, _tol, _niter_max);
    S			d = 0.0, val = lfunc.minimize(d, 1.0);
    x = proceed(x, d * h);
    return val;
}

/*
 *  Update the status of the function to be minimized.
 */
template <class S, class T> void
Minimization<S, T>::update(const T&)
{
}

/*
 *  Print intermediate values
 */
template <class S, class T> void
Minimization<S, T>::print(int i, S val, const T& x) const
{
    std::cerr << std::setw(3) << i << ": (" << val << ')' << x;
}

template <class S, class T> inline int
Minimization<S, T>::near_enough(S a, S b) const
{
    const double	EPS = 1.0e-10;
    return 2.0*fabs(a - b) <= _tol*(fabs(a) + fabs(b) + EPS);
}
 
}

#endif	/* !__TUVectorPP_h	*/
