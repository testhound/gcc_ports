
/* Definitions of target machine for Mindspeed MXT400 Network Processor

   Copyright (C) 1991, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002
   Free Software Foundation, Inc.
   Contributed by Michael Collison and K. Kiran Kumar

   This file is part of GNU CC.

   GNU CC is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   GNU CC is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GNU CC; see the file COPYING.  If not, write to
   the Free Software Foundation, 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA. */

/*
 * 
 * DESCRIPTION: 
 * 
 *
 *  `u' for pointer to insns(==e)
 *  `e' for expression, and `E' for vector of expressions
 *  `i' for integer
 *  `w' for wide
 *  `s' for string,	   `S` for optional string
 *
 *
 * Modes:
 *  `QImode'     "Quarter-Integer" mode represents a single byte integer.
 *  `HImode'     "Half-Integer" mode represents a two-byte integer.
 *  `SImode'     "Single Integer" mode represents a four-byte integer.
 *  `SFmode'     "Single Floating" mode represents a four-byte integer.
 *  `DFmode'     "Double Floating" mode also represents a four-byte integer.
 *  `DImode'     "Double Integer" mode represents an eight-byte integer.
 *
 * Constraints:
 *  =   write only
 *  +   read & write
 *  %   communative
 *   i  immediate integer
 *   m  memory operand
 *   n  known numeric constant at compile time
 *   o  offsettable operand
 *   V  not offsettable operand
 *   p  any memory address (need address_operand predicate)
 *   r  general register
 *  
 * Constant Code Usage:
 *   I  10bsign	constant most insns can contain.
 *   J  0	constant zero.
 *   K  16bu	constant in LIL insn
 *   L  16bs	signed constant in LIL insn
 *   M  16bu	constant in LIH insn.
 *   N  6bu     unsigned 6-bit constant for add & sub
 *   O          mod mask constant
 *
 * Output template substitution codes:
 *
 *  GCC "standard" usages (See [Stallman] sect. 16.4)
 *   ('#' below stands for a digit, designating operand ordinal number)
 *  %#	operand number # as-is (detail interpreted in print_operand)
 *  %c# constant value without syntax that normally indicates an immediate operand.
 *  %n# like %c# except that the value of the constant is negated
 *  %a# substitutes an operand as if it were a memory reference
 *  %l# substitute a `label_ref' into a jump instruction.
 *  %=  outputs a number which is unique to each instruction
 *
 *  %t  emit 3 nops for tsetb or tclrb instructions if we couldn't fill any delay slots.
	The macro DBR_OUTPUT_SEQEND takes care of emitting nops when some or all of the delay
	slots have been filled.
 *
 *  Extra usages  (see implementation in mxt4400.c:print_operand(...);
 *    all take a digit designating operand ordinal number):
 *  %r  register name or the ZERO register if const. 0 in register context
 *  %N  1s complement of a constant
 *  %P  1<<C for a constant C
 *  %K  low 16 bits unsigned constant
 *  %L  low 16 bits signed constant  [Eh? --Pres, 7/1/99]
 *  %M  high order 16 bits of constant (e.g., LIH constant operand) [better as "hi16(%c#)" --Pres, 10/26/99]
 *  %O  modulus "name" (e.g., 0xff -> MOD512), iff const is form (2**n)-1
 *  %A  and-mask width value (0xff -> 8), the 'n' (0 .. 32) in the (2**n)-1 form const
 *  %C  an ESS condition operand  (e.g., NE written as "!ESS_EQ", GT as "ESS_GT", etc.)
 *  %D  a reversed-tested ESS condition operand  (e.g., reversed GT as "ESS_LT", etc.)
 *  %Y  memory value (usually handled by PRINT_OPERAND_ADDRESS(...)) [Eh? --Pres, 7/1/99]
 *  %P  5 bit unsigned constant(for tsetb and tclrb)
 *
 *  Special literal marks:
 *  %*  emit NOP (used for branch delay-slot if not optimized)
 *
 *
 *============================================================================
 * Calling convention:  (Lower addresses at bottom of diagram, stack grows down to 0)
 *  Caller's SP:
 *		incoming arg N-1             --+
 *		incoming arg N-2                \
 *		incoming arg ...                 \
 *		incoming arg 0                    \
 *         	pretend_args		(current_function_pretend_args_size)
 *		register saving r59	\           \
 *		register saving ...	- (sa_size)  \    [FRP==r39 saved w/ others]
 *		register saving r0 	/             - (frame_size)
 *		register saving RPC 	/            /
 *		local variables		- (size)    /
 * SP,FRP in fn:      ...                     -----+
 *	     ??	outgoing arg N-1	\
 *	     ??	outgoing arg N-2	- (current_function_outgoing_args_size)
 *	     ??	outgoing arg ...	/          [ARE THERE ANY OF THESE?]
 *	     ??	outgoing arg 0		/
 * next Callee's SP:
 *============================================================================
 */

#define CHIP_STRING2(val) #val
#define CHIP_STRING(val) CHIP_STRING2(val)

#ifndef MAKER_CHIP
#define MAKER_CHIP m480
#define UPPER_CHIP M480
#endif



#define ASM_SPEC "-I$MAKER_ROOT/$MAKER_REV/$MAKER_SYSTYPE/" CHIP_STRING(MAKER_CHIP) "-elf/include"

/* 
   Names to predefine in the preprocessor for this 
   target machine.  
*/
#define CPP_PREDEFINES  "-Dmaker -DMAKER -DTSP"

#ifdef __m480__
#define CPP_SPEC "\
%{!mmxt4400:%{!mmxt4600:%{!mm480:-D__m480__ -D__m27480__ -D__cx27480__ -Dm480 -D__M480__ -DM480 }}} \
%{mm480:-D__m480__ -Dm480 -D__M480__ -DM480 }"
#endif


/* 
   A C string constant that tells the GNU CC driver program options to 
   pass to `cc1'.  It can also specify how to translate options you 
   give to GNU CC into options for GNU CC to pass to the `cc1'. 
*/
#define CC1_SPEC "%{m480: -m480}"


/*   
   For MXT4nnn libc.a and libg.a are identical.  Note that liboct.a will 
   be in a different install subtree than libvegs.a   That might have 
   to be handled by the user with a 2-path definition in 
   MXT4400_LIBRARY_PATH 
*/
#define LIB_SPEC "-loct -lvregs -lc "

/*
  A C string constant that tells the GNU CC driver program options to
  pass to the linker.  It can also specify how to translate options
  you give to GNU CC into options for GNU CC to pass to the linker.
  (To use gcc.c `spec' syntax, each modified filename needs to be
  surrounded by space characters.  Thus, we use `-T <file>' rather
  than `--script=<file>' to get the `%s' effect.  --Pres, 6Jun2000)
*/
#define LINK_SPEC "%{!T: -T m480-c.mem%s}"

/*  
  Another C string constant used much like `LINK_SPEC'.  The
  difference between the two is that `STARTFILE_SPEC' is used at 
  the very beginning of the command given to the linker.
*/
#define STARTFILE_SPEC "crt0%O%s"


/* 
   Print subsidiary information on the compiler 
   version in use.  
*/
#define TARGET_VERSION fprintf (stderr, " (octave-" CHIP_STRING(MAKER_CHIP) ")");

#define GCC_EXEC_PREFIX_ENV_VAR CHIP_STRING(UPPER_CHIP) "_EXEC_PREFIX"
#define COMPILER_PATH_ENV_VAR   CHIP_STRING(UPPER_CHIP) "_COMPILER_PATH"
#define LIBRARY_PATH_ENV_VAR    CHIP_STRING(UPPER_CHIP) "_LIBRARY_PATH"
#define C_INCLUDE_PATH_ENV_VAR  CHIP_STRING(UPPER_CHIP) "_INCLUDE_PATH"

/* 
   Run-time compilation parameters selecting different 
   hardware subsets. 
*/
extern int target_flags;

#define MASK_DEBUG               0x00000001    
#define MASK_CODE_SDRAM          0x00000002   
#define MASK_DATA_SDRAM          0x00000004   
#define MASK_DATA_SRAM           0x00000008    
#define MASK_UNSAFE_SIGNED_CMP   0x00000010    
#define MASK_480                 0x00000020    

/* 
   Nonzero if we should generate long jumps for compares. 
*/
#define TARGET_LONG_JUMPS 1

/* 
   Debug GO_IF_LEGITIMATE_ADDRESS 
*/
#define TARGET_DEBUG_MODE          (target_flags & MASK_DEBUG)

/* 
   Emit code for m480 
*/
#define TARGET_480                 (target_flags & MASK_480)

/* 
   Assume .text section is in SDRAM 
*/
#define TARGET_CODE_IN_SDRAM       (target_flags & MASK_CODE_SDRAM)

/* 
   Assume .data is in SRAM instead of SDRAM 
*/
#define TARGET_DATA_IN_SDRAM       (target_flags & MASK_DATA_SDRAM)
 
/* 
   Assume .data is in SDRAM instead of SRAM 
*/
#define TARGET_DATA_IN_SRAM        (target_flags & MASK_DATA_SRAM)

/* 
   Don't emit code to deal with overflow in
   signed compares 
*/
#define TARGET_UNSAFE_SIGNED_COMPARE (target_flags & MASK_UNSAFE_SIGNED_CMP)

/* 
   Macro to define tables used to set the flags.
   This is a list in braces of pairs in braces,
   each pair being { "NAME", VALUE }
   where VALUE is the bits to set or minus the bits to clear.
   An empty string NAME is used to identify the default VALUE.  
*/
#define TARGET_SWITCHES                                                                                  \
{                                                                                                        \
  {"debug",                 MASK_DEBUG,             0},                                                  \
  {"480",                   MASK_480,               "Generate code for 480 target"},                     \
  {"data-in-sdram",         MASK_DATA_SDRAM,        "Assume data will reside in sdram section"},         \
  {"no-data-in-sdram",     -MASK_DATA_SDRAM,        "Don't assume data will reside in sdram section"},   \
  {"data-in-sram",          MASK_DATA_SRAM,         "Assume data will reside in sram section"},          \
  {"no-data-in-sram",      -MASK_DATA_SRAM,         "Don't assume data will reside in sram section"},    \
  {"unsafe-signed-compare", MASK_UNSAFE_SIGNED_CMP, "Don't emit compensation code for signed compares"}, \
  {"",                      TARGET_DEFAULT,         0}                                                   \
}

#define TARGET_OPTION_TRANSLATE_TABLE \
  { "--yydebug",   "-yydebug" }



/* 
   Generate code for 480 by default, 
   code in SDRAM, and data in SRAM 
*/
#define TARGET_DEFAULT (MASK_DATA_SRAM)

/*
  Target override options?
*/
#define OVERRIDE_OPTIONS m480_override_options ();



/*********************************/
/* Target machine storage layout */
/********************************/

/* 
   Define this if most significant bit is lowest numbered
   in instructions that operate on numbered bit-fields. 
*/
#define BITS_BIG_ENDIAN 0

/* 
   Define this if most significant byte of a word is the 
   lowest numbered.  
*/
#define BYTES_BIG_ENDIAN 1

/* 
   Define this if most significant word of a multiword number 
   is the lowest numbered.  
*/
#define WORDS_BIG_ENDIAN 1

/* 
   Number of bits in an addressable storage unit 
*/
#define BITS_PER_UNIT 8

/* 
   Width in bits of a "word", which is the contents of a 
   machine register. Note that this is not necessarily the 
   width of data type `int';
*/
#define BITS_PER_WORD 32

/* 
   Width of a word, in units (bytes).  
*/
#define UNITS_PER_WORD 4

/* 
   Width in bits of a pointer.
   See also the macro `Pmode' defined below.  
*/
#define POINTER_SIZE 32

/* 
   Allocation boundary (in *bits*) for storing arguments 
   in argument list.  
*/
#define PARM_BOUNDARY 32

/* 
   Boundary (in *bits*) on which stack pointer should be 
   aligned.  
*/
#define STACK_BOUNDARY 32

/* Allocation boundary (in *bits*) for the code of a 
   function.  
*/
#define FUNCTION_BOUNDARY 32

/* 
   Alignment of field after `int : 0' in a structure.  
*/
#define EMPTY_FIELD_BOUNDARY 32

/* 
   Every structure's size must be a multiple of this.  
*/
#define STRUCTURE_SIZE_BOUNDARY 32

