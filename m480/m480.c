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

#include <stdio.h>
#include "config.h"
#include "system.h"
#include "rtl.h"
#include "regs.h"
#include "hard-reg-set.h"
#include "real.h"
#include "insn-config.h"
#include "conditions.h"
#include "insn-flags.h"
#include "output.h"
#include "insn-attr.h"
#include "flags.h"
#include "recog.h"
#include "reload.h"
#include "expr.h"
#include "obstack.h"
#include "tree.h"
#include "toplev.h"
#include "target.h"
#include "target-def.h"
#include "m480-protos.h"
#include "function.h"
#include "basic-block.h"
#include "sched-int.h"

#undef TARGET_CANNOT_MODIFY_JUMPS_P
#define TARGET_CANNOT_MODIFY_JUMPS_P m480_cannot_modify_jumps_past_reload_p

#define FRM_RELATED(exp)  (do_pending_stack_adjust (), start_sequence (), exp, set_frame_related_p ())


/* GLOBALS */

/* This declaration is used in m480.h by the defn. of ASM_DECLARE_FUNCTION_NAME */
char *octave_function_name;

/* Save information from a "cmpxx" operation until the branch or scc is emitted.  */
rtx m480_compare_op0, m480_compare_op1;

/* global to check and not print special jumps to exit label */
unsigned int saw_brsl;

static void emit_stack_save_insn PARAMS ((int, int, int));
static void emit_stack_restore_insn PARAMS ((int, int, int));
static rtx  set_frame_related_p PARAMS ((void));
static int  m480_emit_add_sequence PARAMS ((rtx *));
static void emit_octave_cbranch PARAMS ((enum rtx_code code, rtx ess_reg, rtx octave_jump_label));
static void m480_gen_signed_compare_and_branch PARAMS ((enum rtx_code, rtx, rtx, int, rtx));
static void m480_gen_signed_compare_and_branch_ignore_overflow PARAMS ((enum rtx_code, rtx, rtx, int, rtx));
static void m480_gen_unsigned_compare_and_branch PARAMS ((enum rtx_code, rtx, rtx, int, rtx));
static rtx  compute_sign PARAMS ((enum rtx_code code, rtx, rtx));
static void m480_gen_unsigned_compare_and_branch PARAMS ((enum rtx_code, rtx, rtx, int, rtx));
static void m480_gen_signed_compare_and_branch_ignore_overflow PARAMS ((enum rtx_code, rtx, rtx, int, rtx));
static void m480_gen_signed_compare_and_branch PARAMS ((enum rtx_code, rtx, rtx, int, rtx));
static rtx compute_sign PARAMS ((enum rtx_code, rtx, rtx));
static int m480_use_dfa_pipeline_interface PARAMS ((void));
static void m480_dependencies_evaluation_hook PARAMS ((rtx, rtx));
static int m480_adjust_cost PARAMS ((rtx, rtx, rtx, int));
static int m480_lmm_adjust_cost PARAMS ((rtx, rtx, int));
static void m480_find_used_regs	PARAMS ((rtx *, void *));
static rtx get_lmm_memory_operand PARAMS ((rtx));
static int get_lmm_words PARAMS ((rtx));
static rtx get_lmm_first_dest PARAMS ((rtx));
static int get_mem_latency_to_reg PARAMS ((rtx, rtx, rtx, int));
static int ccram_latency PARAMS ((rtx, rtx, int));
static int ccram_lock_latency PARAMS ((rtx, rtx, int));
static int spram_latency PARAMS ((rtx, rtx, int));
static int spram_lock_latency PARAMS ((rtx, rtx, int));
static int sram_latency PARAMS ((rtx, rtx, int));
static int sdram_latency PARAMS ((rtx, rtx, int));
static int register_pair_offset PARAMS ((rtx, rtx));
extern enum machine_mode promote_mode PARAMS ((tree, enum machine_mode, int *, int));
static void m480_output_function_prologue PARAMS ((FILE *, HOST_WIDE_INT));
static void m480_output_function_epilogue PARAMS ((FILE *, HOST_WIDE_INT));

#undef TARGET_SCHED_ADJUST_COST
#define TARGET_SCHED_ADJUST_COST m480_adjust_cost

#undef TARGET_SCHED_USE_DFA_PIPELINE_INTERFACE
#define TARGET_SCHED_USE_DFA_PIPELINE_INTERFACE \
  m480_use_dfa_pipeline_interface

#undef TARGET_SCHED_DEPENDENCIES_EVALUATION_HOOK
#define TARGET_SCHED_DEPENDENCIES_EVALUATION_HOOK m480_dependencies_evaluation_hook

#undef TARGET_ASM_FUNCTION_PROLOGUE
#define TARGET_ASM_FUNCTION_PROLOGUE m480_output_function_prologue
#undef TARGET_ASM_FUNCTION_EPILOGUE
#define TARGET_ASM_FUNCTION_EPILOGUE m480_output_function_epilogue

struct gcc_target targetm = TARGET_INITIALIZER;

#define M480_MAX_USES 16
struct insn_reg_use
{
  rtx reg_rtx;
};

static struct insn_reg_use insn_reg_use_table[M480_MAX_USES];
static int insn_reg_use_count;

/* The following function marks insns who produce addresses for load
   and store insns.  Such insns will be placed into M slots because it
   decrease latency time for Itanium1 (see function
   `ia64_produce_address_p' and the DFA descriptions).  */

static void
m480_dependencies_evaluation_hook (head, tail)
     rtx head ATTRIBUTE_UNUSED;
     rtx tail ATTRIBUTE_UNUSED;
{
  return;
}

static int
m480_use_dfa_pipeline_interface ()
{
  return 1;
}

void
m480_machine_dependent_reorg (insn)
     rtx insn ATTRIBUTE_UNUSED;
{
}

void
print_operand_address (file, addr)
     FILE *file;
     rtx addr;
{
  switch (GET_CODE (addr))
    {
    case REG:
      fprintf(file, "0(%s)", reg_names[REGNO(addr)]);
      break;

    case CONST_INT:
      fprintf(file, "%d(%s)", INTVAL(addr), reg_names[ZERO_REGNUM]);
      break;

    case PLUS:
      {
	rtx base = XEXP (addr, 0);
	rtx index = XEXP (addr, 1);

	if (REG_P (base) && REGNO_OK_FOR_BASE_P (REGNO(base)) && 
	    GET_CODE (index) == CONST_INT)
	  {
	    fprintf(file, "%d(%s)", INTVAL (index),
		    reg_names[REGNO (base)]);
	  }
	else if (REG_P (base) && REGNO_OK_FOR_BASE_MEMORY_REGION_P (REGNO(base)) && 
		 CONSTANT_ADDRESS_P (index))
	  {
	    fprintf (file, "%s ", reg_names[REGNO (base)]);
	    output_addr_const (file, index);
	  }
	else if (REG_P (base) && REGNO_OK_FOR_MEMORY_REGION_P (REGNO(base)))
	  {
	    print_operand_address(file, index);
	  }
	else
	  {
	    debug_rtx(addr);
	    abort();
	  }
      }
      break;

    default:
      output_addr_const (file, addr);
      break;
    }
}



int
has_delay_slots()
{
  if (final_sequence || dbr_sequence_length() > 0)
    return 1;

  return 0;
}

void
print_operand (FILE *file, rtx x, char code)
{


  if (x && GET_CODE (x) == AND)
    x = XEXP (x, 0);
    
  switch (code) {
  case 'r':
    if (REG_P (x))
      {
	if (GET_MODE_SIZE (GET_MODE (x)) <= UNITS_PER_WORD)
	  fprintf (file, "%s", reg_names[REGNO (x)]);
	else
	  {
	    int num_regs = HARD_REGNO_NREGS(REGNO(x), GET_MODE(x));
	    int end_regnum = REGNO (x) + (num_regs - 1);
	      
	    fprintf(file, "%s-%s", reg_names[REGNO (x)], reg_names[end_regnum]);
	  }
      }
    else if (x == CONST0_RTX (GET_MODE (x)))
      fprintf (file, "r63");
    else
      output_operand_lossage ("invalid %%r value");
      
    break;
	
  case '>':
    {
      /* Emit 3 nops for tsetb or tclrb instructions if we couldn't fill any delay slots.
	 The macro DBR_OUTPUT_SEQEND takes care of emitting nops when some or all of the delay
	 slots have been filled. */
      int count = 0;

      if (final_sequence == 0)
	{
	  while (count < 3)
	    {
	      fputs ("\n\tnop", file);
	      count++;
	    }
	}
    }
    break;

  case '<':
    {
      /* Emit up to 2 nops for bnzr or bnzi instructions */
      int count = 0;

      if (final_sequence == 0)
	{
	  while (count < 2)
	    {
	      fputs ("\n\tnop", file);
	      count++;
	    }
	}
    }
    break;

  case '*': {
    /* Write a NOP instruction if we are not optimizing branch-delay slots */
      
    /* (SuperH and SPARC check only -- whether optimizing or not -- that there
       is nothing now in a sequence(...) that could be a delay-slot
       insn so we will also.) */
    if (dbr_sequence_length() == 0)
      fputs ("\n\tnop", file);
    break;      
  }

  case 'F':
    {
      int flag = INTVAL(x);

      switch (flag)
	{
	case FLAG_T:
	  fputs("T", file);
	  break;

	case FLAG_UNLNK:
	  fputs("UNLNK", file);
	  break;

	case FLAG_DLNK:
	  fputs("DLNK", file);
	  break;

	case FLAG_FLUSH:
	  fputs("FLUSH", file);
	  break;

	case FLAG_PRE:
	  fputs("PRE", file);
	  break;

	case FLAG_SIGN:
	  fputs("SIGN", file);
	  break;

	case FLAG_Z:
	  fputs("Z", file);
	  break;

	case FLAG_ZB:
	  fputs("ZB", file);
	  break;

	case FLAG_CUME:
	  fputs("CUME", file);
	  break;

	case FLAG_RB:
	  fputs("RB", file);
	  break;

	case FLAG_FP:
	  fputs("FP", file);
	  break;

	case FLAG_NT:
	  fputs("NT", file);
	  break;

	case FLAG_NB:
	  fputs("NB", file);
	  break;

	case FLAG_NC:
	  fputs("NC", file);
	  break;

	case FLAG_S:
	  fputs("S", file);
	  break;

	case FLAG_A:
	  fputs("A", file);
	  break;

	case FLAG_R:
	  fputs("R", file);
	  break;

	case FLAG_NUL:
	  fputs("NUL", file);
	  break;

	case FLAG_GR:
	  fputs("GR", file);
	  break;

	case FLAG_GW:
	  fputs("GW", file);
	  break;

	case FLAG_FR:
	  fputs("FR", file);
	  break;

	case FLAG_GU:
	  fputs("GU", file);
	  break;

	case FLAG_GZ:
	  fputs("GZ", file);
	  break;

	case FLAG_NE:
	  fputs("NE", file);
	  break;

	case FLAG_CLOSE:
	  fputs("CLOSE", file);
	  break;

	case FLAG_RSCH:
	  fputs("RSCH", file);
	  break;

	case FLAG_H:
	  fputs("h", file);
	  break;

	case FLAG_I:
	  fputs("I", file);
	  break;

	case FLAG_DI:
	  fputs("DI", file);
	  break;

	case FLAG_CD:
	  fputs("CD", file);
	  break;

	case FLAG_FPB:
	  fputs("FPB", file);
	  break;

	case FLAG_CSB:
	  fputs("CSB", file);
	  break;

	case FLAG_DELETE:
	  fputs("DELETE", file);
	  break;

	case FLAG_OAM:
	  fputs("OAM", file);
	  break;

	case FLAG_RP:
	  fputs("RP", file);
	  break;

	case FLAG_LOG:
	  fputs("LOG", file);
	  break;

	case -1:
	  break;

	default:
	  output_operand_lossage ("Invalid operand for 'F'");
	  break;
	}
    }
    break;

  case 'C': 
  case 'D':
    /* Write out comparison name.  */
    {
      enum rtx_code c = GET_CODE (x);
	
      if (GET_RTX_CLASS (c) != '<')
	output_operand_lossage ("invalid %%C value");
	
      if (code == 'D')
	c = reverse_condition (c);

      switch (c)
	{
	case NE:
	  fprintf(file, "!ESS_EQ");
	  break;

	case EQ:
	  fprintf(file, "ESS_EQ");
	  break;

	case GT:
	case GTU:
	  fprintf(file, "ESS_GT");
	  break;

	case LT:
	case LTU:
	  fprintf(file, "ESS_LT");
	  break;

	case GE:
	case GEU:
	  fprintf(file, "!ESS_LT");
	  break;

	case LE:
	case LEU:
	  fprintf(file, "!ESS_GT");
	  break;

	default:
	  break;
	  /* do nothing */
	}
    }
    break;
    
  case 'B':
    {
      enum rtx_code c = GET_CODE (x);
      switch (c)
	{
	case NE:
	  fprintf(file, "BANEB");
	  break;

	case EQ:
	  fprintf(file, "BAEB");
	  break;

	case GT:
	case GTU:
	  fprintf(file, "BAGB");
	  break;

	case LT:
	case LTU:
	  fprintf(file, "BALB");
	  break;

	case GE:
	case GEU:
	  fprintf(file, "BAGEB");
	  break;

	case LE:
	case LEU:
	  fprintf(file, "BALEB");
	  break;

	default:
	  break;
	  /* do nothing */
	}
    }
    break;

  case 'X':
    print_modx_value(file, x);
    break;

  case 'Y':
  case 0:
    {
      if (REG_P (x))
	{
	  fprintf (file, "%s", reg_names[REGNO (x)]);
	}
      else if (GET_CODE (x) == MEM)
	{
	  output_address (XEXP(x, 0));
	}
      else if (GET_CODE (x) == HIGH)
	{
	  fprintf (file, "hi16(");
	  output_addr_const (file, XEXP (x, 0));
	  fprintf (file, ")");
	}
      else
	{
	  /* CONST_INT, SYMBOL_REF, and others */
	  /*   (see in final.c)                */
	  if (is_label_ref_expr(x))
	    {
	      print_label_ref_expr(file, x);
	    }
          else
	    output_addr_const (file, x); 
	}
      break;
    }
  default:
    {
      printf ("code=%c\n", code);
      output_operand_lossage ("invalid %%xn code");
    }
  }
}


/* Conditional Branch handling, including delay slots, nullification,
   and branch prediction. */

/* The function 'mostly_false_jump (...)' is taken directly from 
   gcc/config/m88k/m88k.c of circa 13 Sep 1999 as seen in GCC 2.95.2
   There were 2 other heuristics concerning jumps to function return
   and guesses about the directions of jumps based solely on the
   particular condition (e.g., NE vs EQ, etc.) that have been omitted
   below.  -- 2 Feb 2001, Pres */

/* Return truth value of the statement that this conditional branch is likely
   to fall through.  CONDITION, is the condition that JUMP_INSN is testing.  */

int
mostly_false_jump (rtx jump_insn, 
		   rtx condition ATTRIBUTE_UNUSED)
{
  rtx target_label = JUMP_LABEL (jump_insn);
  rtx insnt, insnj;

  /* Much of this isn't computed unless we're optimizing.  */
  if (optimize == 0)
    return 0;

  /* (Right here is where the 'jump to function return' heuristic code
     was in the m88k.c original.) */

  /* Predict loops to loop.  */
  for (insnt = PREV_INSN (target_label);
       insnt && GET_CODE (insnt) == NOTE;
       insnt = PREV_INSN (insnt))
    if (NOTE_LINE_NUMBER (insnt) == NOTE_INSN_LOOP_END)
      return 1;
    else if (NOTE_LINE_NUMBER (insnt) == NOTE_INSN_LOOP_BEG)
      return 0;
    else if (NOTE_LINE_NUMBER (insnt) == NOTE_INSN_LOOP_CONT)
      return 0;

  /* Predict backward branches usually take.  */
  if (final_sequence)
    insnj = NEXT_INSN (PREV_INSN (XVECEXP (final_sequence, 0, 0)));
  else
    insnj = jump_insn;


  return 1;
}


int
spl_ess_comparison_operator (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  enum rtx_code code = GET_CODE (op);

  if (mode != GET_MODE (op) && mode != VOIDmode)
    return 0;

  return (code == EQ || code == NE);
}

const char*
output_cmov_spl(const char* op0,
                rtx op1,
		rtx op2,
		const char* op3,
		rtx op4)
{

  /*
    cmov %3 %r4 %0 [!] ESS_SPECIAL
    cmovi %3 %4 %0 [!] ESS_SPECIAL
   */

  static char buf[100];
  static char cmov[] = "cmov ";
  static char cmovi[] = "cmovi ";

  enum rtx_code code = GET_CODE (op1);

  buf[0] = '\0';
  
  if(REG_P(op4))
    strcat(buf, cmov);
  else
    strcat(buf, cmovi);
  
  strcat(buf, op3);
  
  if(REG_P(op4))
    strcat(buf, " %r4 ");
  else
    strcat(buf, " %4 ");
  

  strcat(buf, op0);

  switch (code)
    {
      /* ess fake register is compared with 1 to
	 determine whether branch is taken */
    case EQ:
      break;
      
    case NE:
      strcat(buf, " !");
      break;
      
    default:
      fprintf(stderr, "Error: Invalid cmov operator");
      abort();
    }

  strcat(buf, reg_names[REGNO(op2)]);

  return buf;
}

