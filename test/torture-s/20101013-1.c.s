	.text
	.file	"/b/build/slave/linux/build/src/src/work/gcc/gcc/testsuite/gcc.c-torture/execute/20101013-1.c"
	.section	.text.main,"ax",@progbits
	.hidden	main
	.globl	main
	.type	main,@function
main:                                   # @main
	.result 	i32
# BB#0:                                 # %entry
	call    	build_ref_for_offset@FUNCTION
	i32.const	$push0=, 0
	return  	$pop0
	.endfunc
.Lfunc_end0:
	.size	main, .Lfunc_end0-main

	.section	.text.build_ref_for_offset,"ax",@progbits
	.type	build_ref_for_offset,@function
build_ref_for_offset:                   # @build_ref_for_offset
	.local  	i32, i32, i32, i32, i32
# BB#0:                                 # %entry
	i32.const	$0=, __stack_pointer
	i32.load	$0=, 0($0)
	i32.const	$1=, 16
	i32.sub 	$4=, $0, $1
	i32.const	$1=, __stack_pointer
	i32.store	$4=, 0($1), $4
	i32.const	$3=, 8
	i32.add 	$3=, $4, $3
	call    	get_addr_base_and_unit_offset@FUNCTION, $3
	i64.load	$push0=, 8($4)
	i64.const	$push1=, 4
	i64.add 	$push2=, $pop0, $pop1
	call    	build_int_cst@FUNCTION, $pop2
	i32.const	$2=, 16
	i32.add 	$4=, $4, $2
	i32.const	$2=, __stack_pointer
	i32.store	$4=, 0($2), $4
	return
	.endfunc
.Lfunc_end1:
	.size	build_ref_for_offset, .Lfunc_end1-build_ref_for_offset

	.section	.text.get_addr_base_and_unit_offset,"ax",@progbits
	.type	get_addr_base_and_unit_offset,@function
get_addr_base_and_unit_offset:          # @get_addr_base_and_unit_offset
	.param  	i32
# BB#0:                                 # %entry
	i64.const	$push0=, 0
	i64.store	$discard=, 0($0), $pop0
	return
	.endfunc
.Lfunc_end2:
	.size	get_addr_base_and_unit_offset, .Lfunc_end2-get_addr_base_and_unit_offset

	.section	.text.build_int_cst,"ax",@progbits
	.type	build_int_cst,@function
build_int_cst:                          # @build_int_cst
	.param  	i64
# BB#0:                                 # %entry
	block
	i64.const	$push0=, 4
	i64.ne  	$push1=, $0, $pop0
	br_if   	$pop1, 0        # 0: down to label0
# BB#1:                                 # %if.end
	return
.LBB3_2:                                # %if.then
	end_block                       # label0:
	call    	abort@FUNCTION
	unreachable
	.endfunc
.Lfunc_end3:
	.size	build_int_cst, .Lfunc_end3-build_int_cst


	.ident	"clang version 3.9.0 "
	.section	".note.GNU-stack","",@progbits