/* 
   No data type wants to be aligned rounder than this.  
*/
#define BIGGEST_ALIGNMENT 32

/* 
   Max reg size: Avoid having to define DI 8 byte integers 
*/
#define MAX_FIXED_MODE_SIZE 64

/* 
   Make strings word-aligned so strcpy from constants 
   will be faster.  
*/
#define CONSTANT_ALIGNMENT(EXP, ALIGN)  \
  (TREE_CODE (EXP) == STRING_CST	\
   && (ALIGN) < BITS_PER_WORD ? BITS_PER_WORD : (ALIGN))

/* 
   If defined, a C expression to compute the alignment for 
   a static variable.  TYPE is the data type, and ALIGN is 
   the alignment that the object would ordinarily have.  
   The value of this macro is used instead of that alignment 
   to align the object.

   If this macro is not defined, then ALIGN is used.

   One use of this macro is to increase alignment of medium-size
   data to make it all fit in fewer cache lines.  Another is to
   cause character arrays to be word-aligned so that `strcpy' calls
   that copy constants to character arrays can be done inline.  
*/

#define DATA_ALIGNMENT(TYPE, ALIGN)					\
  ((((ALIGN) < BITS_PER_WORD)						\
    && (TREE_CODE (TYPE) == ARRAY_TYPE					\
	|| TREE_CODE (TYPE) == UNION_TYPE				\
	|| TREE_CODE (TYPE) == RECORD_TYPE)) ? BITS_PER_WORD : (ALIGN))

/* 
   Set this nonzero if move instructions will actually fail to 
   work when given unaligned data.  
*/
#define STRICT_ALIGNMENT 1

/* 
   A bitfield declared as `int' forces `int' alignment for the struct.  
*/
#define PCC_BITFIELD_TYPE_MATTERS 1

/* 
   Use C++ Thunks, they are faster 
*/
#define DEFAULT_VTABLE_THUNKS 1

/* 
   Make sizes standard 
*/

#define LONG_LONG_TYPE_SIZE 32
#define LONG_TYPE_SIZE 32
#define INT_TYPE_SIZE 32
#define SHORT_TYPE_SIZE 16
#define CHAR_TYPE_SIZE 8
#define FLOAT_TYPE_SIZE 32
#define DOUBLE_TYPE_SIZE 32
#define LONG_DOUBLE_TYPE_SIZE 32

/* 
   Modulo Arithmetic Constants
*/

#define LOW_MODULO_WIDTH 1 /* 2^1 -1 */
#define HIGH_MODULO_WIDTH 2147483647 /* 2^31 -1 */

/* 
   Define if you don't want extended real, but do want 
   to use the software floating point emulator for 
   REAL_ARITHMETIC and decimal <-> binary conversion. 
*/
#define REAL_ARITHMETIC 

/*****************************/
/* Standard register usage.  */
/*****************************/

/* 
   Number of actual hardware registers.  

   The hardware registers are assigned numbers 
   for the compiler from 0 to just below 
   FIRST_PSEUDO_REGISTER.

   All registers that the compiler knows about 
   must be given numbers, even those that are 
   not normally considered general registers.

   We define as hard registers (r0-r145)
    - 128 hardware registers,  
    - 12 special  bits in the ESS register,
    - special link registers (LNK1,LNK2,LNK3)
    - and 3 extra "fake" registers. 
       The 3 "fake" are per GCC specs
       and a desire to optimize frame pointer 
       non-usage where possible.  
*/

/* 
   Internal symbols, so that we don't need to refer 
   to a raw number later 
*/
#define A0_REGNUM	0
#define A1_REGNUM	1
#define A2_REGNUM	2
#define A3_REGNUM	3
#define A4_REGNUM	4
#define A5_REGNUM	5
#define A6_REGNUM	6
#define A7_REGNUM	7
#define AMAX_REGNUM	A7_REGNUM	
/* 
   Note: if AMAX!=A0+7, then need other changes 
*/

#define R0_REGNUM	8
#define R1_REGNUM	9
#define R2_REGNUM	10
#define R3_REGNUM	11
#define R4_REGNUM	12
#define R5_REGNUM	13
#define R6_REGNUM	14
#define R7_REGNUM	15
#define RMAX_REGNUM	R7_REGNUM	
/* 
   Note: if RMAX!=R0+7, then need other changes 
*/


/* 
   Temporary caller saved 
*/
#define TR0_REGNUM	16	
#define TR1_REGNUM	17	
#define TR2_REGNUM	18
#define TR3_REGNUM	19
#define TR4_REGNUM	20
#define TR5_REGNUM	21
#define TR6_REGNUM	22
#define TR7_REGNUM	23
#define TR8_REGNUM      24
#define TR9_REGNUM      25

/* 
   Temporary callee saved registers 
*/
#define TE0_REGNUM      26
#define TE1_REGNUM      27
#define TE2_REGNUM      28
#define TE3_REGNUM      29
#define TE4_REGNUM      30
#define TE5_REGNUM      31
#define TE6_REGNUM      32
#define TE7_REGNUM      33
#define TE8_REGNUM      34
#define TE9_REGNUM      35
#define TE10_REGNUM     36
#define TE11_REGNUM     37

#if 0
/* For C compiler only */
#define SP_REGNUM	38
#endif

#define FRP_REGNUM	39

/* 
   Global CCRam context bucket. 
   Not used by the compiler 
*/
#define CXL0_REGNUM     40
#define CXL1_REGNUM     41
#define CXL2_REGNUM     42
#define CXL3_REGNUM     43
#define CXL4_REGNUM     44
#define CXL5_REGNUM     45
#define CXL6_REGNUM     46
#define CXL7_REGNUM     47
#define CXL8_REGNUM     48
#define CXL9_REGNUM     49
#define CXL10_REGNUM    50
#define CXL11_REGNUM    51
#define CXL12_REGNUM    52
#define CXL13_REGNUM    53
#define CXL14_REGNUM    54
#define CXL15_REGNUM    55

/* 
   Used for Globals.
   Compiler does not use them.
*/
#define G0_REGNUM       56
#define G1_REGNUM       57
#define G2_REGNUM       58
#define G3_REGNUM       59

/*
  Return address Registers
*/
#define RPC_REGNUM	60
#define RPC1_REGNUM     61
#define RPC2_REGNUM     62

/*
  usage?
*/
#define RDBG_REGNUM     62

/* 
   Fake register, always ZERO 
*/
#define ZERO_REGNUM	63	

#define IS_GP_REG(REGNO) ((REGNO) >= 0 && (REGNO) < ZERO_REGNUM)

#define R_OCTCFG_REGNUM	        64
#define R_OCTPC_REGNUM	        65
#define R_OCTESS_REGNUM         66
#define R_OCTTRAP_REGNUM	67
#define R_OCTTRAPEN_REGNUM	68
#define R_OCTBRS_REGNUM 	69
#define R_OCTBRSLAST_REGNUM	70
#define R_OCTID_REGNUM  	71
#define R_REG72_REGNUM          72
#define R_OCTGLBITAG_REGNUM	73
#define R_OCTCTL_REGNUM 	74
#define R_OCTRTC0_REGNUM 	75
#define R_OCTRTC1_REGNUM	76
#define R_OCTRTCTL_REGNUM	77
#define R_OCTINTSAVE_REGNUM	78
#define R_OCTNMISAVE_REGNUM	79
#define R_OCTBMECFG_REGNUM	80
#define R_OCTBMENEXTBUF_REGNUM	81
#define R_OCTOAM_REGNUM 	82
#define R_OCTPIT0_REGNUM	83
#define R_OCTPIT1_REGNUM	84
#define R_OCTTRAPPC_REGNUM	85
#define R_OCTMASKEDTRAP_REGNUM	86
#define R_OCTINTSTATE_REGNUM	87
#define R_OCTNMISTATE_REGNUM    88
#define R_OCTGLOBAL0_REGNUM	89
#define R_OCTGLOBAL1_REGNUM	90
#define R_OCTGLOBAL2_REGNUM    	91
#define R_OCTGLOBAL3_REGNUM    	92
#define R_OCTRSVD93_REGNUM     	93
#define R_OCTRSVD94_REGNUM     	94
#define R_OCTDIAG_REGNUM	95
#define R_OCTLPTR0_REGNUM	96
#define R_OCTLPTR16_REGNUM	97
#define R_OCTLPTR32_REGNUM	98
#define R_OCTLPTR48_REGNUM	99
#define R_OCTLBASE1_REGNUM	100
#define R_OCTLBASE2_REGNUM	101
#define R_OCTLBASE3_REGNUM	102
#define R_OCTRSVD103_REGNUM    	103
#define R_OCTRSVD104_REGNUM    	104
#define R_OCTRANDL_REGNUM	105
#define R_OCTRANDH_REGNUM	106
#define R_OCTACTIVEID_REGNUM	107
#define R_OCTACTIVEID2_REGNUM	108
#define R_OCTACTIVEID3_REGNUM	109
#define R_OCTACTIVECID_REGNUM     	110
#define R_OCTACTIVECIDXOR_REGNUM	111
#define R_OCTPITCNT0_REGNUM     	112
#define R_OCTPITCNT1_REGNUM     	113
#define R_OCTPITROOL0_REGNUM    	114
#define R_OCTPITROLL1_REGNUM    	115
#define R_OCTRSVD116_REGNUM     	116
#define R_OCTRSVD117_REGNUM     	117
#define R_OCTRSVD118_REGNUM     	118
#define R_OCTRSVD119_REGNUM     	119
#define R_OCTRSVD120_REGNUM     	120
#define R_OCTRSVD121_REGNUM     	121
#define R_OCTRSVD122_REGNUM     	122
#define R_OCTRSVD123_REGNUM     	123
#define R_OCTRSVD124_REGNUM     	124
#define R_OCTRSVD125_REGNUM     	125
#define R_OCTRSVD126_REGNUM     	126
#define R_OCTRSVD127_REGNUM     	127

#define FIRST_HARDWARE_REGISTER         64
#define LAST_HARDWARE_REGISTER          127

/***************************************
   Extra hard(fake) registers defined
   to better explain hardware behaviour 
   of m480 in the machine description
****************************************/

/*
  Hard(fake) registers defined to represent
  special bits in ESS register
*/
#define ESS_PIT1_REGNUM         128
#define ESS_PIT0_REGNUM         129
#define ESS_BRALWAYS_REGNUM     130
#define ESS_SRDMARQFULL_REGNUM  131
#define ESS_PDMARQFULL_REGNUM   132
#define ESS_UDMA0RQFULL_REGNUM  133
#define ESS_UDMA1RQFULL_REGNUM  134
#define ESS_BMESWEPD_REGNUM     135
#define ESS_BMEHWEPD_REGNUM     136
#define ESS_BMEEXHAUST_REGNUM   137
#define ESS_INT_REGNUM          138
#define ESS_NMI_REGNUM          139
#define ESS_OVERFLOW_REGNUM     140
#define ESS_CARRY_REGNUM        141

/*
  Hard(fake) registers defined to represent
  implicit link mechanism of ALU instructions
*/
#define LNK1_REGNUM             142
#define LNK2_REGNUM             143
#define LNK3_REGNUM             144

 /* 
   Fake register, argument pointer 
*/
#define AP_REGNUM	        145	

/* 
   Fake register, return stack address pointer 
*/
#define RAP_REGNUM	        146	

/* 
   Fake register, temporary frame pointer 
*/
#define TFP_REGNUM	        147	

/*
  Fake register, imaginary stack pointer for assembly language optimizer. This register should never appear in assembly code and hence
  reload should not have to get involved. */

#define FAKE_SP_REGNUM          148
#define BASE_SDRAM_REGNUM       149
#define BASE_SRAM_REGNUM        150
#define BASE_CCRAM_REGNUM       151
#define BASE_CCRAM_LOCK_REGNUM  152
#define BASE_SIO_REGNUM         153
#define BASE_SPRAM_REGNUM       154
#define BASE_SPRAM_LOCK_REGNUM  155

#define TSS_GET_PTR             156
#define TSS_PUT_PTR             157

/*
  ------
  :NOTE:
  ------
  Be Careful with this macro,  need to be changed 
  whenever new registers are added to the above
  list.
 */
#define LAST_REGNUM             (TSS_PUT_PTR)


/* 
   Handy register names used in m480.md and this file
 */