const char*
output_cbranch (unsigned int ess,
		rtx cond_op, 
		unsigned int reversed,
		const char* ordinal,
		rtx insn)
{
  static char buf[100];
  static char bci[] = "bci ";
  static char bcdi[] = "bcdi ";

  int nop_needed = 0;
  int set_C_flag_p = 0;
  static char C_branch_flag[] = " C";
  int set_BF_flag_p = 0;
  static char BF_branch_flag[] = " BF";
  rtx delay;

  enum rtx_code code = GET_CODE (cond_op);

  if (reversed)
    code = reverse_condition (code);

  buf[0] = '\0';

  if (has_delay_slots())
    {
      if (dbr_sequence_length() == MAX_CBRANCH_DELAY_SLOTS)
	strcat(buf, bcdi);
      else
	strcat(buf, bci);
    }
  else
    {
      strcat(buf, bci);
      nop_needed = 1;
    }

  strcat(buf, "\t");

  /* Add the label to the output string */
  strcat(buf, ordinal);

  if(ess == REG_ESS)
    {
      switch (code)
	{
	case NE:
	  strcat(buf, " !ESS_EQ");
	  break;
	  
	case EQ:
	  strcat(buf, " ESS_EQ");
	  break;
	  
	case GT:
	case GTU:
	  strcat(buf, " ESS_GT");
	  break;
	  
	case LT:
	case LTU:
	  strcat(buf, " ESS_LT");
	  break;
	  
	case GE:
	case GEU:
	  strcat(buf, " !ESS_LT");
	  break;
	  
	case LE:
	case LEU:
	  strcat(buf, " !ESS_GT");
	  break;
	  
	default:
	  /* do nothing */
	  break;
	}
    }
  else
    {
      switch (code)
	{
	  /* ess fake register is compared with 1 to
	     determine whether branch is taken */
	case NE:
	  strcat(buf, " !");
	  break;
	case EQ:
	  break;
	default:
	  fprintf(stderr, "invalid condition code for special ess");
	  abort();
	}

      switch(ess)
	{
	case REG_ESS_PIT1: 
	case REG_ESS_PIT0:
	case REG_ESS_BMEEXHAUST:
	case REG_ESS_BRALWAYS:
	case REG_ESS_SRDMARQFULL:
	case REG_ESS_PDMARQFULL:
	case REG_ESS_UDMA0RQFULL:
	case REG_ESS_UDMA1RQFULL:
	case REG_ESS_BMEHWEPD:
	case REG_ESS_BMESWEPD:
	case REG_ESS_INT:
	case REG_ESS_NMI:	
	case REG_ESS_CARRY:
	case REG_ESS_OVERFLOW:
	  strcat(buf, " ");
	  strcat(buf, reg_names[ess]);
	  break;
	default:
	  fprintf(stderr, "invalid special ess register");
	  abort();
	}
		
    }

  if (final_sequence)
    {
      delay = XVECEXP (final_sequence, 0, 1);
      if ( INSN_ANNULLED_BRANCH_P (insn) )
	{
	  set_C_flag_p  = 1;
	  set_BF_flag_p = (INSN_FROM_TARGET_P (delay) != 0);
	}
      else 
	{
	  /* No annulling of any delay-slot should happen */

	  set_BF_flag_p = ! mostly_false_jump (insn, cond_op);
	  /* BF says branch predicted to be taken */
	}
    }

  /* (The below are somewhat independent because it might be useful
     to predict or nullify a nop in a delay slot.) */

  if (set_C_flag_p)
    strcat(buf, C_branch_flag);

  if (set_BF_flag_p)
    strcat(buf, BF_branch_flag);

  if (nop_needed)
    strcat(buf, "%*");

  return (buf);
}

const char*
output_cond_call(unsigned int ess,
		 rtx cond_op,   
		 rtx call_opnd, 
		 unsigned int reversed,
		 const char* ordinal,
		 rtx insn ATTRIBUTE_UNUSED,
		 const char* rpc_ordinal)
{
  static char buf[100];
  static char bci[] = "bci ";
  static char bcr[] = "bcr ";
  static char bcdi[] = "bcdi ";
  static char bcdr[] = "bcdr ";

  int nop_needed = 0;

  enum rtx_code code = GET_CODE (cond_op);

  if (reversed)
    code = reverse_condition (code);

  buf[0] = '\0';

  if (dbr_sequence_length() == MAX_CBRANCH_DELAY_SLOTS)
    {
      if (immediate_operand(call_opnd, GET_MODE (call_opnd)))
	{
	  strcat(buf, bcdi);
	}
      else
	{
	  strcat(buf, bcdr);
	}
    }
  else 
    {
      if (immediate_operand(call_opnd, GET_MODE(call_opnd)))
	{
	  strcat(buf, bci);
	}
      else
	{
	  strcat(buf, bcr);
	}

      nop_needed = 1;
    }
  /* Add the register ordinal to the output string */
  strcat(buf, ordinal);

  if(ess == REG_ESS)
    {
      switch (code)
	{
	case NE:
	  strcat(buf, " !ESS_EQ");
	  break;
	  
	case EQ:
	  strcat(buf, " ESS_EQ");
	  break;
	  
	case GT:
	case GTU:
	  strcat(buf, " ESS_GT");
	  break;
	  
	case LT:
	case LTU:
	  strcat(buf, " ESS_LT");
	  break;

	case GE:
	case GEU:
	  strcat(buf, " !ESS_LT");
	  break;
	  
	case LE:
	case LEU:
	  strcat(buf, " !ESS_GT");
	  break;

	default:
	  /* do nothing */
	  break;
	}
    }
  else
    {
      switch (code)
	{
	  /* ess fake register is compared with 1 to
	     determine whether branch is taken */
	case NE:
	  strcat(buf, " !");
	  break;
	case EQ:
	  break;
	default:
	  fprintf(stderr, "invalid condition code for special ess");
	  abort();
	}

      switch(ess)
	{
	case REG_ESS_PIT1: 
	case REG_ESS_PIT0:
	case REG_ESS_BMEEXHAUST:
	case REG_ESS_BRALWAYS:
	case REG_ESS_SRDMARQFULL:
	case REG_ESS_PDMARQFULL:
	case REG_ESS_UDMA0RQFULL:
	case REG_ESS_UDMA1RQFULL:
	case REG_ESS_BMEHWEPD:
	case REG_ESS_BMESWEPD:
	case REG_ESS_INT:
	case REG_ESS_NMI:	
	case REG_ESS_CARRY:
	case REG_ESS_OVERFLOW:
	  strcat(buf, " ");
	  strcat(buf, reg_names[ess]);
	  break;
	default:
	  fprintf(stderr, "invalid special ess register");
	  abort();
	}
		
    }
  
  strcat(buf, rpc_ordinal);

  if (nop_needed)
    strcat(buf, "%*");
  
  return (buf);

}

const char*
output_cond_return(unsigned int ess,
		   rtx cond_op,
		   rtx call_opnd ATTRIBUTE_UNUSED,
		   unsigned int reversed,
		   const char* ordinal)
{
  static char buf[100];
  static char bcr[] = "bcr ";
  static char bcdr[] = "bcdr ";

  int nop_needed = 0;
  enum rtx_code code = GET_CODE (cond_op);

  if (reversed)
    code = reverse_condition (code);

  buf[0] = '\0';

  if (dbr_sequence_length() == MAX_CBRANCH_DELAY_SLOTS)
    {
      strcat(buf, bcdr);
    }
  else 
    {
      strcat(buf, bcr);
      nop_needed = 1;
    }

  /* Add the register ordinal to the output string */
  strcat(buf, ordinal);

  if(ess == REG_ESS)
    {
   
      switch (code)
	{
	case NE:
	  strcat(buf, " !ESS_EQ");
	  break;

	case EQ:
	  strcat(buf, " ESS_EQ");
	  break;

	case GT:
	case GTU:
	  strcat(buf, " ESS_GT");
	  break;

	case LT:
	case LTU:
	  strcat(buf, " ESS_LT");
	  break;

	case GE:
	case GEU:
	  strcat(buf, " !ESS_LT");
	  break;

	case LE:
	case LEU:
	  strcat(buf, " !ESS_GT");
	  break;

	default:
	  /* do nothing */
	  break;
	}
    }
  else
    {
      switch (code)
	{
	  /* ess fake register is compared with 1 to
	     determine whether branch is taken */
	case NE:
	  strcat(buf, " !");
	  break;
	case EQ:
	  break;
	default:
	  fprintf(stderr, "invalid condition code for special ess");
	  abort();
	}

      switch(ess)
	{
	case REG_ESS_PIT1: 
	case REG_ESS_PIT0:
	case REG_ESS_BMEEXHAUST:
	case REG_ESS_BRALWAYS:
	case REG_ESS_SRDMARQFULL:
	case REG_ESS_PDMARQFULL:
	case REG_ESS_UDMA0RQFULL:
	case REG_ESS_UDMA1RQFULL:
	case REG_ESS_BMEHWEPD:
	case REG_ESS_BMESWEPD:
	case REG_ESS_INT:
	case REG_ESS_NMI:	
	case REG_ESS_CARRY:
	case REG_ESS_OVERFLOW:
	  strcat(buf, " ");
	  strcat(buf, reg_names[ess]);
	  break;
	default:
	  fprintf(stderr, "invalid special ess register");
	  abort();
	}
		
    }

  if (nop_needed)
    strcat(buf, "%*");
  
  return (buf);

}

bool general_memory_insn(rtx expr)
{
  switch(GET_CODE(expr))
    {
    case PLUS:
      return general_memory_insn(XEXP(expr, 0));
      break;
    case MINUS:
      return general_memory_insn(XEXP(expr, 0));
      break;
    case CONST:
      return general_memory_insn(XEXP(expr, 0));
      break;
    case REG:
      return (REG_P(expr) && 
	      !REGNO_OK_FOR_MEMORY_REGION_P (REGNO (expr)));
      break;
    default:      
      break;
    }
  return false;
}

bool base_spram_memory_insn(rtx expr)
{
  switch(GET_CODE(expr))
    {
    case PLUS:
      return base_spram_memory_insn(XEXP(expr, 0));
      break;
    case MINUS:
      return base_spram_memory_insn(XEXP(expr, 0));
      break;
    case CONST:
      return base_spram_memory_insn(XEXP(expr, 0));
      break;
    case REG:
      {
	if(REG_P(expr))
	  {
	    if(REGNO_OK_FOR_MEMORY_REGION_P (REGNO (expr)))
	      {
		if(REGNO(expr) == BASE_SPRAM_REGNUM ||
		   REGNO(expr) == BASE_SPRAM_LOCK_REGNUM)
		  {
		    return true;
		  }
	      }
	  }
	break;
      }
    default:      
      break;
    }
  return false;
}

const char* output_smm(rtx mem_dst, rtx reg_src, const char* src_fmt, const char* dst_fmt)
{
  static char buf[100];
  
  static char smm[] = "smm ";
  static char smms[] = "smms ";

  static char err[] = "smm print error";

  buf[0] = '\0';

  if(GET_CODE(reg_src) != REG &&
     GET_CODE(mem_dst) != MEM)
    {
      strcat(buf, err);
      return buf;
    }

  if(base_spram_memory_insn(XEXP(mem_dst, 0)))
    {
      strcat(buf, smms);
      strcat(buf, src_fmt);
      strcat(buf, dst_fmt);
      return buf;
    }
  else
    {
      strcat(buf, smm);
      strcat(buf, src_fmt);
      strcat(buf, dst_fmt);
      return buf;
    }

}

const char* output_lmm(rtx reg_dst, rtx mem_src, const char* dst_fmt, const char* src_fmt)
{
  static char buf[100];
  
  static char lmm[] = "lmm ";
  static char lmms[] = "lmms ";


  static char err[] = "lmm print error";

  buf[0] = '\0';

  if(GET_CODE(reg_dst) != REG &&
     GET_CODE(mem_src) != MEM)
    {
      strcat(buf, err);
      return buf;
    }

  if(base_spram_memory_insn(XEXP(mem_src, 0)))
    {
      strcat(buf, lmms);
      strcat(buf, dst_fmt);
      strcat(buf, src_fmt);
      return buf;
    }
  else
    {
      strcat(buf, lmm);
      strcat(buf, dst_fmt);
      strcat(buf, src_fmt);
      return buf;
    }

}

const char* output_lmm_with_lnk(rtx reg_dst, rtx mem_src, const char* dst_fmt, const char* src_fmt, const char *lnk_fmt)
{
  static char buf[100];
  
  static char lmm[] = "lmm ";
  static char lmms[] = "lmms ";


  static char err[] = "lmm print error";

  buf[0] = '\0';

  if(GET_CODE(reg_dst) != REG &&
     GET_CODE(mem_src) != MEM)
    {
      strcat(buf, err);
      return buf;
    }

  if(base_spram_memory_insn(XEXP(mem_src, 0)))
    {
      strcat(buf, lmms);
      strcat(buf, dst_fmt);
      strcat(buf, src_fmt);
      strcat(buf, lnk_fmt);
      return buf;
    }
  else
    {
      strcat(buf, lmm);
      strcat(buf, dst_fmt);
      strcat(buf, src_fmt);
      strcat(buf, lnk_fmt);
      return buf;
    }

}


int
octave_sa_size ()
{
  int sa_size = 0;
  int i;

  for (i = 0; i < FIRST_PSEUDO_REGISTER; i++)
    if (! fixed_regs[i] && ! call_used_regs[i] && regs_ever_live[i] )
      sa_size++;

  if (! current_function_is_leaf)
    sa_size += 1;

  return (sa_size * UNITS_PER_WORD);
}

int
octave_frame_size (int size)
{
  return (ROUND4(size)					   /* Locals area */
	  + octave_sa_size ()		 /* Register Save Area (incl. RPC and FRP) */
	  + ROUND4(current_function_pretend_args_size)); /* incoming stack Args */
}


void
old_octave_split_constant (
			   enum machine_mode mode,
			   HOST_WIDE_INT val,
			   rtx target,
			   int subtargets)
{
  HOST_WIDE_INT low = (val & 0xffff);
  HOST_WIDE_INT high = (val & 0xffff0000);
  rtx temp = subtargets ? gen_reg_rtx (mode) : target;

  emit_insn (gen_rtx (SET, VOIDmode, temp, GEN_INT (low)));
  emit_insn (gen_rtx (SET, VOIDmode, target,
		      gen_rtx (PLUS, mode, temp, GEN_INT (high))));
}

void
octave_sp_plus_long_const_to_reg(FILE *file, HOST_WIDE_INT offset, int out_reg)
{
  if (offset <= 0xffff)
    {
      fprintf(file, "\tmv\tSP %s\t; set SP+offset address\n", reg_names[out_reg]);
      fprintf(file, "\tlail\t%s %d\t;^\n", reg_names[out_reg], offset);
    }
  else
    {
      fprintf(file, "\tmv\tSP %s\t; set SP+offset address\n", reg_names[out_reg]);
      fprintf(file, "\tlail\t%s lo16(%d)\t;^\n", reg_names[out_reg], offset);
      fprintf(file, "\tlaih\t%s hi16(%d)\t;^\n", reg_names[out_reg], offset);
    }

}

static void
emit_stack_save_insn (int start_save_regno, int end_save_regno, int save_offset)
{
  rtx start_save_reg = gen_rtx_REG (SImode, start_save_regno);  

  if (save_offset <= OCTAVE_OFFSET_MAX)
    {
      if (start_save_regno == end_save_regno)
	{
	  FRM_RELATED (emit_insn (gen_movsi (gen_rtx_MEM (SImode, plus_constant (stack_pointer_rtx,
										 save_offset)),
					     start_save_reg)));
	}
      else
	{
	  FRM_RELATED (emit_insn (gen_store_multiple (gen_rtx_MEM (SImode,
								   plus_constant (stack_pointer_rtx,
										  save_offset)),
						      start_save_reg,
						      GEN_INT (end_save_regno - start_save_regno + 1))));
			      
	}
    }
  else
    {
      rtx temp0_reg = gen_rtx_REG (SImode, TR0_REGNUM);
      rtx mem;

      FRM_RELATED (emit_insn (gen_movsi (temp0_reg, stack_pointer_rtx)));
      FRM_RELATED (emit_insn (gen_add (temp0_reg, temp0_reg, GEN_INT (save_offset))));

      mem = gen_rtx_MEM (SImode, temp0_reg);

      if (start_save_regno == end_save_regno)
	{
	  FRM_RELATED (emit_insn (gen_movsi (mem, start_save_reg)));
	}
      else
	{
	  FRM_RELATED (emit_insn (gen_store_multiple (mem,
						      start_save_reg, 
						      GEN_INT (end_save_regno - start_save_regno + 1))));
	}
    }
}


