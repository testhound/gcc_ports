/* Definitions of target machine for Mindspeed MXT400 Network Processor

   Copyright (C) 1991, 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002, 2003
   Free Software Foundation, Inc.
   Contributed by Michael Collison
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

;; Unused yet units
;; ----------------
;;SDRAM Controller 
;;(define_cpu_unit "sdram_controller" "sdram_controller_automaton")

;;(define_cpu_unit "sio" "sio_automaton")

;;(define_cpu_unit "bme_exhaust" "bme_exhaust_automaton")
;;(define_cpu_unit "bme_threshold" "bme_threshold_automaton")
;; SMC read data command fifo (Holds data after it is returned 
;; by SRAM waiting to be delivered to Octave
;(define_cpu_unit "smc_read_data_f0, 
;                  smc_read_data_f1, 
;                  smc_read_data_f2, 
;                  smc_read_data_f3" "smc_read_data_automaton")

;; SMC SRAM write command fifo
;(define_cpu_unit "smc_write_cmd_f0, 
;                  smc_write_cmd_f1, 
;                  smc_write_cmd_f2, 
;                  smc_write_cmd_f3" "smc_write_cmd_automaton")

;; SMC write data command fifo 
;; (Holds data before it is sent to SRAM from Octave
;(define_cpu_unit "smc_write_data_f0, 
;                  smc_write_data_f1, 
;                  smc_write_data_f2, 
;                  smc_write_data_f3" "smc_write_data_automaton")

;; SMC SRAM read command fifo
;(define_cpu_unit "smc_read_data_fifo0,smc_read_data_fifo1" "smc_read_data_automaton")

;(define_cpu_unit "smc_read_data_fifo0" "smc_read_data_automaton")



;;
;;  DFA pipeline description
;;  ========================
;;
(define_automaton "slm_fifo_automaton, 
                   regfile_write_ports_automaton,
                   regfile_read_ports_automaton,
                   ray_sdram_write_pipe_automaton, 
                   ray_sdram_read_pipe_automaton,
                   smc_read_cmd_automaton,
                   smc_write_cmd_automaton,
                   spram_automaton, 
                   ccram_automaton, 
                   sram_automaton, 
                   sdram_automaton, 
                   multiplier_automaton,
                   decode_automaton, 
                   execute_automaton,
                   sls_fifo_automaton,
                   bme_automaton,
                   tss_automaton")

;(automata_option "no-minimization")
(automata_option "time")
(automata_option "v")

;;

;;(define_cpu_unit "octave_alu" "octave_automaton")


;; FIFO for CCRAM, SDRAM, SRAM, and SIO
(define_cpu_unit "slm_fifo" "slm_fifo_automaton")

;; FIFO for SPRAM
(define_cpu_unit "sls_fifo" "sls_fifo_automaton")

(define_cpu_unit "tss" "tss_automaton")

;; Ray CrossBar SDRAM write pipeline
(define_cpu_unit "ray_sdram_write_pipe0, 
                  ray_sdram_write_pipe1, 
                  ray_sdram_write_pipe2, 
                  ray_sdram_write_pipe3" "ray_sdram_write_pipe_automaton")

;; Ray CrossBar SDRAM write data FIFO
;(define_cpu_unit "ray_sdram_write_data_f0, 
;                  ray_sdram_write_data_f1, 
;                  ray_sdram_write_data_f2, 
;                  ray_sdram_write_data_f3" "ray_sdram_write_data_automaton")

;; Ray CrossBar SDRAM read data FIFO
;(define_cpu_unit "ray_sdram_read_data_f0, 
;                  ray_sdram_read_data_f1, 
;                  ray_sdram_read_data_f2, 
;                  ray_sdram_read_data_f3" "ray_sdram_read_data_automaton")

;; Ray CrossBar SDRAM read pipeline
(define_cpu_unit "ray_sdram_read_pipe0, 
                  ray_sdram_read_pipe1, 
                  ray_sdram_read_pipe2, 
                  ray_sdram_read_pipe3" "ray_sdram_read_pipe_automaton");

(define_cpu_unit "smc_read_cmd_fifo0,
		  smc_read_cmd_fifo1" "smc_read_cmd_automaton")

(define_cpu_unit "smc_write_cmd_fifo0, 
                  smc_write_cmd_fifo1" "smc_write_cmd_automaton")


(define_cpu_unit "ccram" "ccram_automaton")
(define_cpu_unit "sdram" "sdram_automaton")
(define_cpu_unit "sram" "sram_automaton")
(define_cpu_unit "spram" "spram_automaton")
(define_cpu_unit "decode" "decode_automaton")
(define_cpu_unit "execute" "execute_automaton")

(define_cpu_unit "multiplier" "multiplier_automaton")

;; The octave register file has two write ports. 
;; So on a given clock cycle two registers may be written
(define_cpu_unit "regfile_write_port0, 
                  regfile_write_port1" "regfile_write_ports_automaton")

;; The octave register file has three read ports. 
;; So on a given clock cycle three registers may be written?
(define_cpu_unit "regfile_read_port0,
                  regfile_read_port1,
                  regfile_read_port2" "regfile_read_ports_automaton")

;; BME - Buffer Manipulation Engine
(define_cpu_unit "bme" "bme_automaton")

;; Alias for either write port in the octave register file
(define_reservation "regfile_write" "(regfile_write_port0 | 
                                      regfile_write_port1)")

;; Alias for either read port in the octave register file
(define_reservation "regfile_read" "(regfile_read_port0 | 
                                     regfile_read_port1 | 
                                     regfile_read_port2)")

(define_reservation "ray_sdram_write_pipe" "ray_sdram_write_pipe0, 
                                            ray_sdram_write_pipe1, 
                                            ray_sdram_write_pipe2, 
                                            ray_sdram_write_pipe3")

(define_reservation "ray_sdram_read_pipe" "ray_sdram_read_pipe0, 
                                           ray_sdram_read_pipe1, 
                                           ray_sdram_read_pipe2, 
                                           ray_sdram_read_pipe3")

; =======================================================================



; ======================== LMM  C C R A M ===========================
;                          --------------
(define_insn_reservation "lmm_ccram" 6
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "1,2"))))
  "decode, 
   execute, 
   slm_fifo, 
   ccram * 2, 
   regfile_write")

(define_insn_reservation "lmm_ccram_reg_odd" 7
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (and (eq_attr "mem_transfer_size" "2")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   execute, 
   slm_fifo, 
   ccram * 2, 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_ccram_2" 7
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "3,4"))))
  "decode,
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   ccram,  
   (ccram + regfile_write),
   regfile_write")


(define_insn_reservation "lmm_ccram_reg_odd_2" 8
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (and (eq_attr "mem_transfer_size" "3,4")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode,
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   ccram,  
   (ccram + regfile_write),
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_ccram_3" 8
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "5,6"))))
  "decode, 
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   (ccram + slm_fifo),
   (ccram + slm_fifo + regfile_write), 
   (ccram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_ccram_reg_odd_3" 9
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (and (eq_attr "mem_transfer_size" "5,6")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   (ccram + slm_fifo),
   (ccram + slm_fifo + regfile_write), 
   (ccram + regfile_write), 
   regfile_write,
   regfile_write")

(define_insn_reservation "lmm_ccram_4" 9
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "7,8"))))
  "decode,
   execute, 
   slm_fifo, 
   (ccram + slm_fifo), 
   (ccram + slm_fifo), 
   (ccram + slm_fifo + regfile_write), 
   (ccram + slm_fifo + regfile_write), 
   (ccram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_ccram_reg_odd_4" 10
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (and (eq_attr "mem_transfer_size" "7,8")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode,
   execute, 
   slm_fifo, 
   (ccram + slm_fifo), 
   (ccram + slm_fifo), 
   (ccram + slm_fifo + regfile_write), 
   (ccram + slm_fifo + regfile_write), 
   (ccram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_ccram_5" 10
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "9,10"))))
  "decode,
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   (ccram + slm_fifo),
   (ccram + slm_fifo + regfile_write), 
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + regfile_write),
   regfile_write")

(define_insn_reservation "lmm_ccram_reg_odd_5" 11
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (and (eq_attr "mem_transfer_size" "9,10")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode,
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   (ccram + slm_fifo),
   (ccram + slm_fifo + regfile_write), 
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + regfile_write),
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_ccram_6" 11
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "11,12"))))
  "decode,
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   (ccram + slm_fifo),
   (ccram + slm_fifo + regfile_write), 
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + regfile_write),
   regfile_write")



(define_insn_reservation "lmm_ccram_reg_odd_6" 12
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (and (eq_attr "mem_transfer_size" "11,12")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode,
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   (ccram + slm_fifo),
   (ccram + slm_fifo + regfile_write), 
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + regfile_write),
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_ccram_7" 12
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "13,14"))))
  "decode,
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   (ccram + slm_fifo),
   (ccram + slm_fifo + regfile_write), 
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + regfile_write),
   regfile_write")


(define_insn_reservation "lmm_ccram_reg_odd_7" 13
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (and (eq_attr "mem_transfer_size" "13,14")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode,
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   (ccram + slm_fifo),
   (ccram + slm_fifo + regfile_write), 
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + regfile_write),
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_ccram_8" 13
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "15,16"))))
  "decode,
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   (ccram + slm_fifo),
   (ccram + slm_fifo + regfile_write), 
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + regfile_write),
   regfile_write")


(define_insn_reservation "lmm_ccram_reg_odd_8" 14
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (and (eq_attr "mem_transfer_size" "15,16")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode,
   execute,
   slm_fifo, 
   (ccram + slm_fifo),
   (ccram + slm_fifo),
   (ccram + slm_fifo + regfile_write), 
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + slm_fifo + regfile_write),
   (ccram + regfile_write),
   regfile_write,
   regfile_write")


; ====================== LMM  S P R A M =======================================
;                        --------------
(define_insn_reservation "lmm_spram" 3
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (eq_attr "mem_transfer_size" "1,2"))))
  "decode, 
   (execute + spram), 
   regfile_write")

(define_insn_reservation "lmm_spram_reg_odd" 4
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (and (eq_attr "mem_transfer_size" "2")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   execute+spram, 
   regfile_write, 
   regfile_write")

(define_insn_reservation "lmm_spram_2" 4
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (eq_attr "mem_transfer_size" "3,4"))))
  "decode, 
   (execute + sls_fifo + spram), 
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_reg_odd_2" 5
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (and (eq_attr "mem_transfer_size" "3,4")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram), 
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_3" 5
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (eq_attr "mem_transfer_size" "5,6"))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_reg_odd_3" 6
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (and (eq_attr "mem_transfer_size" "5,6")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_4" 6
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (eq_attr "mem_transfer_size" "7,8"))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_reg_odd_4" 7
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (and (eq_attr "mem_transfer_size" "7,8")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_5" 7
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (eq_attr "mem_transfer_size" "9,10"))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_reg_odd_5" 8
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (and (eq_attr "mem_transfer_size" "9,10")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_6" 8
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (eq_attr "mem_transfer_size" "11,12"))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_reg_odd_6" 9
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (and (eq_attr "mem_transfer_size" "11,12")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_7" 9
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (eq_attr "mem_transfer_size" "13,14"))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_reg_odd_7" 10
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (and (eq_attr "mem_transfer_size" "13,14")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_8" 10
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (eq_attr "mem_transfer_size" "15,16"))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_reg_odd_8" 11
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram")
                 (and (eq_attr "mem_transfer_size" "15,16")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram), 
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (spram + regfile_write), 
   regfile_write,
   regfile_write")

; ====================== LMM  S P R A M_L O C K =======================================
;                        ----------------------
(define_insn_reservation "lmm_spram_lock" 4
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (eq_attr "mem_transfer_size" "1,2"))))
  "decode, 
   (execute + spram*2), 
   regfile_write")

(define_insn_reservation "lmm_spram_lock_reg_odd" 5
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (and (eq_attr "mem_transfer_size" "2")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + spram*2), 
   regfile_write, 
   regfile_write")

(define_insn_reservation "lmm_spram_lock_2" 5
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (eq_attr "mem_transfer_size" "3,4"))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_lock_reg_odd_2" 6
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (and (eq_attr "mem_transfer_size" "3,4")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_lock_3" 6
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (eq_attr "mem_transfer_size" "5,6"))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_lock_reg_odd_3" 7
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (and (eq_attr "mem_transfer_size" "5,6")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_lock_4" 7
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (eq_attr "mem_transfer_size" "7,8"))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_lock_reg_odd_4" 8
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (and (eq_attr "mem_transfer_size" "7,8")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_lock_5" 8
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (eq_attr "mem_transfer_size" "9,10"))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_lock_reg_odd_5" 9
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (and (eq_attr "mem_transfer_size" "9,10")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_lock_6" 9
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (eq_attr "mem_transfer_size" "11,12"))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_lock_reg_odd_6" 10
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (and (eq_attr "mem_transfer_size" "11,12")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (sls_fifo + spram + regfile_write), 
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_lock_7" 10
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (eq_attr "mem_transfer_size" "13,14"))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_lock_reg_odd_7" 11
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (and (eq_attr "mem_transfer_size" "13,14")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


(define_insn_reservation "lmm_spram_lock_8" 11
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (eq_attr "mem_transfer_size" "15,16"))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (spram + regfile_write), 
   regfile_write")


(define_insn_reservation "lmm_spram_lock_reg_odd_8" 12
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "spram_lock")
                 (and (eq_attr "mem_transfer_size" "15,16")
		      (eq_attr "odd_register_multiload" "true")))))
  "decode, 
   (execute + sls_fifo + spram*2), 
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (sls_fifo + spram + regfile_write),
   (spram + regfile_write), 
   regfile_write,
   regfile_write")


; ====================== SMM  C C R A M =======================================C
;                        --------------
(define_insn_reservation "smm_ccram" 4
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "1,2"))))
  "(decode + regfile_read),
   execute,
   slm_fifo,
   ccram")

(define_insn_reservation "smm_ccram_reg_odd" 5
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "2"))))
  "(decode + regfile_read),
   (execute + regfile_read),
   slm_fifo,
   (slm_fifo + ccram),
   ccram")

(define_insn_reservation "smm_ccram_2" 5
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "3,4"))))
  "(decode + regfile_read),
   (execute + regfile_read),
   slm_fifo,
   (slm_fifo + ccram),
   ccram")


(define_insn_reservation "smm_ccram_3" 6
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "5,6"))))
  "(decode + regfile_read),
   (execute + regfile_read),
   (slm_fifo + regfile_read),
   (slm_fifo + ccram),
   (slm_fifo + ccram),
   ccram")


(define_insn_reservation "smm_ccram_4" 7
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "7,8"))))
  "(decode + regfile_read),
   (execute + regfile_read),
   (slm_fifo + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram),
   (slm_fifo + ccram),
   ccram")


(define_insn_reservation "smm_ccram_5" 8
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "9,10"))))
  "(decode + regfile_read),
   (execute + regfile_read),
   (slm_fifo + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram),
   (slm_fifo + ccram),
   ccram")


(define_insn_reservation "smm_ccram_6" 9
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "11,12"))))
  "(decode + regfile_read),
   (execute + regfile_read),
   (slm_fifo + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram),
   (slm_fifo + ccram),
   ccram")


(define_insn_reservation "smm_ccram_7" 10
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "13,14"))))
  "(decode + regfile_read),
   (execute + regfile_read),
   (slm_fifo + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram),
   (slm_fifo + ccram),
   ccram")


(define_insn_reservation "smm_ccram_8" 12
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "ccram,ccram_lock")
                 (eq_attr "mem_transfer_size" "15,16"))))
  "(decode + regfile_read),
   (execute + regfile_read),
   (slm_fifo + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram + regfile_read),
   (slm_fifo + ccram),
   (slm_fifo + ccram),
   ccram")


; ====================== SMM  S P R A M =======================================C
;                        --------------
(define_insn_reservation "smm_spram" 3
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "spram,spram_lock")
                 (eq_attr "mem_transfer_size" "1,2"))))
  "(decode + regfile_read),
   (execute + sls_fifo), 
   spram")


(define_insn_reservation "smm_spram_2" 4
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "spram,spram_lock")
                 (eq_attr "mem_transfer_size" "3,4"))))
  "(decode + regfile_read),
   (execute + sls_fifo + regfile_read), 
   (sls_fifo + spram),
   spram")


(define_insn_reservation "smm_spram_3" 5
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "spram,spram_lock")
                 (eq_attr "mem_transfer_size" "5,6"))))
  "(decode + regfile_read),
   (execute + sls_fifo + regfile_read), 
   (regfile_read + sls_fifo + spram),
   (sls_fifo + spram),
   spram")


(define_insn_reservation "smm_spram_4" 6
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "spram,spram_lock")
                 (eq_attr "mem_transfer_size" "7,8"))))
  "(decode + regfile_read),
   (execute + sls_fifo + regfile_read), 
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (sls_fifo + spram),
   spram")


(define_insn_reservation "smm_spram_5" 7
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "spram,spram_lock")
                 (eq_attr "mem_transfer_size" "9,10"))))
  "(decode + regfile_read),
   (execute + sls_fifo + regfile_read), 
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (sls_fifo + spram),
   spram")


(define_insn_reservation "smm_spram_6" 8
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "spram,spram_lock")
                 (eq_attr "mem_transfer_size" "11,12"))))
  "(decode + regfile_read),
   (execute + sls_fifo + regfile_read), 
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (sls_fifo + spram),
   spram")


(define_insn_reservation "smm_spram_7" 9
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "spram,spram_lock")
                 (eq_attr "mem_transfer_size" "13,14"))))
  "(decode + regfile_read),
   (execute + sls_fifo + regfile_read), 
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (sls_fifo + spram),
   spram")


(define_insn_reservation "smm_spram_8" 10
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "spram,spram_lock")
                 (eq_attr "mem_transfer_size" "15,16"))))
  "(decode + regfile_read),
   (execute + sls_fifo + regfile_read), 
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (regfile_read + sls_fifo + spram),
   (sls_fifo + spram),
   spram")


; ====================== LMM  S R A M =======================================C
;                        -------------
;
; For SRAM, the read command fifo can hold up to eight 32-bit words. 
; After 8 words we have to use the second fifo entry to hold the request
; Once at the head of the fifo the read request has to wait a minimum 
; of 3 cycles before being sent to SRAM
(define_insn_reservation "lmm_sram" 15
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "1,2"))))
  "decode,
   (execute + slm_fifo), 
   smc_read_cmd_fifo0*3, 
   sram")

(define_insn_reservation "lmm_sram_3_4" 16
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "3,4"))))
  "decode,
   (execute + slm_fifo), 
   smc_read_cmd_fifo0*3, 
   sram")

(define_insn_reservation "lmm_sram_5_6" 17
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "5,6"))))
  "decode,
   (execute + slm_fifo), 
   smc_read_cmd_fifo0*3, 
   sram")

(define_insn_reservation "lmm_sram_7_8" 18
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "7,8"))))
  "decode,
   (execute + slm_fifo), 
   smc_read_cmd_fifo0*3, 
   sram")

(define_insn_reservation "lmm_sram_9_10" 20
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "9,10"))))
  "decode,
   (execute + slm_fifo), 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1, 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1, 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1,
   (sram + smc_read_cmd_fifo0),
   smc_read_cmd_fifo0,
   smc_read_cmd_fifo0,
   sram")

(define_insn_reservation "lmm_sram_11_12" 21
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "11,12"))))
  "decode,
   (execute + slm_fifo), 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1, 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1, 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1,
   (sram + smc_read_cmd_fifo0),
   smc_read_cmd_fifo0,
   smc_read_cmd_fifo0,
   sram")

(define_insn_reservation "lmm_sram_13_14" 22
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "13,14"))))
  "decode,
   (execute + slm_fifo), 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1, 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1, 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1,
   (sram + smc_read_cmd_fifo0),
   smc_read_cmd_fifo0,
   smc_read_cmd_fifo0,
   sram")

(define_insn_reservation "lmm_sram_15_16" 23
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "15,16"))))
  "decode,
   (execute + slm_fifo), 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1, 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1, 
   smc_read_cmd_fifo0 + smc_read_cmd_fifo1,
   (sram + smc_read_cmd_fifo0),
   smc_read_cmd_fifo0,
   smc_read_cmd_fifo0,
   sram")

; ====================== SMM  S R A M =======================================C
;                        -------------
(define_insn_reservation "smm_sram" 14
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "1,2"))))
  "(decode + regfile_read),
   (execute + slm_fifo),
   smc_write_cmd_fifo0*3,
   sram")


(define_insn_reservation "smm_sram_3_4" 18
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "3,4"))))
  "(decode + regfile_read),
   (execute + slm_fifo),
   (slm_fifo + regfile_read),
   smc_write_cmd_fifo0*3, 
   sram")


(define_insn_reservation "smm_sram_5_6" 18
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "5,6"))))
  "(decode + regfile_read),
   (execute + slm_fifo), 
   (slm_fifo + regfile_read),
   smc_write_cmd_fifo0*3, 
   sram")


(define_insn_reservation "smm_sram_7_8" 18
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "7,8"))))
  "(decode + regfile_read),
   (execute + slm_fifo), 
   (slm_fifo + regfile_read),
   smc_write_cmd_fifo0*3, 
   sram")


(define_insn_reservation "smm_sram_9_10" 22
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "9,10"))))
  "(decode + regfile_read),
   (execute + slm_fifo), 
   (slm_fifo + regfile_read),
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (sram + smc_write_cmd_fifo0),
   sram")


(define_insn_reservation "smm_sram_11_12" 22
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "11,12"))))
  "(decode + regfile_read),
   (execute + slm_fifo), 
   (slm_fifo + regfile_read),
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (sram + smc_write_cmd_fifo0),
   smc_write_cmd_fifo0,
   sram")


(define_insn_reservation "smm_sram_13_14" 22
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "13,14"))))
  "(decode + regfile_read),
   (execute + slm_fifo), 
   (slm_fifo + regfile_read),
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (sram + smc_write_cmd_fifo0),
   smc_write_cmd_fifo0,
   sram")


(define_insn_reservation "smm_sram_15_16" 22
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
            (and (eq_attr "memory_region" "sram")
                 (eq_attr "mem_transfer_size" "15,16"))))
  "(decode + regfile_read),
   (execute + slm_fifo), 
   (slm_fifo + regfile_read),
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (smc_write_cmd_fifo0 + smc_write_cmd_fifo1), 
   (sram + smc_write_cmd_fifo0),
   smc_write_cmd_fifo0,
   sram")


; ======================== LMM  S D R A M ===========================
;                          --------------
(define_insn_reservation "lmm_sdram" 32 
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "1,2"))))
  "slm_fifo, 
   ray_sdram_read_pipe, 
   sdram*25, 
   nothing*5")


(define_insn_reservation "lmm_sdram_2" 34
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "3,4"))))
  "slm_fifo, 
   ray_sdram_read_pipe,
   sdram*25, 
   nothing*7")


(define_insn_reservation "lmm_sdram_3" 36
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "5,6"))))
  "slm_fifo, 
   ray_sdram_read_pipe,
   sdram*25, 
   nothing*9")


(define_insn_reservation "lmm_sdram_4" 38
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "7,8"))))
  "slm_fifo, 
   ray_sdram_read_pipe,
   sdram*25, 
   nothing*11")


(define_insn_reservation "lmm_sdram_5" 40
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "9,10"))))
  "slm_fifo, 
   ray_sdram_read_pipe,
   sdram*25, 
   nothing*13")


(define_insn_reservation "lmm_sdram_6" 42
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "11,12"))))
  "slm_fifo, 
   ray_sdram_read_pipe,
   sdram*25, 
   nothing*15")


(define_insn_reservation "lmm_sdram_7" 44
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "13,14"))))
  "slm_fifo, 
   ray_sdram_read_pipe,
   sdram*25, 
   nothing*17")


(define_insn_reservation "lmm_sdram_8" 46
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "lmm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "15,16"))))
  "slm_fifo, 
   ray_sdram_read_pipe,
   sdram*25, 
   nothing*19")

; ======================== SMM  S D R A M ===========================
;                          --------------
(define_insn_reservation "smm_sdram" 32 
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "1,2"))))
  "slm_fifo, 
   ray_sdram_write_pipe, 
   sdram*25, 
   nothing*5")


(define_insn_reservation "smm_sdram_2" 34
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "3,4"))))
  "slm_fifo, 
   ray_sdram_write_pipe,
   sdram*25, 
   nothing*7")


(define_insn_reservation "smm_sdram_3" 36
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "5,6"))))
  "slm_fifo, 
   ray_sdram_write_pipe,
   sdram*25, 
   nothing*9")


(define_insn_reservation "smm_sdram_4" 38
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "7,8"))))
  "slm_fifo, 
   ray_sdram_write_pipe,
   sdram*25, 
   nothing*11")


(define_insn_reservation "smm_sdram_5" 40
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "9,10"))))
  "slm_fifo, 
   ray_sdram_write_pipe,
   sdram*25, 
   nothing*13")


(define_insn_reservation "smm_sdram_6" 42
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "11,12"))))
  "slm_fifo, 
   ray_sdram_write_pipe,
   sdram*25, 
   nothing*15")


(define_insn_reservation "smm_sdram_7" 44
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "13,14"))))
  "slm_fifo, 
   ray_sdram_write_pipe,
   sdram*25, 
   nothing*17")


(define_insn_reservation "smm_sdram_8" 46
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "type" "smm")
	    (and (eq_attr "memory_region" "sdram")
		 (eq_attr "mem_transfer_size" "15,16"))))
  "slm_fifo, 
   ray_sdram_write_pipe,
   sdram*25, 
   nothing*19")

;; ======================================================================================
(define_insn_reservation "multiply" 3
  (and (eq_attr "cpu" "m480")
       (eq_attr "type" "mul,muli,mulh,mulhi"))
  "multiplier*3,regfile_write")

;; Can't read the Software EPD bit in the ESS for 6 clocks, 
;; can't issue another write to the BME_SWEPD reg for 2 clocks

(define_insn_reservation "bme_swepd" 5
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "instruction" "mvrh")
	    (eq_attr "bme_dest_reg" "swepd")))
  "bme*2,nothing*3")

;------------------------------------------------------------------

;;       Auto Memory Update Reservations 
;;       -------------------------------
(define_insn_reservation "auto_mem_update_ccram" 4
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "memory_region" "ccram,ccram_lock")
	    (eq_attr "linked_mem_update" "true")))
  "(decode + regfile_read),
   execute,
   slm_fifo,
   ccram")

(define_insn_reservation "auto_mem_update_spram" 3
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "memory_region" "spram,spram_lock")
	    (eq_attr "linked_mem_update" "true")))
  "(decode + regfile_read),
   (execute + sls_fifo),
   spram")

(define_insn_reservation "auto_mem_update_sdram" 32
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "memory_region" "sdram")
	    (eq_attr "linked_mem_update" "true")))
  "slm_fifo, 
   ray_sdram_write_pipe, 
   sdram*25, nothing*7")

;;
;;   Insns used by define_bypass
;;   --------------------------
(define_insn_reservation "alu3op_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (eq_attr "octave_class" "alu3opabcmod,alu3opabc,alu3op"))
  "decode, execute, regfile_write")
		     
(define_insn_reservation "alu2op_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (eq_attr "octave_class" "alu2op,alu2opbranch,alu2opfind"))
  "decode, execute, regfile_write")

(define_insn_reservation "testbitbranch_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (eq_attr "octave_class" "testbitbranch"))
  "(decode + regfile_read), execute")

(define_insn_reservation "zerobranch_immed_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (eq_attr "octave_class" "zerobranchimmed"))
  "(decode + regfile_read), execute")

(define_insn_reservation "zerobranch_indir_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "octave_class" "zerobranch")
	    (eq_attr "zerobranch_is_call" "false")))
  "(decode + regfile_read), execute")

(define_insn_reservation "zerobranch_call_indir_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "octave_class" "zerobranch")
	    (eq_attr "zerobranch_is_call" "true")))
  "(decode + regfile_read), execute, regfile_write")

(define_insn_reservation "cbranch_indir_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "octave_class" "condbranch")
	    (eq_attr "instruction" "bcr")))
  "(decode + regfile_read), execute")

(define_insn_reservation "branch_indir_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (and (eq_attr "octave_class" "branchop")
	    (eq_attr "instruction" "br")))
  "(decode + regfile_read), execute")

(define_insn_reservation "dmaop_class_reserv" 5
  (and (eq_attr "cpu" "m480")
       (eq_attr "octave_class" "dmaop"))
  "(decode + regfile_read),
   (execute + regfile_read),
   slm_fifo,
   (slm_fifo + ccram),
   ccram")
		     
(define_insn_reservation "movop_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (eq_attr "octave_class" "mvhop,cmovop,compareop,compareopsign"))
  "decode, execute, regfile_write")
		     
(define_insn_reservation "loadop_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (eq_attr "octave_class" "loadop"))
  "decode, execute, regfile_write")
		     
(define_insn_reservation "bitop_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (eq_attr "octave_class" "bitop,bitopimmed"))
  "decode, execute, regfile_write")
		     
(define_insn_reservation "octave_class_reserv" 3
  (and (eq_attr "cpu" "m480")
       (eq_attr "octave_class" "octave,multiregop,multilink"))
  "decode, execute, regfile_write")

; TSS operations (Get/Put) take 15 clocks thru the TSS pipeline
(define_insn_reservation "tss_class_reserv" 17
  (and (eq_attr "cpu" "m480")
       (eq_attr "octave_class" "tssrcop"))
  "decode, execute, tss * 15")


;; If we have an instruction that writes a register that is linked to memory 
;; any instruction that needs the register can get is as a bypass. So cost is 0
(define_bypass 0 "auto_mem_update_ccram" "alu3op_class_reserv,
                                          alu2op_class_reserv,
                                          movop_class_reserv,
                                          loadop_class_reserv,
                                          dmaop_class_reserv,
                                          bitop_class_reserv,
                                          octave_class_reserv,
                                          tss_class_reserv,
                                          auto_mem_update_ccram,
                                          auto_mem_update_spram,
                                          auto_mem_update_sdram")

;; If we have an instruction that writes a register that is linked to memory any 
;; instruction that needs the register can get is as a bypass. So cost is 0
(define_bypass 0 "auto_mem_update_spram" "alu3op_class_reserv,
                                          alu2op_class_reserv,
                                          movop_class_reserv,
                                          dmaop_class_reserv,
                                          loadop_class_reserv,
                                          bitop_class_reserv,
                                          octave_class_reserv,
                                          tss_class_reserv,
                                          auto_mem_update_ccram,
                                          auto_mem_update_spram,
                                          auto_mem_update_sdram")

;; If we have an instruction that writes a register that is linked to memory any 
;; instruction that needs the register can get is as a bypass. So cost is 0
(define_bypass 0 "auto_mem_update_sdram" "alu3op_class_reserv,
                                          alu2op_class_reserv,
                                          movop_class_reserv,
                                          dmaop_class_reserv,
                                          loadop_class_reserv,
                                          bitop_class_reserv,
                                          octave_class_reserv,
                                          tss_class_reserv,
                                          auto_mem_update_ccram,
                                          auto_mem_update_spram,
                                          auto_mem_update_sdram")

;; Most octave instructions use the bypass mechanism in the decode phase of the pipeline 
;; so that it may read the results set by a previous instruction without delay
(define_bypass 0 "alu3op_class_reserv,
                  alu2op_class_reserv,
                  movop_class_reserv,
                  loadop_class_reserv,
                  bitop_class_reserv,
                  octave_class_reserv" "alu3op_class_reserv,
                                        alu2op_class_reserv,
                                        movop_class_reserv,
                                        loadop_class_reserv,
                                        dmaop_class_reserv,
                                        bitop_class_reserv,
                                        octave_class_reserv,
                                        tss_class_reserv,
                                        testbitbranch_class_reserv,
                                        zerobranch_immed_class_reserv,
                                        zerobranch_indir_class_reserv,
                                        auto_mem_update_ccram,
                                        auto_mem_update_spram,
                                        auto_mem_update_sdram")

;; If we are trying to branch to a location specifed in a register, the result is not 
;; available until the execution phase of the branch (i.e no bypass in decode phase of branch). 
;According to the docs this is to avoid a critical path in the hardware. 
(define_bypass 2 "alu3op_class_reserv,
                  alu2op_class_reserv,
                  movop_class_reserv,
                  loadop_class_reserv,
                  bitop_class_reserv,
                  octave_class_reserv" "cbranch_indir_class_reserv,branch_indir_class_reserv")
