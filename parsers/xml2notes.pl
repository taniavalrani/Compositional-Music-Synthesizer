#!/usr/bin/perl -w
use strict;

use lib;
use version; our $VERSION = qv '0.0.1';
use Getopt::Long qw(GetOptions);
use XML::LibXML;

my $EXIT_STATUS = 0;

######################################################################
####################            OPTIONS           ####################
######################################################################
our %OPT = (header    =>  1,
	    parts     =>  1,
	    out       =>  'STDOUT',
    );

GetOptions("header!"    => \$OPT{header},
	   "parts!"     => \$OPT{parts},
	   "help"       => \&usage,
	   "o=s"        => \$OPT{out},
    );

&usage unless @ARGV;
if ($OPT{'out'} == 'STDOUT') {
    $OPT{'out'} = STDOUT;
}

my $public_id = "-//Recordare//DTD MusicXML 3.0 Partwise//EN";
my $system_id = "http://www.musicxml.org/dtds/partwise.dtd";
my $dtd = XML::LibXML::Dtd->new($public_id, $system_id);
my $filename = $ARGV[0];
my $doc = XML::LibXML->new->parse_file($filename);
#$doc->validate($dtd);
my %part_ids;
my %attributes = (
    'key'       =>    undef,
    'mode'      =>    undef,
    'beats'     =>    undef,
    'beat-type' =>    undef,
    );
my $current_part = 0;
my $clock = 0;

sub usage() {
    my $usage = << "EOF";
Usage: $0 [options] file1 [file2 ...]

Options:
    -noheader              Don't include the header in the final output
   
    -noparts               Don't include the parts list in the final 
                           output
    
    -o <out.notes>         Write to output file

    -help                  Print this help text and quit

To Do: Handle chords/polyphonic music, write notes2xml script
EOF
    print $usage;

    exit 0;
}

sub header() {
    my $title = $doc->findvalue('/score-partwise/movement-title');
    print ";\t\t\t\t$title\n";

    foreach my $creator ($doc->findnodes('/score-partwise/identification')) {
	my $composer = $creator->findvalue('./creator[@type="composer"]');
	my $arranger = $creator->findvalue('./creator[@type="arranger"]');
	my $lyricist = $creator->findvalue('./creator[@type="lyricist"]');
	my $other = $creator->findvalue('./creator[not(@type="composer" or @type="arranger" or @type="lyricist")]');
	print ";\t\t\t     Composer: $composer\n" if ($composer ne "");
	print ";\t\t\t     Arranger: $arranger\n" if ($arranger ne "");
	print ";\t\t\t     Lyricist: $lyricist\n" if ($lyricist ne "");
	print ";\t\t\tOther Creator: $other\n" if ($other ne "");

    }
}
		   
sub score_parts {
    print ";\n;Score Parts:\n";

    foreach my $score_part ($doc->findnodes('/score-partwise/part-list/score-part')) {
	my $id = $score_part->getAttribute('id');
	my $part_name = $score_part->findvalue('./part-name');
	$part_ids{$id} = $part_name;
	print ";\tPart Name: $part_name";
	print "\tPart ID: $id";
	print ";\tInstruments: ", $score_part->findvalue('./score-instrument/instrument-name'), "\n";
    }
}

sub process_file() {
    foreach my $measure ($doc->findnodes('/score-partwise/part/measure')) {
	my $partno = $measure->findvalue('../@id');
	if ($partno ne $current_part) {
	    print ";\n;Part $partno: $part_ids{$partno}\n";
	    $current_part = $partno;
	    $clock = 0;
	}
	my $measureno = $measure->getAttribute('number');
	    print ";\tMeasure ", $measureno, "\n";
	    if ($measure->findvalue('./attributes') ne "") {
		my $key = $measure->findvalue('./attributes/key/fifths');
		my $mode = $measure->findvalue('./attributes/key/mode');
		my $beats = $measure->findvalue('./attributes/time/beats');
		my $beat_type = $measure->findvalue('./attributes/time/beat-type');
		my $tempo = $measure->findvalue('./@tempo | .//@tempo');

		
		$attributes{'key'} = $key unless ($key eq "");
		$attributes{'mode'} = $mode unless ($mode eq "");
		$attributes{'beats'} = $beats unless ($beats eq "");
		$attributes{'beat-type'} = $beat_type unless ($beat_type eq "");
		$attributes{'tempo'} = $tempo unless ($tempo eq "");
		
		print ";\t\tKey: ", $attributes{'key'}; 
		print "\tMode: ", $attributes{'mode'};
		print "\tTempo: ", $attributes{'tempo'};
		print "\tTime: ", $attributes{'beats'}, "/", $attributes{'beat-type'};
		print "\n";
	    }
	    
	    
	    
	    foreach my $note ($measure->findnodes('./note')) {
		if ($note->findnodes('./rest')->size > 0) {
		    my $time = $note->findvalue('./duration');
		    $clock += $time unless ($note->findnodes('./chord')->size > 0);
		    next;
		} elsif ($note->findnodes('./pitch')->size > 0) {
		    my $alter = $note->findvalue('./pitch/alter');
		    $alter = 0 if $alter eq "";
		    my $time = on_off($note->findvalue('./duration'), $clock);
		    
		    my $pnote = process_pitches($note->findvalue('./pitch/step'), $alter, $note->findvalue('./pitch/octave'));       	    
		    $clock = @$time[1] unless ($note->findnodes('./chord')->size > 0);
		    print "\t\tNote ", " ", @$time[0], " ", @$time[1], " ", , $pnote, "\n";
		}		
	    }
	}
    }

sub on_off ($$) {
    my $duration = shift;
    my $loc_clock = shift;
    
    my $length = $attributes{'tempo'} / (4 / $duration);
    my $start = $loc_clock;
    my $end = $loc_clock + $length;
    my @time = ($start, $end);
    return \@time;
}

sub process_pitches ($$$) {
    my $lpitch = lc shift;
    my $alter = shift;
    my $noctave = shift;
    my %pitches = (
	c => 0, d => 2, e => 4,
	f => 5, g => 7, a => 9,
	b => 11);

    my $pvalue = $pitches{$lpitch};
    my $pvalue_oct = ($pvalue + $alter) + (++$noctave * 12);
    return $pvalue_oct;
}

######################################################################
####################            MAIN              ####################
######################################################################

header() if $OPT{header};
score_parts() if $OPT{parts};
process_file();    



    
	