static void
emit_stack_restore_insn (int start_restore_regno, int end_restore_regno, int save_offset)
{
  rtx insn;
  rtx start_restore_reg = gen_rtx_REG (SImode, start_restore_regno);  

  if (save_offset <= OCTAVE_OFFSET_MAX)
    {
      if (start_restore_regno == end_restore_regno)
	{
	  insn = emit_insn (gen_movsi (start_restore_reg,
				       gen_rtx_MEM (SImode, 
						    plus_constant (stack_pointer_rtx, save_offset))));
          RTX_FRAME_RELATED_P(insn) = 1;
	}
      else
	{
	  insn = emit_insn (gen_load_multiple (start_restore_reg,
					       gen_rtx_MEM (SImode, 
							    plus_constant (stack_pointer_rtx,save_offset)),
					       GEN_INT (end_restore_regno - start_restore_regno + 1)));
			      
          RTX_FRAME_RELATED_P(insn) = 1;
	}
    }
  else
    {
      rtx temp0_reg = gen_rtx_REG (SImode, TR0_REGNUM);
      rtx mem;

      insn =   emit_insn (gen_movsi (temp0_reg, stack_pointer_rtx));
      RTX_FRAME_RELATED_P(insn) = 1;
      insn = emit_insn (gen_add (temp0_reg, temp0_reg, GEN_INT (save_offset)));
      RTX_FRAME_RELATED_P(insn) = 1;

      mem = gen_rtx_MEM (SImode, temp0_reg);

      if (start_restore_regno == end_restore_regno)
	{
	  insn = emit_insn (gen_movsi (start_restore_reg, mem));
          RTX_FRAME_RELATED_P(insn) = 1;
	}
      else
	{
	  insn = emit_insn (gen_load_multiple (start_restore_reg, mem,
					       GEN_INT (end_restore_regno - start_restore_regno + 1)));
          RTX_FRAME_RELATED_P(insn) = 1;
	}
    }
}

static rtx
set_frame_related_p ()
{
  rtx seq = gen_sequence ();
  end_sequence ();

  if (GET_CODE (seq) == SEQUENCE)
    {
      int i = XVECLEN (seq, 0);
      while (--i >= 0)
	RTX_FRAME_RELATED_P (XVECEXP (seq, 0, i)) = 1;
      return emit_insn (seq);
    }
  else
    {
      seq = emit_insn (seq);
      RTX_FRAME_RELATED_P (seq) = 1;
      return seq;
    }
}


int m480_null_epilogue_p ()
{
  int regno;
  
  if(reload_completed && !get_frame_size())
    {
      for(regno=0; regno<= FIRST_PSEUDO_REGISTER; regno++)
	{
	  if(regs_ever_live[regno] && !call_used_regs[regno])
	    return 0;
	  return 1;
	}
    }
  return 0;
}

void
m480_expand_prologue ()
{

  HOST_WIDE_INT initial_frame_size = get_frame_size ();
  HOST_WIDE_INT frame_size = octave_frame_size (initial_frame_size);
  rtx rtx_frame_size = GEN_INT (frame_size);
  HOST_WIDE_INT reg_offset = ROUND4 (initial_frame_size);
  HOST_WIDE_INT i;
  HOST_WIDE_INT startsave = -1;

  fprintf (asm_out_file, "\t; function_is_leaf=%d\n", current_function_is_leaf);
  fprintf (asm_out_file,
	   "\t; frame_size=%d  local_size=%d  sa_size=%d  stack_inargs_size=%d\n",
	   frame_size, initial_frame_size, octave_sa_size (), current_function_pretend_args_size);
  fprintf (asm_out_file, "\t; (max. out_args=%d   are later pushed by the code)\n",
	   current_function_outgoing_args_size);
  fprintf (asm_out_file, "\t; rpc_live=%d  sp_live=%d  fp_needed=%d  fp_live=%d\n",
	   regs_ever_live[RPC_REGNUM], regs_ever_live[SP_REGNUM],
	   frame_pointer_needed, regs_ever_live[FRP_REGNUM]);

  fprintf (asm_out_file, "\n");


  if (frame_size != 0)
    {
      if (frame_size < 64)
	{
	  FRM_RELATED (emit_insn (gen_sub (stack_pointer_rtx, stack_pointer_rtx, rtx_frame_size)));
	}
      else
	{
	  rtx temp_reg = gen_rtx_REG(SImode, TR0_REGNUM);
	  FRM_RELATED (emit_insn (gen_movsi(temp_reg, rtx_frame_size)));

	  FRM_RELATED (emit_insn (gen_sub (stack_pointer_rtx, stack_pointer_rtx, temp_reg)));
	}
    }


  if (!current_function_is_leaf)
    {
      rtx rpc = gen_rtx_REG (SImode, RPC_REGNUM);
      rtx temp1_reg = gen_rtx_REG (SImode, TR1_REGNUM);

      if (reg_offset < OCTAVE_OFFSET_MAX)
	{

	  FRM_RELATED (emit_insn (gen_movsi (gen_rtx_MEM (SImode, 
							  plus_constant (stack_pointer_rtx, reg_offset)),
					     rpc /*temp1_reg*/)));
	}
      else
	{
	  rtx temp0_reg = gen_rtx_REG (SImode, TR0_REGNUM);
	  FRM_RELATED (emit_insn (gen_movsi (temp0_reg, stack_pointer_rtx)));
	  FRM_RELATED (emit_insn (gen_add (temp0_reg, temp0_reg, GEN_INT (reg_offset))));
	  FRM_RELATED (emit_insn (gen_movsi (temp1_reg, rpc)));

	  FRM_RELATED (emit_insn (gen_movsi (gen_rtx_MEM (SImode, temp0_reg),
					     temp1_reg)));
	}

      reg_offset += UNITS_PER_WORD;
    }

  for (i = 0; i <= FIRST_PSEUDO_REGISTER; i++)
    {
      if ( ! fixed_regs[i] &&	     /* Don't save/rstore fixed registers */
	   ! call_used_regs[i] &&    /* Don't save/rest. caller-(already)-saved regs */
	   regs_ever_live[i] &&	     /* Do save/rest. all live (callee-saved) regs   */
	   i != FIRST_PSEUDO_REGISTER /* (note bounds of loop -- finds end-of-seq) */
	   )
	{
	  if (startsave < 0)
	    startsave = i;
	}
      else
	{
	  if (startsave >= 0)
	    {
	      emit_stack_save_insn (startsave, i - 1, reg_offset);
	      reg_offset += (i - startsave) * UNITS_PER_WORD;
	      startsave = -1;
	    }
	}
    }

  if (frame_pointer_needed)
    {
      FRM_RELATED (emit_insn (gen_movsi (gen_rtx_REG (SImode, FRP_REGNUM),
					 stack_pointer_rtx)));
    }
}		     
	  

void
m480_expand_epilogue ()
{
  HOST_WIDE_INT i;
  HOST_WIDE_INT frame_size = octave_frame_size (get_frame_size ());
  rtx rtx_frame_size = GEN_INT (frame_size);
  HOST_WIDE_INT reg_offset = ROUND4 (get_frame_size ());
  rtx temp0_reg = gen_rtx_REG (SImode, TR0_REGNUM);
  rtx rpc_reg = gen_rtx_REG (SImode, RPC_REGNUM);
  HOST_WIDE_INT startsave = -1;
  rtx insn;

  if (frame_pointer_needed)
    {
      insn = emit_insn (gen_movsi (stack_pointer_rtx,
				   gen_rtx_REG (SImode, FRP_REGNUM)));

      RTX_FRAME_RELATED_P(insn) = 1;
    }

  if (! current_function_is_leaf)
    {
      if (TARGET_480)
	{
	  if (reg_offset < OCTAVE_OFFSET_MAX)
	    {
	      insn = emit_insn (gen_movsi (temp0_reg, 
					   gen_rtx_MEM (SImode, 
							plus_constant (stack_pointer_rtx, reg_offset))));
              RTX_FRAME_RELATED_P(insn) = 1;
	      insn = emit_insn (gen_movsi (rpc_reg, temp0_reg));
              RTX_FRAME_RELATED_P(insn) = 1;
	    }
	  else
	    {
	      insn = emit_insn (gen_movsi (temp0_reg, stack_pointer_rtx));
              RTX_FRAME_RELATED_P(insn) = 1;
	      insn = emit_insn (gen_addsi3 (temp0_reg, temp0_reg, GEN_INT (reg_offset)));
              RTX_FRAME_RELATED_P(insn) = 1;
	      insn = emit_insn (gen_movsi (temp0_reg, gen_rtx_MEM (SImode, temp0_reg)));
              RTX_FRAME_RELATED_P(insn) = 1;
	      insn = emit_insn (gen_movsi (rpc_reg, temp0_reg));
              RTX_FRAME_RELATED_P(insn) = 1;
	    }
	}
      else			/* Assume 480 */
	{
	  if (reg_offset < OCTAVE_OFFSET_MAX)
	    {
	      insn = emit_insn (gen_movsi (rpc_reg, 
					   gen_rtx_MEM (SImode, 
							plus_constant (stack_pointer_rtx, reg_offset))));
              RTX_FRAME_RELATED_P(insn) = 1;
	    }
	  else
	    {
	      insn = emit_insn (gen_movsi (temp0_reg, stack_pointer_rtx));
              RTX_FRAME_RELATED_P(insn) = 1;
	      insn = emit_insn (gen_add (temp0_reg, temp0_reg, GEN_INT (reg_offset)));
              RTX_FRAME_RELATED_P(insn) = 1;
	      insn = emit_insn (gen_movsi (rpc_reg, gen_rtx_MEM (SImode, temp0_reg)));
              RTX_FRAME_RELATED_P(insn) = 1;
	    }

	  reg_offset += UNITS_PER_WORD;
	}
    }
  
  for (i = 0; i <= FIRST_PSEUDO_REGISTER; i++)
    {
      if ( ! fixed_regs[i] &&	     /* Don't save/rstore fixed registers */
	   ! call_used_regs[i] &&    /* Don't save/rest. caller-(already)-saved regs */
	   regs_ever_live[i] &&	     /* Do save/rest. all live (callee-saved) regs   */
	   i != FIRST_PSEUDO_REGISTER /* (note bounds of loop -- finds end-of-seq) */
	   )
	{
	  if (startsave < 0)
	    startsave = i;
	}
      else
	{
	  if (startsave >= 0)
	    {
	      emit_stack_restore_insn (startsave, i - 1, reg_offset);
	      reg_offset += (i - startsave) * UNITS_PER_WORD;
	      startsave = -1;
	    }
	}
    }

  if (frame_size > 63)
    {
      rtx temp0_reg = gen_rtx_REG (SImode, TR0_REGNUM);
      insn = emit_insn (gen_movsi (temp0_reg, rtx_frame_size));
      RTX_FRAME_RELATED_P(insn) = 1;
      insn = emit_insn (gen_add (stack_pointer_rtx, temp0_reg, stack_pointer_rtx));
      RTX_FRAME_RELATED_P(insn) = 1;
    }
  else
    {
      if (frame_size != 0)
	{
	  insn = emit_insn (gen_add (stack_pointer_rtx, stack_pointer_rtx, rtx_frame_size));
	  RTX_FRAME_RELATED_P(insn) = 1;
	}
    }

  insn = emit_jump_insn (gen_c_return(rpc_reg));

  RTX_FRAME_RELATED_P(insn) = 1;
}

int 
octave_eligible_for_epilogue_delay (trial, slot)
     rtx trial;
     int slot; 
{
  if (octave_sa_size()) return 0;
  if (slot!=0) return 0;
  if (get_attr_length (trial) != 4)	return 0;
  return 1;
}

int
octave_aligned_memory_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_CODE (op) == SUBREG)
    {
      if (GET_MODE (op) != mode)
	return 0;
      op = SUBREG_REG (op);
      mode = GET_MODE (op);
    }

  if (reload_in_progress && GET_CODE (op) == REG
      && REGNO (op) >= FIRST_PSEUDO_REGISTER)
    op = reg_equiv_mem[REGNO (op)];

  if (GET_CODE (op) != MEM || GET_MODE (op) != mode
      || ! memory_address_p (mode, XEXP (op, 0)))
    return 0;

  op = XEXP (op, 0);

  if (GET_CODE (op) == PLUS)
    op = XEXP (op, 0);

  return (GET_CODE (op) == REG
	  && REGNO_POINTER_ALIGN (REGNO (op)) >= 4);
}


void
octave_get_aligned_mem (ref, paligned_mem, pbitnum)
     rtx ref;
     rtx *paligned_mem, *pbitnum;
{
  rtx base;
  HOST_WIDE_INT offset = 0;	/* Counts in 'addressable units', */

  if (GET_CODE (ref) == SUBREG)
    {
      offset = SUBREG_BYTE (ref) * UNITS_PER_WORD;

      offset -= (  MIN (UNITS_PER_WORD, GET_MODE_SIZE (GET_MODE (ref)))
		   - MIN (UNITS_PER_WORD,
			  GET_MODE_SIZE (GET_MODE (SUBREG_REG (ref))))  );

      ref = SUBREG_REG (ref);
    }

  if (GET_CODE (ref) != MEM)
    abort ();

  if (reload_in_progress
      && ! memory_address_p (GET_MODE (ref), XEXP (ref, 0)))
    {
      base = find_replacement (&XEXP (ref, 0));

      if (! memory_address_p (GET_MODE (ref), base))
	abort ();
    }
  else
    {
      base = XEXP (ref, 0);
    }

  if (GET_CODE (base) == PLUS)	/* TREE lowering constructed the offset */
    {
      offset += INTVAL (XEXP (base, 1));
      base = XEXP (base, 0);
    }

  *paligned_mem = gen_rtx_MEM (SImode,
			       plus_constant (base, offset & ~3));
  MEM_IN_STRUCT_P (*paligned_mem) = MEM_IN_STRUCT_P (ref);
  MEM_VOLATILE_P (*paligned_mem) = MEM_VOLATILE_P (ref);
  RTX_UNCHANGING_P (*paligned_mem) = RTX_UNCHANGING_P (ref);

  if (GET_MODE (ref) == QImode)
    *pbitnum = GEN_INT ((3-(offset & 0x3)) * 8); /* (CONST_INT i) for i=24,16,8, 0 */
  else if (GET_MODE (ref) == HImode)
    *pbitnum = GEN_INT ((2-(offset & 0x3)) * 8); /* (CONST_INT i) for i=   16, 0 */
  else
    {
      fprintf(stderr, "Unexpected mode, neither QI nor HI, in m480.c:octave_get_aligned_mem()");
      abort();
    }
}


rtx
octave_get_unaligned_address (rtx ref, int extra_offset)
{
  rtx base;
  HOST_WIDE_INT offset = 0;

  if (GET_CODE (ref) != MEM)
    abort ();

  if (reload_in_progress
      && ! memory_address_p (GET_MODE (ref), XEXP (ref, 0)))
    {
      base = find_replacement (&XEXP (ref, 0));

      if (! memory_address_p (GET_MODE (ref), base))
	abort ();
    }
  else
    {
      base = XEXP (ref, 0);
    }

  if (GET_CODE (base) == PLUS)
    {
      offset += INTVAL (XEXP (base, 1));
      base = XEXP (base, 0);
    }

  if (offset + extra_offset > 0)
    {
      rtx dest = gen_reg_rtx(SImode);
      emit_insn (gen_add_for_unaligned(dest, base, GEN_INT (offset + extra_offset)));
      return (dest);
    }
  else
    return base;


}


int
is_aligned_base_pointer(rtx op, enum machine_mode mode)
{

  if (GET_CODE (op) == SUBREG)
    {
      if (GET_MODE (op) != mode)
	return 0;
      op = SUBREG_REG (op);
      mode = GET_MODE (op);
    }

  if (GET_CODE (op) == REG)
    {
      if(cfun->emit->regno_pointer_align == 0)
	return 1; 


      if (mode == SImode || REGNO_POINTER_ALIGN (REGNO (op)) >= 4)
	return 1;
    }

  return 0;
}

static void
octave_set_memflags_1 (rtx x, int in_struct_p, int volatile_p, int unchanging_p)
{
  int i;

  switch (GET_CODE (x)) {
  case SEQUENCE:
  case PARALLEL:
    for (i = XVECLEN (x, 0) - 1; i >= 0; i--)
      octave_set_memflags_1 (XVECEXP (x, 0, i), in_struct_p, volatile_p,
			     unchanging_p);
    break;

  case INSN:
    octave_set_memflags_1 (PATTERN (x), in_struct_p, volatile_p,
			   unchanging_p);
    break;

  case SET:
    octave_set_memflags_1 (SET_DEST (x), in_struct_p, volatile_p,
			   unchanging_p);
    octave_set_memflags_1 (SET_SRC (x), in_struct_p, volatile_p,
			   unchanging_p);
    break;

  case MEM:
    MEM_IN_STRUCT_P (x) = in_struct_p;
    MEM_VOLATILE_P (x) = volatile_p;
    RTX_UNCHANGING_P (x) = unchanging_p;
    break;

  default:
    break;
  }
}

void
octave_set_memflags (rtx insn, rtx ref)
{
  int in_struct_p = MEM_IN_STRUCT_P (ref);
  int volatile_p = MEM_VOLATILE_P (ref);
  int unchanging_p = RTX_UNCHANGING_P (ref);

  if (GET_CODE (ref) != MEM
      || (! in_struct_p && ! volatile_p && ! unchanging_p))
    return;

  octave_set_memflags_1 (insn, in_struct_p, volatile_p, unchanging_p);
}



