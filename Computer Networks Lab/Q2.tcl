
#This program will create two nodes and pass traffice from one node to other node using TCP protocol

#Creating simulator object 
set ns [new Simulator]  

#creating Two color classes for ns object to distinguish the traffic coming from various sources
$ns color 0 blue
$ns color 1 red 

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
#creating array of Nodes   
for {set i 0} {$i<7} {incr i} {
set H($i) [$ns node]
}

#Creating Links
for {set i 1} {$i<7} {incr i} {
$ns duplex-link $H(0) $H($i) 512Kb 10ms SFQ
}

#Orienting The nodes
$ns duplex-link-op $H(0) $H(1) orient left-up
$ns duplex-link-op $H(0) $H(2) orient right-up
$ns duplex-link-op $H(0) $H(3) orient right
$ns duplex-link-op $H(0) $H(4) orient right-down
$ns duplex-link-op $H(0) $H(5) orient left-down
$ns duplex-link-op $H(0) $H(6) orient left

#setting routing protocol to DV

$ns rtproto DV  

#Creating a TCP agent and connecting it to n0 (Basically it defines source node of TCP)
set tcp0 [new Agent/TCP]

#Specifying udp traffic to have red color as defined in the second line of program
$tcp0 set fid_ 1  

$ns attach-agent $H(1) $tcp0   

#Creating a Sink Agent and attaching it to n1
set sink0 [new Agent/TCPSink]
$ns attach-agent $H(4) $sink0

#Connecting TCP agent with Sink agent
$ns connect $tcp0 $sink0

#Creating FTP agent for traffic and attching it to tcp0
set ftp0 [new Application/FTP]
$ftp0 attach-agent $tcp0

#Starting the FTP Traffic
$ns at 0.1 "$ftp0 start"
$ns at 1.5 "$ftp0 stop"

#Specifying the UDP agent
set udp0 [new Agent/UDP]

#Specifying udp traffic to have red color as defined in the second line of program
$udp0 set fid_ 0

#Attaching the UDP agent with n0
$ns attach-agent $H(2) $udp0

#Specifying the Null agent
set null0 [new Agent/Null]

#Attaching the NULL agent with n1
$ns attach-agent $H(5) $null0

#Connecting both udp0 and null0 agents for transferring data between n0 and n1
$ns connect $udp0 $null0

#Specifying the CBR agent to generate the traffic over udp0 agent
set cbr0 [new Application/Traffic/CBR]

#setting rate
$cbr0 set rate_ 256Kb

#Attaching cbr0 with udp0
$cbr0 attach-agent $udp0

#Starting the cbr0 at 0.02 simulaio time
$ns at 0.2 "$cbr0 start"

#Stoping the cbr0 at 1.5 simulation time
$ns at 1.3 "$cbr0 stop"

$ns rtmodel-at 0.5 down $H(0) $H(5)
$ns rtmodel-at 0.9 up $H(0) $H(5)
$ns rtmodel-at 0.7 down $H(0) $H(4)
$ns rtmodel-at 1.2 up $H(0) $H(4)

$ns at 2.0 "finish"

$ns run


