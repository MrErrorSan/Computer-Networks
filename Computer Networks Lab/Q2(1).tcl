set ns [new Simulator]

set nf [open out.nam w]
$ns namtrace-all $nf

$ns color 0 purple
$ns color 1 black

proc finish {} {
        global ns nf
        $ns flush-trace
        close $nf
        exec nam out.nam &
        exit 0
}
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
$ns duplex-link $n0 $n2 2Mb 10ms DropTail
$ns duplex-link-op $n0 $n2 orient right-down 
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns duplex-link-op $n1 $n2 orient right-up
$ns duplex-link $n2 $n3 1.7Mb 20ms DropTail
$ns duplex-link-op $n2 $n3 orient right

set tcp1 [new Agent/TCP]
$ns attach-agent $n1 $tcp1   

set sink3 [new Agent/TCPSink]
$ns attach-agent $n3 $sink3

$ns connect $tcp1 $sink3

set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp1

set udp0 [new Agent/UDP]

$udp0 set fid_ 1

$ns attach-agent $n0 $udp0

set null0 [new Agent/Null]

$ns attach-agent $n3 $null0

$ns connect $udp0 $null0

$ns at 0.5 "$ftp0 start"
$ns at 4.0 "$ftp0 stop"

set cbr0 [new Application/Traffic/CBR]

$cbr0 set packetSize_ 1024

$cbr0 set interval 0.01 

$cbr0 attach-agent $udp0

$ns at 0.1 "$cbr0 start"

$ns at 4.5 "$cbr0 stop"

$ns at 5.0 "finish"


$ns run