#define ESS_REGNUM             R_OCTESS_REGNUM
#define BASE2_REGNUM           R_OCTLBASE2_REGNUM
#define BASE3_REGNUM           R_OCTLBASE3_REGNUM
#define SP_REGNUM              FAKE_SP_REGNUM

/*
  Starting register number allocated for all
  symbolic registers in ALO.
 */
#define FIRST_PSEUDO_REGISTER (LAST_REGNUM + 1)

/* 
   Derrived from above 
*/
#define ARG_NUM_REGS	(AMAX_REGNUM - A0_REGNUM + 1)



/* 
   As evident from the above register internal naming
   the following list shows the default calling convention
   followed by the C compiler.

   This default convention is overridden with user
   specified conventions per function/evh.
*/
   

/*
 *============================================================================
 * Hard(ware) Registers:
 *============================================================================
 * Register Usage:
 * 
 *   Reg #   Mnem      Comments                GCC    Action on function call
 *   r0-7   A0..A7    (nonsaved, input args)   use    unsaved
 *   r8-15  R0..R7    (function return values) use    saved (if pointer)
 *   r16-25 TR0..TR9  (temporary saved)        use    saved
 *   r26-37 TE0..TE11 (temporary unsaved)      use    unsaved; (callee saved)
 *   r38    SP        (stack pointer)          use*   saved
 *   r39    FRP       (frame pointer)          use*   saved (sort of)
 *   r<n>   AP        (Argument pointer)       eliminated unsaved
 *
 *  The following should never be allocated or modified directly by
 *  the compiler.   Access is via ehal interface or hand-coded assembler
 *
 *   r40-55 CXL0..CXL15 (globals, reserved)    rsv    special use only
 *   r56-59 G0..G3      (globals, reserved)    rsv    special use only
 * 
 *   r60,   RPC       (return PC)              use*   saved, return to this address
 *   r63,   ZERO      (always zero)            use*   can't change
 *   r64-147          (reserved)               rsv    special use only                      
 *============================================================================
 */

/* 
   '1' for registers that have pervasive standard uses
   and are not available for the register allocator. 
*/

/*
  SP_REGNUM (r38) -> fixed?
*/

#define FIXED_REGISTERS    \
 {                         \
  /* r0   */   0, 0, 0, 0, 0, 0, 0, 0, \
  /* r8   */   0, 0, 0, 0, 0, 0, 0, 0, \
  /* r16  */   0, 0, 0, 0, 0, 0, 0, 0, \
  /* r24  */   0, 0, 0, 0, 0, 0, 0, 0, \
  /* r32  */   0, 0, 0, 0, 0, 0, 1, 0, \
  /* r40  */   0, 0, 0, 0, 0, 0, 0, 0, \
  /* r48  */   0, 0, 0, 0, 0, 0, 0, 0, \
  /* r56  */   1, 1, 1, 1, 0, 0, 0, 1, \
  /* r64  */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r72  */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r80  */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r88  */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r96  */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r104 */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r112 */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r120 */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r128 */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r136 */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r144 */   1, 1, 1, 1, 1, 1, 1, 1, \
  /* r152 */   1, 1, 1, 1, 1, 1        \
 }


/* 
   1 for registers not available across function calls,
   i.e., assumed needing to be saved (if in use) before 
   a call. These must include the FIXED_REGISTERS and 
   also any registers that can NOT be used without being 
   saved before a call. The latter must include the registers 
   where values are returned and the register where 
   structure-value addresses are passed. Aside from that, you 
   can include as many other registers as you like.  

   On m480, this is all 

   An, 
   Rn, 
   TRn, 
   SP, 
   CXLn, 
   Gn, 
   RPC, 
   RPC1, 
   RDBG,
   ZERO, 
   Hardware registers, and the 
   3 "fake" regs(AP,RAP,TFP).  

   There are 0 below only for the CalleE
   saved registers, 
   
   TE0-TE11, 
   FRP

   If a register has 0 in `CALL_USED_REGISTERS', the compiler
   automatically saves it on function entry and restores it on
   function exit, if the register is used within the function. 
   In case of ALO this macro has to be dynamically changed per
   entry/evh.

   [used in regclass.c]
*/

#define CALL_USED_REGISTERS  \
 {                           \
  /* r0   */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r8   */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r16  */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r24  */   1, 1, 0, 0, 0, 0, 0, 0,   \
  /* r32  */   0, 0, 0, 0, 0, 0, 1, 0,   \
  /* r40  */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r48  */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r56  */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r64  */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r72  */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r80  */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r88  */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r96  */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r104 */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r112 */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r120 */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r128 */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r136 */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r144 */   1, 1, 1, 1, 1, 1, 1, 1,   \
  /* r152 */   1, 1, 1, 1, 1, 1          \
}

/* 
   List the order in which to allocate registers.  
   Each register must be listed once, even those 
   in FIXED_REGISTERS. Make sure this has [FIRST_PSEUDO_REGISTER] 
   number of entries; no one checks 

   [used inregclass.c]
   int reg_alloc_order[FIRST_PSEUDO_REGISTER] = REG_ALLOC_ORDER;]

*/




/* 
   Return number of consecutive hard regs needed starting at 
   reg REGNO to hold something of mode MODE.
   
   This is ordinarily the length in words of a value of 
   mode MODE but can be less for certain modes in special 
   long registers.
*/

#define HARD_REGNO_NREGS(REGNO, MODE)   \
  ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)

/* 
   Value is 1 if hard register REGNO can hold a value of 
   machine-mode MODE. The integer registers can hold any mode.  
*/

#define HARD_REGNO_MODE_OK(REGNO, MODE) m480_hard_regno_mode_ok (REGNO, MODE)

/* 
   Value is 1 if it is a good idea to tie two pseudo registers
   when one has mode MODE1 and one has mode MODE2.
   If HARD_REGNO_MODE_OK could produce different values for 
   MODE1 and MODE2, for any hard reg, then this must be 0 for 
   correct output.  
*/

#define MODES_TIEABLE_P(MODE1, MODE2) 1


/* 
   Register to use for pushing function arguments.  
*/
#define STACK_POINTER_REGNUM SP_REGNUM

/* 
   Base register for access to local variables of the function.  
*/
#define FRAME_POINTER_REGNUM TFP_REGNUM

/* 
   Define this to be where the real frame pointer is if it is 
   not possible to work out the offset between the frame 
   pointer and the automatic variables until after register 
   allocation has taken place.  FRAME_POINTER_REGNUM should 
   point to a special register that we will make sure is eliminated. 
*/
#define HARD_FRAME_POINTER_REGNUM TFP_REGNUM

/* 
   Value should be nonzero if functions must have frame pointers.
   Zero means the frame pointer need not be set up (and parms
   may be accessed via the stack pointer) in functions that seem suitable.
   This is computed in `reload', in reload1.c.  
*/
#define FRAME_POINTER_REQUIRED 0

/* 
   Force -fomit_frame_pointer if using -O 
*/
#define CAN_DEBUG_WITHOUT_FP

/* 
   Base register for access to arguments of the function.  
*/
#define ARG_POINTER_REGNUM AP_REGNUM

/* 
   Fake register that holds the address on the stack of the
   current function's return address.  
*/
#define RETURN_ADDRESS_POINTER_REGNUM RAP_REGNUM

/* 
   Register in which static-chain is passed to a function. 
   This is based on an example; the calling sequence
   doesn\'t seem to specify this.  
*/
#define STATIC_CHAIN_REGNUM R0_REGNUM

/* 
   If the structure value address is not passed in a  
   register, define `STRUCT_VALUE' as an expression 
   returning an RTX for the place where the address is 
   passed.  If it returns 0, the address is passed as 
   an "invisible" first argument.  
*/
#define STRUCT_VALUE 0


/* 
   Define the classes of registers for register constraints 
   in the machine description.  Also define ranges of constants.

   One of the classes must always be named ALL_REGS and include 
   all hard regs. If there is more than one class, another 
   class must be named NO_REGS and contain no registers.

   The name GENERAL_REGS must be the name of a class (or an alias for
   another name such as ALL_REGS).  This is the class of registers
   that is allowed by "g" or "r" in a register constraint.
   Also, registers outside this class are allocated only when
   instructions express preferences for them.

   The classes must be numbered in nondecreasing order; that is,
   a larger-numbered class must never be contained completely
   in a smaller-numbered class.

   For any two classes, it is very desirable that there be another
   class that represents their union.  */
   
enum reg_class 
{ 
  NO_REGS,
  /* 
     Registers 60-62; cannot be replaced by 
     CALL_REGS 
  */  
  LNK_REGS,
  SPL_ESS_REGS,
  RPC_REGS,     
  GENERAL_REGS,
  ZERO_REG,
  EVEN_REG,
  ODD_REG,
  SOFTWARE_REGS,
  HARDWARE_REGS,
  ALL_REGS, 
  LIM_REG_CLASSES
};

#define N_REG_CLASSES (int) LIM_REG_CLASSES

/* Give names of register classes as strings for dump file.   */

#define REG_CLASS_NAMES \
 {                      \
   "NO_REGS",           \
   "LNK_REGS",          \
   "SPL_ESS_REGS",      \
   "RPC_REGS",          \
   "GENERAL_REGS",      \
   "ZERO_REG",          \
   "EVEN_REG",          \
   "ODD_REG",           \
   "SOFTWARE_REGS",     \
   "HARDWARE_REGS",     \
   "ALL_REGS"           \
 }

/* 
   Define which registers fit in which classes.
   This is an initializer for a vector of HARD_REG_SET
   of length N_REG_CLASSES.  
*/


#define REG_CLASS_CONTENTS                                 \
{                                                          \
  { 0UL,           0UL,           0UL,  0UL,  0UL},        \
  { 0UL,           0UL,           0UL,  0UL,  0x0001c000}, \
  { 0UL,           0UL,           0UL,  0UL,  0x00003fff}, \
  { 0UL,           0x70000000UL,  0UL,  0UL,  0UL},        \
  {~0UL,           0x7fffffffUL,  0UL,  0UL,  0UL},        \
  { 0UL,           0x80000000UL,  0UL,  0UL,  0UL},        \
  { 0x55555555UL,  0x55555555UL,  0UL,  0UL,  0UL},        \
  { 0x2AAAAAAAUL,  0xAAAAAAAAUL,  0UL,  0UL,  0UL},        \
  {~0UL,          ~0UL,           0UL,  0UL,  0UL},        \
  { 0UL,           0UL,          ~0UL, ~0UL,  0UL},        \
  {~0UL,          ~0UL,          ~0UL, ~0UL,  0x3fffffff}  \
} 

/* 
   The same information, inverted:
   Return the class number of the smallest class containing
   reg number REGNO.  This could be a conditional expression
   or could index an array.  
*/
#define REGNO_REG_CLASS(REGNO)                                                \
  ( ((REGNO) >= RPC_REGNUM && (REGNO) <= RPC2_REGNUM) ? RPC_REGS              \
   : (REGNO) < ZERO_REGNUM ? GENERAL_REGS                                     \
   : ((REGNO) >= LNK1_REGNUM && (REGNO) <= LNK3_REGNUM) ? LNK_REGS            \
   : ((REGNO) >= ESS_PIT1_REGNUM && (REGNO) <= ESS_CARRY_REGNUM) ? SPL_ESS_REGS \
   : (REGNO) == ZERO_REGNUM ? ZERO_REG                                        \
   : ((REGNO) >= FIRST_HARDWARE_REGISTER &&                                   \
       (REGNO) <= LAST_HARDWARE_REGISTER) ? HARDWARE_REGS                     \
   : (REGNO) < FIRST_PSEUDO_REGISTER ? ALL_REGS                               \
   : NO_REGS)

/* 
   Get reg_class from a letter such as appears 
   in the machine description.  
*/

#define REG_CLASS_FROM_LETTER(C) \
  (  (C) == 'e' ? HARDWARE_REGS	 \
   : (C) == 'h' ? RPC_REGS       \
   : (C) == 'l' ? LNK_REGS       \
   : (C) == 'Z' ? ZERO_REG       \
   : NO_REGS)

/* 
   The class value for index registers, 
   and the one for base regs.  
*/
#define INDEX_REG_CLASS NO_REGS
#define BASE_REG_CLASS GENERAL_REGS

