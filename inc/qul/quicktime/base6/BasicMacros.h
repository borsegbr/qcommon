#ifndef _BasicMacros_H
#define _BasicMacros_H

#define CAT(a, b)				a##b
#define CAT2(a, b)				CAT(a, b)
#define CAT3(a, b, c)			CAT2(CAT2(a, b), c)


#endif	// ifndef _BasicMacros_H