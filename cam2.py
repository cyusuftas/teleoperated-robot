"""
Simply display the contents of the webcam with optional mirroring using OpenCV 
via the new Pythonic cv2 interface.  Press <esc> to quit.
"""

import cv2
import pickle
import time

def show_webcam(mirror=False):
    cam = cv2.VideoCapture(0)
    
    while True:
        try:
            fp = open("shared.pkl")
            shared = pickle.load(fp)
            axis_0 = shared["Axis_0"]
            axis_1 = shared["Axis_1"]
            axis_2 = shared["Axis_2"]
            axis_3 = shared["Axis_3"]
            s0 = shared["s0"]
            s1 = shared["s1"]
            s2 = shared["s2"]
        except:
            print "An exception occurred"
        ret_val, img = cam.read()
        h,w = img.shape[:2]
        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(img,str(s0),(w-w*9/10,h-h/10), font, 1,(50,255,255),1,cv2.LINE_AA)
        cv2.putText(img,str(s1),(w-w*7/10,h-h/10), font, 1,(50,255,255),1,cv2.LINE_AA)
        cv2.putText(img,str(s2),(w-w*5/10,h-h/10), font, 1,(50,255,255),1,cv2.LINE_AA)
        #cv2.putText(img,str(axis_3),(w-w*3/10,h-h/10), font, 1,(50,255,255),1,cv2.LINE_AA)
        cv2.rectangle(img,(int(w-w*9.5/10),int(h-h*2.5/10)),(int(w-w*7.5/10),int(h-h*0.5/10)),(0,255,0),3)
        cv2.rectangle(img,(int(w-w*7/10),int(h-h*2.5/10)),(int(w-w*5/10),int(h-h*0.5/10)),(0,255,0),3)
        cv2.circle(img,(int(0.05*w + 5 + (0.2*w - 5)*float(axis_0)/256),int(0.95*h - 5 + (0.2*h - 5)*(-axis_1)/256)), 5, (0,0,255), -1)
        cv2.circle(img,(int(0.3*w + 5 + (0.2*w - 5)*float(axis_2)/256),int(0.75*h + 5 + (0.2*h - 5)*(axis_3)/256)), 5, (0,0,255), -1)

        
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