/*
  m,o,V,<,>,r,i,n,E,F,G,H,s,g,X,0-9,P
  All the above characters have GCC specific meaning.
  We are free to use any other letters to specify machine
  dependent constraints.
 */


#define CONST_OK_FOR_ZERO(VALUE)      (((VALUE) == 0))
#define CONST_OK_FOR_UI_1BIT(VALUE)   ((VALUE >= 0) && ((VALUE) < 1))
#define CONST_OK_FOR_UI_2BITS(VALUE)  ((VALUE >= 0) && ((VALUE) < 4))
#define CONST_OK_FOR_UI_3BITS(VALUE)  ((VALUE >= 0) && ((VALUE) < 8))
#define CONST_OK_FOR_UI_4BITS(VALUE)  ((VALUE >= 0) && ((VALUE) < 16))
#define CONST_OK_FOR_UI_5BITS(VALUE)  ((VALUE >= 0) && ((VALUE) < 32))
#define CONST_OK_FOR_UI_6BITS(VALUE)  ((VALUE >= 0) && ((VALUE) < 64))
#define CONST_OK_FOR_UI_7BITS(VALUE)  ((VALUE >= 0) && ((VALUE) < 128))
#define CONST_OK_FOR_SI_8BITS(VALUE)  (((VALUE) >= -256) || ((VALUE) <= 255)) 
#define CONST_OK_FOR_SI_9BITS(VALUE)  (((VALUE) >= -512) || ((VALUE) <= 511)) 
#define CONST_OK_FOR_UI_9BITS(VALUE)  ((VALUE >= 0) && ((VALUE) < 512))
#define CONST_OK_FOR_UI_10BITS(VALUE) ((VALUE >= 0) && ((VALUE) < 1024))
#define CONST_OK_FOR_SI_10BITS(VALUE) (((VALUE) >= -1024) || ((VALUE) <= 1023)) 
#define CONST_OK_FOR_UI_12BITS(VALUE) ((VALUE >= 0) && ((VALUE) <= 4095))
#define CONST_OK_FOR_UI_16BITS(VALUE) ((VALUE >= 0) && ((VALUE) <= 65535))
#define CONST_OK_FOR_UI_17BITS(VALUE) ((VALUE >= 0) && ((VALUE) <= 131071))

#define CONST_OK_FOR_LETTER_P(VALUE, C)				   \
   ( (C) == 'I' ? CONST_OK_FOR_ZERO(VALUE)			   \
   : (C) == 'J' ? CONST_OK_FOR_UI_1BIT(VALUE)                      \
   : (C) == 'K' ? CONST_OK_FOR_UI_3BITS(VALUE)                     \
   : (C) == 'L' ? CONST_OK_FOR_UI_5BITS(VALUE)                     \
   : (C) == 'M' ? CONST_OK_FOR_UI_6BITS(VALUE)                     \
   : (C) == 'N' ? CONST_OK_FOR_SI_8BITS(VALUE)                     \
   : (C) == 'O' ? CONST_OK_FOR_UI_9BITS(VALUE)                     \
   : (C) == 'P' ? CONST_OK_FOR_UI_6BITS(-VALUE)                    \
   : 0)

#define EXTRA_CONSTRAINT(OP,CODE)                                  \
     ( (CODE) == 'T' ? reg_or_10bs_operand (OP, SImode)            \
     : (CODE) == 'S' ? lnk_memory_operand (OP, GET_MODE(OP))       \
     : (CODE) == 'R' ? const_ok_for_load_p(OP, SImode)             \
                     : 0)
#define CONST_DOUBLE_OK_FOR_LETTER_P(VALUE, C)	(0)

/* 
   The machine description macro `EXTRA_CONSTRAINT' 
   is passed the operand as its first argument and 
   the constraint letter as its second operand.
*/

/* Define the codes that are matched by predicates  */

#define PREDICATE_CODES \
  {"any_operand", {CONST_INT, CONST_DOUBLE, CONST, SYMBOL_REF,	    \
		   LABEL_REF, SUBREG, REG, MEM, PARALLEL}},	    \
  {"ess_clobbering_plus_operand", {PLUS}},                          \
  {"s_register_operand",          {SUBREG, REG}},		    \
  {"any_memory_operand",          {MEM}},			    \
  {"symbol_operand",              {SYMBOL_REF, LABEL_REF, CONST}},  \
  {"symbol_or_12bu_operand",      {SYMBOL_REF, LABEL_REF, CONST,    \
                                   CONST_INT}},                     \
  {"symbol_or_16bu_operand",      {SYMBOL_REF, LABEL_REF, CONST,    \
                                   CONST_INT}},                     \
  {"symbol_or_17bu_operand",      {SYMBOL_REF, LABEL_REF, CONST,    \
                                   CONST_INT}},                     \
  {"call_operand",                {SYMBOL_REF, REG, CONST_INT}},    \
  {"lnk_register_operand",        {SUBREG,REG}},                    \
  {"spl_ess_reg_operand",         {SUBREG,REG}},                    \
  {"overflow_or_carry_ess_operand", {SUBREG,REG}},                  \
  {"hardware_register_operand",   {SUBREG,REG}},                    \
  {"move_operand",                {SUBREG, REG, CONST_INT, MEM}},   \
  {"reg_or_0_operand",            {SUBREG, REG, CONST_INT}},	    \
  {"reg_or_6bu_operand",          {SUBREG, REG, CONST_INT}},        \
  {"reg_or_6bs_operand",          {SUBREG, REG, CONST_INT}},        \
  {"reg_or_5bu_operand",          {SUBREG, REG, CONST_INT}},        \
  {"reg_or_10bs_operand",         {SUBREG, REG, CONST_INT}},        \
  {"reg_or_lim_operand",          {SUBREG, REG, CONST_INT}},        \
  {"spl_ess_comparison_operator", {EQ, NE}},	                    \
  {"load_multiple_operation",     {PARALLEL}},	                    \
  {"store_multiple_operation",    {PARALLEL}},                      \
  {"lim_operand",                 {CONST_INT}},                     \
  {"c1bu_operand",                {CONST_INT}},                     \
  {"c2bu_operand",                {CONST_INT}},                     \
  {"c3bu_operand",                {CONST_INT}},                     \
  {"c5bu_operand",                {CONST_INT}},                     \
  {"c6bu_operand",                {CONST_INT}},                     \
  {"c6bs_operand",                {CONST_INT}},                     \
  {"c8bs_operand",                {CONST_INT}},                     \
  {"c9bu_operand",                {CONST_INT}},                     \
  {"flag_operand",                {CONST_INT}},                     \
  {"get_flag_operand",            {CONST_INT}},                     \
  {"hec5_flag_operand",           {CONST_INT}},                     \
  {"putc_flag_operand",           {CONST_INT}},                     \
  {"putm_flag_operand",           {CONST_INT}},                     \
  {"putp_flag_operand",           {CONST_INT}},                     \
  {"lnk_offset_operand",          {CONST_INT}},                     \
  {"brsl_flag_operand",           {CONST_INT}},                     \
  {"nibble_operand",              {CONST_INT}},                     \
  {"mod_mask_operand",            {CONST_INT}},                     \
  {"modx_operand",                {CONST_INT}},                     \
  {"const_ok_for_load_p",         {CONST_INT}},                     \
  {"lnk_memory_operand",          {MEM}},

/*******************************************************************************/

/* 
   Given an rtx X being reloaded into a reg required to be
   in class CLASS, return the class of reg to actually use.
   In general this is just CLASS; but on some machines
   in some cases it is preferable to use a more restrictive class.  
*/
#define PREFERRED_RELOAD_CLASS(X,CLASS) (CLASS)

/* 
   Loading and storing HImode or QImode values to and from memory
   usually requires a scratch register.  The exceptions are loading
   QImode and HImode from an aligned address to a general register. 
*/

#define SECONDARY_INPUT_RELOAD_CLASS(CLASS,MODE,IN)			\
((((GET_CODE (IN) == MEM 						\
   || (GET_CODE (IN) == REG && REGNO (IN) >= FIRST_PSEUDO_REGISTER)	\
   || (GET_CODE (IN) == SUBREG						\
       && (GET_CODE (SUBREG_REG (IN)) == MEM				\
	   || (GET_CODE (SUBREG_REG (IN)) == REG			\
	       && REGNO (SUBREG_REG (IN)) >= FIRST_PSEUDO_REGISTER))))	\
  && ((((MODE) == QImode || (MODE) == HImode)			        \
         && ! octave_aligned_memory_operand (IN, MODE))))		\
  || (GET_CODE (IN) == PLUS						\
      && (XEXP (IN, 0) == stack_pointer_rtx				\
	  || XEXP (IN, 0) == frame_pointer_rtx)				\
      && (GET_CODE (XEXP (IN,1)) == CONST_INT				\
	  && !CONST_OK_FOR_UI_17BITS(INTVAL(XEXP (IN,1))))) )		\
 ? GENERAL_REGS								\
 : NO_REGS)


#define SECONDARY_OUTPUT_RELOAD_CLASS(CLASS,MODE,OUT)			\
(((GET_CODE (OUT) == MEM 						\
   || (GET_CODE (OUT) == REG && REGNO (OUT) >= FIRST_PSEUDO_REGISTER)	\
   || (GET_CODE (OUT) == SUBREG						\
       && (GET_CODE (SUBREG_REG (OUT)) == MEM				\
	   || (GET_CODE (SUBREG_REG (OUT)) == REG			\
	       && REGNO (SUBREG_REG (OUT)) >= FIRST_PSEUDO_REGISTER)))) \
  && ((MODE) == HImode || (MODE) == QImode))			        \
 ? GENERAL_REGS								\
 : NO_REGS)

/* 
   Return the maximum number of consecutive registers
   needed to represent mode MODE in a register of class CLASS.  
*/

#define CLASS_MAX_NREGS(CLASS, MODE)				\
 ((GET_MODE_SIZE (MODE) + UNITS_PER_WORD - 1) / UNITS_PER_WORD)


/**************************************************
   Stack layout; function entry, exit and calling.  
***************************************************/

/* 
   Define this if pushing a word on the stack
   makes the stack pointer a smaller address.  
*/
#define STACK_GROWS_DOWNWARD

/* 
   Define this if the nominal address of the stack frame
   is at the high-address end of the local variables;
   that is, each additional local variable allocated
   goes at a more negative offset in the frame.  
*/
/*
#define FRAME_GROWS_DOWNWARD
*/

/* 
   Offset within stack frame to start allocating local variables at.
   If FRAME_GROWS_DOWNWARD, this is the offset to the END of the
   first local allocated.  Otherwise, it is the offset to the BEGINNING
   of the first local allocated.  
*/
#define STARTING_FRAME_OFFSET 0

/* 
   If we generate an insn to push BYTES bytes,
   this says how many the stack pointer really advances by.
   On SPUR, don't define this because there are no push insns.  
*/
/* 
#define PUSH_ROUNDING(BYTES) 
*/

/* 
   Offset of first parameter from the argument pointer register value.  
*/
#define FIRST_PARM_OFFSET(FNDECL) 0

/* 
   Value is the number of bytes of arguments automatically
   popped when returning from a subroutine call.
   FUNDECL is the declaration node of the function (as a tree),
   FUNTYPE is the data type of the function (as a tree),
   or for a library call it is an identifier node for the subroutine name.
   SIZE is the number of bytes of arguments passed on the stack.  
*/

#define RETURN_POPS_ARGS(FUNDECL,FUNTYPE,SIZE) 0

/* 
   Define how to find the value returned by a function.
   VALTYPE is the data type of the value (as a tree).
   If the precise function being called is known, FUNC is its FUNCTION_DECL;
   otherwise, FUNC is 0.  
*/

#define FUNCTION_VALUE(VALTYPE, FUNC)  m480_function_value(VALTYPE, FUNC)

/* 
   Define how to find the value returned by a library function
   assuming the value has mode MODE.  
*/
#define LIBCALL_VALUE(MODE)  gen_rtx (REG, MODE, R0_REGNUM)

/* 
   1 if N is a possible register number for a function value
   as seen by the caller. 
*/
#define FUNCTION_VALUE_REGNO_P(N) ((N) >= R0_REGNUM && (N) <= RMAX_REGNUM)

/* 
   1 if N is a possible register number for function argument passing. 
*/
#define FUNCTION_ARG_REGNO_P(N) (((N) >= A0_REGNUM && (N) <= AMAX_REGNUM))

