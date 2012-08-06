/*
 *  $BJ?@.(B9-19$BG/!JFH!K;:6H5;=QAm9g8&5f=j(B $BCx:n8"=jM-(B
 *  
 *  $BAO:n<T!'?"<G=SIW(B
 *
 *  $BK\%W%m%0%i%`$O!JFH!K;:6H5;=QAm9g8&5f=j$N?&0w$G$"$k?"<G=SIW$,AO:n$7!$(B
 *  $B!JFH!K;:6H5;=QAm9g8&5f=j$,Cx:n8"$r=jM-$9$kHkL)>pJs$G$9!%AO:n<T$K$h(B
 *  $B$k5v2D$J$7$KK\%W%m%0%i%`$r;HMQ!$J#@=!$2~JQ!$Bh;0<T$X3+<($9$kEy$NCx(B
 *  $B:n8"$r?/32$9$k9T0Y$r6X;_$7$^$9!%(B
 *  
 *  $B$3$N%W%m%0%i%`$K$h$C$F@8$8$k$$$+$J$kB;32$KBP$7$F$b!$Cx:n8"=jM-<T$*(B
 *  $B$h$SAO:n<T$O@UG$$rIi$$$^$;$s!#(B
 *
 *  Copyright 1997-2007.
 *  National Institute of Advanced Industrial Science and Technology (AIST)
 *
 *  Creator: Toshio UESHIBA
 *
 *  [AIST Confidential and all rights reserved.]
 *  This program is confidential. Any using, copying, changing or
 *  giving any information concerning with this program to others
 *  without permission by the creator are strictly prohibited.
 *
 *  [No Warranty.]
 *  The copyright holders or the creator are not responsible for any
 *  damages in the use of this program.
 *  
 *  $Id: XvDC.h,v 1.8 2012-08-06 23:54:26 ueshiba Exp $
 */
#ifndef __TUvXvDC_h
#define __TUvXvDC_h

#include "TU/v/ShmDC.h"
#include <X11/extensions/Xvlib.h>

namespace TU
{
namespace v
{
/************************************************************************
*  class XvDC								*
************************************************************************/
class XvDC : public ShmDC, public List<XvDC>::Node
{
  public:
    XvDC(CanvasPane& parentCanvasPane,
	 u_int width=0, u_int height=0, u_int mul=1, u_int div=1)	;
    virtual		~XvDC()						;

    using		ShmDC::operator <<;
    virtual DC&		operator <<(const Point2<int>& p)		;
    virtual DC&		operator <<(const LineP2f& l)			;
    virtual DC&		operator <<(const LineP2d& l)			;
    virtual DC&		operator <<(const Image<u_char>& image)		;
    virtual DC&		operator <<(const Image<s_char>& image)		;
    virtual DC&		operator <<(const Image<short>&  image)		;
    virtual DC&		operator <<(const Image<BGR>&    image)		;
    virtual DC&		operator <<(const Image<ABGR>&   image)		;
    virtual DC&		operator <<(const Image<RGB>&    image)		;
    virtual DC&		operator <<(const Image<RGBA>&   image)		;
    virtual DC&		operator <<(const Image<YUV444>& image)		;
    virtual DC&		operator <<(const Image<YUV422>& image)		;
    virtual DC&		operator <<(const Image<YUYV422>& image)	;
    virtual DC&		operator <<(const Image<YUV411>& image)		;

  protected:
    virtual void	destroyShmImage()				;
    
  private:
    enum FormatId
    {
	I420 = 0x30323449,
	RV15 = 0x35315652,	// RGB 15bits:
	RV16 = 0x36315652,	// RGB 16bits:
	YV12 = 0x32315659,	// 4:2:0 Planar mode:	Y + V + U
	YUY2 = 0x32595559,	// 4:2:2 Packed mode:	Y0 + U + Y1 + V
	UYVY = 0x59565955,	// 4:2:2 Packed mode:	U + Y0 + V + Y1
        BGRA = 0x3		// RGBA 32bits:		B + G + R + A
    };

    template <class S>
    void		createXvImage(const Image<S>& image)		;
    XvPortID		grabPort(XvPortID base_id, u_long num_ports)	;
    
    XvPortID		_port;
    XvImage*		_xvimage;

    static List<XvDC>	_vXvDCList;
};

}
}
#endif	// !__TUvXvDC_h
