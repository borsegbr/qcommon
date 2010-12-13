#ifndef _porting16bit_H
#define _porting16bit_H


#ifndef IS_BIGENDIAN
#   define IS_BIGENDIAN    0
#endif


typedef struct _PixelAddr
{
	unsigned long	p16 : 31;
	unsigned short	b : 1;
} PixelAddr;


#ifdef PLATFORM_BYTEADDR

#	define PA_ADDR16(pa)	((unsigned short *)((pa).p16<<1))

#	define PA_BIND(pa, p)  \
(pa).b=((unsigned long)p)&1; (pa).p16=((((unsigned long)p)-(pa).b)>>1)

#else	// PLATFORM_WORDADDR

#	define PA_ADDR16(pa)	((unsigned short *)(pa).p16)

#	define PA_BIND(pa, p)  \
	(pa).b = 0; (pa).p16=(unsigned long)p

#endif



#define PA_SETNULL(pa)	(pa).p16=0;(pa).b=0
#define PA_ISNULL(pa)	((pa).p16==0&&(pa).b==0)
#define PA_EQ(p1,p2)	((p1).p16==(p2).p16&&(p1).b==(p2).b)
#define PA_LT(p1,p2)	((p1).p16<(p2).p16||(p1).p16==(p2).p16&&(p1).b<(p2).b)
#define PA_GT(p1,p2)	((p1).p16>(p2).p16||(p1).p16==(p2).p16&&(p1).b>(p2).b)

#define PA_DIFF8(p1,p2)	((((p1).p16-(p2).p16)<<1)+((int)(p1).b-(int)(p2).b))


#if IS_BIGENDIAN	// big endian
#   define PA_GET8(pa, n)	\
	((PA_ADDR16(pa)[((pa).b+(n))>>1]>>(8*(1-(((pa).b+(n))&1))))&0xff)
#   define PA_SET8(pa, n, s)  \
	PA_ADDR16(pa)[((pa).b+(n))>>1]&=0xff<<(8*(((pa).b+(n))&1));	\
	PA_ADDR16(pa)[((pa).b+(n))>>1]|=((s)&0xff)<<(8*(1-(((pa).b+(n))&1)))
#else	// little endian
#   define PA_GET8(pa, n)	\
	((PA_ADDR16(pa)[((pa).b+(n))>>1]>>(8*(((pa).b+(n))&1)))&0xff)
#   define PA_SET8(pa, n, s)  \
	PA_ADDR16(pa)[((pa).b+(n))>>1]&=0xff<<(8*(1-(((pa).b+(n))&1)));	\
    PA_ADDR16(pa)[((pa).b+(n))>>1]|=((s)&0xff)<<(8*(((pa).b+(n))&1))
#endif

#define PA_INC(pa)		(pa).p16+=(pa).b;(pa).b=1-(pa).b
#define PA_ADD(pa, n)	(pa).p16+=((n)+(pa).b)>>1; (pa).b+=(n); (pa).b&=1


static void PA_COPY(PixelAddr dst, PixelAddr src, int size)
{
#if 0
	int i;
	for (i = 0; i < size; i++)
	{
		unsigned short t = PA_GET8(src, i);
		PA_SET8(dst, i, t);
	}
#else
	int size2;
	if (dst.b==src.b)
	{
		if (dst.b)
		{
			unsigned short t = PA_GET8(src, 0);
			PA_SET8(dst, 0, t);
			size--;
			size2 = size / 2;
			memcpy(PA_ADDR16(dst)+1, PA_ADDR16(src)+1, size2 * sizeof(short));
		}
		else
		{
			size2 = size / 2;
			memcpy(PA_ADDR16(dst),PA_ADDR16(src), size2 * sizeof(short));
		}
		if (size % 2)
		{
			unsigned short t = PA_GET8(src, size-1);
			PA_SET8(dst, size-1, t);
		}
	}
	else if (dst.b==0)
	{
		unsigned short *ps = PA_ADDR16(src);
		unsigned short *pd = PA_ADDR16(dst);
		unsigned short *pdlast = pd;
		size2 = size/2;
		pdlast = pd + size2;
		for ( ; pd < pdlast; )
		{
			unsigned short s = *ps;
#if IS_BIGENDIAN
			s<<=8;
			ps++;
			s |= (*ps)>>8;
#else
			s>>=8;
			ps++;
			s |= (*ps)<<8;
#endif
			*pd++ = s;
		}
		if (size%2)
		{
			
#if IS_BIGENDIAN
			*pd &= 0x00ff;
			*pd |= (*ps)<<8;
#else
			*pd &= 0xff00;
			*pd |= (*ps);
#endif
		}
	}
	else	// src.b==0
	{
		unsigned short *ps = PA_ADDR16(src);
		unsigned short *pd = PA_ADDR16(dst);
		unsigned short *pslast = ps;
		size2 = size/2;
		pslast = ps + size2;
		for ( ; ps < pslast; )
		{
			unsigned short s = *ps++;
#if IS_BIGENDIAN
			*pd &= 0xff00;
			*pd |= (s >> 8);
			pd++;
			*pd &= 0x00ff;
			*pd |= (s << 8);
#else
			*pd &= 0x00ff;
			*pd |= (s << 8);
			pd++;
			*pd &= 0xff00;
			*pd |= (s >> 8);
#endif
		}
		if (size%2)
		{
#if IS_BIGENDIAN
			*pd &= 0xff00;
			*pd |= (*ps)>>8;
#else
			*pd &= 0x00ff;
			*pd |= (*ps)<<8;
#endif
		}
	}
#endif
}

static void PA_SET (PixelAddr p, unsigned short val, int size)
{
#if 0
	
	int i;
	for (i = 0; i < size; i++)
	{
		PA_SET8(p, i, val);
	}

#else
	unsigned short *pd = PA_ADDR16(p);
	int size2;
	val = (val<<8)|(val&0xff);
	if (p.b)
	{
#if IS_BIGENDIAN
		pd[0] &= 0xff00;
		pd[0] |= val&0xff;
#else
		pd[0] &= 0x00ff;
		pd[0] |= val<<8;
#endif
		pd++;
		size--;
	}
	size2 = size/2;
	memset(pd, val, size2*sizeof(short));
	if (size%2)
	{
#if IS_BIGENDIANS
		pd[0] &= 0x00ff;
		pd[0] |= val<<8;
#else
		pd[0] &= 0xff00;
		pd[0] |= val&0xff;
#endif
		
	}
#endif
}


#define CHARRPORT(n)	(((n)*sizeof(unsigned short)+1)/2)



#endif	/* ifndef _porting16bit_H */
