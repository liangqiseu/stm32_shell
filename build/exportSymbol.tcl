#!/usr/local/bin/tcl8.6

namespace eval buildSymTcl {
	set nm "arm-none-eabi-nm"
	set nmFlag "-gS"
	set srcFile "start.elf"
	set dstFile "symbol.c"
	set fdOut ""
}

proc buildSymTcl::creatSymTcl {} {

	set symbolInfoRaw [eval exec $buildSymTcl::nm $buildSymTcl::nmFlag $buildSymTcl::srcFile]
	regsub -all { +} $symbolInfoRaw " " symbolInfoRaw
	set symbolInfoList [split $symbolInfoRaw \n]
	#puts $buildSymTcl::fdOut $symbolInfoList
	set symTblList {}
	
	foreach symbolEntry $symbolInfoList {
		
		#function name with . is illegal in c file
		if [regexp {^.*\.} $symbolEntry] {
			continue
		}
		
		if [regexp {^(.*) (.*) (.*) (.*)} $symbolEntry total addr size type name] {
		#	puts $buildSymTcl::fdOut "$total || $addr || $size || $type || $name"
		}

		if [regexp {^_(.*)} $name] {
			continue
		} 


		# Capital: Symbol is global     Letter: Symbol is local
		# A --- Absolute symbol,glabal, Interrupt handle func for example;
		# B --- Uninitialized data,stored in BSS, global;
		# C --- common symbol,same as uninitialized data
		# D --- Initialized data,global;
		# F --- File name;
		# T --- Text symbol,global;
		# U --- Symbol is not defined in this file but defined in other file
		# V --- weak object
		# W --- The symbol is a weak symbol that has not been specifically tagged as a weak object symbol


		#puts $buildSymTcl::fdOut "$total || $addr || $size || $type || $name"

		switch -glob $type {
			[DR] {
				puts $buildSymTcl::fdOut "extern int $name;"
				lappend symTblList "	\{\
									0,\
									\"$name\",\
									(char*)\&$name,\
									0x$size,0,0,\
									SYMBOL_GLOBAL | SYMBOL_DATA\
									\},"
			}

			
			[BC] {
				puts $buildSymTcl::fdOut "extern int $name;"
				lappend symTblList "	\{\
									0,\
									\"$name\",\
									(char*)\&$name, \
									0x$size,0,0,\
									SYMBOL_GLOBAL | SYMBOL_BSS\
									\},"
			}

			[A] {
				puts $buildSymTcl::fdOut "extern int $name;"
				lappend symTblList "	\{\
									0,\
									\"$name\",\
									(char*) \&$name,\
									0x$size,0,0,\
									SYMBOL_GLOBAL | SYMBOL_ABS\
									\},"
			}

			[TVW] {		
				if [regexp {^[a-z]*} $name]	{
					continue
				}	
				puts $buildSymTcl::fdOut "extern int $name ();"
				lappend symTblList "	\{\
									0,\
									\"$name\",\
									(char*) $name,\
									0x$size,0,0,\
									SYMBOL_GLOBAL | SYMBOL_TEXT\
									\},"
			}



			default {
				continue
			}


		}
		
	}
	
	set len [llength $symTblList]
	set symTblListSorted [lsort $symTblList]	

	puts $buildSymTcl::fdOut "unsigned int g_symTblLen = $len;"
	puts $buildSymTcl::fdOut ""
	puts $buildSymTcl::fdOut "SYMBOL_TABAL_S symTbl\[$len\] = "
	puts $buildSymTcl::fdOut "\{"
	
	foreach symbolTblEntry $symTblListSorted {
		puts $buildSymTcl::fdOut $symbolTblEntry			
	}
	puts $buildSymTcl::fdOut "\};"

	return 0
}

set buildSymTcl::fdOut [open $buildSymTcl::dstFile w+]

puts $buildSymTcl::fdOut "/********************************/"
puts $buildSymTcl::fdOut "/* filename:symbol.c*/"
puts $buildSymTcl::fdOut "/* creat time: [clock format [clock seconds]] */"
puts $buildSymTcl::fdOut "/********************************/"
puts $buildSymTcl::fdOut "#include \"symbol_api.h\""




set ret [buildSymTcl::creatSymTcl]

close $buildSymTcl::fdOut
