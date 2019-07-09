#ifndef GCC_M480_PROTOS_H
#define GCC_M480_PROTOS_H


/* Functions in m480.c */


extern unsigned int saw_brsl;

extern void   m480_expand_prologue PARAMS ((void));

extern void   m480_expand_epilogue PARAMS ((void));

extern void   trace PARAMS ((const char*));
extern void   trace2 PARAMS ((const char*, const char*));
extern void   m480_dbr_output_seqend PARAMS ((FILE *));


#ifdef RTX_CODE
int m480_eligible_for_delay_simple PARAMS ((rtx, rtx));
int m480_eligible_for_delay_from_target PARAMS ((rtx));
int m480_eligible_for_delay_from_fallthrough PARAMS ((rtx));
extern int octave_can_combine PARAMS ((rtx, rtx));
extern int  branch_after_brsl PARAMS ((rtx));
extern void  branch_displacement_warning PARAMS ((rtx, const char *));
extern int  in_instruction_group PARAMS ((rtx));
extern int  any_operand PARAMS ((rtx, enum machine_mode));
extern int  m480_hard_regno_mode_ok PARAMS ((unsigned int, enum machine_mode));
extern rtx    octave_get_unaligned_address PARAMS ((rtx ref, int extra_offset));

extern void   emit_mvrh_insn PARAMS ((rtx, rtx));

extern rtx    m480_gen_store_multiple PARAMS ((int, int, rtx, int, int, int, int, int));

extern rtx    m480_gen_load_multiple PARAMS ((int, int, rtx, int, int, int, int, int));

extern int    m480_gen_movstrqi PARAMS ((rtx *));

extern int    octave_emit_add_sequence PARAMS ((rtx *operands, enum machine_mode mode));

extern void   m480_override_options PARAMS((void));

extern void   m480_gen_compare_and_branch PARAMS ((enum rtx_code, rtx, rtx , int, rtx));

extern struct rtx_def *m480_gen_compare_reg PARAMS ((enum rtx_code, rtx, rtx, int));

extern char  *output_bci_bcdi PARAMS ((rtx, int, int, rtx));

extern void octave_rtx_debug PARAMS ((rtx, char *, char *, int ));
extern int output_indir_cbranch PARAMS((void));

extern bool m480_cannot_modify_jumps_past_reload_p PARAMS ((void));
extern void m480_output_label PARAMS ((FILE *f, const char* name));
void  m480_asm_declare_object_name PARAMS ((FILE *f, const char* name, tree decl));
void m480_asm_declare_function_name PARAMS ((FILE *f, char* name, tree decl));
void m480_asm_output_align PARAMS ((FILE* , unsigned int));

extern int m480_tablejump_p PARAMS ((rtx));
extern unsigned HOST_WIDE_INT octave_and_value PARAMS ((unsigned long ));
extern unsigned HOST_WIDE_INT octave_mask_width PARAMS ((unsigned long ));
extern int has_delay_slots PARAMS ((void));
extern void print_operand_address PARAMS ((FILE *, rtx ));
extern void print_operand PARAMS ((FILE *, rtx , char ));
extern void print_modx_value PARAMS ((FILE *, rtx));
extern int mod_mask_operand PARAMS ((rtx, enum machine_mode));
extern int octave_split_constant PARAMS (( enum rtx_code, enum machine_mode, HOST_WIDE_INT, rtx, rtx, int));
extern void old_octave_split_constant PARAMS (( enum machine_mode , HOST_WIDE_INT , rtx , int ));
extern int octave_aligned_memory_operand PARAMS (( register rtx , enum machine_mode ));
extern void octave_get_aligned_mem PARAMS ((rtx , rtx *, rtx *));
extern void octave_set_memflags PARAMS ((rtx , rtx ));
extern int m480_null_epilogue_p PARAMS ((void));
extern void octave_sp_plus_long_const_to_reg PARAMS ((FILE *, HOST_WIDE_INT, int ));
extern int octave_eligible_for_epilogue_delay PARAMS ((rtx , int ));
extern int is_aligned_base_pointer PARAMS ((rtx , enum machine_mode ));
extern int sf_move_operand PARAMS ((register rtx , enum machine_mode ));
extern int fputs_unlocked PARAMS ((const char *, FILE *));
extern int octave_sa_size PARAMS ((void));
extern int octave_frame_size PARAMS((int size));
extern int mostly_false_jump PARAMS ((rtx , rtx));
extern void warning VPARAMS ((const char *format, ...));

