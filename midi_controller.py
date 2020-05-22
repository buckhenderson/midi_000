import mido

mido.get_input_names()
mido.get_output_names()
port2 = mido.open_output('loopMIDI Port 2 3')
channel = 2
channel_max = 3
try:
    with mido.open_input('loopMIDI Port 0') as inport:
        while True:
            for msg in inport:
                print(msg)
                #print(dir(msg))
                if msg.type == 'control_change' and msg.control == 1:
                    channel += 1
                    # channel = 0
                    if channel > channel_max:
                        channel = 2
                else:
                    msg_copy = msg
                    msg_copy.channel = channel
                    # output = mido.Message(msg.type, note=msg.note, velocity=msg.velocity, channel=2)
                    port2.send(msg_copy)
                    print(msg_copy)
                    print('sent on channel {}'.format(channel))
except:
    pass