int get_mem_offset_const(rtx expr)
{
  switch(GET_CODE(expr))
    {
    case PLUS:
      return get_mem_offset_const(XEXP(expr, 1));
      break;
    case MINUS:
      return get_mem_offset_const(XEXP(expr, 1));
      break;
    case CONST:
      return get_mem_offset_const(XEXP(expr, 0));
      break;
    case CONST_INT:
      return INTVAL(expr);
      break;
    default:
      return -1;
    }
}

int
load_multiple_operation (op,mode)
     rtx op;
     enum machine_mode mode ATTRIBUTE_UNUSED;	
{
  int i;
  unsigned int rd1;

  int count;
  int offset;

  /* 
     check the first load operation to make 
     further checking safe.
  */
     
  rtx set = XVECEXP(op, 0, 0);

  if(/* make sure operation is a SET */
     (GET_CODE(set) != SET) ||
     /* make sure dst is a REG */
     (GET_CODE(SET_DEST (set)) != REG) ||
     /* make sure src is a MEM  */
     (! (((GET_CODE(SET_SRC (set))) == MEM) &&
	 get_mem_offset_const(XEXP( SET_SRC (set), 0)) != -1)))
    {
      return 0;
    }

  /*
    check the remaining load operations
  */

  count = XVECLEN (op, 0);

  rd1 = REGNO(SET_DEST(set));
  offset =  get_mem_offset_const(XEXP( SET_SRC (set), 0));

  for(i=0; i< count; i++)
    {
      rtx set_rtx = XVECEXP(op, 0, i);
      rtx set_src = SET_SRC (set_rtx);
      rtx set_dst = SET_DEST (set_rtx);

      if(/* make sure operation is a SET */
	 (GET_CODE(set_rtx) != SET) ||
	 /* make sure dst is a REG */
	 (GET_CODE(set_dst) != REG) ||
	 /* and is in sequential order */
	 (REGNO (set_dst) != rd1 + i)  ||
	 /* make sure src is a MEM  */
	 (! (((GET_CODE(set_src)) == MEM) &&
	     (get_mem_offset_const(XEXP(set_src, 0)) == offset + i*4))))
	  
	{
	  return 0;
	}
    }
  
  return 1;
}

int
store_multiple_operation (op, mode)
     rtx op;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
  int i;
  unsigned int rd1;

  int count;
  int offset;

  /* 
     check the first store operation to make 
     further checking safe.
  */
    
  rtx set = XVECEXP(op, 0, 0);

  if(/* make sure operation is a SET */
     (GET_CODE(set) != SET) ||
     /* make sure dst is a REG */
     (GET_CODE(SET_SRC (set)) != REG) ||
     /* make sure src is a MEM  */
     (! ((GET_CODE(SET_DEST (set)) == MEM ) &&
	 ( get_mem_offset_const(XEXP(SET_DEST(set), 0)) != -1))))
    {
      return 0;
    }

  /*
    check the remaining store operations
  */

  count = XVECLEN (op, 0);
  
  rd1 = REGNO(SET_SRC(set));
  offset = get_mem_offset_const(XEXP(SET_DEST(set), 0));

  for(i=0; i< count; i++)
    {
      rtx set_rtx = XVECEXP(op, 0, i);
      rtx set_src = SET_SRC (set_rtx);
      rtx set_dst = SET_DEST (set_rtx);

      if(/* make sure operation is a SET */
	 (GET_CODE(set_rtx) != SET) ||
	 /* make sure src is a REG */
	 (GET_CODE(set_src) != REG) ||
	 /* and is in sequential order */
	 (REGNO (set_src) != rd1 + i)  ||
	 /* make sure dst is a MEM  */
	 (! (((GET_CODE(set_dst)) == MEM) &&
	     (get_mem_offset_const(XEXP(set_dst, 0))) == offset + i*4 )))
	{
	  return 0;
	}
    }
  
  return 1;
}


int
s_register_operand (op, mode)
     register rtx op;
     enum machine_mode mode;
{
  if (GET_MODE (op) != mode && mode != VOIDmode)
    return 0;

  if (GET_CODE (op) == SUBREG)
    op = SUBREG_REG (op);

  return (REG_P (op)
	  && (REGNO (op) >= FIRST_PSEUDO_REGISTER
	      || REGNO_REG_CLASS (REGNO (op)) != NO_REGS));
}

int
any_memory_operand (op, mode)
     register rtx op;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
  return (GET_CODE (op) == MEM
	  || (GET_CODE (op) == SUBREG && GET_CODE (SUBREG_REG (op)) == REG)
	  || (reload_in_progress && GET_CODE (op) == REG
	      && REGNO (op) >= FIRST_PSEUDO_REGISTER)
	  || (reload_in_progress && GET_CODE (op) == SUBREG
	      && GET_CODE (SUBREG_REG (op)) == REG
	      && REGNO (SUBREG_REG (op)) >= FIRST_PSEUDO_REGISTER));
}

int
reg_or_0_operand (register rtx op,
		  enum machine_mode mode)
{
  return op == const0_rtx || register_operand (op, mode);
}

int
reg_or_6bu_operand (register rtx op,
		    enum machine_mode mode)
{
  return ((GET_CODE (op) == CONST_INT
	   && (unsigned HOST_WIDE_INT) INTVAL (op) < 64)
	  || register_operand (op, mode));
}

int
reg_or_6bs_operand (register rtx op,
		    enum machine_mode mode)
{
  return ((GET_CODE (op) == CONST_INT
	   && (((unsigned HOST_WIDE_INT) INTVAL (op) < 64)
	       || ((unsigned HOST_WIDE_INT) -INTVAL (op) < 64)))
	  || register_operand (op, mode));
}

int
reg_or_5bu_operand (register rtx op,
		    enum machine_mode mode)
{
  return ((GET_CODE (op) == CONST_INT
	   && (unsigned HOST_WIDE_INT) INTVAL (op) < 32)
	  || register_operand (op, mode));
}

int
reg_or_10bs_operand (register rtx op,
		     enum machine_mode mode)
{
  return ((GET_CODE (op) == CONST_INT
	   && CONST_OK_FOR_SI_10BITS(INTVAL(op)))
	  || register_operand (op, mode));
}

int
lim_operand (register rtx op,
	     enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT
	  && (CONST_OK_FOR_UI_17BITS(INTVAL (op))
	      || CONST_OK_FOR_UI_17BITS(INTVAL (op))));
}

int
move_operand (register rtx op, enum machine_mode mode)
{
  if (!general_operand(op, mode))
    return 0;

  if (CONSTANT_ADDRESS_P(op) && GET_CODE (op) != CONST_INT)
    return 0;

  if (GET_CODE (op) == CONST_INT)
    {
      if (lim_operand(op, mode))
	return 1;
      else
	return 0;
    }
  else
    return 1;
}

int
sf_move_operand (register rtx op, enum machine_mode mode)
{
  if (!general_operand(op, mode))
    return 0;

  if (GET_CODE (op) == CONST_DOUBLE)
    return 0;
  else
    return 1;
}


int
reg_or_lim_operand (register rtx op,
		    enum machine_mode mode)
{
  return (lim_operand (op, mode)
	  || register_operand (op, mode));
}


int
c5bu_operand (register rtx op,
	      enum machine_mode mode)
{
  return ((GET_CODE (op) == CONST_INT  && (mode == SImode)
	   && (unsigned HOST_WIDE_INT) INTVAL (op) <= 32));
}

int
c9bu_operand (register rtx op,
	      enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((GET_CODE (op) == CONST_INT
           && (unsigned HOST_WIDE_INT) INTVAL (op) <= 512));
}

int
c6bu_operand (register rtx op,
	      enum machine_mode mode ATTRIBUTE_UNUSED)
{

  return ((GET_CODE (op) == CONST_INT
	   && (unsigned HOST_WIDE_INT) INTVAL (op) <= 64));
}

int
c6bs_operand (register rtx op,
	      enum machine_mode mode ATTRIBUTE_UNUSED)
{

  return ((GET_CODE (op) == CONST_INT
	   &&  (((unsigned HOST_WIDE_INT) INTVAL (op) < 64)
		|| ((unsigned HOST_WIDE_INT) -INTVAL (op) < 64))));
}

int
c1bu_operand (register rtx op,
	      enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT
	  && ( ((unsigned HOST_WIDE_INT) INTVAL (op) == 0) || 
	       ((unsigned HOST_WIDE_INT) INTVAL (op) == 1)));
}

int
c2bu_operand (register rtx op,
	      enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT &&
	  ((unsigned HOST_WIDE_INT) INTVAL (op) <= 3));
}

int
c3bu_operand (register rtx op,
	      enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return (GET_CODE (op) == CONST_INT
	  && ( (unsigned HOST_WIDE_INT) INTVAL (op) <= 7));
}


int
c8bs_operand (register rtx op,
	      enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((GET_CODE (op) == CONST_INT
           && (unsigned HOST_WIDE_INT) INTVAL (op) < 0xffff ));
}

int 
flag_operand (register rtx opnd ATTRIBUTE_UNUSED, 
	      enum machine_mode mode ATTRIBUTE_UNUSED)

{
  if (GET_CODE (opnd) == CONST_INT &&
      (((INTVAL(opnd) <= FLAG_CD) && 
	(INTVAL(opnd) >= FLAG_UNLNK)) ||
       INTVAL(opnd) == -1))
    return 1;
  else
    return 0;
}

int get_flag_operand_v31 (rtx op,
			  enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (GET_CODE (op) != CONST_INT)
    return 0;

  switch (INTVAL (op))
    {
    case FLAG_FP:
    case FLAG_RSCH:
    case FLAG_S:
    case FLAG_A:
    case FLAG_NB:
    case FLAG_NE:
    case FLAG_NT:
    case FLAG_R:
    case FLAG_CLOSE:
      return 1;

    default:
      return 0;
    }
}

int get_flag_operand_v4 (rtx op,
			 enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (GET_CODE (op) != CONST_INT)
    return 0;

  switch (INTVAL (op))
    {
    case FLAG_RSCH:
    case FLAG_CLOSE:
    case FLAG_A:
    case FLAG_FPB:
    case FLAG_NT:
    case FLAG_NB:
    case FLAG_NC:
    case FLAG_S:
    case FLAG_CSB:
    case FLAG_DELETE:
    case FLAG_T:
      return 1;

    default:
      return 0;
    }
}

int get_flag_operand (rtx op,
		      enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (GET_CODE (op) == CONST_INT
      && INTVAL(op) == -1)
    return 1;

  return (get_flag_operand_v31(op, mode)
	  || get_flag_operand_v4(op, mode));
}

int putc_flag_operand_v31 (rtx op,
			   enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (GET_CODE (op) != CONST_INT)
    return 0;

  switch (INTVAL (op))
    {
    case FLAG_FP:
    case FLAG_FR:
    case FLAG_GR:
    case FLAG_GU:
    case FLAG_GW:
    case FLAG_GZ:
    case FLAG_NB:
    case FLAG_NC:
    case FLAG_R:
    case FLAG_I:
      return 1;

    default:
      return 0;
    }
}

int putc_flag_operand_v4 (rtx op,
			  enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (GET_CODE (op) != CONST_INT)
    return 0;

  switch (INTVAL (op))
    {
    case FLAG_GZ:
    case FLAG_A:
    case FLAG_FP:
    case FLAG_OAM:
    case FLAG_NB:
    case FLAG_RP:
    case FLAG_I:
    case FLAG_FR:
    case FLAG_GR:
    case FLAG_GU:
    case FLAG_LOG:
      return 1;

    default:
      return 0;
    }
}

int putc_flag_operand (rtx op,
		       enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (GET_CODE (op) == CONST_INT
      && INTVAL(op) == -1)
    return 1;

  return (putc_flag_operand_v31(op, mode)
	  || putc_flag_operand_v4(op, mode));
}

int putm_flag_operand (rtx op,
		       enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (GET_CODE (op) == CONST_INT
      && INTVAL(op) == -1)
    return 1;

  return (putc_flag_operand_v31(op, mode));
}


int putp_flag_operand (rtx op,
		       enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (GET_CODE (op) == CONST_INT
      && INTVAL(op) == -1)
    return 1;

  return putc_flag_operand (op, mode);
}

int hec5_flag_operand (rtx op,
		       enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (GET_CODE (op) == CONST_INT)
    {
      if (INTVAL (op) == FLAG_CUME
	  || INTVAL (op) == -1)
	{
	  return 1;
	}
    }

  return 0;
}


int
hardware_register_operand(register rtx opnd,
			  enum machine_mode mode ATTRIBUTE_UNUSED)
{

  if(register_operand(opnd, mode) && 
     ((REGNO(opnd) >= FIRST_HARDWARE_REGISTER) &&
      (REGNO(opnd) <= LAST_HARDWARE_REGISTER)))
    {
      return 1;
    }
  else
    {
      return 0;
    }
}


int 
lnk_register_operand (register rtx opnd, 
		      enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (register_operand(opnd, mode) && 
      ((REGNO(opnd) >= LNK1_REGNUM) &&
       (REGNO(opnd) <= LNK3_REGNUM)))
    return 1;
  else
    return 0;
}

int 
overflow_or_carry_ess_operand (register rtx opnd, 
			       enum machine_mode mode)
{
  if (register_operand(opnd, mode))
    {
      switch (REGNO (opnd))
	{
	case ESS_OVERFLOW_REGNUM:
	case ESS_CARRY_REGNUM:
	  return 1;

	default:
	  return 0;
	}
    }
  else
    return 0;
}

int 
spl_ess_reg_operand (register rtx opnd, 
		     enum machine_mode mode)
{
  if (register_operand(opnd, mode))
    {
      switch (REGNO (opnd))
	{
	case ESS_PIT1_REGNUM:
	case ESS_PIT0_REGNUM:
	case ESS_BRALWAYS_REGNUM:
	case ESS_SRDMARQFULL_REGNUM:
	case ESS_PDMARQFULL_REGNUM:
	case ESS_UDMA0RQFULL_REGNUM:
	case ESS_UDMA1RQFULL_REGNUM:
	case ESS_BMESWEPD_REGNUM:
	case ESS_BMEHWEPD_REGNUM:
	case ESS_BMEEXHAUST_REGNUM:
	case ESS_INT_REGNUM:
	case ESS_NMI_REGNUM:
	  return 1;

	default:
	  return 0;
	}
    }
  else
    return 0;
}

int 
lnk_offset_operand (register rtx opnd, 
		    enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (GET_CODE (opnd) == CONST_INT &&
      (INTVAL(opnd) >= 0 &&
       INTVAL(opnd) <= 62*4 &&
       INTVAL(opnd) % 4 == 0))
    {
      return 1;
    }

  return 0;
}

int 
lnk_memory_operand (register rtx op, 
		    enum machine_mode mode ATTRIBUTE_UNUSED)
{
  rtx mem_operand, op0, op1;
  rtx lnk_reg;
  rtx lnk_reg_offset;

  if (GET_CODE (op) != MEM)
    return 0;

  mem_operand = XEXP (op, 0);
  if (GET_CODE (mem_operand) == PLUS)
    {
      op0 = XEXP (mem_operand, 0);
      op1 = XEXP (mem_operand, 1);

      if (REG_P (op0) && REGNO_OK_FOR_MEMORY_REGION_P (REGNO (op0)))
	{

	  if (GET_CODE (op1) == PLUS)
	    {
	      lnk_reg = XEXP (op1, 0);
	      lnk_reg_offset = XEXP (op1, 1);

	      if (REG_P (lnk_reg) && REGNO_OK_FOR_LNK_P (REGNO (lnk_reg)))
		{
		  if (lnk_offset_operand (lnk_reg_offset, VOIDmode))
		    return 1;
		}
	    }

	  return 0;
	}
      else
	{
	  lnk_reg = op0;
	  lnk_reg_offset = op1;

	  if (REG_P (lnk_reg) && REGNO_OK_FOR_LNK_P (REGNO (lnk_reg)))
	    {
	      if (lnk_offset_operand (lnk_reg_offset, VOIDmode))
		return 1;
	    }

	  return 0;
	}
    }
  else
    return 0;
}

int 
brsl_flag_operand (register rtx opnd, 
		   enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if ((GET_CODE (opnd) == CONST_INT) &&
      (INTVAL (opnd) == FLAG_UNLNK || 
       INTVAL (opnd) == FLAG_DLNK ||
       INTVAL (opnd) == FLAG_FLUSH ||
       INTVAL (opnd) == FLAG_PRE ||
       INTVAL (opnd) == -1))
    return 1;
  else
    return 0;
}

int
nibble_operand (register rtx op,
		enum machine_mode mode ATTRIBUTE_UNUSED)
{
  return ((GET_CODE (op) == CONST_INT
	   && (((unsigned HOST_WIDE_INT) INTVAL (op) == 0)
	       || ((unsigned HOST_WIDE_INT) INTVAL (op) == 4)
	       || ((unsigned HOST_WIDE_INT) INTVAL (op) == 8)
	       || ((unsigned HOST_WIDE_INT) INTVAL (op) == 12)
	       || ((unsigned HOST_WIDE_INT) INTVAL (op) == 16)
	       || ((unsigned HOST_WIDE_INT) INTVAL (op) == 20)
	       || ((unsigned HOST_WIDE_INT) INTVAL (op) == 24)
	       || ((unsigned HOST_WIDE_INT) INTVAL (op) == 28)
	       )));
}

