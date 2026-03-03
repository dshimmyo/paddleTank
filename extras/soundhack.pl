#!/usr/bin/perl
use Cwd;
use Cwd 'abs_path';
use Cwd qw(chdir);

#chdir("../assets/audio/");
my $framelength = @ARGV[0];#max 32 "20"in hex
my $frequency = @ARGV[1];# min 8 max 107 middleC C4 = 48
my $outfile = @ARGV[2]; #"../assets/audio/perlTest.sfx";

if (!@ARGV) {
    print "Usage: \n";
    print "./soundhack.pl <framelength> <frequency> <filename>\n\n";
    print "framelength default 8\n";
    print "frequency default 48 (middle c, c4) \n";
    print "default file: ../assets/audio/perlTest.sfx\n\n";

    $framelength = 8;
    $frequency = 48;
    $outfile = "../assets/audio/perlTest.sfx";
    print "continue with defaults? (y/n): ";
    my $userword = <STDIN>; # I moved chomp to a new line to make it more readable
    chomp $userword; # Get rid of newline character at the end
    exit 0 if ($userword eq ""); # If empty string, exit.
    if ($userword == "y" || $userword == "Y")
    {} else {exit 0;};
}
else
{
    # $framelength = @ARGV[0];
    if ($framelength > 32) {$framelength = 32;};
    print ("framelength set to ". $framelength ."\n");
};
#
#$test = @ARGV[0];
#text file test
my $output = "0800 0007 0606 2a2a 2a2a 0007 0606 2a2a
2a2a 0007 0606 2a2a 2a2a 0007 0606 2a2a
2a2a 0007 0606 2a2a 2a2a 0007 0606 2a2a
2a2a 0007 0606 2a2a 2a2a 0007 0606 1010
1010";


# open(my $filehandle, '>', $outfile) or die "Could not open file: $!";
# print $filehandle $output;
# close $filehandle;

#binary test
open(my $out, '>:raw', $outfile) or die "Unable to open: $!";
#print $out pack('s<', 255 );
my $hex = sprintf("0x%X", $framelength);
#my $firstbyte = ($hex."00");
print $out pack('s<', hex($hex) );
my $hexFreq = sprintf("0x%X", ($frequency * 257));
#my $freqByte = ($hexFreq.$hexFreq);

#print $out pack('s>', hex($hex));

for ($i=0; $i<$framelength;$i++){
    print $out pack('s>', hex("0007") );#Amp 0,1
    print $out pack('s>', hex("0606") );#Amp 2,3
    # print $out pack('s>', hex("2a2a") );#freq 0,1
    # print $out pack('s>', hex("2a2a") );#freq 0,1
    print $out pack('s<', hex($hexFreq) );
    print $out pack('s<', hex($hexFreq) );

};
# print $out pack('s<', 39);
# print $out pack('s<', 255);
# print $out pack('s<', 39);
close($out);


#sfx file format
#0800 0007 0606 2a2a 2a2a 0007 0606 2a2a
#2a2a 0007 0606 2a2a 2a2a 0007 0606 2a2a
#2a2a 0007 0606 2a2a 2a2a 0007 0606 2a2a
#2a2a 0007 0606 2a2a 2a2a 0007 0606 1010
#1010 


exit 0; 