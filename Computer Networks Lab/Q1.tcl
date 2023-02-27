
#This program will create two nodes and pass traffice from one node to other node using TCP protocol

#Creating simulator object 
set ns [new Simulator]   

#Creating the nam file
set nf [open out.nam w]      
#It opens the file 'out.nam' for writing and gives it the file handle 'nf'. 
$ns namtrace-all $nf

# Open the trace file
#  set nf [open out.tr w]
#  $ns trace-all $nf

#    we are opening a newtrace file named as "out" and also telling that data must be stored in .tr [trace] format.
#   "nf" is the file handler that we are used here to handle the trace file.
#   "w" means write i.e the file out.tr is opened for writing.
#   "r" means reading 
#   The second line tells the simulator to trace each packet on every link in the topology and for that we give file handler nf for the simulator ns.



#Finish Procedure  (closes the trace file and starts nam)
proc finish {} {
        global ns nf
        $ns flush-trace
        close $nf
        exec nam out.nam &
        exit 0
        }
			
#The trace data is flushed into the file by using command $ns flush-trace and then file is closed.
        
#Creating array of nodes
for {set y 0} {$y < 7} {incr y 1} { 
  set n($y) [$ns node]
}

#Creating a duplex link between two nodes (connect the nodes n0 and n1 with a duplex link with the bandwidth 1Megabit, a delay of 10ms and a SFQ queue)
set z 1
for {set x 0} {$x < 6} {incr x 1} { 
  $ns duplex-link $n($x) $n($z) 512Kb 5ms DropTail
  incr z 1
}
$ns duplex-link $n(6) $n(0) 512Kb 5ms DropTail
$ns duplex-link-op $n(0) $n(1) orient right-up
$ns duplex-link-op $n(1) $n(2) orient right-up
$ns duplex-link-op $n(2) $n(3) orient right
$ns duplex-link-op $n(3) $n(4) orient left-down
$ns duplex-link-op $n(4) $n(5) orient left
$ns duplex-link-op $n(5) $n(6) orient left-down
$ns duplex-link-op $n(6) $n(0) orient left-up

#setting routing protocol to DV

$ns rtproto DV  

#Specifying the UDP agent
set udp0 [new Agent/UDP]

#Attaching the UDP agent with n0
$ns attach-agent $n(0) $udp0

#Specifying the Null agent
set null0 [new Agent/Null]

#Attaching the NULL agent with n1
$ns attach-agent $n(3) $null0

#Connecting both udp0 and null0 agents for transferring data between n0 and n1
$ns connect $udp0 $null0

#Specifying the CBR agent to generate the traffic over udp0 agent
set cbr0 [new Application/Traffic/CBR]

#Each packet having 1000 bytes
$cbr0 set packetSize_ 1000

#EAch packet will be generated after 5ms i.e., 100 packets per second
$cbr0 set interval 0.01 

#Attaching cbr0 with udp0
$cbr0 attach-agent $udp0

#Starting the cbr0 at 0.02 simulaio time
$ns at 0.02 "$cbr0 start"

$ns rtmodel-at 0.4 down $n(2) $n(3)
$ns rtmodel-at 1.0 up $n(2) $n(3)

#Stoping the cbr0 at 1.5 simulation time
$ns at 1.5 "$cbr0 stop"

$ns at 2.0 "finish"

$ns run