int
mod_mask_operand (register rtx op, enum machine_mode mode ATTRIBUTE_UNUSED)
{
  unsigned HOST_WIDE_INT val;
  unsigned HOST_WIDE_INT andval;

  if (GET_CODE (op) != CONST_INT) return (0);
  val = INTVAL(op);
  andval = octave_and_value(val);
  return (andval && andval!=32);
}

int
symbol_operand (register rtx op,
		enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case SYMBOL_REF:
    case LABEL_REF:
      return 1;
    case CONST:
      op = XEXP (op, 0);
      return ((GET_CODE (XEXP (op, 0)) == SYMBOL_REF
	       || GET_CODE (XEXP (op, 0)) == LABEL_REF)
	      && GET_CODE (XEXP (op, 1)) == CONST_INT);
    default:
      return 0;
    }
}

int
symbol_or_12bu_operand (register rtx op,
			enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (immediate_operand(op, mode))
    {
      if (GET_CODE (op) == CONST_INT)
	return CONST_OK_FOR_UI_12BITS(INTVAL(op));
      
      return 1;
    }
  else
    return 0;
}

int
symbol_or_16bu_operand (register rtx op,
			enum machine_mode mode ATTRIBUTE_UNUSED)
{
  if (immediate_operand(op, mode))
    {
      if (GET_CODE (op) == CONST_INT)
	return CONST_OK_FOR_UI_16BITS(INTVAL(op));
      
      return 1;
    }
  else
    return 0;
}


int
symbol_or_17bu_operand (register rtx op,
			enum machine_mode mode ATTRIBUTE_UNUSED)
{
  switch (GET_CODE (op))
    {
    case SYMBOL_REF:
    case LABEL_REF:
      return 1;
    case CONST:

      /* FIXME */
    default:
      return 0;
    }
}


int
ess_clobbering_plus_operand (op, mode)
     rtx op;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
  return (GET_CODE (op) == PLUS
	  && (XEXP (op, 0) == stack_pointer_rtx
	      || XEXP (op, 0) == frame_pointer_rtx)
	  && (GET_CODE (XEXP (op,1)) == CONST_INT
	      && !CONST_OK_FOR_UI_17BITS(INTVAL(XEXP (op,1)))));
}

int
call_operand (op, mode)
     rtx op;
     enum machine_mode mode;
{
  return (immediate_operand(op, mode) || REG_P(op));
}

int
octave_split_constant(code, mode, val, target, source, subtargets)
     enum rtx_code code;
     enum machine_mode mode ATTRIBUTE_UNUSED;
     HOST_WIDE_INT val;
     rtx target;
     rtx source;
     int subtargets ATTRIBUTE_UNUSED;
{
  HOST_WIDE_INT low = val & 0xffff;
  HOST_WIDE_INT high = (unsigned) val >> 16;

  if (low & 0x8000)
    {
      high++;
      low |= 0xffff0000;
    }

  switch (code)
    {
    case SET:
      emit_insn(gen_movsi_addr32(target, GEN_INT (val)));
      break;

    case PLUS:
      if (!rtx_equal_p(target, source))
	emit_move_insn(target, source);

      if (CONST_OK_FOR_UI_17BITS(val))
	{
	  emit_insn (gen_addsi3_noclobber(target, target, GEN_INT (val)));
	}
      else if (CONST_OK_FOR_UI_17BITS(val))
	{
	  emit_insn (gen_addsi3_noclobber(target, target, GEN_INT (val)));
	}
      else
	{

	  emit_insn (gen_addsi3_noclobber(target, target, GEN_INT (low)));
	  emit_insn (gen_addsi3_noclobber(target, target, GEN_INT (high << 16)));
	}
      break;

    default:
      break;
    }
  return 0;
}



void octave_rtx_debug (register rtx rx, char *cmt, char *file, int line)
{
  fflush (stdout);
  fflush (stderr);
  fprintf (stderr, "\nOCTAVE_RTX_DEBUG:%s %s:%d ", cmt, file, line);
  debug_rtx (rx);
  fflush (stdout);
  fflush (stderr);
}

int
mm_constrained_p (op)
     rtx op;
{
  if (GET_CODE(op) == MEM) {
    rtx inner = XEXP(op, 0);
    if (GET_CODE(inner) == REG) /* if it's exactly a register, we're fine */
      return(1);
    if (GET_CODE(inner) == PLUS) {
      rtx plus_op0 = XEXP(inner, 0);
      if (GET_CODE(plus_op0) == REG) {
	rtx plus_op1 = XEXP(inner, 1);
	if ((GET_CODE(plus_op1) == CONST_INT) &&
	    (INTVAL(plus_op1) < 128) &&
	    /* This is bogus, should be -127, but that seems to cause problems */
	    (INTVAL(plus_op1) >= 0))
	  return(1);
      }
    }
  }
  return(0);
}

rtx
m480_gen_compare_reg (code, x, y, force_into_reg)
     enum rtx_code code;
     rtx x, y;
     int force_into_reg;
{
  enum machine_mode mode
    = GET_MODE (x) == VOIDmode ? GET_MODE (y) : GET_MODE (x);
  enum machine_mode cc_mode = SELECT_CC_MODE (code, x, y);
  rtx cc_reg;

  if (!force_into_reg)
    {
      if (!register_operand(x, GET_MODE (x)))
	x = force_reg(GET_MODE (x), x);

      if (cc_mode == CCmode)
	{
	  if (!reg_or_6bu_operand(y, mode))
	    y = force_reg(mode, y);
	}

      if (cc_mode == CCUNSmode)
	{
	  if (!reg_or_10bs_operand(y, mode))
	    y = force_reg(mode, y);
	}
    }
  else
    {
      if (!register_operand(x, GET_MODE (x)))
	x = force_reg(mode, x);

      if (!register_operand(y, GET_MODE (y)))
	y = force_reg(mode, y);
    }

  cc_reg = gen_rtx_REG (cc_mode, ESS_REGNUM);
  emit_insn (gen_rtx_SET (VOIDmode, cc_reg,
			  gen_rtx_COMPARE (cc_mode, x, y)));
  return cc_reg;
}

void
m480_gen_compare_and_branch (code, x, y, force_into_reg, octave_label_rtx)
     enum rtx_code code;
     rtx x, y;
     int force_into_reg;
     rtx octave_label_rtx;
{
  enum machine_mode cc_mode = SELECT_CC_MODE (code, x, y);

  if (cc_mode == CCUNSmode)
    m480_gen_unsigned_compare_and_branch(code, x, y, force_into_reg, octave_label_rtx);
  else
    {
      if (TARGET_UNSAFE_SIGNED_COMPARE)
	m480_gen_signed_compare_and_branch_ignore_overflow (code, x, y, force_into_reg, octave_label_rtx);
      else
	m480_gen_signed_compare_and_branch(code, x, y, force_into_reg, octave_label_rtx);
    }
}

static void
m480_gen_unsigned_compare_and_branch (code, x, y, force_into_reg, octave_label_rtx)
     enum rtx_code code;
     rtx x, y;
     int force_into_reg;
     rtx octave_label_rtx;
{
  enum machine_mode mode
    = GET_MODE (x) == VOIDmode ? GET_MODE (y) : GET_MODE (x);
  rtx cc_reg = gen_rtx_REG (CCUNSmode, ESS_REGNUM);

  if (!force_into_reg)
    {
      if (!register_operand(x, GET_MODE (x)))
	x = force_reg(GET_MODE (x), x);

      if (!reg_or_10bs_operand(y, mode))
	y = force_reg(mode, y);
    }
  else
    {
      if (!register_operand(x, GET_MODE (x)))
	x = force_reg(mode, x);

      if (!register_operand(y, GET_MODE (y)))
	y = force_reg(mode, y);
    }

  emit_insn (gen_rtx_SET (VOIDmode, cc_reg,
			  gen_rtx_COMPARE (CCUNSmode, x, y)));
  emit_octave_cbranch(code, cc_reg, octave_label_rtx);
}

static void
m480_gen_signed_compare_and_branch_ignore_overflow (code, x, y, force_into_reg, octave_label_rtx)
     enum rtx_code code;
     rtx x, y;
     int force_into_reg;
     rtx octave_label_rtx;
{
  enum machine_mode mode
    = GET_MODE (x) == VOIDmode ? GET_MODE (y) : GET_MODE (x);
  rtx cc_reg = gen_rtx_REG (CCmode, ESS_REGNUM);

  if (!force_into_reg)
    {
      if (!register_operand(x, GET_MODE (x)))
	x = force_reg(GET_MODE (x), x);

      if (!reg_or_10bs_operand(y, mode))
	y = force_reg(mode, y);
    }
  else
    {
      if (!register_operand(x, GET_MODE (x)))
	x = force_reg(mode, x);

      if (!register_operand(y, GET_MODE (y)))
	y = force_reg(mode, y);
    }

  emit_insn (gen_rtx_SET (VOIDmode, cc_reg,
			  gen_rtx_COMPARE (CCmode, x, y)));
  emit_octave_cbranch(code, cc_reg, octave_label_rtx);
}

static void
m480_gen_signed_compare_and_branch (code, x, y, force_into_reg, octave_label_rtx)
     enum rtx_code code;
     rtx x, y;
     int force_into_reg ATTRIBUTE_UNUSED;
     rtx octave_label_rtx;
{
  rtx temp_reg1 = gen_reg_rtx(SImode);
  rtx temp_reg2 = gen_reg_rtx(SImode);
  rtx temp_reg3 = gen_reg_rtx(SImode);
  rtx temp_reg4 = gen_reg_rtx(SImode);
  rtx cc_reg = gen_rtx_REG (CCmode, ESS_REGNUM);
  rtx sign_result;

  emit_move_insn (temp_reg1, GEN_INT(0x80000000));

  sign_result = compute_sign(code, x, temp_reg1);

  emit_insn (gen_movsicc (temp_reg2, 
			  gen_rtx_fmt_ee (code, VOIDmode, temp_reg1, const0_rtx),
			  temp_reg1, const0_rtx));

  emit_insn (gen_xor(temp_reg3, x, y));

  emit_insn (gen_and (gen_rtx_REG (SImode, REG_63), temp_reg3, temp_reg1));

  emit_insn (gen_cmov_ess_cc_mode(temp_reg4, 
				  gen_rtx_fmt_ee (EQ, VOIDmode, temp_reg2, sign_result),
				  temp_reg2, sign_result));
  emit_insn (gen_rtx_SET (VOIDmode, cc_reg,
			  gen_rtx_COMPARE (CCmode, temp_reg4, const0_rtx)));
  emit_octave_cbranch(NE, cc_reg, octave_label_rtx);
}

static void
emit_octave_cbranch (code, ess_reg, octave_jump_label)
     enum rtx_code code;
     rtx ess_reg;
     rtx octave_jump_label;
{
  emit_jump_insn (gen_rtx_SET (VOIDmode, pc_rtx,
			       gen_rtx_IF_THEN_ELSE (VOIDmode,
						     gen_rtx_fmt_ee (code,
								     VOIDmode,
								     ess_reg,
								     const0_rtx),
						     gen_rtx_LABEL_REF (VOIDmode, octave_jump_label),
						     pc_rtx)));
}

static rtx
compute_sign(code, x, immed_val)
     enum rtx_code code;
     rtx x, immed_val;
{
  rtx result;

  switch (code)
    {
    case NE:
      return (immed_val);
      break;

    case EQ:
      return (const0_rtx);

    case GT:
    case GE:
      result = gen_reg_rtx(SImode);
      emit_insn (gen_and(result, x, immed_val));
      emit_insn (gen_xor(result, result, immed_val));
      return result;

    case LT:
    case LE:
      result = gen_reg_rtx(SImode);
      emit_insn (gen_and(result, x, immed_val));
      return result;

    case GTU:
    case LEU:
    case GEU:
    case LTU:
      fprintf(stderr, "Unexpected unsigned condition code found in sign comparison code");
      abort();
      return 0;
      break;

    default:
      return 0;/* do nothing */
    }

}

void
trace(s)
     const char *s;
{
  fprintf (stderr, s);
}

void
trace2(s1, s2)
     const char *s1, *s2;
{
  fprintf (stderr, s1, s2);
}


int
memory_cycle_cost()
{
  if (TARGET_480)
    {
      if (!TARGET_DATA_IN_SDRAM)
	return SRAM_MEMORY_CYCLE_COST;
      else
	return SDRAM_MEMORY_CYCLE_COST;
    }
  else
    return SRAM_MEMORY_CYCLE_COST;
}

rtx
m480_gen_load_multiple (base_regno, count, from, up, write_back, unchanging_p,
			in_struct_p, scalar_p)
     int base_regno;
     int count;
     rtx from;
     int up;
     int write_back ATTRIBUTE_UNUSED;
     int unchanging_p;
     int in_struct_p;
     int scalar_p;
{
  int i = 0, j;
  rtx result;
  int sign = up ? 1 : -1;
  rtx mem;

  result = gen_rtx_PARALLEL (VOIDmode,
			     rtvec_alloc (count));

  for (j = 0; i < count; i++, j++)
    {
      mem = gen_rtx_MEM (SImode, plus_constant (from, j * 4 * sign));
      RTX_UNCHANGING_P (mem) = unchanging_p;
      MEM_IN_STRUCT_P (mem) = in_struct_p;
      MEM_SCALAR_P (mem) = scalar_p;
      XVECEXP (result, 0, i)
	= gen_rtx_SET (VOIDmode, gen_rtx_REG (SImode, base_regno + j), mem);
    }

  return result;
}

rtx
m480_gen_store_multiple (base_regno, count, to, up, write_back, unchanging_p,
			 in_struct_p, scalar_p)
     int base_regno;
     int count;
     rtx to;
     int up;
     int write_back ATTRIBUTE_UNUSED;
     int unchanging_p;
     int in_struct_p;
     int scalar_p;
{
  int i = 0, j;
  rtx result;
  int sign = up ? 1 : -1;
  rtx mem;

  result = gen_rtx_PARALLEL (VOIDmode,
			     rtvec_alloc (count));

  for (j = 0; i < count; i++, j++)
    {
      mem = gen_rtx_MEM (SImode, plus_constant (to, j * 4 * sign));
      RTX_UNCHANGING_P (mem) = unchanging_p;
      MEM_IN_STRUCT_P (mem) = in_struct_p;
      MEM_SCALAR_P (mem) = scalar_p;

      XVECEXP (result, 0, i)
	= gen_rtx_SET (VOIDmode, mem, gen_rtx_REG (SImode, base_regno + j));
    }

  return result;
}

int
odd_register_p(opnd)
     rtx opnd;
{
  if (REG_P (opnd))
    if ((REGNO(opnd) % 2) == 0)
      return 0;
    else
      return 1;
  else
    return 0;
}

int
memory_region_ccram(opnd)
     rtx opnd;
{
  int retVal = 0;
  rtx mem_operand;

  if (GET_CODE (opnd) != MEM)
    return 0;

  mem_operand = XEXP (opnd, 0);

  if (GET_CODE (mem_operand) == PLUS)
    {
      rtx op0 = XEXP (mem_operand, 0);
      rtx op1 = XEXP (mem_operand, 1);

      if (GET_CODE (op0) == REG &&
	  (REGNO (op0) == REG_BASE_CCRAM ||
	   REGNO (op0) == REG_BASE3))
	return 1;

      if (GET_CODE (op1) == REG &&
	  (REGNO (op1) == REG_BASE_CCRAM ||
	   REGNO (op1) == REG_BASE3))
	return 1;
    }

  return retVal;
}

int
memory_region_ccram_lock(opnd)
     rtx opnd;
{
  int retVal = 0;
  rtx mem_operand;

  if (GET_CODE (opnd) != MEM)
    return 0;

  mem_operand = XEXP (opnd, 0);

  if (GET_CODE (mem_operand) == PLUS)
    {
      rtx op0 = XEXP (mem_operand, 0);
      rtx op1 = XEXP (mem_operand, 1);

      if (GET_CODE (op0) == REG &&
	  (REGNO (op0) == REG_BASE_CCRAM_LOCK ||
	   REGNO (op0) == REG_BASE3))
	return 1;

      if (GET_CODE (op1) == REG &&
	  (REGNO (op1) == REG_BASE_CCRAM_LOCK ||
	   REGNO (op1) == REG_BASE3))
	return 1;
    }

  return retVal;
}

int
memory_region_spram(opnd)
     rtx opnd;
{
  int retVal = 0;
  rtx mem_operand;

  if (GET_CODE (opnd) != MEM)
    return 0;

  mem_operand = XEXP (opnd, 0);

  if (GET_CODE (mem_operand) == PLUS)
    {
      rtx op0 = XEXP (mem_operand, 0);
      rtx op1 = XEXP (mem_operand, 1);

      if (GET_CODE (op0) == REG &&
	  (REGNO (op0) == REG_BASE_SPRAM ||
	   REGNO (op0) == REG_BASE2))
	return 1;

      if (GET_CODE (op1) == REG &&
	  (REGNO (op1) == REG_BASE_SPRAM ||
	   REGNO (op1) == REG_BASE2))
	return 1;
    }

  return retVal;
}

