import mido

#Read the midi file as a mido object
mid = mido.MidiFile("silent_night_easy (1).mid")

song_array = []

#Play all tracks of the song back to back
for i, track in enumerate(mid.tracks):
    print('Track {}: {}'.format(i, track.name))
    
    #Read all MIDI messages in order from the track
    for msg in track:
        
        #We only want to sleep on messages that are actual commands 
        if not msg.is_meta:
            
            #Parse a message from the string into a mido object
            m = mido.Message.from_str(str(msg))
            
            #We only care about notes to play
            if m.type == "note_on":
                song_array.append(m.note)

print(song_array)
            