extern rtx get_asm_dir_at_line PARAMS ((unsigned int));
extern void delete_asm_directive PARAMS ((unsigned int));
extern void m480_handle_note_related_insn PARAMS ((unsigned int));

extern bool m480_mem_addr_can_trap PARAMS ((rtx));

struct rtx_def *m480_function_arg PARAMS ((CUMULATIVE_ARGS, enum machine_mode, tree, int));

extern void m480_machine_dependent_reorg PARAMS ((rtx));
/*   ----------------------functions of external visibility ----------------------------- */
int m480_gen_clrstrqi PARAMS ((rtx *));
int mod_mask PARAMS ((unsigned HOST_WIDE_INT));
int memory_cycle_cost PARAMS ((void));
int load_multiple_operation PARAMS ((rtx, enum machine_mode));
int store_multiple_operation PARAMS ((rtx, enum machine_mode));
int s_register_operand PARAMS ((rtx, enum machine_mode));
int any_memory_operand PARAMS ((rtx, enum machine_mode));
int reg_or_0_operand PARAMS ((rtx, enum machine_mode));
int reg_or_6bu_operand PARAMS ((rtx, enum machine_mode));
int reg_or_6bs_operand PARAMS ((rtx, enum machine_mode));
int reg_or_5bu_operand PARAMS ((rtx, enum machine_mode));
int reg_or_10bs_operand PARAMS ((rtx, enum machine_mode));
int lim_operand PARAMS ((rtx, enum machine_mode));
int move_operand PARAMS ((rtx, enum machine_mode));
int sf_move_operand PARAMS ((rtx, enum machine_mode));
int reg_or_lim_operand PARAMS ((rtx, enum machine_mode));
int c5bu_operand PARAMS ((rtx, enum machine_mode));
int c9bu_operand PARAMS ((rtx, enum machine_mode));
int c6bu_operand PARAMS ((rtx, enum machine_mode));
int c6bs_operand PARAMS ((rtx, enum machine_mode));
int c8bs_operand PARAMS ((rtx, enum machine_mode));
int c1bu_operand PARAMS ((rtx, enum machine_mode));
int c2bu_operand PARAMS ((rtx, enum machine_mode));
int c3bu_operand PARAMS ((rtx, enum machine_mode));
int c20bu_operand PARAMS ((rtx, enum machine_mode));
int flag_operand PARAMS ((rtx, enum machine_mode)); 
int get_flag_operand PARAMS ((rtx, enum machine_mode)); 
int get_flag_operand_v31 PARAMS ((rtx, enum machine_mode)); 
int get_flag_operand_v4 PARAMS ((rtx, enum machine_mode)); 
int putc_flag_operand PARAMS ((rtx, enum machine_mode)); 
int putc_flag_operand_v31 PARAMS ((rtx, enum machine_mode)); 
int putc_flag_operand_v4 PARAMS ((rtx, enum machine_mode)); 
int putm_flag_operand PARAMS ((rtx, enum machine_mode)); 
int putp_flag_operand PARAMS ((rtx, enum machine_mode)); 
int hec5_flag_operand PARAMS ((rtx, enum machine_mode)); 
int hardware_register_operand PARAMS ((rtx, enum machine_mode));
int lnk_register_operand PARAMS ((rtx, enum machine_mode));
int spl_ess_reg_operand PARAMS ((rtx, enum machine_mode));
int overflow_or_carry_ess_operand PARAMS ((rtx, enum machine_mode));
int hardware_register_operand PARAMS ((rtx, enum machine_mode));
int lnk_offset_operand PARAMS ((rtx, enum machine_mode));
int lnk_memory_operand PARAMS ((rtx, enum machine_mode));
int brsl_flag_operand PARAMS ((rtx, enum machine_mode));
int nibble_operand PARAMS ((rtx, enum machine_mode));
int mod_mask_operand PARAMS ((rtx, enum machine_mode));
int mod_mask PARAMS ((unsigned HOST_WIDE_INT));
int mod_mask_value PARAMS ((rtx, enum machine_mode));
int symbol_operand PARAMS ((rtx, enum machine_mode));
int symbol_or_12bu_operand PARAMS ((rtx, enum machine_mode));
int symbol_or_16bu_operand PARAMS ((rtx, enum machine_mode));
int symbol_or_17bu_operand PARAMS ((rtx, enum machine_mode));
int reg_indirect_operand PARAMS ((rtx, enum machine_mode));
int imm_indirect_operand PARAMS ((rtx, enum machine_mode));
int imm_offset_indirect_operand PARAMS ((rtx, enum machine_mode));
int ess_clobbering_plus_operand PARAMS ((rtx, enum machine_mode));
int spl_ess_comparison_operator PARAMS ((rtx, enum machine_mode));
int call_operand PARAMS ((rtx, enum machine_mode));
int octave_split_constant PARAMS ((enum rtx_code, enum machine_mode,HOST_WIDE_INT, rtx, rtx, int));
void octave_rtx_debug PARAMS ((register rtx, char *, char *, int));
int mm_constrained_p PARAMS ((rtx));
rtx m480_gen_compare_reg PARAMS ((enum rtx_code, rtx, rtx, int));
void m480_gen_compare_and_branch PARAMS ((enum rtx_code, rtx, rtx, int, rtx));
int memory_cycle_cost PARAMS ((void));
rtx m480_gen_load_multiple PARAMS ((int, int, rtx, int, int, int, int, int));
rtx m480_gen_store_multiple PARAMS ((int, int, rtx, int, int, int, int, int));
rtx m480_function_value PARAMS ((tree, tree));
int octave_emit_add_sequence PARAMS ((rtx *, enum machine_mode));
void m480_override_options PARAMS ((void));
int modx_operand PARAMS ((rtx, enum machine_mode));
int const_ok_for_load_p PARAMS ((rtx, enum machine_mode));
int reg_offset_operand PARAMS ((rtx, enum machine_mode));
extern int m480_legitimate_address_p PARAMS ((enum machine_mode, rtx, int));
extern int m480_legitimate_address_strict_p PARAMS ((rtx, enum machine_mode));
extern int m480_legitimate_address_nonstrict_p PARAMS ((rtx, enum machine_mode));
extern int m480_legitimate_base_plus_offset_p PARAMS ((rtx, int));
void print_label_ref_expr PARAMS ((FILE*, rtx));
bool is_label_ref_expr PARAMS ((rtx));