/* 
   Round up to a multiple of 4 bytes.  
*/
#define ROUND4(X) (((X) + 3) & ~ 3)

/* 
   Define this macro if an argument declared as `char' or `short' in a
   prototype should actually be passed as an `int'.  In addition to
   avoiding errors in certain cases of mismatch, it also makes for
   better code on certain machines. 
*/

#define PROMOTE_PROTOTYPES 1

/* 
   Define this macro if it is advisable to hold scalars in registers
   in a wider mode than that declared by the program.  In such cases, 
   the value is constrained to be within the bounds of the declared
   type, but kept valid in the wider mode.  The signedness of the
   extension may differ from that of the type.

   We promote any value smaller than SImode up to SImode. 
*/

#define PROMOTE_MODE(MODE, UNSIGNEDP, TYPE)	\
  if (GET_MODE_CLASS (MODE) == MODE_INT		\
      && GET_MODE_SIZE (MODE) < 4)		\
    (MODE) = SImode;

/* 
   Define this if function arguments should also be promoted using the above
   procedure.  
*/

#define PROMOTE_FUNCTION_ARGS

/* 
   Likewise, if the function return value is promoted.  
*/

#define PROMOTE_FUNCTION_RETURN

/* 
   The definition of this macro implies that there are cases where
   a scalar value cannot be returned in registers.

   Any structure is returned in memory 
*/

#define RETURN_IN_MEMORY(TYPE) 						\
  (TYPE_MODE ((TYPE)) == BLKmode && int_size_in_bytes (TYPE) > 32)

/* 
   Define this to be 1 if all structs and unions must be returned in memory. If you
   define this macro to be 0, then the conventions used to decide if structs/unions
   are returned in memory are decided by RETURN_IN_MEMORY 
*/
#define DEFAULT_PCC_STRUCT_RETURN 0


/* 
   Define a data type for recording info about an argument list
   during the scan of that argument list.  This data type should
   hold all necessary information about the function itself
   and about the args processed so far, enough to enable macros
   such as FUNCTION_ARG to determine where the next arg should go.
*/

#define CUMULATIVE_ARGS int

/* 
   Initialize a variable CUM of type CUMULATIVE_ARGS
   for a call to a function whose data type is FNTYPE.
   For a library call, FNTYPE is 0.
*/

#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,INDIRECT)  (CUM) = 0

/* 
   Define intermediate macro to compute the size (in registers) of an argument 
*/

#define OCTAVE_ARG_SIZE(MODE, TYPE, NAMED)				\
((MODE) != BLKmode							\
 ? (ROUND4(GET_MODE_SIZE (MODE))/4) 	                                \
 : (ROUND4(int_size_in_bytes (TYPE))/4))

/* 
   Update the data in CUM to advance over an argument
   of mode MODE and data type TYPE.
   (TYPE is null for libcalls where that information may not be available.)  
*/

#define FUNCTION_ARG_ADVANCE(CUM, MODE, TYPE, NAMED)			\
  if (MUST_PASS_IN_STACK (MODE, TYPE))					\
   (CUM) = ARG_NUM_REGS;	                                        \
  else									\
   (CUM) += OCTAVE_ARG_SIZE (MODE, TYPE, NAMED)

/* 
   Determine where to put an argument to a function.
   Value is zero to push the argument on the stack,
   or a hard register in which to store the argument.

   MODE is the argument's machine mode.
   TYPE is the data type of the argument (as a tree).
    This is null for libcalls where that information may
    not be available.
   CUM is a variable of type CUMULATIVE_ARGS which gives info about
    the preceding args and about the function being called.
   NAMED is nonzero if this argument is a named parameter
    (otherwise it is an extra parameter matching an ellipsis).  */

/* 
   Define where a function finds its arguments. 
*/

/* 
   If (NAMED == 0), as for vararg or stdarg ellipses, say argument is
   passed in the stack (function_prologue will indeed make it pass in the
   stack if necessary).  
*/

#define FUNCTION_ARG(CUM, MODE, TYPE, NAMED)	m480_function_arg(CUM, MODE, TYPE, NAMED)

/* 
   For an arg passed partly in registers and partly in memory,
   this is the number of registers used.
   For args passed entirely in registers or entirely in memory, zero.  
*/

#define FUNCTION_ARG_PARTIAL_NREGS(CUM, MODE, TYPE, NAMED)	                    \
((CUM) < ARG_NUM_REGS && ARG_NUM_REGS < (CUM) + OCTAVE_ARG_SIZE (MODE, TYPE, NAMED) \
                      && NAMED                                                      \
 ? ARG_NUM_REGS - (CUM) : 0)

/* 
   Specify the padding direction of arguments.

   We must pad upwards in order to be able to pass args in
   registers.  
*/

#define FUNCTION_ARG_PADDING(MODE, TYPE)	upward



/* 
   This macro generates the assembly code for function entry.
   FILE is a stdio stream to output the code to.
   SIZE is an int: how many units of temporary storage to allocate.
   Refer to the array `regs_ever_live' to determine which registers
   to save; `regs_ever_live[I]' is nonzero if register number I
   is ever used in the function.  This macro is responsible for
   knowing which registers should not be saved even if used.  
*/

/* 
   Output assembler code to FILE to increment profiler label # LABELNO
   for profiling a function entry.  
*/

#define FUNCTION_PROFILER(FILE, LABELNO) {                           \
   fprintf (stderr, "FUNCTION_PROFILER; Profiling not supported\n"); \
   abort (); \
   }

/* 
   EXIT_IGNORE_STACK should be nonzero if, when returning from a function,
   the stack pointer does not matter.  The value is tested only in
   functions that have frame pointers.
   No definition is equivalent to always zero.  
*/

#define EXIT_IGNORE_STACK  1

/* 
   This macro generates the assembly code for function exit,
   on machines that need it.  If FUNCTION_EPILOGUE is not defined
   then individual return instructions are generated for each
   return statement.  Args are same as for FUNCTION_PROLOGUE.

   The function epilogue should not depend on the current stack pointer!
   It should use the frame pointer only.  This is mandatory because
   of alloca; we also take advantage of it to omit stack adjustments
   before returning.  
*/


/* 
   Temps for building stack offsets 
*/

/* 
   Commented out the following C specific macros as
   We do not have epilogue in ALO.
 */

/* #define EPILOGUE_USES(REGNO) ((REGNO)==TR0_REGNUM || (REGNO)==TR1_REGNUM) */


/* #define DELAY_SLOTS_FOR_EPILOGUE 1 */

/* #define ELIGIBLE_FOR_EPILOGUE_DELAY(trial, slots_filled) \ */
/*   (octave_eligible_for_epilogue_delay (trial, slots_filled)) */


/* 
   Using the 'bcdi' the m27480 can have two commited slots 
*/

#define MAX_CBRANCH_DELAY_SLOTS  2



/* 
   Output assembler code for a block containing the constant parts
   of a trampoline, leaving space for the variable parts.

   The trampoline should set the static chain pointer to value placed
   into the trampoline and should branch to the specified routine.
   Note that RPC has been set to the address of the trampoline, so we
   can use it for addressability of the two data items.  Trampolines
   are always aligned to FUNCTION_BOUNDARY, which is 32 bits.
*/
#define TRAMPOLINE_TEMPLATE(FILE)			         \
{							         \
  fprintf ((FILE), "\tlmm 12(RPC) RPC-RPC\n");	                 \
  fprintf ((FILE), "\tbr ZERO(RPC)\n");	                         \
  fprintf ((FILE), "\tlmm 16(RPC) R0-R0\t;Delay slot\n");	 \
  fprintf ((FILE), "\t.word 0\t;static chain value\n");	         \
  fprintf ((FILE), "\t.word 0\t;function address\n");	         \
}

/* 
   Length in units of the trampoline for entering a nested function.  
*/
#define TRAMPOLINE_SIZE  20

/* 
   Emit RTL insns to initialize the variable parts of a trampoline.
   FNADDR is an RTX for the address of the function's pure code.
   CXT is an RTX for the static chain value for the function.  
*/
#define INITIALIZE_TRAMPOLINE(TRAMP, FNADDR, CXT)                       \
{									\
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant ((TRAMP), 8)),	\
		  (CXT));						\
  emit_move_insn (gen_rtx (MEM, SImode, plus_constant ((TRAMP), 12)),	\
		  (FNADDR));						\
}


/* 
   If the memory address ADDR is relative to the frame pointer,
   correct it to be relative to the stack pointer instead.
   This is for when we don't use a frame pointer.
   ADDR should be a variable name.

   #define FIX_FRAME_POINTER_ADDRESS(ADDR,DEPTH)  
*/

/* 
   Store in the variable DEPTH the initial difference between the
   frame pointer reg contents and the stack pointer reg contents,
   as of the start of the function body.  This depends on the layout
   of the fixed parts of the stack frame and on how registers are saved.
*/
/*
#define INITIAL_FRAME_POINTER_OFFSET(DEPTH) DEPTH = 0
*/

#define ELIMINABLE_REGS {                                   \
	{ ARG_POINTER_REGNUM, STACK_POINTER_REGNUM },       \
	{ ARG_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM },  \
	{ FRAME_POINTER_REGNUM, STACK_POINTER_REGNUM},	    \
	{ FRAME_POINTER_REGNUM, HARD_FRAME_POINTER_REGNUM}}

#define CAN_ELIMINATE(FROM, TO)   1

/* 
   Define the offset between two registers, one to be eliminated, and the other
   its replacement, at the start of a routine.  
*/

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)			\
{ if ((FROM) == FRAME_POINTER_REGNUM)					\
    (OFFSET) = 0;					                \
  else if ((FROM) == ARG_POINTER_REGNUM)				\
    (OFFSET) = octave_sa_size ()					\
		+ ROUND4 (get_frame_size ());				\
}


/* 
   Addressing modes, and classification of registers for them.  
*/

/* 
   Macros to check register numbers against specific register classes.  
*/

/* 
   These assume that REGNO is a hard or pseudo reg number.
   They give nonzero only if REGNO is a hard reg of the suitable class
   or a pseudo reg currently allocated to a suitable hard reg.
   Since they use reg_renumber, they are safe only once reg_renumber
   has been allocated, which happens in local-alloc.c.  
*/

#define REGNO_OK_FOR_BASE_MEMORY_REGION_P(REGNO) ((REGNO) == REG_BASE2 || (REGNO) == REG_BASE3)

#define REGNO_OK_FOR_MEMORY_REGION_P(REGNO) ((REGNO) == REG_BASE_SDRAM || (REGNO) == REG_BASE_SRAM \
					     || (REGNO) == REG_BASE_CCRAM || (REGNO) == REG_BASE_CCRAM_LOCK \
					     || (REGNO) == REG_BASE_SIO || (REGNO) == REG_BASE_SPRAM \
					     || (REGNO) == REG_BASE_SPRAM_LOCK)

#define REGNO_OK_FOR_LNK_P(REGNO) ((REGNO) == REG_LNK1 || (REGNO) == REG_LNK2 || (REGNO) == REG_LNK3)

#define REGNO_OK_GP_P(REGNO) \
  (((REGNO) >= A0_REGNUM && (REGNO) < ZERO_REGNUM) || (REGNO) == TFP_REGNUM || (REGNO) == AP_REGNUM)

#define REGNO_OK_GP_OR_PSEUDO_STRICT_P(REGNO) \
    (REGNO_OK_GP_P(((REGNO) < FIRST_PSEUDO_REGISTER) ? (REGNO) : reg_renumber[(REGNO)]))

#define REGNO_OK_GP_OR_PSEUDO_NONSTRICT_P(REGNO) \
    (REGNO_OK_GP_P(REGNO) || (REGNO) >= FIRST_PSEUDO_REGISTER)

#define REGNO_OK_FOR_INDEX_P(REGNO) REGNO_OK_GP_OR_PSEUDO_STRICT_P(REGNO)

#define REGNO_OK_FOR_BASE_P(REGNO) REGNO_OK_GP_OR_PSEUDO_STRICT_P(REGNO)

/* 
   Now macros that check whether X is a register and also,
   strictly, whether it is in a specified class.

   These macros are specific to the SPUR, and may be used only
   in code for printing assembler insns and in conditions for
   define_optimization.  
*/

/* 
   1 if X is an fp register.  
*/

