#ifndef _COMMON_SPP_H_
#define _COMMON_SPP_H_

	#define XGC_ARGS_0(...) 
	#define XGC_ARGS_1(P1, ...) P1
	#define XGC_ARGS_2(P1, P2, ...) P2
	#define XGC_ARGS_3(P1, P2, P3, ...) P3
	#define XGC_ARGS_4(P1, P2, P3, P4, ...) P4
	#define XGC_ARGS_5(P1, P2, P3, P4, P5, ...) P5
	#define XGC_ARGS_6(P1, P2, P3, P4, P5, P6, ...) P6
	#define XGC_ARGS_7(P1, P2, P3, P4, P5, P6, P7, ...) P7

	#define XGC_ARGS_N(N,...) XGC_JOIN(XGC_ARGS_, N) ( __VA_ARGS__ )

	#define XGC_JOIN_X(x, y)			x##y
	#define XGC_JOIN(x, y)				XGC_JOIN_X(x, y)
	#define XGC_JOIN_WITH(with, x, y)	XGC_JOIN( x, XGC_JOIN(with, y) )

	#define XGC_LINEID(x)					XGC_JOIN(x, __LINE__)

	#define XGC_STRING1(x)					#x
	#define XGC_STRING2(x)					XGC_STRING1(x)
	#define XGC_STRING3(x)					XGC_STRING2(x)
	#define XGC_STRING(x)					XGC_STRING3(x)

	#define XGC_STATIC_BLOCK(...) \
		namespace { \
			static struct XGC_LINEID(DUMMY) { \
				XGC_LINEID(DUMMY)() {      \
					__VA_ARGS__ \
				} \
			} XGC_LINEID(__dummy__); \
		}

	//enumerates "X" from 1 to num with "INTER" as separator
	// XGC_ENUM(3,class T, = void XGC_COMMA) --> class T1 = void, class T2 = void, class T3 = void
	#define XGC_ENUM(num, X, INTER)                  XGC_ENUM_ ## num (X, XGC_UNPAR(INTER) )

	// enumerates using COMMA as separator 
	#define XGC_ENUM_D(num,X)                        XGC_ENUM_ ## num (X, XGC_COMMA)

	// XGC_PARAM(3, T, P, XGC_COMMA) --> T1 P1, T2 P2, T3 P3
	#define XGC_PARAM(num, X, P, INTER)				XGC_PARAM_ ## num (X, P, XGC_UNPAR(INTER) )

	// enumerates using COMMA as separator 
	#define XGC_PARAM_D(num,X,P)					XGC_PARAM_ ## num (X, P, XGC_COMMA)

	// Repeats "X" which should be a macro(N) num times, REPEAT_Z starts from 0
	// #define DO(N) T##N;
	// XGC_REPEAT(3,DO) --> DO(1) DO(2) DO(3) --> T1; T2; T3;
	#define XGC_REPEAT(num, X)                       XGC_REPEAT_ ## num (XGC_UNPAR(X))
	#define XGC_REPEAT_Z(num, X)                     X(0) XGC_REPEAT_ ## num (XGC_UNPAR(X))

	#define XGC_REPEAT_BASE(num, X, base)            XGC_REPEAT_BASE_ ## num \
		(XGC_UNPAR(X),XGC_UNPAR(base))
	#define XGC_REPEAT_BASE_Z(num, X, base)          X(0,base) XGC_REPEAT_BASE_ ## num \
		(XGC_UNPAR(X), XGC_UNPAR(base))

	#define XGC_IF(num,X)                            XGC_IF_ ## num (XGC_UNPAR(X))

	#define XGC_MAIN_REPEAT(num, X)                  XGC_MAIN_REPEAT_ ## num (XGC_UNPAR(X))
	#define XGC_MAIN_REPEAT_Z(num, X)                X(0) XGC_MAIN_REPEAT_ ## num (XGC_UNPAR(X))
	#define XGC_COMMA                                XGC_UNPAR(,)
	#define XGC_SPACE 
	#define XGC_COMMA_IF(num)                        XGC_IF(num,XGC_COMMA)

	#define XGC_UNPAR(...) __VA_ARGS__

	#define XGC_ENUM_0(X, INTER)  
	#define XGC_ENUM_1(X, INTER)  X##1
	#define XGC_ENUM_2(X, INTER)  XGC_ENUM_1(X,  XGC_UNPAR(INTER)) INTER X##2
	#define XGC_ENUM_3(X, INTER)  XGC_ENUM_2(X,  XGC_UNPAR(INTER)) INTER X##3
	#define XGC_ENUM_4(X, INTER)  XGC_ENUM_3(X,  XGC_UNPAR(INTER)) INTER X##4
	#define XGC_ENUM_5(X, INTER)  XGC_ENUM_4(X,  XGC_UNPAR(INTER)) INTER X##5
	#define XGC_ENUM_6(X, INTER)  XGC_ENUM_5(X,  XGC_UNPAR(INTER)) INTER X##6
	#define XGC_ENUM_7(X, INTER)  XGC_ENUM_6(X,  XGC_UNPAR(INTER)) INTER X##7
	#define XGC_ENUM_8(X, INTER)  XGC_ENUM_7(X,  XGC_UNPAR(INTER)) INTER X##8
	#define XGC_ENUM_9(X, INTER)  XGC_ENUM_8(X,  XGC_UNPAR(INTER)) INTER X##9
	#define XGC_ENUM_10(X, INTER) XGC_ENUM_9(X,  XGC_UNPAR(INTER)) INTER X##10
	#define XGC_ENUM_11(X, INTER) XGC_ENUM_10(X, XGC_UNPAR(INTER)) INTER X##11
	#define XGC_ENUM_12(X, INTER) XGC_ENUM_11(X, XGC_UNPAR(INTER)) INTER X##12
	#define XGC_ENUM_13(X, INTER) XGC_ENUM_12(X, XGC_UNPAR(INTER)) INTER X##13
	#define XGC_ENUM_14(X, INTER) XGC_ENUM_13(X, XGC_UNPAR(INTER)) INTER X##14
	#define XGC_ENUM_15(X, INTER) XGC_ENUM_14(X, XGC_UNPAR(INTER)) INTER X##15
	#define XGC_ENUM_16(X, INTER) XGC_ENUM_15(X, XGC_UNPAR(INTER)) INTER X##16
	#define XGC_ENUM_17(X, INTER) XGC_ENUM_16(X, XGC_UNPAR(INTER)) INTER X##17
	#define XGC_ENUM_18(X, INTER) XGC_ENUM_17(X, XGC_UNPAR(INTER)) INTER X##18
	#define XGC_ENUM_19(X, INTER) XGC_ENUM_18(X, XGC_UNPAR(INTER)) INTER X##19
	#define XGC_ENUM_20(X, INTER) XGC_ENUM_19(X, XGC_UNPAR(INTER)) INTER X##20
	#define XGC_ENUM_21(X, INTER) XGC_ENUM_20(X, XGC_UNPAR(INTER)) INTER X##21
	#define XGC_ENUM_22(X, INTER) XGC_ENUM_21(X, XGC_UNPAR(INTER)) INTER X##22
	#define XGC_ENUM_23(X, INTER) XGC_ENUM_22(X, XGC_UNPAR(INTER)) INTER X##23
	#define XGC_ENUM_24(X, INTER) XGC_ENUM_23(X, XGC_UNPAR(INTER)) INTER X##24
	#define XGC_ENUM_25(X, INTER) XGC_ENUM_24(X, XGC_UNPAR(INTER)) INTER X##25
	#define XGC_ENUM_26(X, INTER) XGC_ENUM_25(X, XGC_UNPAR(INTER)) INTER X##26
	#define XGC_ENUM_27(X, INTER) XGC_ENUM_26(X, XGC_UNPAR(INTER)) INTER X##27
	#define XGC_ENUM_28(X, INTER) XGC_ENUM_27(X, XGC_UNPAR(INTER)) INTER X##28
	#define XGC_ENUM_29(X, INTER) XGC_ENUM_28(X, XGC_UNPAR(INTER)) INTER X##29
	#define XGC_ENUM_30(X, INTER) XGC_ENUM_29(X, XGC_UNPAR(INTER)) INTER X##30
	#define XGC_ENUM_31(X, INTER) XGC_ENUM_30(X, XGC_UNPAR(INTER)) INTER X##31
	#define XGC_ENUM_32(X, INTER) XGC_ENUM_31(X, XGC_UNPAR(INTER)) INTER X##32
	#define XGC_ENUM_MAX(X, INTER) XGC_ENUM_10(X, INTER) /* Change this up to 32 */

	#define XGC_PARAM_0(X, P, INTER)  
	#define XGC_PARAM_1(X, P, INTER)  X##1 P##1
	#define XGC_PARAM_2(X, P, INTER)  XGC_PARAM_1(X, P,  XGC_UNPAR(INTER)) INTER X##2 P##2
	#define XGC_PARAM_3(X, P, INTER)  XGC_PARAM_2(X, P,  XGC_UNPAR(INTER)) INTER X##3 P##3
	#define XGC_PARAM_4(X, P, INTER)  XGC_PARAM_3(X, P,  XGC_UNPAR(INTER)) INTER X##4 P##4
	#define XGC_PARAM_5(X, P, INTER)  XGC_PARAM_4(X, P,  XGC_UNPAR(INTER)) INTER X##5 P##5
	#define XGC_PARAM_6(X, P, INTER)  XGC_PARAM_5(X, P,  XGC_UNPAR(INTER)) INTER X##6 P##6
	#define XGC_PARAM_7(X, P, INTER)  XGC_PARAM_6(X, P,  XGC_UNPAR(INTER)) INTER X##7 P##7
	#define XGC_PARAM_8(X, P, INTER)  XGC_PARAM_7(X, P,  XGC_UNPAR(INTER)) INTER X##8 P##8
	#define XGC_PARAM_9(X, P, INTER)  XGC_PARAM_8(X, P,  XGC_UNPAR(INTER)) INTER X##9 P##9
	#define XGC_PARAM_10(X, P, INTER) XGC_PARAM_9(X, P,  XGC_UNPAR(INTER)) INTER X##10 P##10
	#define XGC_PARAM_11(X, P, INTER) XGC_PARAM_10(X, P, XGC_UNPAR(INTER)) INTER X##11 P##11
	#define XGC_PARAM_12(X, P, INTER) XGC_PARAM_11(X, P, XGC_UNPAR(INTER)) INTER X##12 P##12
	#define XGC_PARAM_13(X, P, INTER) XGC_PARAM_12(X, P, XGC_UNPAR(INTER)) INTER X##13 P##13
	#define XGC_PARAM_14(X, P, INTER) XGC_PARAM_13(X, P, XGC_UNPAR(INTER)) INTER X##14 P##14
	#define XGC_PARAM_15(X, P, INTER) XGC_PARAM_14(X, P, XGC_UNPAR(INTER)) INTER X##15 P##15
	#define XGC_PARAM_16(X, P, INTER) XGC_PARAM_15(X, P, XGC_UNPAR(INTER)) INTER X##16 P##16
	#define XGC_PARAM_17(X, P, INTER) XGC_PARAM_16(X, P, XGC_UNPAR(INTER)) INTER X##17 P##17
	#define XGC_PARAM_18(X, P, INTER) XGC_PARAM_17(X, P, XGC_UNPAR(INTER)) INTER X##18 P##18
	#define XGC_PARAM_19(X, P, INTER) XGC_PARAM_18(X, P, XGC_UNPAR(INTER)) INTER X##19 P##19
	#define XGC_PARAM_20(X, P, INTER) XGC_PARAM_19(X, P, XGC_UNPAR(INTER)) INTER X##20 P##20
	#define XGC_PARAM_21(X, P, INTER) XGC_PARAM_20(X, P, XGC_UNPAR(INTER)) INTER X##21 P##21
	#define XGC_PARAM_22(X, P, INTER) XGC_PARAM_21(X, P, XGC_UNPAR(INTER)) INTER X##22 P##22
	#define XGC_PARAM_23(X, P, INTER) XGC_PARAM_22(X, P, XGC_UNPAR(INTER)) INTER X##23 P##23
	#define XGC_PARAM_24(X, P, INTER) XGC_PARAM_23(X, P, XGC_UNPAR(INTER)) INTER X##24 P##24
	#define XGC_PARAM_25(X, P, INTER) XGC_PARAM_24(X, P, XGC_UNPAR(INTER)) INTER X##25 P##25
	#define XGC_PARAM_26(X, P, INTER) XGC_PARAM_25(X, P, XGC_UNPAR(INTER)) INTER X##26 P##26
	#define XGC_PARAM_27(X, P, INTER) XGC_PARAM_26(X, P, XGC_UNPAR(INTER)) INTER X##27 P##27
	#define XGC_PARAM_28(X, P, INTER) XGC_PARAM_27(X, P, XGC_UNPAR(INTER)) INTER X##28 P##28
	#define XGC_PARAM_29(X, P, INTER) XGC_PARAM_28(X, P, XGC_UNPAR(INTER)) INTER X##29 P##29
	#define XGC_PARAM_30(X, P, INTER) XGC_PARAM_29(X, P, XGC_UNPAR(INTER)) INTER X##30 P##30
	#define XGC_PARAM_31(X, P, INTER) XGC_PARAM_30(X, P, XGC_UNPAR(INTER)) INTER X##31 P##31
	#define XGC_PARAM_32(X, P, INTER) XGC_PARAM_31(X, P, XGC_UNPAR(INTER)) INTER X##32 P##32
	#define XGC_PARAM_MAX(X, P, INTER) XGC_PARAM_10(X, P, INTER) /* Change this up to 32 */

	#define XGC_REPEAT_0(X) 
	#define XGC_REPEAT_1(X) X(1)
	#define XGC_REPEAT_2(X) XGC_REPEAT_1(X) X(2) 
	#define XGC_REPEAT_3(X) XGC_REPEAT_2(X) X(3) 
	#define XGC_REPEAT_4(X) XGC_REPEAT_3(X) X(4) 
	#define XGC_REPEAT_5(X) XGC_REPEAT_4(X) X(5) 
	#define XGC_REPEAT_6(X) XGC_REPEAT_5(X) X(6) 
	#define XGC_REPEAT_7(X) XGC_REPEAT_6(X) X(7) 
	#define XGC_REPEAT_8(X) XGC_REPEAT_7(X) X(8) 
	#define XGC_REPEAT_9(X) XGC_REPEAT_8(X) X(9) 
	#define XGC_REPEAT_10(X) XGC_REPEAT_9(X) X(10) 
	#define XGC_REPEAT_11(X) XGC_REPEAT_10(X) X(11) 
	#define XGC_REPEAT_12(X) XGC_REPEAT_11(X) X(12) 
	#define XGC_REPEAT_13(X) XGC_REPEAT_12(X) X(13) 
	#define XGC_REPEAT_14(X) XGC_REPEAT_13(X) X(14) 
	#define XGC_REPEAT_15(X) XGC_REPEAT_14(X) X(15) 
	#define XGC_REPEAT_16(X) XGC_REPEAT_15(X) X(16) 
	#define XGC_REPEAT_17(X) XGC_REPEAT_16(X) X(17) 
	#define XGC_REPEAT_18(X) XGC_REPEAT_17(X) X(18) 
	#define XGC_REPEAT_19(X) XGC_REPEAT_18(X) X(19) 
	#define XGC_REPEAT_20(X) XGC_REPEAT_19(X) X(20) 
	#define XGC_REPEAT_21(X) XGC_REPEAT_20(X) X(21) 
	#define XGC_REPEAT_22(X) XGC_REPEAT_21(X) X(22) 
	#define XGC_REPEAT_23(X) XGC_REPEAT_22(X) X(23) 
	#define XGC_REPEAT_24(X) XGC_REPEAT_23(X) X(24) 
	#define XGC_REPEAT_25(X) XGC_REPEAT_24(X) X(25) 
	#define XGC_REPEAT_26(X) XGC_REPEAT_25(X) X(26) 
	#define XGC_REPEAT_27(X) XGC_REPEAT_26(X) X(27) 
	#define XGC_REPEAT_28(X) XGC_REPEAT_27(X) X(28) 
	#define XGC_REPEAT_29(X) XGC_REPEAT_28(X) X(29) 
	#define XGC_REPEAT_30(X) XGC_REPEAT_29(X) X(30) 
	#define XGC_REPEAT_31(X) XGC_REPEAT_30(X) X(31) 
	#define XGC_REPEAT_32(X) XGC_REPEAT_31(X) X(32) 
	#define XGC_REPEAT_MAX(X) XGC_REPEAT_10(X)  /* Change this up to 32 */

	#define XGC_REPEAT_BASE_0(X,base) 
	#define XGC_REPEAT_BASE_1(X,base) X(1, base)
	#define XGC_REPEAT_BASE_2(X,base) XGC_REPEAT_BASE_1(X,base) X(2, base) 
	#define XGC_REPEAT_BASE_3(X,base) XGC_REPEAT_BASE_2(X,base) X(3, base) 
	#define XGC_REPEAT_BASE_4(X,base) XGC_REPEAT_BASE_3(X,base) X(4, base) 
	#define XGC_REPEAT_BASE_5(X,base) XGC_REPEAT_BASE_4(X,base) X(5, base) 
	#define XGC_REPEAT_BASE_6(X,base) XGC_REPEAT_BASE_5(X,base) X(6, base) 
	#define XGC_REPEAT_BASE_7(X,base) XGC_REPEAT_BASE_6(X,base) X(7, base) 
	#define XGC_REPEAT_BASE_8(X,base) XGC_REPEAT_BASE_7(X,base) X(8, base) 
	#define XGC_REPEAT_BASE_9(X,base) XGC_REPEAT_BASE_8(X,base) X(9, base) 
	#define XGC_REPEAT_BASE_10(X,base) XGC_REPEAT_BASE_9(X,base) X(10, base) 
	#define XGC_REPEAT_BASE_11(X,base) XGC_REPEAT_BASE_10(X,base) X(11, base) 
	#define XGC_REPEAT_BASE_12(X,base) XGC_REPEAT_BASE_11(X,base) X(12, base) 
	#define XGC_REPEAT_BASE_13(X,base) XGC_REPEAT_BASE_12(X,base) X(13, base) 
	#define XGC_REPEAT_BASE_14(X,base) XGC_REPEAT_BASE_13(X,base) X(14, base) 
	#define XGC_REPEAT_BASE_15(X,base) XGC_REPEAT_BASE_14(X,base) X(15, base) 
	#define XGC_REPEAT_BASE_16(X,base) XGC_REPEAT_BASE_15(X,base) X(16, base) 
	#define XGC_REPEAT_BASE_17(X,base) XGC_REPEAT_BASE_16(X,base) X(17, base) 
	#define XGC_REPEAT_BASE_18(X,base) XGC_REPEAT_BASE_17(X,base) X(18, base) 
	#define XGC_REPEAT_BASE_19(X,base) XGC_REPEAT_BASE_18(X,base) X(19, base) 
	#define XGC_REPEAT_BASE_20(X,base) XGC_REPEAT_BASE_19(X,base) X(20, base) 
	#define XGC_REPEAT_BASE_21(X,base) XGC_REPEAT_BASE_20(X,base) X(21, base) 
	#define XGC_REPEAT_BASE_22(X,base) XGC_REPEAT_BASE_21(X,base) X(22, base) 
	#define XGC_REPEAT_BASE_23(X,base) XGC_REPEAT_BASE_22(X,base) X(23, base) 
	#define XGC_REPEAT_BASE_24(X,base) XGC_REPEAT_BASE_23(X,base) X(24, base) 
	#define XGC_REPEAT_BASE_25(X,base) XGC_REPEAT_BASE_24(X,base) X(25, base) 
	#define XGC_REPEAT_BASE_26(X,base) XGC_REPEAT_BASE_25(X,base) X(26, base) 
	#define XGC_REPEAT_BASE_27(X,base) XGC_REPEAT_BASE_26(X,base) X(27, base) 
	#define XGC_REPEAT_BASE_28(X,base) XGC_REPEAT_BASE_27(X,base) X(28, base) 
	#define XGC_REPEAT_BASE_29(X,base) XGC_REPEAT_BASE_28(X,base) X(29, base) 
	#define XGC_REPEAT_BASE_30(X,base) XGC_REPEAT_BASE_29(X,base) X(30, base) 
	#define XGC_REPEAT_BASE_31(X,base) XGC_REPEAT_BASE_30(X,base) X(31, base) 
	#define XGC_REPEAT_BASE_32(X,base) XGC_REPEAT_BASE_31(X,base) X(32, base) 
	#define XGC_REPEAT_BASE_MAX(X,base) XGC_REPEAT_BASE_10(X,base)  /* Change this up to 32 */

	#define XGC_MAIN_REPEAT_1(X) X(1)
	#define XGC_MAIN_REPEAT_2(X) XGC_MAIN_REPEAT_1(X) X(2) 
	#define XGC_MAIN_REPEAT_3(X) XGC_MAIN_REPEAT_2(X) X(3) 
	#define XGC_MAIN_REPEAT_4(X) XGC_MAIN_REPEAT_3(X) X(4) 
	#define XGC_MAIN_REPEAT_5(X) XGC_MAIN_REPEAT_4(X) X(5) 
	#define XGC_MAIN_REPEAT_6(X) XGC_MAIN_REPEAT_5(X) X(6) 
	#define XGC_MAIN_REPEAT_7(X) XGC_MAIN_REPEAT_6(X) X(7) 
	#define XGC_MAIN_REPEAT_8(X) XGC_MAIN_REPEAT_7(X) X(8) 
	#define XGC_MAIN_REPEAT_9(X) XGC_MAIN_REPEAT_8(X) X(9) 
	#define XGC_MAIN_REPEAT_10(X) XGC_MAIN_REPEAT_9(X) X(10) 
	#define XGC_MAIN_REPEAT_11(X) XGC_MAIN_REPEAT_10(X) X(11) 
	#define XGC_MAIN_REPEAT_12(X) XGC_MAIN_REPEAT_11(X) X(12) 
	#define XGC_MAIN_REPEAT_13(X) XGC_MAIN_REPEAT_12(X) X(13) 
	#define XGC_MAIN_REPEAT_14(X) XGC_MAIN_REPEAT_13(X) X(14) 
	#define XGC_MAIN_REPEAT_15(X) XGC_MAIN_REPEAT_14(X) X(15) 
	#define XGC_MAIN_REPEAT_16(X) XGC_MAIN_REPEAT_15(X) X(16) 
	#define XGC_MAIN_REPEAT_17(X) XGC_MAIN_REPEAT_16(X) X(17) 
	#define XGC_MAIN_REPEAT_18(X) XGC_MAIN_REPEAT_17(X) X(18) 
	#define XGC_MAIN_REPEAT_19(X) XGC_MAIN_REPEAT_18(X) X(19) 
	#define XGC_MAIN_REPEAT_20(X) XGC_MAIN_REPEAT_19(X) X(20) 
	#define XGC_MAIN_REPEAT_21(X) XGC_MAIN_REPEAT_20(X) X(21) 
	#define XGC_MAIN_REPEAT_22(X) XGC_MAIN_REPEAT_21(X) X(22) 
	#define XGC_MAIN_REPEAT_23(X) XGC_MAIN_REPEAT_22(X) X(23) 
	#define XGC_MAIN_REPEAT_24(X) XGC_MAIN_REPEAT_23(X) X(24) 
	#define XGC_MAIN_REPEAT_25(X) XGC_MAIN_REPEAT_24(X) X(25) 
	#define XGC_MAIN_REPEAT_26(X) XGC_MAIN_REPEAT_25(X) X(26) 
	#define XGC_MAIN_REPEAT_27(X) XGC_MAIN_REPEAT_26(X) X(27) 
	#define XGC_MAIN_REPEAT_28(X) XGC_MAIN_REPEAT_27(X) X(28) 
	#define XGC_MAIN_REPEAT_29(X) XGC_MAIN_REPEAT_28(X) X(29) 
	#define XGC_MAIN_REPEAT_30(X) XGC_MAIN_REPEAT_29(X) X(30) 
	#define XGC_MAIN_REPEAT_31(X) XGC_MAIN_REPEAT_30(X) X(31) 
	#define XGC_MAIN_REPEAT_32(X) XGC_MAIN_REPEAT_31(X) X(32) 
	#define XGC_MAIN_REPEAT_MAX(X) XGC_MAIN_REPEAT_10(X)  /* Change this up to 32 */

	#define XGC_IF_0(X) 
	#define XGC_IF_1(X) X
	#define XGC_IF_2(X) X
	#define XGC_IF_3(X) X
	#define XGC_IF_4(X) X
	#define XGC_IF_5(X) X
	#define XGC_IF_6(X) X
	#define XGC_IF_7(X) X
	#define XGC_IF_8(X) X
	#define XGC_IF_9(X) X
	#define XGC_IF_10(X) X
	#define XGC_IF_11(X) X
	#define XGC_IF_12(X) X
	#define XGC_IF_13(X) X
	#define XGC_IF_14(X) X
	#define XGC_IF_15(X) X
	#define XGC_IF_16(X) X
	#define XGC_IF_17(X) X
	#define XGC_IF_18(X) X
	#define XGC_IF_19(X) X
	#define XGC_IF_20(X) X
	#define XGC_IF_21(X) X
	#define XGC_IF_22(X) X
	#define XGC_IF_23(X) X
	#define XGC_IF_24(X) X
	#define XGC_IF_25(X) X
	#define XGC_IF_26(X) X
	#define XGC_IF_27(X) X
	#define XGC_IF_28(X) X
	#define XGC_IF_29(X) X
	#define XGC_IF_30(X) X
	#define XGC_IF_31(X) X
	#define XGC_IF_32(X) X
	#define XGC_IF_MAX(X) XGC_IF_10(X) /* Change this up to 32 */

#endif // _COMMON_SPP_H_