const char* output_cmov_spl PARAMS ((const char*, rtx, rtx, const char*, rtx));
const char* output_cbranch PARAMS ((unsigned int, rtx, unsigned int, const char*, rtx));
const char* output_cond_call PARAMS ((unsigned int, rtx, rtx, unsigned int, const char*, rtx, const char*));
const char* output_cond_return PARAMS ((unsigned int, rtx, rtx, unsigned int, const char*));
const char* output_brsl PARAMS ((rtx *, rtx));

const char* output_lmm PARAMS ((rtx, rtx, const char*, const char*));
const char* output_lmm_with_lnk PARAMS ((rtx, rtx, const char*, const char*, const char *));
const char* output_smm PARAMS ((rtx, rtx, const char*, const char*));
bool base_spram_memory_insn PARAMS ((rtx expr));
bool general_memory_insn PARAMS ((rtx expr));
int get_mem_offset_const PARAMS ((rtx expr));
int memory_region_p PARAMS ((rtx));
int memory_region_ccram PARAMS ((rtx));
int memory_region_ccram_lock PARAMS ((rtx));
int memory_region_spram PARAMS ((rtx));
int memory_region_spram_lock PARAMS ((rtx));
int memory_region_sdram PARAMS ((rtx));
int memory_region_sram PARAMS ((rtx));
int memory_region_sio PARAMS ((rtx));
int odd_register_p PARAMS ((rtx));

/*  ------------------------ end external visibility ----------------------------- */



#endif



#if defined(TREE_CODE) && defined(RTX_CODE)

extern rtx    m480_function_arg PARAMS ((CUMULATIVE_ARGS, enum machine_mode, tree, int));

extern rtx    m480_function_value PARAMS ((tree, tree));

#endif


#endif
