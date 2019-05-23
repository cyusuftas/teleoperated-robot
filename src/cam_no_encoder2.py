"""
Simply display the contents of the webcam with optional mirroring using OpenCV 
via the new Pythonic cv2 interface.  Press <esc> to quit.
"""

import cv2
import pickle
import time
rot_speed = 5
veh_speed = 10
color = (150, 0, 0)

def show_webcam(mirror=False):
    cam = cv2.VideoCapture(1)
    
    while True:
        try:
            fp = open("shared.pkl")
            shared = pickle.load(fp)
            axis_0 = shared["Axis_0"]
            axis_1 = shared["Axis_1"]
            axis_2 = shared["Axis_2"]
            axis_3 = shared["Axis_3"]
            rate = shared["rate"]
            show_js = shared["show_js"]
            show_vehicle = shared["show_vehicle"]
            m1_rpm = shared["m1_rpm"]
            m2_rpm = shared["m2_rpm"]
            m3_rpm = shared["m3_rpm"]
            batt = shared["battery"]
            shot = shared["shot"]
            current_state_index = shared["current_state_index"]
            speed_mode = shared["speed_mode"]
        except:
            print "An exception occurred"
        ret_val, img = cam.read()
        h,w = img.shape[:2]
        font = cv2.FONT_HERSHEY_SIMPLEX
##        cv2.putText(img,str(rate),(w-w*9/10,h-h/10), font, 1,(50,255,255),1,cv2.LINE_AA)
##        cv2.putText(img,str(rate2),(w-w*7/10,h-h/10), font, 1,(50,255,255),1,cv2.LINE_AA)
##        cv2.putText(img,str(s2),(w-w*5/10,h-h/10), font, 1,(50,255,255),1,cv2.LINE_AA)
##        cv2.putText(img,str(m0_rpm),(w-w*3/10,h-h/10), font, 1,(50,255,255),1,cv2.LINE_AA)
        
        cv2.putText(img,"Battery Voltage (1S):",(5,30), font, 0.5,color,1,cv2.LINE_AA)
        batt = int(batt) / 100.0
        battery = str(batt) + "V"
        cv2.putText(img,battery,(180,30), font, 0.5,color,1,cv2.LINE_AA)
        if show_js:
            cv2.rectangle(img,(int(w-w*9.5/10),int(h-h*2.5/10)),(int(w-w*7.5/10),int(h-h*0.5/10)),(0,255,0),2)
            cv2.rectangle(img,(int(w-w*7/10),int(h-h*2.5/10)),(int(w-w*5/10),int(h-h*0.5/10)),(0,255,0),2)
            cv2.circle(img,(int(0.05*w + 5 + (0.2*w - 10)*float(axis_0)/256),int(0.95*h - 5 + (0.2*h - 10)*(-axis_1)/256)), 5, (0,0,255), -1)
            cv2.circle(img,(int(0.3*w + 5 + (0.2*w - 10)*float(axis_2)/256),int(0.75*h + 5 + (0.2*h - 10)*(axis_3)/256)), 5, (0,0,255), -1)

        if show_vehicle:
            #draw triangle representing vehicle
            cv2.line(img, (500,50), (600,50), (0,125,255), 2)
            cv2.line(img, (500,50), (550,137), (0,125,255), 2)
            cv2.line(img, (550,137), (600,50), (0,125,255), 2)
            line_length = 10 + shot * 20
            cv2.line(img, (540,50), (540,50-line_length), (0,0,255), 2)
            cv2.line(img, (560,50), (560,50-line_length), (0,0,255), 2)
            cv2.line(img, (520,50-line_length), (580,50-line_length), (0,0,255), 2)

            #print rpm values
            cv2.putText(img,str(m2_rpm),(480,47), font, 0.5,(50,255,255),1,cv2.LINE_AA)
            cv2.putText(img,str(m3_rpm),(602,47), font, 0.5,(50,255,255),1,cv2.LINE_AA)
            cv2.putText(img,str(m1_rpm),(544,153), font, 0.5,(50,255,255),1,cv2.LINE_AA)
        
        #print parameters
        cv2.putText(img,"Rate: ",(500,250), font, 0.5, color, 1 ,cv2.LINE_AA)
        cv2.putText(img,str(rate),(600,250), font, 0.5, color, 1 ,cv2.LINE_AA)
        cv2.putText(img,"Speed Mode:",(500,300), font, 0.5, color, 1 ,cv2.LINE_AA)
        cv2.putText(img,str(speed_mode),(600,300), font, 0.5, color, 1 ,cv2.LINE_AA)
        cv2.putText(img,"Show JS: ",(500,350), font, 0.5, color, 1 ,cv2.LINE_AA)
        if show_js:
            js_txt = "ON"
        else:
            js_txt = "OFF"
        cv2.putText(img,str(js_txt),(600,350), font, 0.5, color, 1 ,cv2.LINE_AA)
        cv2.putText(img,"Show Veh.: ",(500,400), font, 0.5, color, 1 ,cv2.LINE_AA)
        if show_vehicle:
            vh_txt = "ON"
        else:
            vh_txt = "OFF"
        cv2.putText(img,str(vh_txt),(600,400), font, 0.5, color, 1 ,cv2.LINE_AA)

        #draw rectangle to highlight current parameter
        rect_dist = 235 + current_state_index * 50
        cv2.rectangle(img,(495,rect_dist),(635,rect_dist + 20),(0,255,0),2)


        
        if mirror: 
            img = cv2.flip(img, 0)
        cv2.imshow('my webcam', img)
        if cv2.waitKey(1) == 27: 
            break  # esc to quit
    cv2.destroyAllWindows()


def main():
    show_webcam(mirror=False)


if __name__ == '__main__':
    main()