#define FP_REG_P(X) (REG_P (X) && REGNO_OK_FOR_BASE_P (REGNO (X)))


/* 
   Maximum number of registers that can appear in a valid memory address.  
*/

#define MAX_REGS_PER_ADDRESS 1

/* 
   Recognize any constant value that is a valid address.  
   We don't include symbolics ; we want to lim symbolic addresses into registers.  
*/

#define CONSTANT_ADDRESS_P(X)   \
  (GET_CODE (X) == LABEL_REF || GET_CODE (X) == SYMBOL_REF		\
   || GET_CODE (X) == CONST_INT || GET_CODE (X) == CONST		\
   || GET_CODE (X) == HIGH)

/* 
   Nonzero if the constant value X is a legitimate general operand.
   It is given that X satisfies CONSTANT_P or is a CONST_DOUBLE.  
*/

#define LEGITIMATE_CONSTANT_P(X)		  \
 (GET_CODE (X) == CONST_INT			  \
  || GET_CODE (X) == SYMBOL_REF                   \
  || GET_CODE (X) == LABEL_REF                    \
  || GET_CODE (X) == CONST                        \
  || GET_CODE (X) == HIGH)

/* 
   Define this target if GCC should generate calls to the ANSI C library functions
   memcpy and memset rather than bcopy and bzero 
*/
#define TARGET_MEM_FUNCTIONS

/* 
   The macros REG_OK_FOR..._P assume that the arg is a REG rtx
   and check its validity for a certain class.
   We have two alternate definitions for each of them.
   The usual definition accepts all pseudo regs; the other rejects
   them unless they have been allocated suitable hard regs.
   The symbol REG_OK_STRICT causes the latter definition to be used.

   Most source files want to accept pseudo regs in the hope that
   they will get allocated to the class that the insn wants them to be in.
   Source files for reload pass need to be strict.
   After reload, it makes no difference, since pseudo regs have
   been eliminated by then.  
*/

/* 
   Nonzero if X is a hard reg that can be used as a base reg
   or if it is a pseudo reg.  
*/
#ifdef REG_OK_STRICT
#define REG_OK_STRICT_P 1
#define REG_OK_FOR_BASE_P(X)  REGNO_OK_GP_OR_PSEUDO_STRICT_P (REGNO (X))
#else
#define REG_OK_STRICT_P 0
#define REG_OK_FOR_BASE_P(X)  REGNO_OK_GP_OR_PSEUDO_NONSTRICT_P (REGNO (X))
#endif

/* 
   Nonzero if X is a hard reg that can be used as an index
   or if it is a pseudo reg.  
*/

#define REG_OK_FOR_INDEX_P(X)  0

/* 
   GO_IF_LEGITIMATE_ADDRESS recognizes an RTL expression
   that is a valid memory address for an instruction.
   The MODE argument is the machine mode for the MEM expression
   that wants to use this address.

   The actual legitimate addresses must be REG+SMALLINT or REG+REG.
   Actually, REG+REG is not legitimate for stores, so 
   it is obtained only by combination on loads.
*/


#define GO_PRINTF(x)	trace(x)
#define GO_PRINTF2(x,y)	trace2(x,y)
#define GO_DEBUG_RTX(x) debug_rtx(x)


#define RTX_OK_FOR_ALIGN(X, MODE)                                       \
  is_aligned_base_pointer(X, MODE)

#define RTX_OK_FOR_BASE_P(X)						\
  ((GET_CODE (X) == REG && REG_OK_FOR_BASE_P (X))			\
  || (GET_CODE (X) == SUBREG						\
      && GET_CODE (SUBREG_REG (X)) == REG				\
      && REG_OK_FOR_BASE_P (SUBREG_REG (X))))

#define RTX_OK_FOR_INDEX_P(X) 0

/*
  FIXME: check this value
*/
#define OCTAVE_OFFSET_MAX 511

#define RTX_OK_FOR_OFFSET_480(X)                                       \
  (GET_CODE (X) == CONST_INT && INTVAL (X) >= -512 && INTVAL (X) <= 511)

#define RTX_OK_FOR_OFFSET_P(X)  RTX_OK_FOR_OFFSET_480(X)

#define GO_IF_BASE_PLUS_OFFSET(MODE, X, ADDR)                           \
{                                                                       \
      register rtx op0 = XEXP (X, 0);			                \
      register rtx op1 = XEXP (X, 1);			                \
      if (RTX_OK_FOR_BASE_P (op0) && RTX_OK_FOR_OFFSET_P (op1))         \
	{						                \
	    goto ADDR;					                \
	}						                \
      else if (REG_P (op0) &&                                           \
	       REGNO_OK_FOR_MEMORY_REGION_P (REGNO (op0)))              \
        {                                                               \
           if (RTX_OK_FOR_BASE_P (op1))                                 \
             goto ADDR;                                                 \
        }                                                               \
      else if (REG_P (op0) &&                                           \
               REGNO_OK_FOR_BASE_MEMORY_REGION_P(REGNO (op0)))          \
        {                                                               \
              if (immediate_operand (op1, Pmode))                       \
                goto ADDR;                                              \
        }                                                               \
      else if (REG_P (op0) && REGNO_OK_FOR_LNK_P(REGNO (op0)))          \
        {                                                               \
              if (lnk_offset_operand (op1, VOIDmode))                   \
                goto ADDR;                                              \
        }                                                               \
}


#define GO_IF_LEGITIMATE_SIMPLE_ADDRESS(MODE, X, ADDR)                  \
{                                                                       \
  register rtx xinsn = (X);						\
									\
  if (TARGET_DEBUG_MODE)						\
    {									\
      GO_PRINTF2 ("\n========== GO_IF_LEGITIMATE_ADDRESS, %sstrict\n",	\
		  (REG_OK_STRICT_P) ? "" : "not ");			\
      GO_DEBUG_RTX (xinsn);						\
    }									\
                                                                        \
  if (RTX_OK_FOR_BASE_P (X))                                            \
    goto ADDR;					                        \
  else if (GET_CODE (X) == PLUS)			                \
    {							                \
      register rtx op0 = XEXP (X, 0);			                \
      register rtx op1 = XEXP (X, 1);			                \
                                                                        \
      if (REG_P (op0) && GET_CODE (op1) != PLUS)                        \
        GO_IF_BASE_PLUS_OFFSET(MODE, X, ADDR);                          \
                                                                        \
      if (REG_P (op1) && GET_CODE (op0) != PLUS)                        \
        GO_IF_BASE_PLUS_OFFSET(MODE, X, ADDR);                          \
                                                                        \
      if (GET_CODE (op0) == PLUS && REG_P (op1))                        \
        {                                                               \
          if (REGNO_OK_FOR_MEMORY_REGION_P (REGNO (op1)))               \
            GO_IF_BASE_PLUS_OFFSET(MODE, op0, ADDR);                    \
        }                                                               \
                                                                        \
      if (GET_CODE (op1) == PLUS && REG_P (op0))                        \
        {                                                               \
          if (REGNO_OK_FOR_MEMORY_REGION_P (REGNO (op0)))               \
            GO_IF_BASE_PLUS_OFFSET(MODE, op1, ADDR);                    \
        }                                                               \
    }							                \
  if (TARGET_DEBUG_MODE)						\
    GO_PRINTF ("Not a legitimate address\n");				\
}

/* 
   Now accept the simple address, or, for SImode only, an AND of a simple
   address that turns off the low two bits.  
   
   The construct (AND (address) (const_int 4)) is our clue to the GCC alias
   analysis that it can't make any assumptions about aliasing, since we could
   be accessing any byte or half-word in a word. 
*/


#ifdef REG_OK_STRICT
#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)				\
do {									\
  if (m480_legitimate_address_p ((MODE), (X), 1))			\
    goto ADDR;								\
} while (0)

#else
#define GO_IF_LEGITIMATE_ADDRESS(MODE, X, ADDR)				\
do {									\
  if (m480_legitimate_address_p ((MODE), (X), 0))			\
    goto ADDR;								\
} while (0)

#endif
/* 
   Try machine-dependent ways of modifying an illegitimate address
   to be legitimate.  If we find one, return the new, valid address.

   This macro is used in only one place: `memory_address' in explow.c.

   OLDX is the address as it was before break_out_memory_refs was called.
   In some cases it is useful to look at this to decide what needs to be done.

   MODE and WIN are passed so that this macro can use
   GO_IF_LEGITIMATE_ADDRESS.

   It is always safe for this macro to do nothing.  It exists to recognize
   opportunities to optimize the output.  For now do nothing. 
*/

#define LEGITIMIZE_ADDRESS(X,OLDX,MODE,WIN) {}

/* 
   Go to LABEL if ADDR (a legitimate address expression)
   has an effect that depends on the machine mode it is used for.
   On the SPUR this is never true.  
*/

#define GO_IF_MODE_DEPENDENT_ADDRESS(ADDR,LABEL) \
{ if (GET_CODE (ADDR) == AND) goto LABEL; }


/* 
   Some of the following definitions are designated Miscellaneous Parameters
   as described in [Stallman], section 17.19 
*/

/* 
   Specify the machine mode that this machine uses
   for the index in the tablejump instruction.  
*/
#define CASE_VECTOR_MODE SImode

/* 
   Define this if the tablejump instruction expects the table
   to contain offsets from the address of the table.
   Do not define this if the table should contain absolute addresses.  
*/
#define CASE_VECTOR_PC_RELATIVE 0

/* 
   Define if operations between registers always perform the operation
   on the full register even if a narrower mode is specified.  
*/
#define WORD_REGISTER_OPERATIONS

/* 
   Define if loading in MODE, an integral mode narrower than BITS_PER_WORD
   will either ZERO_EXTEND or SIGN_EXTEND to the bits outside of MODE.
   Return SIGN_EXTEND or ZERO_EXTEND for modes for which that happens,
   and NIL for other modes. 
*/
#define LOAD_EXTEND_OP(MODE) (((MODE) == QImode || (MODE) == HImode) ? ZERO_EXTEND : NIL)

/* 
   Specify the cost of a branch insn; roughly the number of extra insns that
   should be added to avoid a branch.

   Set this to 3 on the 4400 since that is roughly the average cost of an
   unscheduled conditional branch.  
*/

#define BRANCH_COST ((optimize_size == 0) ? 3 : 2)

/* 
   A C expression for the cost of moving data of mode MODE between
   a register and memory. A value of 4 is the default. 
*/
#define MEMORY_MOVE_COST(MODE,CLASS,TO_P) \
   (TARGET_DATA_IN_SDRAM ? SDRAM_MEMORY_CYCLE_COST : SRAM_MEMORY_CYCLE_COST)

/* 
   Define this as 1 if `char' should by default be signed; else as 0.  
*/
#define DEFAULT_SIGNED_CHAR 1

/* 
   Max number of bytes we can move from memory to memory
   in one reasonably fast instruction.  
*/
#define MOVE_MAX 4

/* 
   Define this to be nonzero if shift instructions ignore all but the low-order
   few bits. 
*/
/* 
#define SHIFT_COUNT_TRUNCATED 1 
*/

/* 
   Value is 1 if truncating an integer of INPREC bits to OUTPREC bits
   is done just by pretending it is already truncated.  
*/
#define TRULY_NOOP_TRUNCATION(OUTPREC, INPREC) 1

#define DBR_OUTPUT_SEQEND(FILE)	   m480_dbr_output_seqend(FILE)

#define ELIGIBLE_FOR_DELAY_SIMPLE(TRIAL, INSN) m480_eligible_for_delay_simple(TRIAL, INSN)

#define ELIGIBLE_FOR_DELAY_FROM_TARGET(TRIAL) m480_eligible_for_delay_from_target(TRIAL)

#define ELIGIBLE_FOR_DELAY_FROM_FALLTHROUGH(TRIAL) m480_eligible_for_delay_from_fallthrough(TRIAL)

/* 
   We assume that the store-condition-codes instructions store 0 for false
   and some other value for true.  This is the value stored for true.  
*/
#define STORE_FLAG_VALUE 1

/* 
   Specify the machine mode that pointers have.
   After generation of rtl, the compiler makes no further distinction
   between pointers and any other objects of this machine mode.  
*/
#define Pmode SImode

/* 
   A function address in a call instruction
   is a byte address (for indexing purposes)
   so give the MEM rtx a byte's mode.  
*/
#define FUNCTION_MODE SImode

