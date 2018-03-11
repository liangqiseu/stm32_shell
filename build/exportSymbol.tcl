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
	puts $buildSymTcl::fdOut $symbolInfoList
	
	foreach symbolEntry $symbolInfoList {
		
		#function name with . is illegal in c file
		if [regexp {^.*\.} $symbolEntry] {
			continue
		}
		
		if [regexp {^(.*) (.*) (.*) (.*)} $symbolEntry total addr size type name] {
			puts $buildSymTcl::fdOut "$total || $addr || $size || $type || $name"
		}
	}
	return 0
}

set buildSymTcl::fdOut [open $buildSymTcl::dstFile w+]

puts $buildSymTcl::fdOut "/********************************/"
puts $buildSymTcl::fdOut "/* filename:symbol.c*/"
puts $buildSymTcl::fdOut "/* creat time: [clock format [clock seconds]] */"
puts $buildSymTcl::fdOut "/********************************/"
puts $buildSymTcl::fdOut ""

set ret [buildSymTcl::creatSymTcl]

close $buildSymTcl::fdOut