int
memory_region_spram_lock(opnd)
     rtx opnd;
{
  int retVal = 0;
  rtx mem_operand;

  if (GET_CODE (opnd) != MEM)
    return 0;

  mem_operand = XEXP (opnd, 0);

  if (GET_CODE (mem_operand) == PLUS)
    {
      rtx op0 = XEXP (mem_operand, 0);
      rtx op1 = XEXP (mem_operand, 1);

      if (GET_CODE (op0) == REG &&
	  (REGNO (op0) == REG_BASE_SPRAM_LOCK ||
	   REGNO (op0) == REG_BASE2))
	return 1;

      if (GET_CODE (op1) == REG &&
	  (REGNO (op1) == REG_BASE_SPRAM_LOCK ||
	   REGNO (op1) == REG_BASE2))
	return 1;
    }

  return retVal;
}

int
memory_region_sdram(opnd)
     rtx opnd;
{
  int retVal = 0;
  rtx mem_operand;

  if (GET_CODE (opnd) != MEM)
    return 0;

  mem_operand = XEXP (opnd, 0);

  if (GET_CODE (mem_operand) == PLUS)
    {
      rtx op0 = XEXP (mem_operand, 0);
      rtx op1 = XEXP (mem_operand, 1);

      if (GET_CODE (op0) == REG &&
	  REGNO (op0) == REG_BASE_SDRAM)
	return 1;

      if (GET_CODE (op1) == REG &&
	  REGNO (op1) == REG_BASE_SDRAM)
	return 1;
    }

  return retVal;
}

int
memory_region_sram(opnd)
     rtx opnd;
{
  int retVal = 0;
  rtx mem_operand;

  if (GET_CODE (opnd) != MEM)
    return 0;

  mem_operand = XEXP (opnd, 0);

  if (GET_CODE (mem_operand) == PLUS)
    {
      rtx op0 = XEXP (mem_operand, 0);
      rtx op1 = XEXP (mem_operand, 1);

      if (GET_CODE (op0) == REG &&
	  REGNO (op0) == REG_BASE_SRAM)
	return 1;

      if (GET_CODE (op1) == REG &&
	  REGNO (op1) == REG_BASE_SRAM)
	return 1;
    }

  return retVal;
}

int
memory_region_sio(opnd)
     rtx opnd;
{
  int retVal = 0;
  rtx mem_operand;

  if (GET_CODE (opnd) != MEM)
    return 0;

  mem_operand = XEXP (opnd, 0);

  if (GET_CODE (mem_operand) == PLUS)
    {
      rtx op0 = XEXP (mem_operand, 0);
      rtx op1 = XEXP (mem_operand, 1);

      if (GET_CODE (op0) == REG &&
	  REGNO (op0) == REG_BASE_SIO)
	return 1;

      if (GET_CODE (op1) == REG &&
	  REGNO (op1) == REG_BASE_SIO)
	return 1;
    }

  return retVal;
}

int
memory_region_p(opnd)
     rtx opnd;
{
  if (memory_region_ccram(opnd))
    return MEMORY_REGION_CCRAM;

  if (memory_region_ccram_lock(opnd))
    return MEMORY_REGION_CCRAM_LOCK;

  if (memory_region_spram(opnd))
    return MEMORY_REGION_SPRAM;

  if (memory_region_spram_lock(opnd))
    return MEMORY_REGION_SPRAM_LOCK;

  if (memory_region_sdram(opnd))
    return MEMORY_REGION_SDRAM;

  if (memory_region_sram(opnd))
    return MEMORY_REGION_SRAM;

  if (memory_region_sio(opnd))
    return MEMORY_REGION_SIO;

  return MEMORY_REGION_NONE;
}

rtx
m480_function_value (valtype, func)
     tree valtype;
     tree func ATTRIBUTE_UNUSED;
{
  int reg = R0_REGNUM;
  enum machine_mode mode = TYPE_MODE (valtype);
  int unsignedp = TREE_UNSIGNED (valtype);

  mode = promote_mode (valtype, mode, &unsignedp, 1);

  return gen_rtx (REG, mode, reg);
}


int
m480_gen_movstrqi (operands)
     rtx *operands;
{
  HOST_WIDE_INT in_words_to_go, out_words_to_go, last_bytes;
  rtx bytes_rtx	= operands[2];
  rtx align_rtx = operands[3];
  rtx src, dst;
  rtx scratch_reg;
  HOST_WIDE_INT orig_bytes = INTVAL (bytes_rtx);
  HOST_WIDE_INT bytes = orig_bytes;
  HOST_WIDE_INT src_align = INTVAL (align_rtx);
  HOST_WIDE_INT dst_align = src_align;
  rtx orig_src	= operands[1];
  rtx orig_dst	= operands[0];
  rtx tmp;
  rtx mem;
  int i, words, ofs=0;
  rtx st_src, st_dst, fin_src, fin_dst;
  int dst_unchanging_p, dst_in_struct_p, src_unchanging_p, src_in_struct_p;
  int dst_scalar_p, src_scalar_p;

  if (bytes <= 4 || bytes > MAX_BLOCK_MOVE_BYTES)
    return 0;

  if (INTVAL (operands[3]) & 3)
    return 0;

  st_dst = XEXP (operands[0], 0);
  st_src = XEXP (operands[1], 0);

  dst_unchanging_p = RTX_UNCHANGING_P (operands[0]);
  dst_in_struct_p = MEM_IN_STRUCT_P (operands[0]);
  dst_scalar_p = MEM_SCALAR_P (operands[0]);
  src_unchanging_p = RTX_UNCHANGING_P (operands[1]);
  src_in_struct_p = MEM_IN_STRUCT_P (operands[1]);
  src_scalar_p = MEM_SCALAR_P (operands[1]);

  fin_dst = dst = copy_to_mode_reg (SImode, st_dst);
  fin_src = src = copy_to_mode_reg (SImode, st_src);

  in_words_to_go =  INTVAL (operands[2]) / 4;
  out_words_to_go = INTVAL (operands[2]) / 4;
  last_bytes = INTVAL (operands[2]) & 3;

  tmp = XEXP (orig_src, 0);
  if (GET_CODE (tmp) == REG)
    {
      if (REGNO_POINTER_ALIGN (REGNO (tmp)) > src_align)
	src_align = REGNO_POINTER_ALIGN (REGNO (tmp));
    }
  else if (GET_CODE (tmp) == PLUS
	   && GET_CODE (XEXP (tmp, 0)) == REG
	   && GET_CODE (XEXP (tmp, 1)) == CONST_INT)
    {
      HOST_WIDE_INT c = INTVAL (XEXP (tmp, 1));
      int a = REGNO_POINTER_ALIGN (REGNO (XEXP (tmp, 0)));

      if (a > src_align)
	{
          if (a >= 4 && c % 4 == 0)
	    src_align = 4;
          else if (a >= 2 && c % 2 == 0)
	    src_align = 2;
	}
    }
	
  tmp = XEXP (orig_dst, 0);
  if (GET_CODE (tmp) == REG)
    {
      if (REGNO_POINTER_ALIGN (REGNO (tmp)) > dst_align)
	dst_align = REGNO_POINTER_ALIGN (REGNO (tmp));
    }
  else if (GET_CODE (tmp) == PLUS
	   && GET_CODE (XEXP (tmp, 0)) == REG
	   && GET_CODE (XEXP (tmp, 1)) == CONST_INT)
    {
      HOST_WIDE_INT c = INTVAL (XEXP (tmp, 1));
      int a = REGNO_POINTER_ALIGN (REGNO (XEXP (tmp, 0)));

      if (a > dst_align)
	{
          if (a >= 4 && c % 4 == 0)
	    dst_align = 4;
          else if (a >= 2 && c % 2 == 0)
	    dst_align = 2;
	}
    }

  if (src_align & 3)
    return 0;

  if (dst_align & 3)
    return 0;

  if (src_align == 4 && dst_align == 4)
    {
      words = bytes / 4;

      for (i = 0; in_words_to_go > 0; i += 8)
	{
	  if (in_words_to_go > 8)
	    {
	      emit_insn (m480_gen_load_multiple (A0_REGNUM, 8, src, TRUE, TRUE,
						 src_unchanging_p,
						 src_in_struct_p,
						 src_scalar_p));
	      emit_insn (gen_rtx_SET (GET_MODE (src), src,
				      plus_constant (src, 8 * UNITS_PER_WORD)));
	    }

	  else if (in_words_to_go != 1)
	    emit_insn (m480_gen_load_multiple (A0_REGNUM, in_words_to_go, src, TRUE, 
					       FALSE, src_unchanging_p,
					       src_in_struct_p, src_scalar_p));
	  else
	    {

	      mem = gen_rtx_MEM(SImode, src);
	      RTX_UNCHANGING_P (mem) = src_unchanging_p;
	      MEM_IN_STRUCT_P (mem) = src_in_struct_p;
	      MEM_SCALAR_P (mem) = src_scalar_p;
	      emit_move_insn (gen_rtx_REG (SImode, A0_REGNUM), mem);
	    }

	  if (out_words_to_go > 8)
	    {
	      emit_insn (m480_gen_store_multiple (A0_REGNUM, 8, dst, TRUE, TRUE,
						  dst_unchanging_p,
						  dst_in_struct_p,
						  dst_scalar_p));
	      emit_insn (gen_rtx_SET (GET_MODE (dst), dst,
				      plus_constant (dst, 8 * UNITS_PER_WORD)));
	    }
	  else if (out_words_to_go != 1)
	    emit_insn (m480_gen_store_multiple (A0_REGNUM, out_words_to_go,
						dst, TRUE, 
						(last_bytes == 0
						 ? FALSE : TRUE),
						dst_unchanging_p,
						dst_in_struct_p,
						dst_scalar_p));
	  else
	    {
	      mem = gen_rtx_MEM (SImode, dst);
	      RTX_UNCHANGING_P (mem) = dst_unchanging_p;
	      MEM_IN_STRUCT_P (mem) = dst_in_struct_p;
	      MEM_SCALAR_P (mem) = dst_scalar_p;
	      emit_move_insn (mem, gen_rtx_REG (SImode, A0_REGNUM));
	    }

	  in_words_to_go -= in_words_to_go < 8 ? in_words_to_go : 8;
	  out_words_to_go -= out_words_to_go < 8 ? out_words_to_go : 8;
	}

      bytes -= words * 4;
      ofs += words * 4;
    }

  while (bytes > 0)
    {
      scratch_reg = gen_reg_rtx (QImode);

      emit_move_insn (scratch_reg,
		      change_address (orig_src, QImode,
				      plus_constant (XEXP (orig_src, 0),
						     ofs)));

      emit_move_insn (change_address (orig_dst, QImode,
				      plus_constant (XEXP (orig_dst, 0),
						     ofs)),
		      scratch_reg);
      bytes -= 1;
      ofs += 1;
    }

  return 1;
}

int
m480_gen_clrstrqi (operands)
     rtx *operands;
{
  HOST_WIDE_INT out_words_to_go, last_bytes;
  rtx bytes_rtx	= operands[1];
  rtx align_rtx = operands[2];
  rtx dst;
  HOST_WIDE_INT orig_bytes = INTVAL (bytes_rtx);
  HOST_WIDE_INT bytes = orig_bytes;

  rtx orig_dst	= operands[0];
  int i, words, ofs=0, nregs = 0;
  rtx st_dst, fin_dst;
  int dst_unchanging_p, dst_in_struct_p;
  int dst_scalar_p;

  if (bytes <= 4 || bytes > MAX_BLOCK_MOVE_BYTES)
    return 0;

  if (INTVAL (align_rtx) & 3)
    return 0;

  st_dst = XEXP (operands[0], 0);

  dst_unchanging_p = RTX_UNCHANGING_P (operands[0]);
  dst_in_struct_p = MEM_IN_STRUCT_P (operands[0]);
  dst_scalar_p = MEM_SCALAR_P (operands[0]);

  fin_dst = dst = copy_to_mode_reg (SImode, st_dst);

  out_words_to_go = INTVAL (bytes_rtx) / 4;
  last_bytes = INTVAL (bytes_rtx) & 3;

  if (out_words_to_go > ARG_NUM_REGS)
    nregs = ARG_NUM_REGS;
  else
    nregs = out_words_to_go;

  for (i = 0; i < nregs; i++)
    {
      emit_move_insn (gen_rtx (REG, SImode, A0_REGNUM + i),
		      const0_rtx);
    }

  words = bytes / 4;

  for (i = 0; out_words_to_go > 0; i += 8)
    {
      if (out_words_to_go > 8)
	{
	  emit_insn (m480_gen_store_multiple (A0_REGNUM, 8, dst, TRUE, TRUE,
					      dst_unchanging_p,
					      dst_in_struct_p,
					      dst_scalar_p));
	  emit_insn (gen_rtx_SET (GET_MODE (dst), dst,
				  plus_constant (dst, 8 * UNITS_PER_WORD)));
	}
      else
	emit_insn (m480_gen_store_multiple (A0_REGNUM, out_words_to_go,
					    dst, TRUE, 
					    (last_bytes == 0
					     ? FALSE : TRUE),
					    dst_unchanging_p,
					    dst_in_struct_p,
					    dst_scalar_p));

      out_words_to_go -= out_words_to_go < 8 ? out_words_to_go : 8;
    }

  bytes -= words * 4;
  ofs += words * 4;

  while (bytes > 0)
    {
      emit_move_insn (change_address (orig_dst, QImode,
				      plus_constant (XEXP (orig_dst, 0),
						     ofs)),
		      const0_rtx);
      bytes -= 1;
      ofs += 1;
    }
  return 1;
}


struct rtx_def *
m480_function_arg (cum, mode, type, named)
     CUMULATIVE_ARGS cum;	/* current arg information */
     enum machine_mode mode;	/* current arg mode */
     tree type;			/* type of the argument or 0 if lib support */
     int named;			/* != 0 for normal args, == 0 for ... args */
{
  rtx ret;

  if ((type == void_type_node) || cum >= ARG_NUM_REGS 
      || !named  
      || MUST_PASS_IN_STACK(mode, type))
    {
      ret = 0;
    }
  else
    {
      ret = gen_rtx (REG, mode, A0_REGNUM + cum);
    }

  return ret;
}

int
octave_emit_add_sequence(operands, mode)
     rtx *operands;
     enum machine_mode mode;
{
  if (!TARGET_480)
    return 0;

  if (mode != SImode)
    return 0;

  if (!reload_in_progress)
    {
      if (!REG_P (operands[0]))
	operands[0] = force_reg(SImode, operands[0]);

      if (!REG_P (operands[1]))
	operands[1] = force_reg(SImode, operands[1]);
    }

  return m480_emit_add_sequence(operands);

}

static
int m480_emit_add_sequence(operands)
     rtx *operands;

{
  if (reload_in_progress) 
    {
      if (GET_CODE (operands[2]) == CONST_INT)
	{
	  octave_split_constant (PLUS, SImode, INTVAL (operands[2]), operands[0],
				 operands[1], 0);
	  return 1;
	}
      else
	{
	  emit_insn (gen_add(operands[0],
			     operands[1],
			     operands[2]));
	  return 1;
	}
    }
  else
    {
      if (GET_CODE (operands[2]) == CONST_INT)
	{
	  if (reg_or_6bu_operand(operands[2], VOIDmode))
	    {
	      emit_insn (gen_add(operands[0],
				 operands[1],
				 operands[2]));
	      return 1;
	    }
	  else if ((INTVAL (operands[2]) < 0) && CONST_OK_FOR_UI_6BITS( -INTVAL (operands[2]) ) )
	    {
	      operands[2] = GEN_INT( -INTVAL (operands[2] ) );
	      emit_insn (gen_sub(operands[0],
				 operands[1],
				 operands[2]));
	      return 1;
	    }
	  else
	    {
	      octave_split_constant (PLUS, SImode, INTVAL (operands[2]), operands[0],
				     operands[1],
				     (reload_in_progress || reload_completed ? 0
				      : preserve_subexpressions_p ()));
	      return 1;
	    }
	}
      else
	{
	  emit_insn (gen_add(operands[0],
			     operands[1],
			     operands[2]));
	  return 1;
	}
    }
}

void
m480_override_options ()
{
}

int
const_ok_for_load_p (rtx opnd,
		     enum machine_mode mode)
{
  return (const_int_operand (opnd, mode) &&
	  CONST_OK_FOR_UI_17BITS(INTVAL(opnd)));
}

int
modx_operand (register rtx opnd,
	      enum machine_mode mode)
{
  if (const_int_operand (opnd, mode) &&
      (INTVAL(opnd) >= LOW_MODULO_WIDTH ||
       INTVAL(opnd) <= HIGH_MODULO_WIDTH))
    return 1;
  else
    return 0;
}


static const char *mod_names[] = { 
  "bogus",   /* 0 */
  "MOD2",    /* 1 */   
  "MOD4",    /* 2 */ 
  "MOD8",    /* 3 */
  "MOD16",   /* 4 */
  "MOD32",   /* 5 */
  "MOD64",   /* 6 */
  "MOD128",  /* 7 */
  "MOD256",  /* 8 */
  "MOD512",  /* 9 */
  "MOD1K",   /* 10 */
  "MOD2K",   /* 11 */
  "MOD4K",   /* 12 */
  "MOD8K",   /* 13 */
  "MOD16K",  /* 14 */
  "MOD32K",  /* 15 */
  "MOD64K",  /* 16 */
  "MOD128K", /* 17 */
  "MOD256K", /* 18 */
  "MOD512K", /* 19 */
  "MOD1M",   /* 20 */
  "MOD2M",   /* 21 */
  "MOD4M",   /* 22 */
  "MOD8M",   /* 23 */
  "MOD16M",  /* 24 */
  "MOD32M",  /* 25 */
  "MOD64M",  /* 26 */
  "MOD128M", /* 27 */
  "MOD256M", /* 28 */
  "MOD512M", /* 29 */
  "MOD1G",   /* 30 */
  "MOD2G",   /* 31 */
  "",        /* 32 */
};


