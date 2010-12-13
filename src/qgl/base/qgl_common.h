#ifndef _qgl_common_H
#define _qgl_common_H

#include "qgl.h"


/* .. Move to common.h in the future, BEGIN */

#define SET_GET(type, name)	\
	const type & Get##name () const { return m_##name; }	\
	type & Get##name () { return m_##name; }	\
	void Set##name (const type &v) { m_##name = v; }

#define SET_GET2(type, prop, member)    \
	const type & Get##prop () const { return m_##member; }	\
	type & Get##prop () { return m_##member; }	\
	void Set##prop (const type &v) { m_##member = v; }


typedef QglReal     Phq;
typedef Phq &		PhqRef;
typedef const Phq & PhqCref;

/* .. Move to common.h in the future, END */

#endif  /* ifndef _qgl_common_H */

