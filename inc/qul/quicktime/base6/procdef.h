#define DEFINE_PROC_0(name)																\
	RESULT_TYPE COMPLETE_NAME(name) )													\
		{																				\
		return CALL_PROC(name) );														\
		}

#define DEFINE_PROC_1(name,t1,p1)														\
	RESULT_TYPE COMPLETE_NAME(name) D_COMMA() t1 p1)									\
		{																				\
		return CALL_PROC(name) C_COMMA() p1);											\
		}

#define DEFINE_PROC_2(name,t1,p1,t2,p2)													\
	RESULT_TYPE COMPLETE_NAME(name) D_COMMA() t1 p1,t2 p2)								\
		{																				\
		return CALL_PROC(name) C_COMMA() p1,p2);										\
		}

#define DEFINE_PROC_3(name,t1,p1,t2,p2,t3,p3)											\
	RESULT_TYPE COMPLETE_NAME(name) D_COMMA() t1 p1,t2 p2,t3 p3)						\
		{																				\
		return CALL_PROC(name) C_COMMA() p1,p2,p3);										\
		}

#define DEFINE_PROC_4(name,t1,p1,t2,p2,t3,p3,t4,p4)										\
	RESULT_TYPE COMPLETE_NAME(name) D_COMMA() t1 p1,t2 p2,t3 p3,t4 p4)					\
		{																				\
		return CALL_PROC(name) C_COMMA() p1,p2,p3,p4);									\
		}

#define DEFINE_PROC_5(name,t1,p1,t2,p2,t3,p3,t4,p4,t5,p5)								\
	RESULT_TYPE COMPLETE_NAME(name) D_COMMA() t1 p1,t2 p2,t3 p3,t4 p4,t5 p5)			\
		{																				\
		return CALL_PROC(name) C_COMMA() p1,p2,p3,p4,p5);								\
		}

#define DEFINE_PROC_6(name,t1,p1,t2,p2,t3,p3,t4,p4,t5,p5,t6,p6)							\
	RESULT_TYPE COMPLETE_NAME(name) D_COMMA() t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6)		\
		{																				\
		return CALL_PROC(name) C_COMMA() p1,p2,p3,p4,p5,p6);							\
		}

#define DEFINE_PROC_7(name,t1,p1,t2,p2,t3,p3,t4,p4,t5,p5,t6,p6,t7,p7)					\
	RESULT_TYPE COMPLETE_NAME(name) D_COMMA() t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7)\
		{																				\
		return CALL_PROC(name) C_COMMA() p1,p2,p3,p4,p5,p6,p7);							\
		}

#define DEFINE_PROC_8(name,t1,p1,t2,p2,t3,p3,t4,p4,t5,p5,t6,p6,t7,p7,t8,p8)				\
	RESULT_TYPE COMPLETE_NAME(name) D_COMMA()											\
		t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7,t8 p8)								\
		{																				\
		return CALL_PROC(name) C_COMMA() p1,p2,p3,p4,p5,p6,p7,p8);						\
		}

#define DEFINE_PROC_9(name,t1,p1,t2,p2,t3,p3,t4,p4,t5,p5,t6,p6,t7,p7,t8,p8,t9,p9)		\
	RESULT_TYPE COMPLETE_NAME(name) D_COMMA()											\
	t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7,t8 p8,t9 p9)								\
		{																				\
		return CALL_PROC(name) C_COMMA() p1,p2,p3,p4,p5,p6,p7,p8,p9);					\
		}

#define DEFINE_PROC_10(name,t1,p1,t2,p2,t3,p3,t4,p4,t5,p5,t6,p6,t7,p7,t8,p8,t9,p9,ta,pa)\
	RESULT_TYPE COMPLETE_NAME(name) D_COMMA()											\
	t1 p1,t2 p2,t3 p3,t4 p4,t5 p5,t6 p6,t7 p7,t8 p8,t9 p9,ta pa)						\
		{																				\
		return CALL_PROC(name) C_COMMA() p1,p2,p3,p4,p5,p6,p7,p8,p9,pa);				\
		}
