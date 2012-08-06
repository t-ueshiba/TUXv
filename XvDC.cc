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
 *  $Id: XvDC.cc,v 1.10 2012-08-06 23:54:26 ueshiba Exp $
 */
#include "TU/v/XvDC.h"
#include <stdexcept>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>

namespace TU
{
namespace v
{
/************************************************************************
*  static functions							*
************************************************************************/
static void
EVvXvDC(::Widget, XtPointer client_data, XEvent*, Boolean*)
{
    XvDC&	vXvDC = *(XvDC*)client_data;
    vXvDC.repaintAll();
}

/************************************************************************
*  class XvDC								*
************************************************************************/
template <class S> void
XvDC::createXvImage(const Image<S>& image)
{
    if (_xvimage == 0 ||
	_xvimage->width != image.width() || _xvimage->height != image.height())
    {
	destroyShmImage();
	_xvimage = XvShmCreateImage(colormap().display(), _port, UYVY, 0,
				    image.width(), image.height(), xShmInfo());
	if (_xvimage == 0)
	    return;
	_xvimage->data = attachShm(_xvimage->data_size);
	if (_xvimage->data == 0)
	{
	    XFree(_xvimage);
	    _xvimage = 0;
	    return;
	}
    }

    for (int v = 0; v < image.height(); ++v)
    {
	ImageLine<YUV422> imageLine((YUV422*)_xvimage->data +
				    v * _xvimage->width,
				    _xvimage->width);
	imageLine.fill((const S*)image[v]);
    }
}

XvDC::XvDC(CanvasPane& parentCanvasPane,
	   u_int width, u_int height, u_int mul, u_int div)
    :ShmDC(parentCanvasPane, width, height, mul, div),
     _port(~0), _xvimage(0)
{
    u_int		nadaptors;
    XvAdaptorInfo*	adaptorInfo;
    if (XvQueryAdaptors(colormap().display(),
			RootWindow(colormap().display(),
				   colormap().vinfo().screen),
			&nadaptors, &adaptorInfo) != Success)
	throw std::domain_error("TU::v::XvDC::XvDC: failed to get adaptor info!");
    for (int i = 0; _port == ~0 && i < nadaptors; ++i)
    {
	if (adaptorInfo[i].type & XvImageMask)
	{
	    int	nfmts;
	    XvImageFormatValues*
		fmtVal = XvListImageFormats(colormap().display(),
					    adaptorInfo[i].base_id,
					    &nfmts);
	    for (int j = 0; j < nfmts; ++j)
		if (fmtVal[j].id == UYVY)
		{
		    _port = grabPort(adaptorInfo[i].base_id,
				     adaptorInfo[i].num_ports);
		    break;
		}
		
	    if (fmtVal != 0)
		XFree(fmtVal);
	}
    }

    if (adaptorInfo != 0)
	XvFreeAdaptorInfo(adaptorInfo);

    if (_port == ~0)
	throw std::domain_error("TU::v::XvDC::XvDC: failed to grab port!");

    _vXvDCList.push_front(*this);

    XtAddEventHandler(window().widget(), StructureNotifyMask,
		      FALSE, EVvXvDC, (XtPointer)this);
}

XvDC::~XvDC()
{
    if (_xvimage != 0)
	XFree(_xvimage);

    XtRemoveEventHandler(window().widget(), StructureNotifyMask,
			 FALSE, EVvXvDC, (XtPointer)this);

    for (List<XvDC>::Iterator iter = _vXvDCList.begin();
	 iter != _vXvDCList.end(); ++iter)
	if (&(*iter) == this)
	{
	    _vXvDCList.erase(iter);
	    break;
	}

    XvUngrabPort(colormap().display(), _port, CurrentTime);
}

DC&
XvDC::operator <<(const Point2<int>& p)
{
    return ShmDC::operator <<(p);
}

DC&
XvDC::operator <<(const LineP2f& l)
{
    return ShmDC::operator <<(l);
}

DC&
XvDC::operator <<(const LineP2d& l)
{
    return ShmDC::operator <<(l);
}

DC&
XvDC::operator <<(const Image<u_char>& image)
{
    return ShmDC::operator <<(image);
}

DC&
XvDC::operator <<(const Image<s_char>& image)
{
    return ShmDC::operator <<(image);
}

DC&
XvDC::operator <<(const Image<short>& image)
{
    return ShmDC::operator <<(image);
}

DC&
XvDC::operator <<(const Image<BGR>& image)
{
    return ShmDC::operator <<(image);
}

DC&
XvDC::operator <<(const Image<ABGR>& image)
{
    return ShmDC::operator <<(image);
}

DC&
XvDC::operator <<(const Image<RGB>& image)
{
    return ShmDC::operator <<(image);
}

DC&
XvDC::operator <<(const Image<RGBA>& image)
{
    return ShmDC::operator <<(image);
}

DC&
XvDC::operator <<(const Image<YUV444>& image)
{
    createXvImage(image);
    if (_xvimage != 0)
    {
	XvShmPutImage(colormap().display(), _port, drawable(), gc(), _xvimage,
		      0, 0, _xvimage->width, _xvimage->height,
		      log2devR(offset()[0]), log2devR(offset()[1]),
		      log2devR(_xvimage->width), log2devR(_xvimage->height),
		      True);
	XFlush(colormap().display());
    }
    else
	ShmDC::operator <<(image);
    
    return *this;
}

DC&
XvDC::operator <<(const Image<YUV422>& image)
{
    createXvImage(image);
    if (_xvimage != 0)
    {
	XvShmPutImage(colormap().display(), _port, drawable(), gc(), _xvimage,
		      0, 0, _xvimage->width, _xvimage->height,
		      log2devR(offset()[0]), log2devR(offset()[1]),
		      log2devR(_xvimage->width), log2devR(_xvimage->height),
		      True);
	XFlush(colormap().display());
    }
    else
	ShmDC::operator <<(image);
    
    return *this;
}

DC&
XvDC::operator <<(const Image<YUYV422>& image)
{
    createXvImage(image);
    if (_xvimage != 0)
    {
	XvShmPutImage(colormap().display(), _port, drawable(), gc(), _xvimage,
		      0, 0, _xvimage->width, _xvimage->height,
		      log2devR(offset()[0]), log2devR(offset()[1]),
		      log2devR(_xvimage->width), log2devR(_xvimage->height),
		      True);
	XFlush(colormap().display());
    }
    else
	ShmDC::operator <<(image);
    
    return *this;
}

DC&
XvDC::operator <<(const Image<YUV411>& image)
{
    createXvImage(image);
    if (_xvimage != 0)
    {
	XvShmPutImage(colormap().display(), _port, drawable(), gc(), _xvimage,
		      0, 0, _xvimage->width, _xvimage->height,
		      log2devR(offset()[0]), log2devR(offset()[1]),
		      log2devR(_xvimage->width), log2devR(_xvimage->height),
		      True);
	XFlush(colormap().display());
    }
    else
	ShmDC::operator <<(image);
    
    return *this;
}

void
XvDC::destroyShmImage()
{
    ShmDC::destroyShmImage();
    if (_xvimage != 0)
    {
	XFree(_xvimage);
	_xvimage = 0;
    }
}

XvPortID
XvDC::grabPort(XvPortID base_id, u_long num_ports)
{
    for (int i = 0; i < num_ports; ++i)
    {
	List<XvDC>::ConstIterator iter = _vXvDCList.begin();
	for (; iter != _vXvDCList.end(); ++iter)
	    if (base_id + i == iter->_port)
		break;			// This port has already been used.
	if ((iter == _vXvDCList.end()) &&
	    (XvGrabPort(colormap().display(), base_id + i, CurrentTime)
	     == Success))
	    return base_id + i;
    }

    return ~0;
}
 
}
}