unsigned int 
octave_and_value (unsigned long val)
{
  unsigned int bit;
  bool valid_modx = false;

  if (val == 0) 
    return(0); /*bogus*/

  val++; /* So should now be power of 2 */

  for(bit = 1; bit < 32; bit++)
    {
      if(val == (0x1 << bit))
	{
	  valid_modx = true;
	  break;
	}
    }

  if(valid_modx)
    return bit;
  else
    return 0; /*bogus*/
}

void
print_modx_value(FILE *file, rtx x)
{
  /* Write the modulus name for a constant.  */
  unsigned HOST_WIDE_INT val;
  
  if (!modx_operand (x,SImode))
    output_operand_lossage ("invalid %%X value");
  
  val = octave_and_value(INTVAL(x));
  
  if (val>0 && val<=32) 
    {
      fprintf (file, "%s", mod_names[val]);
    } 
  else 
    output_operand_lossage ("can't resolve %%X value");

}

const char*
output_brsl(rtx *brslFlags,
	    rtx insn ATTRIBUTE_UNUSED)
{
  char *instrBuff = xcalloc(1, 80);

  strcpy(instrBuff, "brsl\t");

  if (INTVAL(brslFlags[0]) == FLAG_UNLNK)
    strcat(instrBuff, " UNLNK");
  if (INTVAL(brslFlags[1]) == FLAG_DLNK)
    strcat(instrBuff, " DLNK");
  if (INTVAL(brslFlags[2]) == FLAG_FLUSH)
    strcat(instrBuff, " FLUSH");
  if (INTVAL(brslFlags[3]) == FLAG_PRE)
    strcat(instrBuff, " PRE");

  return instrBuff;
}
int
any_operand (op, mode)
     rtx op ATTRIBUTE_UNUSED;
     enum machine_mode mode ATTRIBUTE_UNUSED;
{
  return 1;
}

void
m480_dbr_output_seqend (file)
     FILE *file;
{
  if (final_sequence != NULL_RTX)
    {
      rtx insn = XVECEXP (final_sequence, 0, 0);
      enum attr_type insn_attr_type;
      int count;

      if (recog_memoized (insn) < 0)
	fatal_insn ("unrecognized insn: ", insn);

      insn_attr_type = get_attr_type (insn);
      count = dbr_sequence_length();

      if (insn_attr_type == TYPE_BNZR || 
	  insn_attr_type == TYPE_BNZI ||
	  insn_attr_type == TYPE_BZR ||
	  insn_attr_type == TYPE_BZI)
	{
	  while (count < 2)
	    {
	      fputs("\tnop\n", file);
	      count++;
	    }
	}

      if (insn_attr_type == TYPE_TCLRB 
	  || insn_attr_type == TYPE_TSETB
	  || insn_attr_type == TYPE_CMP_BR
	  || insn_attr_type == TYPE_CMPI_BR)
	{
	  while (count < 3)
	    {
	      fputs("\tnop\n", file);
	      count++;
	    }
	}
    }
}

int 
m480_hard_regno_mode_ok (regno, mode)
     unsigned int regno;
     enum machine_mode mode;
{
  switch ((int) mode)
    {
    case VOIDmode:
    case QImode:
    case HImode:
    case SImode:
    case SFmode:
      return 1;
      

    case CCmode:
    case CC_CYmode:
    case CC_NOOVmode:
      return (regno == REG_ESS);
      
    case DImode:
      if (IS_GP_REG(regno) && ((regno & 1) == 0))
	return 1;
      else
	return 0;
      
    default:
      return 0;
    }
}

void
m480_asm_output_align(file, log)
     FILE* file ATTRIBUTE_UNUSED;
     unsigned int log ATTRIBUTE_UNUSED;
{
/*    if (log != 0)		       */
/*      fprintf (file, "\t.align %d\n", (1<<(log))); */
}


int
m480_legitimate_address_p (mode, addr, strict)
     enum machine_mode mode;
     rtx addr;
     int strict;
{
  if (strict)
    return (m480_legitimate_address_strict_p (addr, mode));
  else
    return (m480_legitimate_address_nonstrict_p (addr, mode));
}


int
m480_legitimate_address_strict_p (addr, mode)
     rtx addr;
     enum machine_mode mode;
{
  if (REG_P (addr) && REGNO_OK_FOR_BASE_P (REGNO (addr)))
    return 1;
      
  if (GET_CODE (addr) == PLUS)
    {
      rtx op0 = XEXP (addr, 0);
      rtx op1 = XEXP (addr, 1);

      if (REG_P (op0) && REG_P (op1))
	{
	  if (REGNO_OK_FOR_MEMORY_REGION_P(REGNO(op0)))
	    return m480_legitimate_address_strict_p (op1, mode);
	}	      

      if (REG_P (op0) && GET_CODE (op1) != PLUS)
	return m480_legitimate_base_plus_offset_p (addr, 1);

      if (REG_P (op1) && GET_CODE (op0) != PLUS)
	return m480_legitimate_base_plus_offset_p (addr, 1);

      if (GET_CODE (op1) == PLUS && REG_P (op0))
	{
	  if (REGNO_OK_FOR_MEMORY_REGION_P (REGNO (op0)))
	    return m480_legitimate_base_plus_offset_p (op1, 1);
	}

      return 0;
    }
  else
    return 0;
}

int
m480_legitimate_address_nonstrict_p (addr, mode)
     rtx addr;
     enum machine_mode mode;
{
  if (REG_P (addr) && REGNO_OK_GP_OR_PSEUDO_NONSTRICT_P (REGNO (addr)))
    return 1;
      
  if (GET_CODE (addr) == PLUS)
    {
      rtx op0 = XEXP (addr, 0);
      rtx op1 = XEXP (addr, 1);

      if (REG_P (op0) && REG_P (op1))
	{
	  if (REGNO_OK_FOR_MEMORY_REGION_P(REGNO(op0)))
	    return m480_legitimate_address_nonstrict_p (op1, mode);
	}	      

      if (REG_P (op0) && GET_CODE (op1) != PLUS)
	return m480_legitimate_base_plus_offset_p (addr, 0);

      if (REG_P (op1) && GET_CODE (op0) != PLUS)
	return m480_legitimate_base_plus_offset_p (addr, 0);

      if (GET_CODE (op1) == PLUS && REG_P (op0))
	{
	  if (REGNO_OK_FOR_MEMORY_REGION_P (REGNO (op0)))
	    return m480_legitimate_base_plus_offset_p (op1, 0);
	}

      return 0;
    }
  else
    return 0;
}

int m480_legitimate_base_plus_offset_p (addr, strict)
     rtx addr;
     int strict;
{
  rtx op0 = XEXP (addr, 0);
  rtx op1 = XEXP (addr, 1);

  if (REG_P (op0) && REGNO_OK_FOR_BASE_MEMORY_REGION_P(REGNO (op0)))
    {
      if (immediate_operand (op1, Pmode))
	return 1;
      else
	return 0;
    }
  else if (REG_P (op0) && REGNO_OK_FOR_LNK_P(REGNO (op0)))
    {
      if (lnk_offset_operand (op1, VOIDmode))
	return 1;
      else
	return 0;
    }
  else if (REG_P (op0) && !strict)
    {
      if (REGNO_OK_GP_OR_PSEUDO_NONSTRICT_P (REGNO (op0))
	  && RTX_OK_FOR_OFFSET_P (op1))
	{
	  return 1;
	}
      else
	return 0;
    }
  else if (REG_P (op0) && strict)
    {
      if (REGNO_OK_FOR_BASE_P (REGNO (op0))
	  && RTX_OK_FOR_OFFSET_P (op1))
	{
	  return 1;
	}
      else
	return 0;
    }
  else
    return 0;
}



bool
m480_cannot_modify_jumps_past_reload_p ()
{
/*   return (reload_completed || reload_in_progress); */
  return 0;
}

void m480_output_label(FILE *f, const char* name)
{
  fprintf(f, "%s:=\n", name); 
}


void m480_asm_declare_function_name(FILE *f ATTRIBUTE_UNUSED, char* name, tree decl ATTRIBUTE_UNUSED)
{							
  octave_function_name = name;				
} 

void  m480_asm_declare_object_name(FILE *f, const char* name, tree decl)
{							
  assemble_name (f, name);                          
  if (TREE_PUBLIC(decl))                            
    fputs ("::\n", f);                              
  else                                               
    fputs (":=\n", f);                              
}


int
m480_tablejump_p (insn)
     rtx insn;
{
  rtx pattern;
  rtx i1, i2;

  if (GET_CODE (insn) != JUMP_INSN)
    return 0;

  pattern = PATTERN (insn);
  if (GET_CODE (pattern) != PARALLEL)
    return 0;

  if (XVECLEN (pattern, 0) != 2)
    return 0;

  i1 = XVECEXP (pattern, 0, 0);
  if (GET_CODE (i1) == SET
      && SET_DEST(i1) == pc_rtx
      && register_operand (SET_SRC (i1), Pmode))
    {
      i2 = XVECEXP (pattern, 0, 1);

      if (GET_CODE (i2) != USE)
	return 0;

      if (immediate_operand (XEXP (i2, 0), VOIDmode))
	return 1;
      else
	return 0;
    }
  else
    return 0;
}

bool is_label_ref_expr (expr)
     rtx expr;
{
  return (GET_CODE (expr) == LABEL_REF && LABEL_NAME(XEXP(expr,0)));
}

void print_label_ref_expr (file, label_ref)
     FILE *file;
     rtx label_ref;
{
  fprintf (file, "%s", LABEL_NAME(XEXP(label_ref,0)));
}

/* Given a register range rdM - rdN (where N > M) and a register rd in the range,
   return it's register pair offset in the range. */
static int
register_pair_offset (first_reg, reg)
     rtx first_reg;
     rtx reg;
{
  int offset;

  if (!REG_P (first_reg))
    return 0;

  if (!REG_P (reg))
    return 0;

  if (REGNO (reg) < REGNO (first_reg))
    return 0;

  offset = (REGNO (reg) - REGNO (first_reg))/2;
  return offset;
}

/* Return the ccram latency to a register (lmm_dest) assuming a register range 
   bounded by 'first_lmm_dest' + 'words_to_load' */

static int
ccram_latency (first_lmm_dest, lmm_dest, words_to_load)
     rtx first_lmm_dest;
     rtx lmm_dest;
     int words_to_load;
{
  if (words_to_load <= 2)
    {
      if (!odd_register_p(first_lmm_dest) || words_to_load == 1)
	return 6;
      else
	return 7;       /* Odd register and two word load, add one clock */
    }

  if (!odd_register_p(first_lmm_dest))
    return 6 + register_pair_offset (first_lmm_dest, lmm_dest);
  else
    return 7 + register_pair_offset (first_lmm_dest, lmm_dest);
}

/* Return the ccram lock latency to a register (lmm_dest) assuming a register range 
   bounded by 'first_lmm_dest' + 'words_to_load' */

static int
ccram_lock_latency (first_lmm_dest, lmm_dest, words_to_load)
     rtx first_lmm_dest;
     rtx lmm_dest;
     int words_to_load;
{
  return ccram_latency (first_lmm_dest, lmm_dest, words_to_load);
}

/* Return the spram latency to a register (lmm_dest) assuming a register range 
   bounded by 'first_lmm_dest' + 'words_to_load' */

static int
spram_latency (first_lmm_dest, lmm_dest, words_to_load)
     rtx first_lmm_dest;
     rtx lmm_dest;
     int words_to_load;
{
  if (words_to_load <= 2)
    return 2;

  if (!odd_register_p(first_lmm_dest))
    return 2 + register_pair_offset (first_lmm_dest, lmm_dest);
  else
    return 3 + register_pair_offset (first_lmm_dest, lmm_dest);
}

/* Return the spram lock latency to a register (lmm_dest) assuming a register range 
   bounded by 'first_lmm_dest' + 'words_to_load' */

static int
spram_lock_latency (first_lmm_dest, lmm_dest, words_to_load)
     rtx first_lmm_dest;
     rtx lmm_dest;
     int words_to_load;
{
  if (words_to_load <= 2)
    return 3;

  return 5 + register_pair_offset (first_lmm_dest, lmm_dest);
}

/* Return the sram latency to a register (lmm_dest) assuming a register range 
   bounded by 'first_lmm_dest' + 'words_to_load' */

static int
sram_latency (first_lmm_dest, lmm_dest, words_to_load)
     rtx first_lmm_dest;
     rtx lmm_dest;
     int words_to_load;
{
  if (words_to_load <= 2)
    {
      if (!odd_register_p(first_lmm_dest) || words_to_load == 1)
	return 15;
      else
	return 16;       /* Odd register and two word load, add one clock */
    }

  if (!odd_register_p(first_lmm_dest))
    return 15 + register_pair_offset (first_lmm_dest, lmm_dest);
  else
    return 16 + register_pair_offset (first_lmm_dest, lmm_dest);
}

/* Return the sdram latency to a register (lmm_dest) assuming a register range 
   bounded by 'first_lmm_dest' + 'words_to_load' */

static int
sdram_latency (first_lmm_dest, lmm_dest, words_to_load)
     rtx first_lmm_dest;
     rtx lmm_dest;
     int words_to_load;
{
  if (words_to_load <= 2)
    {
      if (!odd_register_p(first_lmm_dest) || words_to_load == 1)
	return 32;
      else
	return 33;       /* Odd register and two word load, add one clock */
    }

  if (!odd_register_p(first_lmm_dest))
    return 32 + register_pair_offset (first_lmm_dest, lmm_dest);
  else
    return 33 + register_pair_offset (first_lmm_dest, lmm_dest);
}

/* Return the memory latency to a register (lmm_dest) assuming a register range bounded by 
   'first_lmm_dest' + 'words_to_load' */
static int
get_mem_latency_to_reg(mem_operand, first_lmm_dest, lmm_dest, words_to_load)
     rtx mem_operand;
     rtx first_lmm_dest;
     rtx lmm_dest;
     int words_to_load;
{
  if (memory_region_ccram (mem_operand))
    return ccram_latency (first_lmm_dest, lmm_dest, words_to_load);

  if (memory_region_ccram_lock (mem_operand))
    return ccram_lock_latency (first_lmm_dest, lmm_dest, words_to_load);

  if (memory_region_spram (mem_operand))
    return spram_latency (first_lmm_dest, lmm_dest, words_to_load);

  if (memory_region_spram_lock (mem_operand))
    return spram_lock_latency (first_lmm_dest, lmm_dest, words_to_load);

  if (memory_region_sram (mem_operand))
    return sram_latency (first_lmm_dest, lmm_dest, words_to_load);

  if (memory_region_sdram (mem_operand))
    return sdram_latency (first_lmm_dest, lmm_dest, words_to_load);

  return 0;
}


/* Given a  lmm instruction return the first register in the destination register range */

static rtx
get_lmm_first_dest (insn)
     rtx insn;
{
  rtx pat = PATTERN (insn);

  if (get_attr_octave_class (insn) == OCTAVE_CLASS_MULTILOAD)
    {
      if (get_attr_reg_linked_to_mem(insn) == REG_LINKED_TO_MEM_FALSE)
	return XEXP (XVECEXP (pat, 0, 0), 0);
      else
	return XEXP (XVECEXP (XVECEXP (pat, 0, 0), 0, 0), 0);
    }
  else if (get_attr_octave_class (insn) == OCTAVE_CLASS_SINGLELOAD)
    {
      if (get_attr_reg_linked_to_mem(insn) == REG_LINKED_TO_MEM_FALSE)
	return XEXP (pat, 0);
      else
	return XEXP (XVECEXP (pat, 0, 0), 0);
    }
  return NULL_RTX;
}

/* Return the number of words being transfered in a lmm instruction */
static int
get_lmm_words (insn)
     rtx insn;
{
  rtx pat = PATTERN (insn);

  if (get_attr_octave_class (insn) == OCTAVE_CLASS_MULTILOAD)
    {
      if (get_attr_reg_linked_to_mem(insn) == REG_LINKED_TO_MEM_FALSE)
	return XVECLEN (pat, 0);
      else
	return XVECLEN (XVECEXP (pat, 0, 0), 0);
    }
  else if (get_attr_octave_class (insn) == OCTAVE_CLASS_SINGLELOAD)
    return 1;
  else
    return 0;
}

/* Return the memory operand in a lmm instruction */
static rtx
get_lmm_memory_operand (insn)
     rtx insn;
{
  rtx pat = PATTERN (insn);

  if (get_attr_octave_class (insn) == OCTAVE_CLASS_MULTILOAD)
    {
      if (get_attr_reg_linked_to_mem(insn) == REG_LINKED_TO_MEM_FALSE)
	return (XEXP (XVECEXP (pat, 0, 0), 1));
      else
	return (XEXP (XVECEXP (XVECEXP (pat, 0, 0), 0, 0), 1));
    }

  if (get_attr_octave_class (insn) == OCTAVE_CLASS_SINGLELOAD)
    {
      if (get_attr_reg_linked_to_mem(insn) == REG_LINKED_TO_MEM_FALSE)
	return XEXP (pat, 1);
      else
	return XEXP (XVECEXP (pat, 0, 0), 1);
    }

  return NULL_RTX;
}


