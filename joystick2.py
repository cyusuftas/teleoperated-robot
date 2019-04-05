import pygame
import serial, time, struct
import pickle

#arduinoData = serial.Serial('COM4',9600)
#ser = serial.Serial('COM29', 9600)

#wait for serial connection before write operation
time.sleep(1)

# Define some colors
BLACK    = (   0,   0,   0)
WHITE    = ( 255, 255, 255)

# This is a simple class that will help us print to the screen
# It has nothing to do with the joysticks, just outputting the
# information.
class TextPrint:
    def __init__(self):
        self.reset()
        self.font = pygame.font.Font(None, 20)

    def printa(self, screen, textString):
        textBitmap = self.font.render(textString, True, BLACK)
        screen.blit(textBitmap, [self.x, self.y])
        self.y += self.line_height
        
    def reset(self):
        self.x = 10
        self.y = 10
        self.line_height = 15
        
    def indent(self):
        self.x += 10
        
    def unindent(self):
        self.x -= 10
    

pygame.init()
 
# Set the width and height of the screen [width,height]
size = [500, 700]
screen = pygame.display.set_mode(size)

pygame.display.set_caption("My Game")

#Loop until the user clicks the close button.
done = False

# Used to manage how fast the screen updates
clock = pygame.time.Clock()

# Initialize the joysticks
pygame.joystick.init()
    
# Get ready to print
textPrint = TextPrint()

#declare an array holding joystick values
js = [0,0,0,0,0]
#global state
#global state_changed
#global current_state_index

current_state_index = 0
state = [0, 0, 0]       #st_var0, st_var1, st_var_2
button1_pressed = False
button5_pressed = False
button2_pressed = False
button0_pressed = False
# -------- Main Program Loop -----------
while done==False:
    # EVENT PROCESSING STEP
    for event in pygame.event.get(): # User did something
        if event.type == pygame.QUIT: # If user clicked close
            done=True # Flag that we are done so we exit this loop
        
        # Possible joystick actions: JOYAXISMOTION JOYBALLMOTION JOYBUTTONDOWN JOYBUTTONUP JOYHATMOTION
        if event.type == pygame.JOYBUTTONDOWN:
            print("Joystick button pressed.")
        if event.type == pygame.JOYBUTTONUP:
            print("Joystick button released.")
            
 
    # DRAWING STEP
    # First, clear the screen to white. Don't put other drawing commands
    # above this, or they will be erased with this command.
    screen.fill(WHITE)
    textPrint.reset()

    # Get count of joysticks
    joystick_count = pygame.joystick.get_count()

    textPrint.printa(screen, "Number of joysticks: {}".format(joystick_count) )
    textPrint.indent()
    
    # For each joystick:
    for i in range(joystick_count):
        joystick = pygame.joystick.Joystick(i)
        joystick.init()
    
        textPrint.printa(screen, "Joystick {}".format(i) )
        textPrint.indent()
    
        # Get the name from the OS for the controller/joystick
        name = joystick.get_name()
        textPrint.printa(screen, "Joystick name: {}".format(name) )
        
        # Usually axis run in pairs, up/down for one, and left/right for
        # the other.
        axes = joystick.get_numaxes()
        textPrint.printa(screen, "Number of axes: {}".format(axes) )
        textPrint.indent()
        
        for i in range( axes ):
            #if(i == 1):
            #    data[0] = joystick.get_axis( i ) * 200 + 200
            axis = joystick.get_axis( i )
            js[i] = axis
            #if i==0:
            #    data[0] = (int)(axis * 255 + 255)/2
            #    time.sleep(0.2)
            textPrint.printa(screen, "Axis {} value: {:>6.3f}".format(i, axis) )
        textPrint.unindent()
            
        buttons = joystick.get_numbuttons()
        textPrint.printa(screen, "Number of buttons: {}".format(buttons) )
        textPrint.indent()

        for i in range( buttons ):
            button = joystick.get_button( i )
            #take button info and save it into array 
            if i == 11:
                js[4] = int(button)
            if i == 1:
                if(not button):
                    button1_pressed = False
                if(not button1_pressed):
                    if(button):
                        print "button 1 pressed"
                        state[current_state_index] += 1
                        button1_pressed = True
            if i == 5:
                if(not button):
                    button5_pressed = False
                if(not button5_pressed):
                    if(button):
                        print "button 5 pressed"
                        state[current_state_index] -= 1
                        button5_pressed = False
            if i == 2:
                if(not button):
                    button2_pressed = False
                if(not button2_pressed):
                    if(button):
                        print "button 2 pressed"
                        if(current_state_index == 2):
                            current_state_index = 0
                        else:
                            current_state_index += 1
                        button2_pressed = False

            if i == 0:
                if(not button):
                    button0_pressed = False
                if(not button0_pressed):
                    if(button):
                        print "button 0 pressed"
                        if(current_state_index == 0):
                            current_state_index = 2
                        else:
                            current_state_index -= 1
                        button0_pressed = False
                
            
                
            textPrint.printa(screen, "Button {:>2} value: {}".format(i,button) )
        textPrint.unindent()
            
        # Hat switch. All or nothing for direction, not like joysticks.
        # Value comes back in an array.
        hats = joystick.get_numhats()
        textPrint.printa(screen, "Number of hats: {}".format(hats) )
        textPrint.indent()

        for i in range( hats ):
            hat = joystick.get_hat( i )
            textPrint.printa(screen, "Hat {} value: {}".format(i, str(hat)) )
        textPrint.unindent()
        
        textPrint.unindent()

        #print data[0]
        #arduino.write(struct.pack('>4B',data[0],data[1],data[2],data[3]))
        #data2 = arduino.readline()
        #if (data2):
        #    print data2
    # ALL CODE TO DRAW SHOULD GO ABOVE THIS COMMENT

    k = int(js[0] * 127) + 128
    l = int(-js[1] * 127) + 128
    m = int(js[2] * 127) + 128
    n = int(js[3] * 127) + 128

    shared = {"Axis_0":k, "Axis_1":l, "Axis_2":m, "Axis_3":n, "s0": state[0],"s1": state[1], "s2": state[2]}
    fp = open("shared.pkl","w")
    pickle.dump(shared,fp)
    
    #print js axis values
    #print(k)
    
    #write joystick value to arduino
    #ser.write(struct.pack('>5B',k,l,m,n,js[4]))
    
    #time delay is required if target microcontroller takes more time to complete a loop
    time.sleep(0.1)        
    
    # Go ahead and update the screen with what we've drawn.
    pygame.display.flip()

    # Limit to 20 frames per second
    clock.tick(120)
    
# Close the window and quit.
# If you forget this line, the program will 'hang'
# on exit if running from IDLE.
pygame.quit ()