/* 
   Nonzero if access to memory by bytes is slow and undesirable.  
*/
#define SLOW_BYTE_ACCESS 1

/* 
   This is BSD, so it wants DBX format.  
*/
/*
#define DBX_DEBUGGING_INFO  <= mxt4400 doesn't debug yet 
*/
#define DBX_DEBUGGING_INFO

/* 
   assume SDB_DEBUGGING_INFO 
*/
#ifndef PREFERRED_DEBUGGING_TYPE	
#define PREFERRED_DEBUGGING_TYPE DBX_DEBUG
#endif

/* 
   Do not break .stabs pseudos into continuations.  
*/
#define DBX_CONTIN_LENGTH 0

/* 
   Don't try to use the `x' type-cross-reference character in DBX data.
   Also has the consequence of putting each struct, union or enum
   into a separate .stabs, containing only cross-refs to the others.  
*/
#define DBX_NO_XREFS

/* 
   Define this if addresses of constant functions
   shouldn't be put through pseudo regs where they can be cse'd.
   Desirable on machines where ordinary constants are expensive
   but a CALL with constant address is cheap.  
*/
#define NO_FUNCTION_CSE

/* 
   Compute the cost of computing a constant rtl expression RTX
   whose rtx-code is CODE.  The body of this macro is a portion
   of a switch statement.  If the code is computed here,
   return it with a return statement.  Otherwise, break from the switch.  
*/
#define LOGICAL_CODE_P(C) ((C) == NOT || (C) == AND \
                           || (C) == IOR || (C) == XOR)

#define SHIFT_CODE_P(C) ((C) == ASHIFT || (C) == ASHIFTRT       \
			 || (C) == LSHIFTRT || (C) == ROTATE)

#define CONST_COSTS(RTX,CODE,OUTER_CODE) \
  case CONST_INT:						\
    if (OUTER_CODE == SET)		                        \
      {                                                         \
        if (lim_operand(RTX, SImode))                           \
          return 0;                                             \
        else                                                    \
          return 4;                                             \
      }                                                         \
    if (OUTER_CODE == PLUS)		                        \
      {                                                         \
        if (CONST_OK_FOR_UI_17BITS(INTVAL(RTX))                 \
            || CONST_OK_FOR_UI_17BITS(INTVAL(RTX)))             \
          return 0;						\
        else                                                    \
          return 4;                                             \
      }                                                         \
    if (OUTER_CODE == MINUS || OUTER_CODE == COMPARE)		\
      {                                                         \
        if (CONST_OK_FOR_UI_6BITS(INTVAL(RTX)))                 \
          return 0;						\
        else                                                    \
          return 4;                                             \
      }                                                         \
    if (LOGICAL_CODE_P(OUTER_CODE))                             \
      {                                                         \
        if (CONST_OK_FOR_SI_10BITS(INTVAL(RTX)))                \
          return 0;						\
        else                                                    \
          return 4;                                             \
      }                                                         \
    if (SHIFT_CODE_P(OUTER_CODE))                               \
      return 0;                                                 \
    if (RTX == const0_rtx)                                      \
      return 0;                                                 \
    return 4;                                                   \
  case HIGH:                                                    \
      return 1;                                                 \
  case SYMBOL_REF:						\
    if (OUTER_CODE == LO_SUM)                                   \
      return 0;                                                 \
  case CONST:							\
  case LABEL_REF:						\
    return 4;							\
  case CONST_DOUBLE:						\
    return 4;

#define RTX_COSTS(X, CODE, OUTER_CODE)				        \
    case MEM:                                                           \
      return COSTS_N_INSNS(3);                                          \
    case PLUS:								\
    case MINUS:								\
       if (GET_MODE_CLASS (GET_MODE (X)) == MODE_FLOAT)                 \
         return COSTS_N_INSNS(1) * 50;                                  \
       else                                                             \
         {                                                              \
           return (COSTS_N_INSNS (1)				        \
	           + rtx_cost (XEXP ((X), 0), CODE)		        \
	           + rtx_cost (XEXP ((X), 1), CODE));                   \
	 }                                                              \
    case ZERO_EXTRACT:                                                  \
    case SIGN_EXTRACT:                                                  \
    case ZERO_EXTEND:                                                   \
    case SIGN_EXTEND:                                                   \
         return (COSTS_N_INSNS (1)                                      \
	         + rtx_cost (XEXP ((X), 0), CODE));		        \
    case MULT:								\
       if (GET_MODE_CLASS (GET_MODE (X)) == MODE_FLOAT)                 \
         return COSTS_N_INSNS(1) * 100;                                 \
       else if (TARGET_480)                                             \
       /* Multiply is a function call on the 480 */                     \
         return COSTS_N_INSNS(1) * 70;                                  \
    case AND:								\
    case IOR:								\
    case XOR:								\
      return (COSTS_N_INSNS (1)				                \
	      + rtx_cost (XEXP ((X), 0), CODE)		                \
	      + rtx_cost (XEXP ((X), 1), CODE));                        \
    case ASHIFT:							\
      if (GET_CODE (XEXP ((X), 0)) == AND                               \
	  && (GET_CODE (XEXP ((X), 1)) == CONST_INT                     \
	      && (INTVAL (XEXP ((X), 1)) == 3)))                        \
        {                                                               \
          rtx op = XEXP ((X), 0);                                       \
          rtx op1 = XEXP (op, 0);                                       \
          rtx op2 = XEXP (op, 1);                                       \
                                                                        \
          if (REG_P (op1) && (GET_CODE (op2) == CONST_INT               \
			      && INTVAL (op2) == 3))                    \
            return 3;                                                   \
	}                                                               \
    case ASHIFTRT:							\
    case LSHIFTRT:							\
    case ROTATE:                                                        \
      return (COSTS_N_INSNS (1)				                \
	      + rtx_cost (XEXP ((X), 0), CODE)		                \
	      + rtx_cost (XEXP ((X), 1), CODE));                        \
    case UMAX:                                                          \
    case UMIN:                                                          \
      return (COSTS_N_INSNS (1)				                \
	      + rtx_cost (XEXP ((X), 0), CODE)		                \
	      + rtx_cost (XEXP ((X), 1), CODE));                        \
    case NEG:                                                           \
      return (COSTS_N_INSNS (1)				                \
	      + rtx_cost (XEXP ((X), 0), NEG));		                \
    case UMOD:								\
      if (GET_CODE (XEXP ((X),0)) == NEG                                \
	  && (GET_CODE (XEXP ((X), 1)) == CONST_INT                     \
	      && INTVAL (XEXP ((X), 1)) == 32))                         \
        return 3;                                                       \
    case MOD: 								\
      return COSTS_N_INSNS (GET_MODE_CLASS (GET_MODE (X)) == MODE_FLOAT	\
			  ? 100 : 60);

/* 
   Cost of various memory accesses. These are approximate values 
*/

#define SDRAM_MEMORY_CYCLE_COST 20
#define SRAM_MEMORY_CYCLE_COST  6

#define MAX_BLOCK_MOVE_BYTES ((optimize_size) ? 128 : 256)


/* 
   Add any extra modes needed to represent the condition code.

   For the m27480, we need separate modes when unsigned 
   (logical) comparisons are being done 
*/

#define EXTRA_CC_MODES CC(CCUNSmode, "CCUNS")      \
                       CC(CC_CYmode, "CC_CY")      \
                       CC(CC_NOOVmode, "CC_NOOV") 

/* 
   Given a comparison code (EQ, NE, etc.) and the first 
   operand of a COMPARE, return the mode to be used for 
   the comparison.  CCUNSmode should be used  for 
   unsigned comparisons. CCmode should be used in all 
   other cases.  
*/

#define SELECT_CC_MODE(OP,X,Y)                                          \
  ((OP) == GTU || (OP) == LTU || (OP) == GEU || (OP) == LEU ? CCUNSmode \
   : CCmode)

/* 
   Define the information needed to generate branch and scc insns.  This is
   stored from the compare operation.  Note that we can't use "rtx" here
   since it hasn't been defined!  
*/

extern struct rtx_def *m480_compare_op0, *m480_compare_op1;


/* 
   Control the assembler format that we output.  
*/

/*
  Output at beginning of assembler file.  
*/

#define ASM_FILE_START(FILE)                            \
{							\
    fprintf (FILE, "# 1 \"%s\" \n", asm_file_name);	\
    fprintf (FILE, "\n");                               \
}


/* 
   Output to assembler file text saying following lines
   may contain character constants, extra white space, comments, etc.  
*/

#define ASM_APP_ON ""

/* 
   Output to assembler file text saying following lines
   no longer contain unusual constructs.  
*/

#define ASM_APP_OFF ""

/* 
   Output before read-only data.  
*/

#define TEXT_SECTION_ASM_OP ".text"

/* 
   Output before writable data.  
*/

#define DATA_SECTION_ASM_OP ".data"

#define CONST_SECTION_ASM_OP ".section const_section"

/* 
   A list of names for sections other than the standard ones, which are
   'in_text' and 'in_data' (and .bss if BSS_SECTION_ASM_OP is defined). 
*/
#define EXTRA_SECTIONS in_const

#define EXTRA_SECTION_FUNCTIONS	\
  CONST_SECTION_FUNCTION

#define READONLY_DATA_SECTION const_section

#define CONST_SECTION_FUNCTION						\
void									\
const_section ()							\
{									\
 if (in_section != in_const)					        \
    {									\
      fprintf (asm_out_file, "%s\n", CONST_SECTION_ASM_OP);		\
      in_section = in_const;						\
    }									\
}

/* 
   How to refer to registers in assembler output.
   This sequence is indexed by compiler's hard-register-number (see above).
   The last three are "fake" and should in assembly code.)    
*/

#define REGISTER_NAMES                                                   \
{"r0",      "r1",      "r2",    "r3",    "r4",   "r5",    "r6",    "r7",   \
 "r8",      "r9",      "r10",   "r11",   "r12",  "r13",   "r14",   "r15",  \
 "r16",     "r17",     "r18",   "r19",   "r20",  "r21",   "r22",   "r23",  \
 "r24",     "r25",     "r26",   "r27",   "r28",  "r29",   "r30",   "r31",  \
 "r32",     "r33",     "r34",   "r35",   "r36",  "r37",   "r38",   "r39",  \
 "r40",     "r41",     "r42",   "r43",   "r44",  "r45",   "r46",   "r47",  \
 "r48",     "r49",     "r50",   "r51",   "r52",  "r53",   "r54",   "r55",  \
 "r56",     "r57",     "r58",   "r59",   "r60",  "r61",   "r62",   "r63",  \
 "r64",     "r65",     "r66",   "r67",   "r68",  "r69",   "r70",   "r71",  \
 "r72",     "r73",     "r74",   "r75",   "r76",  "r77",   "r78",   "r79",  \
 "r80",     "r81",     "r82",   "r83",   "r84",  "r85",   "r86",   "r87",  \
 "r88",     "r89",     "r90",   "r91",   "r92",  "r93",   "r94",   "r95",  \
 "r96",     "r97",     "r98",   "r99",   "r100", "BASE2", "BASE3", "r103", \
 "r104",    "r105",    "r106",  "r107",  "r108", "r109",  "r110",  "r111", \
 "r112",    "r113",    "r114",  "r115",  "r116", "r117",  "r118",  "r119", \
 "r120",    "r121",    "r122",  "r123",  "r124", "r125",  "r126",  "r127", \
 "ESS26",   "ESS25",   "ESS14", "ESS8",  "ESS7", "ESS6",  "ESS5",  "ESS4", \
 "ESS3",    "ESS2",    "ESS1",  "ESS0",  "ESS31", "ESS30", "LNK1", "LNK2", \
 "LNK3",     "AP",     "RAP",   "TEMP_FP", "TEMP_SP", "SDRAM", "SRAM",     \
 "CCRAM", "CCRAM_LOCK", "SIO",  "SPRAM",   "SPRAM_LOCK", "GET_PTR", "PUT_PTR"}

/* 
   How to renumber registers for dbx and gdb.  
*/

#define DBX_REGISTER_NUMBER(REGNO) (REGNO)

/* 
   Mainly for -dp debugging 
*/
#define ASM_COMMENT_START "\t;#"	

/* 
   This is how to output the definition of a user-level label named NAME,
   such as the label on a static function or variable NAME.  
*/

