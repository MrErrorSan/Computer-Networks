#set a 20
#set b [gets stdin]
#puts "b: $b"
#if {$a==20} {
#	puts "a: $a"
#}
#for {set x 0} {$x<5} {incr x} {
#	puts "$x"
#}

puts "Enter a number greater than 4"
set in [gets stdin]
puts "OUTPUT:-"
if {$in>4} {
	if {$in %2 == 0} {
		for {set i 4} {$i<$in} {incr i} {
			if {$i %2 == 0} {
				puts "$i "
			}
		}
	}
	if {$in %2 != 0} {
		for {set i 3} {$i<$in} {incr i} {
			if {$i %2 != 0} {
				puts "$i "
			}
		}
	}
}
if {$in<=4} {
	puts "NUMBER IS SMALL..."
}
