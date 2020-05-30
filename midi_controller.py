import threading
import mido

mido.get_input_names()
mido.get_output_names()
global port2
port2 = mido.open_output('loopMIDI Port 2 4')
global channel
channel = 2
global channel_max
channel_max = 3

global stop
stop = False

def device():
    try:
        with mido.open_input('loopMIDI Port 1') as inport:
            global channel
            global channel_max
            print('opened input')
            while not stop:
                print('in while loop')
                for msg in inport:
                    print('incoming msg: {}'.format(msg))
                    if msg.type == 'control_change' and msg.control == 1:
                        print('in if')
                        channel += 1
                        # channel = 0
                        if channel > channel_max:
                            channel = 2
                    else:
                        print('in else')
                        msg_copy = msg
                        print('copied msg')
                        msg_copy.channel = channel
                        print('sending message')
                        port2.send(msg_copy)
                        print('sent message')
                        print('outgoing msg: {}'.format(msg_copy))
            print('out of loop')
    except:
        pass


def piano():
    try:
        with mido.open_input('MIDI Matrix Encoder 0') as inport:
            global channel
            global channel_max
            print('opened input')
            while not stop:
                print('in while loop')
                for msg in inport:
                    print('incoming msg: {}'.format(msg))
                    if msg.type == 'control_change' and msg.control == 1:
                        print('in if')
                        channel += 1
                        # channel = 0
                        if channel > channel_max:
                            channel = 2
                    else:
                        print('in else')
                        msg_copy = msg
                        print('copied msg')
                        msg_copy.channel = channel
                        print('sending message')
                        port2.send(msg_copy)
                        print('sent message')
                        print('outgoing msg: {}'.format(msg_copy))
            print('out of loop')
    except:
        pass



try:
    midi_thread = threading.Thread(target=device)
    midi_thread.start()
    piano_thread = threading.Thread(target=piano)
    piano_thread.start()
except KeyboardInterrupt:
    pass