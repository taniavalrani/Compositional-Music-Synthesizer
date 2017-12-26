# Score:
* Reads an input (.notes) file 
* Writes a note_list: a list of 3-element arrays (onset, offset, pitch) 
* Can Output the note_list

Sample usage:
`./main yankee.q.notes -m 4/4 -p 35 -t 120 -b 1 -a 2`

## Fitness Landscape Objects

### Meter:
* build_note_list: 
** makes a vector of onsets 
** quantizes the list with quantize_list 
** scores the pips to determine the tactus
* quantize_list: 
* builds a pip deque: an ascending deque of numbers in pip_time intervals. Defaults to 35.
* builds a quantized_meter map that contains the unquantized onsets as keys; quantized onsets as values
* score_pips:
** Builds a map (pip_scores) of the score for each pip. Will be used to determine tactus
** As of now (10/24): only increases the score when a quantized onset falls on a pip. Should also increase the score for larger durations
* Need to build compute_tactus_level!




Meter landscape object:






Pitch landscape object:



Preference landscape object:



Preference object for agent:


Agent (voice):


---- Other based on agents

# Compositional-Music-Synthesizer
# Compositional-Music-Synthesizer