static void
m480_find_used_regs (xptr, data)
     rtx *xptr;
     void *data ATTRIBUTE_UNUSED;
{
  int i, j;
  enum rtx_code code;
  const char *fmt;
  rtx x = *xptr;

  /* repeat is used to turn tail-recursion into iteration since GCC
     can't do it when there's no return value.  */
 repeat:
  if (x == 0)
    return;

  code = GET_CODE (x);
  if (REG_P (x))
    {
      if (insn_reg_use_count == M480_MAX_USES)
	return;

      insn_reg_use_table[insn_reg_use_count].reg_rtx = x;
      insn_reg_use_count++;
    }

  /* Recursively scan the operands of this expression.  */

  for (i = GET_RTX_LENGTH (code) - 1, fmt = GET_RTX_FORMAT (code); i >= 0; i--)
    {
      if (fmt[i] == 'e')
	{
	  /* If we are about to do the last recursive call
	     needed at this level, change it into iteration.
	     This function is called enough to be worth it.  */
	  if (i == 0)
	    {
	      x = XEXP (x, 0);
	      goto repeat;
	    }

	  m480_find_used_regs (&XEXP (x, i), data);
	}
      else if (fmt[i] == 'E')
	for (j = 0; j < XVECLEN (x, i); j++)
	  m480_find_used_regs (&XVECEXP (x, i, j), data);
    }
}


static int
m480_lmm_adjust_cost (insn, dep_insn, cost)
     rtx insn;
     rtx dep_insn;
     int cost;
{
  int latency = 0;
  int max_latency = 0;
  int i;

  switch (get_attr_octave_class (insn))
    {
    case OCTAVE_CLASS_LOADOP:
    case OCTAVE_CLASS_ALU3OPABCMOD:
    case OCTAVE_CLASS_ALU3OPABC:
    case OCTAVE_CLASS_ALU2OPBRANCH:
    case OCTAVE_CLASS_ALU3OPSX:
    case OCTAVE_CLASS_BITOP:
    case OCTAVE_CLASS_BITOPIMMED:
    case OCTAVE_CLASS_MVOP:
    case OCTAVE_CLASS_CMOVOP:
    case OCTAVE_CLASS_DMAOP:
    case OCTAVE_CLASS_ALU3OP:
    case OCTAVE_CLASS_ZEROBRANCH:
    case OCTAVE_CLASS_ALUOP:
    case OCTAVE_CLASS_ALU2OPFIND:
    case OCTAVE_CLASS_TSSRCOP:
    case OCTAVE_CLASS_OCTAVE:
    case OCTAVE_CLASS_SINGLESTORE:
    case OCTAVE_CLASS_MULTISTORE:
    case OCTAVE_CLASS_MVHOP:
    case OCTAVE_CLASS_ALU2OP:
    case OCTAVE_CLASS_TESTBITBRANCH:
      {
	insn_reg_use_count = 0;
	note_uses (&PATTERN (insn), m480_find_used_regs, NULL);

	for (i = 0; i < insn_reg_use_count; i++)
	  {
	    if (reg_mentioned_p (insn_reg_use_table[i].reg_rtx, dep_insn)
		&& REG_P (insn_reg_use_table[i].reg_rtx))
	      {
		rtx mem_operand = get_lmm_memory_operand (dep_insn);
		rtx first_dest_reg = get_lmm_first_dest (dep_insn);

		int words_to_load = 0;
	      
		if (mem_operand)
		  {
		    words_to_load = get_lmm_words(dep_insn);

		    if (REGNO (insn_reg_use_table[i].reg_rtx) >= REGNO (first_dest_reg) 
			&& (REGNO (insn_reg_use_table[i].reg_rtx) < (REGNO (first_dest_reg) + words_to_load)))
		      {
			latency = get_mem_latency_to_reg (mem_operand, first_dest_reg, insn_reg_use_table[i].reg_rtx, words_to_load);

			/* It is possible that 'dep_insn' (a lmm) is setting more that one register needed in 'insn'. If
			   it's possible that the latency to the registers is different. If so we don't want to reduce the
			   scheduling cost below the largest latency we find. */

			if (max_latency == 0)
			  max_latency = latency;

			if (latency > max_latency)
			  max_latency = latency;
		      }
		  }
	      }
	  }
	if (max_latency < cost)
	  return max_latency;
	else
	  return cost;
      }
	
    default:
      return cost;
    }
}

/* Adjust the cost of a scheduling dependency.  Return the new cost of
   a dependency LINK or INSN on DEP_INSN.  COST is the current cost.  */

static int
m480_adjust_cost (insn, link, dep_insn, cost)
     rtx insn;
     rtx link;
     rtx dep_insn;
     int cost;
{
  if (recog_memoized (insn) < 0)
    return cost;

  if (recog_memoized (dep_insn) < 0)
    return cost;

  if (REG_NOTE_KIND (link) != 0)
    return 0;

  if (REG_NOTE_KIND (link) == 0)
    {
      enum attr_octave_class oct_class = get_attr_octave_class (dep_insn);

      if (oct_class == OCTAVE_CLASS_MULTILOAD
	  || oct_class == OCTAVE_CLASS_SINGLELOAD)
	{
	  return m480_lmm_adjust_cost (insn, dep_insn, cost);
	}

      /* Data dependency; DEP_INSN writes a register that INSN reads
	 some cycles later.  */
      switch (get_attr_octave_class (insn))
	{
	  /* If we have a 'br rn' and 'rn' is set in the previous instruction the cost must be adjusted to 2.
	     This is because the 'br' attempts to read 'rn' before it has been written back to the register */
	case OCTAVE_CLASS_BRANCHOP:
	  if (recog_memoized (dep_insn)
	      && (INSN_CODE (dep_insn) >= 0)
	      && (get_attr_octave_class (dep_insn) == OCTAVE_CLASS_LOADOP
		  || get_attr_octave_class (dep_insn) == OCTAVE_CLASS_ALU3OPABCMOD
		  || get_attr_octave_class (dep_insn) == OCTAVE_CLASS_ALU3OPABC
		  || get_attr_octave_class (dep_insn) == OCTAVE_CLASS_ALU2OPBRANCH
		  || get_attr_octave_class (dep_insn) == OCTAVE_CLASS_ALU3OPSX
		  || get_attr_octave_class (dep_insn) == OCTAVE_CLASS_BITOP
		  || get_attr_octave_class (dep_insn) == OCTAVE_CLASS_BITOPIMMED
		  || get_attr_octave_class (dep_insn) == OCTAVE_CLASS_MVOP
		  || get_attr_octave_class (dep_insn) == OCTAVE_CLASS_CMOVOP))
	    {
	      return 2;
	    }

	  /* Load dependent on a instruction that probably is setting up an address */
	case OCTAVE_CLASS_MULTILOAD:
	case OCTAVE_CLASS_SINGLELOAD:
	  return 1;
	  
	default:
	  break;
	}
    }

  return cost;
}

void
emit_mvrh_insn (op0, op1)
     rtx op0;
     rtx op1;
{
  static int mvrh_to_octoam = 0;

  switch (REGNO (op0))
    {
    case REG_64:
      emit_insn (gen_mvrh_to_octcfg(op1));
      break;

    case REG_67:
      emit_insn (gen_mvrh_to_octtrap(op1));
      break;

    case REG_68:
      emit_insn (gen_mvrh_to_octtrapen(op1));
      break;

    case REG_73:
      emit_insn (gen_mvrh_to_octglbitag(op1));
      break;

    case REG_74:
      emit_insn (gen_mvrh_to_octctl(op1));
      break;

    case REG_77:
      emit_insn (gen_mvrh_to_octrtcctl(op1));
      break;

    case REG_80:
      emit_insn (gen_mvrh_to_bme_cfg(op1));
      break;

    case REG_81:
      emit_insn (gen_mvrh_to_bmenextbuf (op1));
      break;

    case REG_85:
      emit_insn (gen_mvrh_to_octtrappc(op1));
      break;

    case REG_93:
      if (!mvrh_to_octoam)
	{
	  emit_insn (gen_mvrh_to_octoam_atmheader(op1));
	  mvrh_to_octoam = 1;
	}
      else
	{
	  emit_insn (gen_mvrh_to_octoam_atmpayload(op1));
	  mvrh_to_octoam = 0;
	}
      break;

    case REG_94:
      emit_insn (gen_mvrh_to_software_epd(op1));
      break;

    case REG_95:
      emit_insn (gen_mvrh_to_octdiag(op1));
      break;

    case REG_96:
      emit_insn (gen_mvrh_to_octlptr0(op1));
      break;

    case REG_97:
      emit_insn (gen_mvrh_to_octlptr16(op1));
      break;

    case REG_98:
      emit_insn (gen_mvrh_to_octlptr32(op1));
      break;

    case REG_99:
      emit_insn (gen_mvrh_to_octlptr48(op1));
      break;

    case REG_100:
      emit_insn (gen_mvrh_to_octlbase1(op1));
      break;

    case REG_BASE2:
      emit_insn (gen_mvrh_to_octlbase2(op1));
      break;

    case REG_BASE3:
      emit_insn (gen_mvrh_to_octlbase3(op1));
      break;

    case REG_107:
      emit_insn (gen_mvrh_to_activeid(op1));
      break;

    case REG_108:
      emit_insn (gen_mvrh_to_activeid2(op1));
      break;

    case REG_109:
      emit_insn (gen_mvrh_to_activeid3(op1));
      break;

    case REG_110:
      emit_insn (gen_mvrh_to_activecid(op1));
      break;

    case REG_111:
      emit_insn (gen_mvrh_to_activecidxor(op1));
      break;

    default:
    case REG_69:
    case REG_78:		/* Interrupt Save Register. Does not have other side effects */
    case REG_79:		/* NMI Save Register. Does not have other side effects */
    case REG_87:		/* Octave interrupt state register. Does not have other side effects */
    case REG_88:		/* Octave NMI state register. Does not have other side effects */
    case REG_89:		/* Octave Global Register. No known side-effects */
    case REG_90:		/* Octave Global Register. No known side-effects */
    case REG_91:		/* Octave Global Register. No known side-effects */
    case REG_92:		/* Octave Global Register. No known side-effects */
      emit_insn (gen_movsi_insn(op0, op1));
      break;
    }

  return;
}



/* asm directive handling */

/* asm directive list handling */
struct asm_node
{
  unsigned int line;
  rtx asm_rtx;
  struct asm_node *next;
};

/* global to hold list of asm directives being created */
struct asm_node *asm_list = NULL;


void m480_handle_note_related_insn(line)
     unsigned int line;
{
  rtx asm_dir = get_asm_dir_at_line(line);
  
  if (asm_dir && GET_CODE (asm_dir) == ASM_INPUT)
    {
      const char *string = XSTR (asm_dir, 0);
      
      if (string[0])
	{
	  fprintf (asm_out_file, "\t%s\n", string);
	}
      delete_asm_directive(line);
    }
}

/* delete the asm directive associated with note at 'line' */
void
delete_asm_directive(line)
     unsigned int line;
{
  struct asm_node *old_link, *temp;

  temp = asm_list;

  while(temp != NULL)
    {
      if(temp->line == line)
	{
	  /* if node to be deleted is the first node in the list */
	  if(temp == asm_list)
	    {
	      asm_list = temp->next;
	      free(temp);
	      return;
	    }
	  /* deleting an intermediate asm directive */
	  else
	    {
	      old_link->next = temp->next;
	      free(temp);
	      return;
	    }
	}
      /* traverse further and store old node location */
      else
	{
	  old_link = temp;
	  temp = temp->next;
	}
    }

  fprintf(stderr, "Asm Direcitve at line note %d not found", line); 
  abort();
}

/* get asm directive associated with note at 'line' */
rtx
get_asm_dir_at_line(line)
     unsigned int line;
{
  struct asm_node *temp = asm_list;

  while(temp != NULL)
    {
      if(temp->line == line)
	return temp->asm_rtx;
      
      temp = temp->next;
    }

  return NULL;
}

/* in m480 addresses never trap */
bool
m480_mem_addr_can_trap(x)
     rtx x ATTRIBUTE_UNUSED;
{
  return 0;
}

int
branch_after_brsl (insn)
     rtx insn;
{
  rtx prev_insn;

  if (recog_memoized (insn) < 0)
    return 0;

  if (GET_CODE (insn) != JUMP_INSN)
    return 0;

  if (get_attr_type (insn) != TYPE_BRANCH)
    return 0;

  prev_insn = prev_real_insn (insn);

  if(!prev_insn)
    return 0;

  if (recog_memoized (prev_insn) < 0)
    return 0;

  if (get_attr_type (prev_insn) != TYPE_BRSL)
    return 0;

  return 1;
}

static void
m480_output_function_prologue (stream, size)
     FILE *stream ATTRIBUTE_UNUSED;
     HOST_WIDE_INT size ATTRIBUTE_UNUSED;
{
}

static void
m480_output_function_epilogue (stream, size)
     FILE *stream ATTRIBUTE_UNUSED;
     HOST_WIDE_INT size ATTRIBUTE_UNUSED;
{
}

void
branch_displacement_warning (insn, instruction_string)
     rtx insn;
     const char *instruction_string;
{
  rtx line_note = previous_insn (insn);

  if (GET_CODE (line_note) == NOTE)
    {
      if (NOTE_LINE_NUMBER (line_note) >= 0)
	{
	  warning_with_file_and_line (NOTE_SOURCE_FILE (line_note),
				      NOTE_LINE_NUMBER (line_note),
				      "'%s' branch displacement out of range. Generating compenstation code",
				      instruction_string);
	}
    }
  else
    warning ("'%s' branch displacement out of range. Generating compenstation code",
	     instruction_string);
}

int
m480_eligible_for_delay_simple (trial, insn)
     rtx trial;
     rtx insn;
{
  enum attr_octave_class oct_class;

  if(GET_CODE(insn) == CALL_INSN)
    return 1;

  if (recog_memoized (trial) < 0)
    return 1;

  /* Check to see if the trial insn is a lmm */

  oct_class = get_attr_octave_class (trial);

  /* If we are being called from 'fill_simple_delay_slots' disallow lmm's in delay slots.
     This is because moving lmm's into delay slots typically has the effect of moving them
     closer to the insns that consume their results. */

  if (oct_class == OCTAVE_CLASS_MULTILOAD
      || oct_class == OCTAVE_CLASS_SINGLELOAD)
    {
      return 0;
    }
  else
    return 1;
}

int
m480_eligible_for_delay_from_target (insn)
     rtx insn;
{
  enum attr_octave_class oct_class;

  if (recog_memoized (insn) < 0)
    return 1;

  oct_class = get_attr_octave_class (insn);

  /* Don't ever move a TSS or DMA isntruction from the target of a branch because we don't 
     know if the instuction would be executed and they are expensive. */

  if (oct_class == OCTAVE_CLASS_TSSRCOP
      || oct_class == OCTAVE_CLASS_DMAOP)
    {
      return 0;
    }
  else
    return 1;
}

int
m480_eligible_for_delay_from_fallthrough (insn)
     rtx insn;
{
  enum attr_octave_class oct_class;

  if (recog_memoized (insn) < 0)
    return 1;

  oct_class = get_attr_octave_class (insn);

  if (oct_class == OCTAVE_CLASS_TSSRCOP
      || oct_class == OCTAVE_CLASS_DMAOP)
    {
      return 0;
    }
  else
    return 1;
}

/* 
   do not allow compare and branch
   to be combined as 'cmp ... br='
   as the later allows smaller displacement
   than former and may cause link errors.
*/

int
octave_can_combine (insn1, insn2)
     rtx insn1 ATTRIBUTE_UNUSED;
     rtx insn2 ATTRIBUTE_UNUSED;
{
  /*
  enum attr_octave_class insn1_class;
  enum attr_octave_class insn2_class;

  if (recog_memoized (insn1) < 0)
    return 1;

  if (recog_memoized (insn2) < 0)
    return 1;

  insn1_class = get_attr_octave_class (insn1);
  insn2_class = get_attr_octave_class (insn2);

  if (insn1_class == OCTAVE_CLASS_CONDBRANCHIMMED)
    {
      if (insn2_class == OCTAVE_CLASS_COMPAREOP)
	{
	  if (get_attr_type(insn2) == TYPE_COMPARE)
	    return 0;
	  else
	    return 1;
	}

      return 1;
    }

  if (insn1_class == OCTAVE_CLASS_COMPAREOP)
    {
      if (insn2_class == OCTAVE_CLASS_CONDBRANCHIMMED)
	{
	  if (get_attr_type(insn1) == TYPE_COMPARE)
	    return 0;
	  else
	    return 1;
	}
    }
  */

  return 1;
}

int
octave_local_label (label_ref)
     rtx label_ref;
{
  rtx label;

  if (GET_CODE (label_ref) != LABEL_REF)
    return 0;

  label = XEXP (label_ref, 0);
  if (is_label_ref_expr (label))
    {
      return 1;
    }
  else
    return 0;
}