#define ASM_OUTPUT_LABEL(FILE,NAME)	m480_output_label(FILE, NAME)


/* 
   This is how to output a command to make the user-level label named NAME
   defined for reference from other files.  

   We define ASM_GLOBALIZE_LABEL to do nothing. This is because GCC uses
   ASM_GLOBALIZE_LABEL followed by ASM_OUTPUT_LABEL when it want to output
   the defintion of a function or variable. GCC assumes that ASM_GLOBALIZE_LABEL
   output something like '.global <variable name>'. It then assumes that 
   ASM_OUTPUT_LABEL simply spits out the label. The 4400 assembler uses

   <label name>:: to indicate a global symbol

   and

   <label name>:= to indicate a local symbol

   This does not fit the GCC model. To make things work we make ASM_GLOBAL_LABEL
   do nothind and output the label along with the correct suffix in 
   ASM_DECLARE_FUNCTION_NAME and ASM_DECLARE_OBJECT_NAME. In both cases if these
   are defined GCC uses them instead of ASM_OUTPUT_LABEL.
*/

#define ASM_GLOBALIZE_LABEL(FILE,NAME)

/* This is experimental. */
#define ASM_OUTPUT_GLOBAL_LABEL(FILE,NAME) \
  do { fprintf(FILE, "%s::\n", NAME); } while (0); 


/* 
   The prefix to add to user-visible assembler symbols. 
*/

#define USER_LABEL_PREFIX ""

/* 
   This is how to output an internal numbered label where
   PREFIX is the class of label and NUM is the number within the class.  
*/

#define ASM_OUTPUT_INTERNAL_LABEL(FILE,PREFIX,NUM)	\
  fprintf (FILE, "%s%d:\n", PREFIX, NUM)

#define ASM_OUTPUT_LOCAL_LABEL(FILE,NAME)               \
  fprintf (FILE, "%s:\n", NAME)
/* 
   This is how to store into the string LABEL
   the symbol_ref name of an internal numbered label where
   PREFIX is the class of label and NUM is the number within the class.
   This is suitable for output with `assemble_name'.  
*/

#define ASM_GENERATE_INTERNAL_LABEL(LABEL,PREFIX,NUM)	\
  sprintf (LABEL, "*%s%d", PREFIX, NUM)

#define ASM_OUTPUT_DOUBLE(FILE,VALUE)  \
  ASM_OUTPUT_FLOAT(FILE,VALUE)

/* 
   This is how to output an assembler line defining a `float' constant.  
*/

#define ASM_OUTPUT_FLOAT(FILE,VALUE)                            \
{								\
    long t;							\
    REAL_VALUE_TO_TARGET_SINGLE ((VALUE), t);			\
    fprintf (FILE, "\t.long 0x%lx\n", t & 0xffffffff);		\
}

/* 
   This is how to output an assembler line defining an `int' constant.  
*/

#define ASM_OUTPUT_INT(FILE,VALUE)  \
( fprintf (FILE, "\t.long "),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

/* 
   Likewise for `char' and `short' constants.  
*/

#define ASM_OUTPUT_SHORT(FILE,VALUE)  \
( fprintf (FILE, "\t.halfword "),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

#define ASM_OUTPUT_CHAR(FILE,VALUE)  \
( fprintf (FILE, "\t.byte "),			\
  output_addr_const (FILE, (VALUE)),		\
  fprintf (FILE, "\n"))

/* 
   This is how to output an assembler line for a numeric constant byte.  
*/

#define ASM_OUTPUT_BYTE(FILE,VALUE)  \
  fprintf (FILE, "\t.byte 0x%x\n", (VALUE))

/* 
   This is how to output code to push a register on the stack.
   It need not be very fast code.  
*/

#define ASM_OUTPUT_REG_PUSH(FILE,REGNO)  \
  fprintf (FILE, "\tsubi\tSP 4 SP\n\tsmm %s 0(SP)\t;REG_PUSH\n", reg_names[REGNO])

/* 
   This is how to output an insn to pop a register from the stack.
   It need not be very fast code.  
*/

#define ASM_OUTPUT_REG_POP(FILE,REGNO)  \
  fprintf (FILE, "\tlmm\t0(SP) %s\n\tadd\tSP 4 SP\t;REG_POP\n", reg_names[REGNO])

/* 
   Define this macro if it is ok to have jump tables in the text section 
*/

#define JUMP_TABLES_IN_TEXT_SECTION 1

/* 
   Define this if the label before a jump-table needs to be output specially 
*/

/* #define ASM_OUTPUT_CASE_LABEL(FILE, PREFIX, NUM, TABLE) */

/* 
   Define this if something special must be output at the end of a jump-table 
*/

#define ASM_OUTPUT_CASE_END(FILE, NUM, TABLE)

/* 
   This is how to output an element of a case-vector that is absolute.  
*/

#define ASM_OUTPUT_ADDR_VEC_ELT(FILE, VALUE)

/* 
   This is how to output an element of a case-vector that is relative.
*/

#define ASM_OUTPUT_ADDR_DIFF_ELT(FILE, BODY, VALUE, REL)

/* 
   This is how to output an assembler line
   that says to advance the location counter
   to a multiple of 2**LOG bytes.  
*/

#define ASM_OUTPUT_ALIGN(FILE,LOG) m480_asm_output_align(FILE,LOG)  
 

#define ASM_OUTPUT_SKIP(FILE,SIZE)  \
  fprintf (FILE, "\t.byte 0 [%d]\n", (SIZE))

/* 
   This says how to output an assembler line
   to define a global common symbol.  
*/

#define ASM_OUTPUT_COMMON(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".comm ", FILE),			\
  assemble_name (FILE, (NAME)),		\
  fprintf (FILE, ",%u\n", (ROUNDED)))

/* 
   This says how to output an assembler line
   to define a local common symbol.  
*/

#define ASM_OUTPUT_LOCAL(FILE, NAME, SIZE, ROUNDED)  \
( fputs (".lcomm ", FILE),			\
  assemble_name (FILE, (NAME)),		\
  fprintf (FILE, ",%u\n", (ROUNDED)))

#define ASM_OUTPUT_SOURCE_LINE(FILE, LINE) \
  fprintf (FILE, "\t;.line\t%d\n", (LINE));

#define ASM_OUTPUT_SOURCE_FILENAME(FILE, NAME)			\
  fprintf (FILE, "\n\t;.file\t%s\n", (NAME));

/* 
   Store in OUTPUT a string (made with alloca) containing
   an assembler-name for a local static variable named NAME.
   LABELNO is an integer which is different for each call.  
*/

#define ASM_FORMAT_PRIVATE_NAME(OUTPUT, NAME, LABELNO)	\
( (OUTPUT) = (char *) alloca (strlen ((NAME)) + 10),	\
  sprintf ((OUTPUT), "%s__%d", (NAME), (LABELNO)))

/* 
   Define results of standard character escape sequences.  
*/

#define TARGET_ESC 39
#define TARGET_BELL 007
#define TARGET_BS 010
#define TARGET_TAB 011
#define TARGET_NEWLINE 012
#define TARGET_VT 013
#define TARGET_FF 014
#define TARGET_CR 015

/* 
   Define which CODE values are valid punctuation characters for use in
   PRINT_OPERAND 
*/ 

#define PRINT_OPERAND_PUNCT_VALID_P(CODE)	\
  ((CODE) == '*' || (CODE) == '<' || (CODE) == '>')

/* 
   Print operand X (an rtx) in assembler syntax to file FILE.
   CODE is a letter or dot (`z' in `%z0') or 0 if no letter was specified.
   For `%' followed by punctuation, CODE is the punctuation and X is null.
*/

#define PRINT_OPERAND(FILE, X, CODE)  print_operand (FILE, X, CODE)


/* 
   Print a memory address as an operand to reference that memory location.  
*/

#define PRINT_OPERAND_ADDRESS(FILE, ADDR) print_operand_address(FILE, ADDR)


#define OUTPUT_ADDR_CONST_EXTRA(STREAM, X, FAIL)                        \
   do									\
     if (GET_CODE (X) == UNSPEC && XVECLEN ((X), 0) == 1)	        \
       {								\
 	switch (XINT ((X), 1))						\
 	  {								\
 	  case U_LO16:					                \
 	    fputs ("lo16(", (STREAM));				        \
                                                                        \
            if(is_label_ref_expr(XVECEXP ((X), 0, 0)))                  \
 	        print_label_ref_expr(STREAM, (XVECEXP ((X), 0, 0)));        \
	    else                                                        \
	       output_addr_const ((STREAM), XVECEXP ((X), 0, 0));	\
                                                                        \
 	    fputs (")", (STREAM));				        \
 	    break;							\
 	  case U_LO17:						        \
 	    fputs ("lo17(", (STREAM));				        \
                                                                        \
            if(is_label_ref_expr(XVECEXP ((X), 0, 0)))                  \
 	        print_label_ref_expr(STREAM, (XVECEXP ((X), 0, 0)));    \
	    else                                                        \
	       output_addr_const ((STREAM), XVECEXP ((X), 0, 0));	\
                                                                        \
 	    fputs (")", (STREAM));				        \
 	    break;							\
 	  default:							\
 	    goto FAIL;							\
 	  }								\
 	break;								\
       }								\
     else if (GET_CODE (X) == HIGH)	                                \
      {                                                                 \
        fputs ("hi16(", (STREAM));				        \
                                                                        \
        if(is_label_ref_expr(XEXP(X,0)))                                \
 	   print_label_ref_expr(STREAM, XEXP(X,0));                     \
	else                                                            \
	   output_addr_const ((STREAM), XEXP(X,0));	                \
                                                                        \
        fputs (")", (STREAM));				                \
        break;                                                          \
      }                                                                 \
      else                                                              \
       goto FAIL;							\
   while (0)


#define OCTAVE_RTX_DEBUG(rtx,cmt) octave_rtx_debug((rtx),(cmt),__FILE__,__LINE__)






/* 
   This macro produces the initial definition of a function name.
   We'll use it in the prologue and epilogue.  
*/

extern char *octave_function_name;

/*   
     A C statement (sans semicolon) to output to the stdio stream
     STREAM any text necessary for declaring the name NAME of a
     function which is being defined.  This macro is responsible for
     outputting the label definition (perhaps using
     `ASM_OUTPUT_LABEL').  The argument DECL is the `FUNCTION_DECL'
     tree node representing the function.

     If this macro is not defined, then the function name is defined in
     the usual manner as a label (by means of `ASM_OUTPUT_LABEL'). 
*/

#define ASM_DECLARE_FUNCTION_NAME(FILE,NAME,DECL) m480_asm_declare_function_name(FILE,NAME,DECL)


/*   
     A C statement (sans semicolon) to output to the stdio stream
     STREAM any text necessary for declaring the name NAME of an
     initialized variable which is being defined.  This macro must
     output the label definition (perhaps using `ASM_OUTPUT_LABEL').
     The argument DECL is the `VAR_DECL' tree node representing the
     variable.

     If this macro is not defined, then the variable name is defined in
     the usual manner as a label (by means of `ASM_OUTPUT_LABEL'). 
*/

#define ASM_DECLARE_OBJECT_NAME(FILE,NAME,DECL)	   m480_asm_declare_object_name(FILE,NAME,DECL)

/* 
   Define this so gcc does not output a call to __main, since we
   are not currently supporting c++. 
*/
#define INIT_SECTION_ASM_OP  1

#define OBJECT_FORMAT_ELF

/* 
   We don't want '_cleanup' in libc to be called since we are not handling file I/O
   and because it pulls in alot of code. 
*/
#define EXIT_BODY

#define TABLEJUMP_P(INSN) m480_tablejump_p(INSN)
/* 
   Prologue will force 'regs_ever_live[RPC_REGNUM] == 1' for non-leaf
   functions. This global remembers whether or not regalloc also
   asserted that for its own purposes.  (The fact is used in the
   coding of each "call..."-form instruction.) 
*/

extern int octave_rpc_was_ever_live;


#define HANDLE_NOTE_RELATED_INSN(LINE) m480_handle_note_related_insn(LINE)

#define MACHINE_DEPENDENT_REORG(INSN) m480_machine_dependent_reorg (INSN)


#define MEM_ADDR_CAN_TRAP(x) m480_mem_addr_can_trap(x)

#define TARGET_CAN_COMBINE(I1, I2) octave_can_combine(I1,I2)
